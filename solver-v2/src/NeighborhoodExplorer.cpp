
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


/// make something to break ties 

bool NeighborhoodExplorer::bestImprovement(Move* mv, std::shared_ptr<State> st) {
    //    model->shuffleMask();
    Move* bestMove = new Move();
    //    mv->first = model->getNonFixedBinaryVariable(Random::Integer(0, model->getNonFixedBinaryVariables().size() - 1));
    //    mv->flip();
    //    calculateDeltaChange(mv);
    //    int violationChange = delta.first;
    //    int objectiveChange = delta[0];
    bestMove->setDeltaChangeSize(model->getConstraints().size());
    //    bestMove->copy(mv);
    bool improvement = false;
    //    std::cout <<model->getMask().size() << std::endl;
    for (IntegerVariable* iv : model->getMask()) {
        //    for (unsigned i = 0; i < model->getNonFixedBinaryVariables().size(); i++) {
        //        for(IntegerVariable* iv : *model->getIntegerVariables()){
        //        IntegerVariable* iv = model->getMaskAt(i);
        assert(!iv->isDef() && !iv->isFixed());
        mv->first = iv;
        //        mv->deltaValueFirst = 1 - iv->getCurrentValue() - iv->getCurrentValue();
        mv->flip();
        calculateDeltaChange(mv);

        bool fewerViolations = false;
        bool moreViolations = false;
        //        for (int i : mv->getDeltaChanges()) {
        //            std::cout << i << " ";
        //        }
        //        std::cout << std::endl;


        for (unsigned j = 1; j < mv->getDeltaVector().size(); j++) { // No ties in favor of newest move

            if (mv->getDeltaVector()[j] < bestMove->getDeltaVector()[j]) {
                //                std::cout << "prob never here" << std::endl;
                //                debug;
                //                std::cout << "fewer violations" << std::endl;
                fewerViolations = true;
                break;
            } else if (mv->getDeltaVector()[j] > bestMove->getDeltaVector()[j]) {
                moreViolations = true;
                //                std::cout << "more violations" << std::endl;
                break;
            }
        }
        if (!moreViolations) {
            if (fewerViolations || mv->getDeltaVector()[0] <= bestMove->getDeltaVector()[0]) {
                //                for (int i : delta) {
                //                    std::cout << i << " ";
                //                }
                //                std::cout << std::endl;
                //                bestDelta = delta;
                //                std::cout << "got new best move " << std::endl;
                //            violationChange = delta.first;
                //            objectiveChange = delta.second;
                improvement = true;
                bestMove->copy(mv);
            }
        }
    }
    if (improvement) {
        //    std::cout << "commit delta" << std::endl;
        //        for (int i : bestMove->getDeltaChanges()) {
        //            std::cout << i << " ";
        //        }
        //        std::cout << std::endl;
        //        std::cout << "this was one best iter " << std::endl;

        unsigned changeAt = 0;
        std::vector<int> bestDelta = bestMove->getDeltaVector();
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
                //            std::cout << "commit move" << std::endl;
                commitMove(bestMove, st);
                //                std::cout << "obj val " << st->getObjectiveValue() << std::endl;
                return true;
            }
        } else if (bestDelta[0] < 0) {
            commitMove(bestMove, st);
            //            std::cout << "obj val " << st->getObjectiveValue() << std::endl;
            return true;

            //        std::cout << "commit move" << std::endl;
        }
    }

    //    return true;
    //    if (violationChange <= 0 && objectiveChange < 0) {
    //       commitMove(bestMove, st);
    //    } else {
    //    delete bestMove;
    //        std::cout << "no improving move" << std::endl;
    //        return false;
    //        std::exit(1);


    delete bestMove;
    return false;
    //    std::cout << "changed variable " << bestMove->first->getID() << std::endl;
    //    std::cout << "violation change " << violationChange << " objective change " << objectiveChange << std::endl;
    //    return true;
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
        std::cout << "Only making flip moves" << std::endl;
    }
}

//bool NeighborhoodExplorer::firstImprovement(Move* mv, Model * st) {
//
//}

//template<typename returnType>


/// Not using priority of constraints yet

