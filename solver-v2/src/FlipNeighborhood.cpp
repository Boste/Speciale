
#include "FlipNeighborhood.hpp"
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

//template<typename returnType>


/// make something to break ties 

//bool FlipNeighborhood::bestImprovement(std::shared_ptr<State> st) {
//    //    model->shuffleMask();
//    std::vector<int>& delta = bestMove->getDeltaVector();
//    for (int& d : delta) {
//        d = 0;
//    }
//    //    Move* bestMove = new Move(model->getConstraints().size(),FLIP);
//    //    mv->first = model->getNonFixedBinaryVariable(Random::Integer(0, model->getNonFixedBinaryVariables().size() - 1));
//    //    mv->flip();
//    //    calculateDeltaChange(mv);
//    //    int violationChange = delta.first;
//    //    int objectiveChange = delta[0];
//    //    bestMove->setDeltaChangeSize(model->getConstraints().size());
//    //        bestMove->copy(mv);
//    bool improvement = false;
//    //    std::cout <<model->getMask().size() << std::endl;
//    for (IntegerVariable* iv : model->getMask()) {
//        //    for (unsigned i = 0; i < model->getNonFixedBinaryVariables().size(); i++) {
//        //        for(IntegerVariable* iv : *model->getIntegerVariables()){
//        //        IntegerVariable* iv = model->getMaskAt(i);
//        assert(!iv->isDef() && !iv->isFixed());
//        mv->first = iv;
//        //        mv->deltaValueFirst = 1 - iv->getCurrentValue() - iv->getCurrentValue();
//        mv->flip();
//        calculateDeltaChange(mv);
//
//        bool fewerViolations = false;
//        bool moreViolations = false;
//        //        for (int i : mv->getDeltaChanges()) {
//        //            std::cout << i << " ";
//        //        }
//        //        std::cout << std::endl;
//
//
//        for (unsigned j = 1; j < mv->getDeltaVector().size(); j++) { // No ties in favor of newest move
//
//            if (mv->getDeltaVector()[j] < bestMove->getDeltaVector()[j]) {
//                //                std::cout << "prob never here" << std::endl;
//                //                
//                //                std::cout << "fewer violations" << std::endl;
//                fewerViolations = true;
//                break;
//            } else if (mv->getDeltaVector()[j] > bestMove->getDeltaVector()[j]) {
//                moreViolations = true;
//                //                std::cout << "more violations" << std::endl;
//                break;
//            }
//        }
//        if (!moreViolations) {
//            if (fewerViolations || mv->getDeltaVector()[0] <= bestMove->getDeltaVector()[0]) {
//                //                for (int i : delta) {
//                //                    std::cout << i << " ";
//                //                }
//                //                std::cout << std::endl;
//                //                bestDelta = delta;
//                //                std::cout << "got new best move " << std::endl;
//                //            violationChange = delta.first;
//                //            objectiveChange = delta.second;
//                improvement = true;
//                bestMove->copy(mv);
//            }
//        }
//    }
//    if (improvement) {
//        //    std::cout << "commit delta" << std::endl;
//        //        for (int i : bestMove->getDeltaChanges()) {
//        //            std::cout << i << " ";
//        //        }
//        //        std::cout << std::endl;
//        //        std::cout << "this was one best iter " << std::endl;
//
//        unsigned changeAt = 0;
//        std::vector<int> bestDelta = bestMove->getDeltaVector();
//        for (unsigned i = 1; i < bestDelta.size(); i++) {
//            if (bestDelta[i] != 0) {
//                changeAt = i;
//                break;
//            }
//        }
//        if (changeAt > 0) {
//            if (bestDelta[changeAt] > 0) {
//                //                delete bestMove;
//                return false;
//            } else {
//                //            std::cout << "commit move" << std::endl;
//                commitMove(bestMove, st);
//                //                std::cout << "obj val " << st->getObjectiveValue() << std::endl;
//                return true;
//            }
//        } else if (bestDelta[0] < 0) {
//            commitMove(bestMove, st);
//            //            std::cout << "obj val " << st->getObjectiveValue() << std::endl;
//            return true;
//
//            //        std::cout << "commit move" << std::endl;
//        }
//    }
//
//    //    return true;
//    //    if (violationChange <= 0 && objectiveChange < 0) {
//    //       commitMove(bestMove, st);
//    //    } else {
//    //    delete bestMove;
//    //        std::cout << "no improving move" << std::endl;
//    //        return false;
//    //        std::exit(1);
//
//
//    //    delete bestMove;
//    return false;
//    //    std::cout << "changed variable " << bestMove->first->getID() << std::endl;
//    //    std::cout << "violation change " << violationChange << " objective change " << objectiveChange << std::endl;
//    //    return true;
//}
/// Wrong name
//void FlipNeighborhood::randomWalk(std::shared_ptr<State> st) {
////    if (mv->moveType == FLIP) {
//        unsigned var = Random::Integer(0, model->getNonFixedBinaryVariables().size() - 1);
//        //        if (var < 0 || model->getIntegerVariables()->size() <= var) {
//        //            std::cout << "var " << var << std::endl;
//        //            std::cout << model->getIntegerVariables()->size() << std::endl;
//        //        }
//        assert(var < model->getNonFixedBinaryVariables().size());
//        mv->first = model->getNonFixedBinaryVariable(var);
//        mv->flip();
//        commitMove(mv, st);
////    } else {
////        std::cout << "Only making flip moves" << std::endl;
////    }
//}

//bool FlipNeighborhood::firstImprovement(Move* mv, Model * st) {
//
//}

//template<typename returnType>

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
        invar->addChange(variable->getID(), mv->getVariableChange());
    }
    bool legal = true;
    for (updateType invar : queue) {

        legal = invar->calculateDeltaValue();
        if (!legal) {
            break;
        }
        if (invar->getDeltaValue() != 0) {
            for (updateType inv : model->getUpdate(invar)) {
                inv->addChange(invar->getVariableID(), invar->getDeltaValue());
            }
        }
    }
    if (!legal) {
        for (updateType invar : queue) {
            invar->calculateDeltaValue();
        }
    } else {
        for (unsigned i = 0; i < change.size(); i++) {
            change[i] += model->getEvaluationInvariantNr(i)->getDeltaValue();
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

    }
    for (unsigned i = 0; i < model->getEvaluationInvariants().size(); i++) {
        evaluation[i] = model->getEvaluationInvariantNr(i)->getCurrentValue();
    }

    return true;
}

//void FlipNeighborhood::makeMove(Move* mv, std::shared_ptr<State> st) {
//    commitMove(mv, st);
//}