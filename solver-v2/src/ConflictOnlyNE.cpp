
#include "ConflictOnlyNE.hpp"
//#include "State.hpp"

ConflictOnlyNE::ConflictOnlyNE(std::shared_ptr<Model> model, std::shared_ptr<State> st) {
    this->model = model;
    this->state = st;
    calculated.resize(model->getAllVariables().size(), 0);
    lastSuggested = model->getViolatedConstraints().size()*2;
}

//MinConflictFlipNE::MinConflictFlipNE(const MinConflictFlipNE& orig) {
//}

ConflictOnlyNE::~ConflictOnlyNE() {
    //    std::cout << "Destructing MinConflictFlipNE" << std::endl;
}

Move* ConflictOnlyNE::next() {
    if (firstMove) {
        iter++;
        firstMove = false;
        lastSuggested = suggested;
        moveCounter = 0;
        suggested = 0;
        moveIterator = model->getViolatedConstraints().begin();


    }

    if (model->getViolatedConstraints().empty()) {
        return NULL;
    }
    while (moveIterator != model->getViolatedConstraints().end()) {
        while (moveCounter < (*moveIterator).second->getVariablePointers().size()) {
            if (calculated.at((*moveIterator).second->getVariablePointers().at(moveCounter)->getID()) != iter) {
                suggested++;
                int id = (*moveIterator).second->getVariablePointers().at(moveCounter)->getID();
                calculated.at(id) = iter;
                Variable* var = model->getVariable(id);

                moveCounter++;
                Move* mv = new Move(var, (1 - var->getCurrentValue()) - var->getCurrentValue());
                mv->deltaVector.resize(state->getEvaluation().size(), 0);
//                mv->setID(mv->var->getID());
                return mv;


            } else {
                moveCounter++;
            }
        }
        moveIterator++;        
        moveCounter = 0;
    }
    firstMove = true;
    return NULL;



    //    Variable* var = model->getVariable(this->var);
    //
    //    moveCounter++;
    //    Move* mv = new Move(var, (1 - var->getCurrentValue()) - var->getCurrentValue());
    //    mv->deltaVector.resize(state->getEvaluation().size(), 0);
    //    return mv;
}
//Move* ConflictOnlyNE::next() {
//    
//    Variable* var = model->getVariable(this->var);
//
//    moveCounter++;
//    Move* mv = new Move(var, (1 - var->getCurrentValue()) - var->getCurrentValue());
//    mv->deltaVector.resize(state->getEvaluation().size(), 0);
//    return mv;
//}

//bool ConflictOnlyNE::hasNext() {
//    if (firstMove) {
//        iter++;
//        firstMove = false;
//        lastSuggested = suggested;
//        moveCounter = 0;
//        suggested = 0;
//        moveIterator = model->getViolatedConstraints().begin();
//
//
//    }
//
//    if (model->getViolatedConstraints().empty()) {
//        return false;
//    }
//    while (moveIterator != model->getViolatedConstraints().end()) {
//        while (moveCounter < (*moveIterator).second->getVariablePointers().size()) {
//            if (calculated.at((*moveIterator).second->getVariablePointers().at(moveCounter)->getID()) != iter) {
//                suggested++;
//                var = (*moveIterator).second->getVariablePointers().at(moveCounter)->getID();
//                calculated.at(var) = iter;
//                return true;
//            } else {
//                moveCounter++;
//            }
//        }
//        moveIterator++;
//        moveCounter = 0;
//    }
//    firstMove = true;
//    return false;
//    //    if (moveCounter < (*moveIterator).second->getVariablePointers().size()) {
//    //
//    //        if (calculated.at((*moveIterator).second->getVariablePointers().at(moveCounter)->getID()) != iter) {
//    //            var = (*moveIterator).second->getVariablePointers().at(moveCounter)->getID();
//    //
//    //            return true;
//    //        } else {
//    //            
//    //        }
//    //    } else {
//    //        moveIterator++;
//    //    }
//    //    int rand = Random::Integer(model->getViolatedConstraints().size() - 1);
//
//}

unsigned ConflictOnlyNE::getSize() {
    return lastSuggested;
}

Move* ConflictOnlyNE::nextRandom() {
    moveIterator = model->getViolatedConstraints().begin();
    int rand = Random::Integer(0, model->getViolatedConstraints().size() - 1);
    for (int i = 0; i < rand; i++) {
        moveIterator++;
    }
    int rand2 = Random::Integer(0, (*moveIterator).second->getVariablePointers().size() - 1);
    Variable* var = (*moveIterator).second->getVariablePointers().at(rand2);


    //        Variable* iv = model->getMaskAt(Random::Integer(0, (int) model->getMask().size() - 1));
    //    randomCounter++;
    //    //    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
    Move* mv = new Move(var, (1 - var->getCurrentValue()) - var->getCurrentValue());
    //    //    mv->deltaVector.resize(state->getEvaluation().size());
    mv->deltaVector.resize(state->getEvaluation().size(), 0);
    return mv;
}

//bool ConflictOnlyNE::hasNextRandom() {
//    return false;
//    if (randomCounter < randomMovesWanted) {
//        return true;
//    } else {
//        randomCounter = 0;
//        return false;
//    }
//}

//void ConflictOnlyNE::setRandomCounter(unsigned numberOfRandomMoves) {
//    randomMovesWanted = numberOfRandomMoves;
//}

bool ConflictOnlyNE::calculateDelta(Move* mv) {
    std::vector<int>& change = mv->getDeltaVector();
    for (unsigned i = 0; i < change.size(); i++) {
        model->getEvaluationInvariantNr(i)->calculateDeltaValue();
    }

    Variable* variable = mv->var;
    calculated.at(variable->getID()) = iter;
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

bool ConflictOnlyNE::commitMove(Move* mv) {
    firstMove = true;
    lastSuggested = suggested;
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