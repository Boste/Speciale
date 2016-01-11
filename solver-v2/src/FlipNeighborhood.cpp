
#include "FlipNeighborhood.hpp"
#include "Flip2Neighborhood.hpp"
//#include "State.hpp"

FlipNeighborhood::FlipNeighborhood(std::shared_ptr<Model> model, std::shared_ptr<State> st) {
    this->model = model;
    this->state = st;
}

//FlipNeighborhood::FlipNeighborhood(const FlipNeighborhood& orig) {
//}

FlipNeighborhood::~FlipNeighborhood() {
    //    std::cout << "Destructing FlipNeighborhood" << std::endl;
}
Move* FlipNeighborhood::next() {
    Variable* iv = model->getMaskAt(moveCounter);
    moveCounter++;
    //    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
    //    mv->deltaVector.resize(model->getPriorityVectors().size());
    mv->deltaVector.resize(model->getPriorityVectors().size(), 0);
    return mv;
}

bool FlipNeighborhood::hasNext() {
    if (moveCounter < model->getMask().size()) {
        return true;
    } else {
        moveCounter = 0;
        return false;
    }
}
unsigned FlipNeighborhood::getSize(){
    return model->getMask().size();
}

Move* FlipNeighborhood::nextRandom() {
    Variable* iv = model->getMaskAt(Random::Integer(0, (int) model->getMask().size() - 1));
    randomCounter++;
    //    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
    //    mv->deltaVector.resize(model->getPriorityVectors().size());
    mv->deltaVector.resize(model->getPriorityVectors().size(), 0);
    return mv;
}

bool FlipNeighborhood::hasNextRandom() {
    if (randomCounter < randomMovesWanted) {
        return true;
    } else {
        randomCounter = 0;
        return false;
    }
}

void FlipNeighborhood::setRandomCounter(unsigned numberOfRandomMoves) {
    randomMovesWanted = numberOfRandomMoves;
}

bool FlipNeighborhood::calculateDelta(Move* mv) {

    std::vector<int>& change = mv->getDeltaVector();

    Variable* variable = mv->var;
    propagation_queue& queue = model->getPropagationQueue(variable);
    updateVector& update = model->getUpdate(variable);

    for (updateType& invar : update) {
        invar->proposeChange(variable->getID(), mv->getVariableChange());
    }
    bool legal = true;
    for (updateType invar : queue) {

        legal = invar->calculateDeltaValue();
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
            invar->calculateDeltaValue();
        }
    } else {
        for (unsigned i = 0; i < change.size(); i++) {
            change[i] = model->getEvaluationInvariantNr(i)->getDeltaValue();
        }
    }


    return legal;
}

bool FlipNeighborhood::commitMove(Move* mv) {
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
//                    std::unordered_map<unsigned, invariant>& vioCons = model->getViolatedConstraints();
                    model->removeViolatedConstraint(invar->getInvariantPointers().back());

                }
            } else {
                if (!invar->getInvariantPointers().back()->inViolatedConstraints()) {
//                    std::unordered_map<unsigned, invariant>& vioCons = model->getViolatedConstraints();
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

//void FlipNeighborhood::makeMove(Move* mv, std::shared_ptr<State> st) {
//    commitMove(mv, st);
//}