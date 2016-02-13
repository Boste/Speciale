
#include "EvalFlipNE.hpp"
//#include "State.hpp"

EvalFlipNE::EvalFlipNE(std::shared_ptr<Storage> model, std::shared_ptr<State> st) {
    this->model = model;
    this->state = st;
}


EvalFlipNE::~EvalFlipNE() {
}
Move* EvalFlipNE::next() {
    if (moveCounter < getSize()) {
        Variable* iv = model->getEvaluationVariableNr(moveCounter);
        moveCounter++;
        Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
        mv->deltaVector.resize(state->getEvaluation().size(), 0);
        return mv;
    } else {
        moveCounter = 0;
        return NULL;
    }
}
unsigned EvalFlipNE::getSize(){
    return model->getEvaluationVariables().size();
}

Move* EvalFlipNE::nextRandom() {
    Variable* iv = model->getEvaluationVariableNr(Random::Integer(0, (int) model->getEvaluationVariables().size() - 1));
    randomCounter++;
    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
    mv->deltaVector.resize(state->getEvaluation().size(), 0);
    return mv;
}

bool EvalFlipNE::calculateDelta(Move* mv) {
    std::vector<int>& change = mv->getDeltaVector();
    for (unsigned i = 0; i < change.size(); i++) {
        model->getEvaluationInvariantNr(i)->calculateDelta();
    }
    Variable* variable = mv->var;
    propagation_queue& queue = model->getPropagationQueue(variable);
    updateVector& update = model->getUpdate(variable);
    for (updateType& invar : update) {
        invar->proposeChange(variable->getID(), mv->getVariableChange());
    }
    bool allowed = true;
    for (updateType invar : queue) {

        allowed = invar->calculateDelta();
        if (!allowed) {
            break;
        }
        if (invar->getDeltaValue() != 0) {
            for (updateType inv : model->getUpdate(invar)) {
                inv->proposeChange(invar->getVariableID(), invar->getDeltaValue());
            }
        }
    } 
    if (!allowed) {
        for (updateType invar : queue) {
            invar->calculateDelta();
        }
    } else {
        for (unsigned i = 0; i < change.size(); i++) {
            change[i] = model->getEvaluationInvariantNr(i)->getDeltaValue();
        }
    }


    return allowed;
}

bool EvalFlipNE::commitMove(Move* mv) {
    moveCounter = 0;
    Variable* var = mv->getVar();
    std::vector<int>& evaluation = state->getEvaluation();

    // Skal genberegne!!!!!
    bool legal = calculateDelta(mv);
    if (!legal) {
        return false;
    }
    var->setCurrentValue(1 - var->getCurrentValue());
    propagation_queue queue = model->getPropagationQueue(var);
    for (updateType invar : queue) {

        invar->updateValue();
        if (invar->representConstraint()) {
            if (invar->getCurrentValue() == 0) {
                if (invar->getInvariantPointers().back()->inViolatedConstraints()) {
                    model->removeViolatedConstraint(invar->getInvariantPointers().back());
                }
            } else {
                if (!invar->getInvariantPointers().back()->inViolatedConstraints()) {
                    model->addViolatedConstraint(invar->getInvariantPointers().back());
                }
            }
        }
    }
    for (unsigned i = 0; i < model->getEvaluationInvariants().size(); i++) {
        evaluation[i] = model->getEvaluationInvariantNr(i)->getCurrentValue();
    }

    return true;
}