void NeighborhoodExplorer::calculateDeltaChange(Move* mv) {
    std::vector<int>& change = mv->getDeltaVector();
    for (unsigned i = 0; i < change.size(); i++) {
        change[i] = 0;
    }
    //    std::cout << "size of change " << change.size() << std::endl;
    if (mv->moveType == FLIP) {
        IntegerVariable* variable = mv->first;
        propagation_queue queue = model->getPropagationQueue(variable);
        updateVector update = model->getUpdate(variable);
        //        int violationChange = 0;
        //        int objectiveChange = 0;
        //    std::cout << "Variable " << variableNumber << std::endl;
        //        std::cout << update.size() << std::endl;
        //        std::cout << queue.size() << std::endl;
        //        std::cout << "Sizes of update and queue " << std::endl;
        for (updateType invar : update) {
            invar->addChange(variable->getID(), mv->deltaValueFirst);
        }
        for (updateType invar : queue) {
            //        for (unsigned i = 0; i < update.size(); i++) {
            //            std::cout << i << std::endl;
            //            std::shared_ptr<Invariant> invar = update.at(i);
            //            Invariant* invar = st->getInvariants()->at(updateVector->at(i));
            //            invar->addChange(variable->getID(), mv->deltaValueFirst);
            //            if (invar->getID() == 253122) {
            //                std::cout << "should change objective" << std::endl;
            //            }
            invar->calculateDeltaValue();
            //            std::cout << "Is used by constraint " << invar->isUsedByConstraint() << " delta value " << invar->getDeltaValue() << std::endl;
            if (invar->isUsedByConstraint()) {
                //            if (invar->getPriority() == 0) {
                //                change[0] += model->getObjectives()->at(invar->getUsedInObjective())->setDeltaViolationDegree();

                std::shared_ptr<Constraint> cons = invar->getConstraint(); //model->getConstraintsWithPriority(priority)->at(invar->getConstraintNumber());

                //            } else {
                int priority = invar->getPriority();
                //                    change[priority] += cons->setDeltaViolationDegree();
                if (invar->getPriority() == 0) {
                    //                    std::cout << "change in obj " << invar->getDeltaValue() << std::endl;
                    change[0] += invar->getDeltaValue();
                } else {
                    change[priority] += cons->setDeltaViolation();
                    //                    std::cout << "change in violation " << cons->getDeltaViolation() << std::endl;

                }
                //                for(int i : change){
                //                    std::cout << i << " ";
                //                }
                //                std::cout << std::endl;
                //                violationChange += st->getHardConstraints()->at(invar->getUsedInConstraint())->setDeltaViolation();


            }
        }
        //        for (int i : mv->getDeltaChanges()) {
        //            std::cout << i << " ";
        //        }
        //        std::cout << std::endl;
        //        std::cout << "in delta" << std::endl;
        //    std::cout << std::endl;
        //        std::pair<int, int> change(violationChange, objectiveChange);
        //        return change;
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
    //    std::cout << "Should never reach this" << std::endl;
    //    exit(1);
    //    return change;    
}

void NeighborhoodExplorer::commitMove(Move* mv, std::shared_ptr<State> st) {
    if (mv->moveType == FLIP) {
        IntegerVariable* var = mv->first;
        std::vector<int> evaluation(mv->getDeltaVector().size());
        // Skal genberegne!!!!!
        calculateDeltaChange(mv);
        propagation_queue queue = model->getPropagationQueue(var);
        for (updateType invar : queue) {
            //        for (unsigned i = 0; i < update->size(); i++) {

            //            std::shared_ptr<Invariant> invar = update->at(i);
            //            Invariant* invar = st->getInvariants()->at(update->at(i));
            invar->updateValue();

            if (invar->isUsedByConstraint()) {

                if (invar->getPriority() > 0) {
                        //            if (invar->getPriority() > 0) {
                        std::shared_ptr<Constraint> cons = invar->getConstraint(); // model->getConstraintsWithPriority(invar->getPriority())->at(invar->getConstraintNumber());
                        evaluation.at(cons->getPriority()) += cons->updateViolation(); 


                } else {
                    evaluation.at(0) += invar->getDeltaValue();
                }
            }
        }
        st->updateEvaluation(evaluation);
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