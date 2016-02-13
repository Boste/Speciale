
#include "RestrictedFlipNE.hpp"

RestrictedFlipNE::RestrictedFlipNE(std::shared_ptr<Storage> model, std::shared_ptr<State> st) {
    this->model = model;
    this->state = st;
    small = model->getMask().size() < 10000;
    probability = 5000.0 / model->getMask().size();
    assert(probability != 0);
}


RestrictedFlipNE::~RestrictedFlipNE() {
}

Move* RestrictedFlipNE::next() {
    while (Random::Double() > probability) {
        moveCounter++;
    }
    if (moveCounter < model->getMask().size()) {


        Variable* var = model->getMaskAt(moveCounter);
        assert(!var->isFixed());
        moveCounter++;
        //    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
        Move* mv = new Move(var, (1 - var->getCurrentValue()) - var->getCurrentValue());
        //    mv->deltaVector.resize(state->getEvaluation().size());
        mv->deltaVector.resize(state->getEvaluation().size(), 0);
        
        return mv;
        
    } else {
        moveCounter = 0;
        return NULL;
    }
}

unsigned RestrictedFlipNE::getSize() {
    return model->getMask().size();
}

Move * RestrictedFlipNE::nextRandom() {
    Variable* iv = model->getMaskAt(Random::Integer(0, (int) model->getMask().size() - 1));
    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
    mv->deltaVector.resize(state->getEvaluation().size(), 0);
    return mv;
}

bool RestrictedFlipNE::calculateDelta(Move * mv) {

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
    bool legal = true;
    for (updateType invar : queue) {

        legal = invar->calculateDelta();
        if (!legal) {
            break;
        }
        if (invar->getDeltaValue() != 0) {
            for (updateType inv : model->getUpdate(invar)) {
                inv->proposeChange(invar->getVariableID(), invar->getDeltaValue());
            }
        }
    }
    if (!legal) {
        for (updateType invar : queue) {
            invar->calculateDelta();
        }
    } else {
        for (unsigned i = 0; i < change.size(); i++) {
            change[i] = model->getEvaluationInvariantNr(i)->getDeltaValue();
        }
    }


    return legal;
}


bool RestrictedFlipNE::commitMove(Move * mv) {
    moveCounter = 0;
    Variable* var = mv->getVar();
    std::vector<int>& evaluation = state->getEvaluation();


    for (unsigned i = 0; i < mv->getDeltaVector().size(); i++) {
        model->getEvaluationInvariantNr(i)->calculateDelta();
    }

    Variable* variable = mv->var;
    propagation_queue& queue = model->getPropagationQueue(variable);
    updateVector& update = model->getUpdate(variable);

    for (updateType& invar : update) {
        invar->proposeChange(variable->getID(), mv->getVariableChange());
    }
    for (updateType invar : queue) {
        invar->calculateDelta();
        for (updateType inv : model->getUpdate(invar)) {
            inv->proposeChange(invar->getVariableID(), invar->getDeltaValue());
        }
    }
    var->setCurrentValue(1 - var->getCurrentValue());

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