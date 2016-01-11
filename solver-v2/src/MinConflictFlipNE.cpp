
#include "MinConflictFlipNE.hpp"
//#include "State.hpp"

MinConflictFlipNE::MinConflictFlipNE(std::shared_ptr<Model> model, std::shared_ptr<State> st) {
    this->model = model;
    this->state = st;
}

//MinConflictFlipNE::MinConflictFlipNE(const MinConflictFlipNE& orig) {
//}

MinConflictFlipNE::~MinConflictFlipNE() {
    //    std::cout << "Destructing MinConflictFlipNE" << std::endl;
}

Move* MinConflictFlipNE::next() {
    //    std::cout << "Use iterator and variable pointers, remember to fix the pointers" << std::endl;
    //    Variable* var;
    //    debug;'    std::cout << moveCounter << std::endl;
    //    std::cout << moveCounter << std::endl;

    //    if (moveCounter < moveIterator->second->getVariablePointers().size()) {
    //        debug;
    Variable* var = varsInNeighborhood.at(moveCounter);
    //    assert(!var->isDef() && !var->isFixed());
    //    } else {
    //        //        debug;
    //     
    //    moveIterator++;
    //        moveCounter = 0;
    //        if (moveIterator == model->getViolatedConstraints().end()) {
    //            //            debug;
    //
    //            var = moveIterator->second->getVariablePointers().at(moveCounter);
    //            //            moveIterator++;
    //        } else {
    //            //            debug;
    //            //            std::cout << moveIterator->second->getVariablePointers().size() << std::endl;
    //            var = moveIterator->second->getVariablePointers().at(moveCounter);
    //        }
    //    }
    //    //    debug;
    //    Variable* iv = model->getMaskAt(moveCounter);
    moveCounter++;
    //    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
    Move* mv = new Move(var, (1 - var->getCurrentValue()) - var->getCurrentValue());
    //    mv->deltaVector.resize(model->getPriorityVectors().size());
    mv->deltaVector.resize(model->getPriorityVectors().size(), 0);
    //    std::cout << var->getID() << std::endl;
    return mv;
}

bool MinConflictFlipNE::hasNext() {

    if (firstMove) {
        if (model->getViolatedConstraints().empty()) {
            return false;
        }
        firstMove = false;
        int rand = Random::Integer(model->getViolatedConstraints().size() - 1);
        std::unordered_map<unsigned, invariant>::iterator moveIterator = model->getViolatedConstraints().begin();

        for (int i = 0; i < rand; i++) {
            moveIterator++;
        }
        InvariantContainer invariantQueue;
        invariantQueue.push_back(moveIterator->second);
        while (!invariantQueue.empty()) {
            invariant inv = invariantQueue.back();
            invariantQueue.pop_back();
            for (invariant invar : inv->getInvariantPointers()) {
                invariantQueue.push_back(invar);
            }
            for (Variable* var : inv->getVariablePointers()) {
                //                assert(!var->isDef());
                if (!var->isFixed()) {
                    varsInNeighborhood.push_back(var);
                }
            }
        }

    }
    //    debug;
    //    std::cout << "Constriants in conflict " << model->getViolatedConstraints().size() << std::endl;
    //    for (std::pair<unsigned, invariant> inv : model->getViolatedConstraints()) {
    //        std::cout << inv.second->getVariablePointers().size() << std::endl;
    //        std::cout << inv.second->getInvariantPointers().size() << std::endl;
    //    }
    //    debug;

    //    moveIterator++;
    //    while (moveIterator != model->getViolatedConstraints().end()) {
    //        std::cout <<  moveIterator->first<< " violations " << state->getEvaluation().at(1) << std::endl;
    if (moveCounter >= varsInNeighborhood.size()) {
        moveCounter = 0;
        firstMove = true;
//        std::vector<Variable*> tmp;
        varsInNeighborhood.clear(); // .swap(tmp); //resize(0); //.clear();
        return false;
        //            moveIterator++;
        //            continue;
    }

    //        debug;
    return true;
    //    }
    //    debug;

    //    moveCounter = 0;
    //    firstMove = true;
    //    return false;
}

unsigned MinConflictFlipNE::getSize() {
    return varsInNeighborhood.size();
}

Move* MinConflictFlipNE::nextRandom() {
    Variable* iv = model->getMaskAt(Random::Integer(0, (int) model->getMask().size() - 1));
    randomCounter++;
    //    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
    //    mv->deltaVector.resize(model->getPriorityVectors().size());
    mv->deltaVector.resize(model->getPriorityVectors().size(), 0);
    return mv;
}

bool MinConflictFlipNE::hasNextRandom() {
    return false;
    if (randomCounter < randomMovesWanted) {
        return true;
    } else {
        randomCounter = 0;
        return false;
    }
}

void MinConflictFlipNE::setRandomCounter(unsigned numberOfRandomMoves) {
    randomMovesWanted = numberOfRandomMoves;
}

bool MinConflictFlipNE::calculateDelta(Move* mv) {
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

bool MinConflictFlipNE::commitMove(Move* mv) {
    moveCounter = 0;
    firstMove = true;
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
                    //                    vioCons[invar->getInvariantPointers().back()->getID()] = invar->getInvariantPointers().back();
                    //                    invar->getInvariantPointers().back()->setInViolatedConstraints(true);

                }
            }
        }
    }
    for (unsigned i = 0; i < model->getEvaluationInvariants().size(); i++) {
        evaluation[i] = model->getEvaluationInvariantNr(i)->getCurrentValue();
    }

    return true;
}

//void MinConflictFlipNE::makeMove(Move* mv, std::shared_ptr<State> st) {
//    commitMove(mv, st);
//}