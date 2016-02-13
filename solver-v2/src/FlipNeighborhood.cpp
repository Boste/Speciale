
#include "FlipNeighborhood.hpp"
//#include "State.hpp"

FlipNeighborhood::FlipNeighborhood(std::shared_ptr<Storage> model, std::shared_ptr<State> st) {
    this->model = model;
    this->state = st;
    this->size = model->getMask().size();
}


FlipNeighborhood::~FlipNeighborhood() {
}

Move* FlipNeighborhood::next() {
    if(moveCounterStart == size){
        moveCounterStart = 0;
        return NULL;
    }

    Variable* var;
    if(moveCounter < model->getMask().size()) {
        var = model->getMaskAt(moveCounter);

    } else {
        moveCounter = 0;
        var = model->getMaskAt(moveCounter);
    }
        Move* mv = new Move(var, (1 - var->getCurrentValue()) - var->getCurrentValue());
    moveCounter++;
    mv->deltaVector.resize(state->getEvaluation().size(), 0);
    moveCounterStart++;
    return mv;

}

unsigned FlipNeighborhood::getSize() {
    return size;
}

Move* FlipNeighborhood::nextRandom() {
    Variable* var = model->getMaskAt(Random::Integer(0, (int) model->getMask().size() - 1));
    randomCounter++;

    Move* mv = new Move(var, (1 - var->getCurrentValue()) - var->getCurrentValue());
    mv->deltaVector.resize(state->getEvaluation().size(), 0);
    return mv;
}


bool FlipNeighborhood::calculateDelta(Move* mv) {
    for (unsigned i = 0; i < mv->getDeltaVector().size(); i++) {
        model->getEvaluationInvariantNr(i)->calculateDelta();
    }
    std::vector<int>& change = mv->getDeltaVector();

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

bool FlipNeighborhood::commitMove(Move* mv) {
    moveCounterStart = 0;
    
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