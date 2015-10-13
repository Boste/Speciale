
#include "NeighborhoodExplorer.hpp"
#include "State.hpp"

NeighborhoodExplorer::NeighborhoodExplorer(std::shared_ptr<Model> model) {
    this->model = model;
}

//NeighborhoodExplorer::NeighborhoodExplorer(const NeighborhoodExplorer& orig) {
//}

NeighborhoodExplorer::~NeighborhoodExplorer() {
    //    std::cout << "Destructing NeighborhoodExplorer" << std::endl;
}

//template<typename returnType>

bool NeighborhoodExplorer::bestImprovement(Move* mv, std::shared_ptr<State> st) {
    //    model->shuffleMask();
    Move* bestMove = new Move();
    std::vector<int> bestDelta = calculateDeltaChange(mv);
    //    int violationChange = delta.first;
    //    int objectiveChange = delta[0];
    bestMove->copy(mv);
    std::cout <<model->getMask().size() << std::endl;
    for (IntegerVariable* iv : model->getMask()) {
//        debug;
        //    for (unsigned i = 0; i < model->getNonFixedBinaryVariables().size(); i++) {
        //        for(IntegerVariable* iv : *model->getIntegerVariables()){
        //        IntegerVariable* iv = model->getMaskAt(i);

        mv->first = iv;
        mv->deltaValueFirst = 1 - iv->getCurrentValue() - iv->getCurrentValue();
        std::vector<int> delta = calculateDeltaChange(mv);
        bool best = true;
        for (unsigned j = 1; j < bestDelta.size(); j++) { // No ties in favor of newest move
            if (delta[j] > bestDelta[j]) {
                best = false;
                break;
            }
        }

        if (best && delta[0] <= bestDelta[0]) {
            bestDelta = delta;
            //            violationChange = delta.first;
            //            objectiveChange = delta.second;
            bestMove->copy(mv);
        }
    }
//    debug;
    unsigned changeAt = 0;
    for (unsigned i = 1; i < bestDelta.size(); i++) {
        if (bestDelta[i] != 0) {
            changeAt = i;
            break;
        }
    }
    if (changeAt > 0) {
        if (bestDelta[changeAt] > 0) {
            delete bestMove;
            return false;
        } else {
            commitMove(bestMove, st);
        }
    } else if (bestDelta[0] < 0) {
        commitMove(bestMove, st);
    }
    //    return true;
    //    if (violationChange <= 0 && objectiveChange < 0) {
    //       commitMove(bestMove, st);
    //    } else {
    //    delete bestMove;
    //        std::cout << "no improving move" << std::endl;
    //        return false;
    //        sleep(1);
    //        std::exit(1);


    delete bestMove;
    //    std::cout << "changed variable " << bestMove->first->getID() << std::endl;
    //    std::cout << "violation change " << violationChange << " objective change " << objectiveChange << std::endl;

    return true;
}

void NeighborhoodExplorer::randomWalk(Move* mv, std::shared_ptr<State> st) {
    if (mv->moveType == FLIP) {
        unsigned var = Random::Integer(0, model->getNonFixedBinaryVariables().size() - 1);
        //        if (var < 0 || model->getIntegerVariables()->size() <= var) {
        //            std::cout << "var " << var << std::endl;
        //            std::cout << model->getIntegerVariables()->size() << std::endl;
        //        }
        assert(var < model->getNonFixedBinaryVariables().size());
        mv->first = model->getNonFixedBinaryVariable(var);
        mv->flip();
        commitMove(mv, st);
    } else {

    }
}

//bool NeighborhoodExplorer::firstImprovement(Move* mv, Model * st) {
//
//}

//template<typename returnType>


/// Not using priority of constraints yet

std::vector<int> NeighborhoodExplorer::calculateDeltaChange(Move* mv) {
    std::vector<int> change(model->getConstraints().size());

    if (mv->moveType == FLIP) {
        IntegerVariable* variable = mv->first;
        updateVector update = model->getUpdate(variable); // FIX ME !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //        int violationChange = 0;
        //        int objectiveChange = 0;
        //    std::cout << "Variable " << variableNumber << std::endl;
        for(updateType invar : update){
//        for (unsigned i = 0; i < update.size(); i++) {
            //            std::cout << i << std::endl;
//            std::shared_ptr<Invariant> invar = update.at(i);
            //            Invariant* invar = st->getInvariants()->at(updateVector->at(i));
            invar->addChange(variable->getID(), mv->deltaValueFirst);
            invar->calculateDeltaValue();
            if (invar->isUsedByConstraint()) {
                //            if (invar->getPriority() == 0) {
                //                change[0] += model->getObjectives()->at(invar->getUsedInObjective())->setDeltaViolationDegree();

                std::shared_ptr<Constraint> cons = invar->getConstraint(); //model->getConstraintsWithPriority(priority)->at(invar->getConstraintNumber());

                //            } else {
                int priority = invar->getPriority();
                if (priority == 0) {
                    change[priority] += cons->setDeltaViolationDegree();

                } else if (priority > 0) {
                    change[priority] += cons->setDeltaViolation();
                }
                //                violationChange += st->getHardConstraints()->at(invar->getUsedInConstraint())->setDeltaViolation();
            }
        }
        //    std::cout << std::endl;
        //        std::pair<int, int> change(violationChange, objectiveChange);
        return change;
    } else if (mv->moveType == SWAP) {
        std::cout << "Supposed to do swap" << std::endl;
        //        std::pair<int, int> change(0, 0);
        //        return change;
    } else {
        assert(mv->moveType == VALUECHANGE);
        std::cout << "Supposed to do value change of 1 to 3 variables" << std::endl;
        //        std::pair<int, int> change(0, 0);
        //        return change;
    }
    //    return std::pair<int,int> change = new pair<int,int>(violationChange,objectiveChange);
    //        variable->setCurrentValue(newValue);
    std::cout << "Should never reach this" << std::endl;
    exit(1);
    return change;
}

void NeighborhoodExplorer::commitMove(Move* mv, std::shared_ptr<State> st) {
    if (mv->moveType == FLIP) {
        IntegerVariable* var = mv->first;

        // Skal genberegne!!!!!
        calculateDeltaChange(mv);
        updateVector update = model->getUpdate(var); // FIX ME !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        for (updateType invar : update) {
            //        for (unsigned i = 0; i < update->size(); i++) {

            //            std::shared_ptr<Invariant> invar = update->at(i);
            //            Invariant* invar = st->getInvariants()->at(update->at(i));
            invar->updateValue();
            if (invar->isUsedByConstraint()) {
                //            if (invar->getPriority() > 0) {
                std::shared_ptr<Constraint> cons = invar->getConstraint(); // model->getConstraintsWithPriority(invar->getPriority())->at(invar->getConstraintNumber());
                if (cons->getPriority() > 0) {
                    st->numberOfViolations += cons->updateViolation(); // FIX ME !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                }
                if (invar->getPriority() == 0) {
                    cons->updateViolationDegree();
                    //                            model->getObjectives()->at(invar->getUsedInObjective())->updateViolationDegree();
                }
            }
        }
        var->setCurrentValue(1 - var->getCurrentValue());
    } else if (mv->moveType == SWAP) {
        std::cout << "Supposed to do swap" << std::endl;
    } else {
        assert(mv->moveType == VALUECHANGE);
        std::cout << "Supposed to do value change of 1 to 3 variables" << std::endl;
    }
}

void NeighborhoodExplorer::makeMove(Move* mv, std::shared_ptr<State> st) {
    commitMove(mv, st);
}