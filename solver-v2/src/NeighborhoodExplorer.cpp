
#include "NeighborhoodExplorer.hpp"

NeighborhoodExplorer::NeighborhoodExplorer() {
    
}

NeighborhoodExplorer::NeighborhoodExplorer(const NeighborhoodExplorer& orig) {
}

NeighborhoodExplorer::~NeighborhoodExplorer() {
//    std::cout << "Destructing NeighborhoodExplorer" << std::endl;
}

//template<typename returnType>

bool NeighborhoodExplorer::bestImprovement(Move* mv, State* st) {
    st->shuffleMask();
    Move* bestMove = new Move();
    std::pair<int, int> delta = calculateDeltaChange(mv, st);
    int violationChange = delta.first;
    int objectiveChange = delta.second;
    bestMove->copy(mv);
    for (unsigned i = 0; i < st->getIntegerVariables()->size(); i++) {
        IntegerVariable* variable = st->getIntegerVariables()->at(st->maskAt(i));
        mv->first = variable;
        mv->deltaValueFirst = 1 - variable->getCurrentValue() - variable->getCurrentValue();
        std::pair<int, int> delta = calculateDeltaChange(mv, st);
        if (delta.first <= violationChange) {
            if (delta.second <= objectiveChange) {
                violationChange = delta.first;
                objectiveChange = delta.second;
                bestMove->copy(mv);
            }
        }
    }
    if (violationChange <= 0 && objectiveChange < 0) {
        commitMove(bestMove, st);
    } else {
        delete bestMove;
//        std::cout << "no improving move" << std::endl;
        return false;
        //        sleep(1);
        //        std::exit(1);

    }
    delete bestMove;
//    std::cout << "changed variable " << bestMove->first->getID() << std::endl;
//    std::cout << "violation change " << violationChange << " objective change " << objectiveChange << std::endl;

    return true;
}

void NeighborhoodExplorer::randomWalk(Move* mv, State* st) {
    if (mv->moveType == FLIP) {
        int var = Random::Integer(0,st->getNumberOfVariables()-1);
        if(var < 0 || st->getNumberOfVariables() <= var){
            std::cout << "var " << var << std::endl;
            std::cout << st->getNumberOfVariables() << std::endl;
        }
        assert(var < st->getNumberOfVariables() && 0 <= var);
        mv->first = st->getIntegerVariable(var);
        mv->flip();
        commitMove(mv,st);
    } else {

    }
}

//bool NeighborhoodExplorer::firstImprovement(Move* mv, State * st) {
//
//}

//template<typename returnType>

std::pair<int, int> NeighborhoodExplorer::calculateDeltaChange(Move* mv, State * st) {
    if (mv->moveType == FLIP) {
        IntegerVariable* variable = mv->first;
        std::vector<int>* updateVector = variable->getUpdateVector();
        int violationChange = 0;
        int objectiveChange = 0;
        //    std::cout << "Variable " << variableNumber << std::endl;
        for (unsigned i = 0; i < updateVector->size(); i++) {
            Invariant* invar = st->getInvariants()->at(updateVector->at(i));
            invar->addChange(variable->getID(), mv->deltaValueFirst);
            invar->calculateDeltaValue();
            if (invar->getUsedInConstraint() != -1) {
                //            std::cout << "Used in Constraint " << std::endl;
                violationChange += st->getConstraints()->at(invar->getUsedInConstraint())->setDeltaViolation();
            }
            if (invar->getUsedInObjective() != -1) {
                //            std::cout << "Used in objective " << std::endl;
                objectiveChange += st->getObjectives()->at(invar->getUsedInObjective())->setDeltaViolationDegree();
            }
        }
        //    std::cout << std::endl;
        std::pair<int, int> change(violationChange, objectiveChange);
        return change;
    } else if (mv->moveType == SWAP) {
        std::cout << "Supposed to do swap" << std::endl;
        std::pair<int, int> change(0, 0);
        return change;
    } else {
        assert(mv->moveType == VALUECHANGE);
        std::cout << "Supposed to do value change of 1 to 3 variables" << std::endl;
        std::pair<int, int> change(0, 0);
        return change;
    }
    //    return std::pair<int,int> change = new pair<int,int>(violationChange,objectiveChange);
    //        variable->setCurrentValue(newValue);
}

void NeighborhoodExplorer::commitMove(Move* mv, State * st) {
    if (mv->moveType == FLIP) {
        IntegerVariable* var = mv->first;

        // Skal genberegne!!!!!
        calculateDeltaChange(mv, st);
        std::vector<int>* update = var->getUpdateVector();
        for (unsigned i = 0; i < update->size(); i++) {
            Invariant* invar = st->getInvariants()->at(update->at(i));
            invar->updateValue();
            if (invar->getUsedInConstraint() != -1) {
                st->numberOfViolations += st->getConstraints()->at(invar->getUsedInConstraint())->updateViolation();
            }

            if (invar->getUsedInObjective() != -1) {
                st->getObjectives()->at(invar->getUsedInObjective())->updateViolationDegree();
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
void NeighborhoodExplorer::makeMove(Move* mv, State* st){
    commitMove(mv,st);
}