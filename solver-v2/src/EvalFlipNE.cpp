
#include "EvalFlipNE.hpp"
//#include "State.hpp"

EvalFlipNE::EvalFlipNE(std::shared_ptr<Model> model, std::shared_ptr<State> st) {
    this->model = model;
    this->state = st;
}

//FlipObjNE::FlipObjNE(const FlipObjNE& orig) {
//}

EvalFlipNE::~EvalFlipNE() {
    //    std::cout << "Destructing FlipObjNE" << std::endl;
}
Move* EvalFlipNE::next() {
    if (moveCounter < getSize()) {
        Variable* iv = model->getEvaluationVariableNr(moveCounter);
        moveCounter++;
        //    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
        Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
        //    mv->deltaVector.resize(state->getEvaluation().size());
        mv->deltaVector.resize(state->getEvaluation().size(), 0);
        return mv;
    } else {
        moveCounter = 0;
//        Move* mv = NULL;
        return NULL;
    }
}
//Move* EvalFlipNE::next() {
//    
//    Variable* iv = model->getEvaluationVariableNr(moveCounter);
//    moveCounter++;
//    //    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
//    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
//    //    mv->deltaVector.resize(state->getEvaluation().size());
//    mv->deltaVector.resize(state->getEvaluation().size(), 0);
//    return mv;
//}

//bool EvalFlipNE::hasNext() {
//    
//    if (moveCounter < model->getEvaluationVariables().size()) {
//        return true;
//    } else {
//        moveCounter = 0;
//        
//        return false;
//    }
//}
unsigned EvalFlipNE::getSize(){
    return model->getEvaluationVariables().size();
}

Move* EvalFlipNE::nextRandom() {
    Variable* iv = model->getEvaluationVariableNr(Random::Integer(0, (int) model->getEvaluationVariables().size() - 1));
    randomCounter++;
    //    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
    //    mv->deltaVector.resize(state->getEvaluation().size());
    mv->deltaVector.resize(state->getEvaluation().size(), 0);
    return mv;
}

//bool EvalFlipNE::hasNextRandom() {
//    if (randomCounter < randomMovesWanted) {
//        return true;
//    } else {
//        randomCounter = 0;
//        return false;
//    }
//}

//void EvalFlipNE::setRandomCounter(unsigned numberOfRandomMoves) {
//    randomMovesWanted = numberOfRandomMoves;
//}

bool EvalFlipNE::calculateDelta(Move* mv) {
    std::vector<int>& change = mv->getDeltaVector();
    for (unsigned i = 0; i < change.size(); i++) {
        model->getEvaluationInvariantNr(i)->calculateDeltaValue();
    }
    Variable* variable = mv->var;
    propagation_queue& queue = model->getPropagationQueue(variable);
    updateVector& update = model->getUpdate(variable);
    for (updateType& invar : update) {
        invar->proposeChange(variable->getID(), mv->getVariableChange());
    }
    bool allowed = true;
    for (updateType invar : queue) {

        allowed = invar->calculateDeltaValue();
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
            invar->calculateDeltaValue();
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

//void FlipObjNE::makeMove(Move* mv, std::shared_ptr<State> st) {
//    commitMove(mv, st);
//}