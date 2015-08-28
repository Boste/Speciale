//#include <c++/4.8/bits/stl_vector.h>

#include "LSSpace.hpp"


//using namespace Gecode;

LSSpace::LSSpace() {
    //        std::cout << "constructed" << std::endl;

}

//std::vector<IntegerVariable*>* LSSpace::addIntVariablesToState(Gecode::IntVarArray* vars) {
//    return 
//}


//void LSSpace::addInvariantToIntVariable(int variableNumber, int invariantNumber) {
//    st->getIntegerVariables()->at(variableNumber)->addToUpdate(invariantNumber);
//
//}

//void LSSpace::linear(std::vector<int>* coefficients, vector<IntegerVariable*>* variables, int relation, int upperbound, int type) {
//    //        std::cout << variables << std::endl;
//    Sum* sumInvariant = new Sum(variables, coefficients);
//
//    //        std::cout << sumInvariant << std::endl;
//    for (unsigned i = 0; i < coefficients->size(); i++) {
//        addInvariantToIntVariable(variables->at(i)->getID(), st->getInvariants()->size());
//    }
//    st->getInvariants()->push_back(sumInvariant);
//    Linear* LinearConstraint = new Linear(sumInvariant, upperbound, relation);
//    if (type == HARD) {
//        sumInvariant->usedByConstraint(st->getHardConstraints()->size());
//        st->getHardConstraints()->push_back(LinearConstraint);
//
//    } 
//    
//}

void LSSpace::optimizeSolution(int time, shared_ptr<State> st) {
    NeighborhoodExplorer* NE = new NeighborhoodExplorer();
    //    std::cout << "NE created" << std::endl;
    //    std::cout << "Segmentation fault right after this " << std::endl;
    IntegerVariable* var = st->getIntegerVariable(0);
    //    std::cout << "segmentation fault before this" << std::endl;
    Move* mv = new Move(var, 1 - var->getCurrentValue() - var->getCurrentValue(), FLIP);
    //        std::cout << __LINE__ << std::endl;
    //    NE.bestImprovement<int>()
    //    NE->randomWalk(mv,st);

    //    double timelimit = 10;
    double timelimit = (double) time;
    double usedTime = 0;
    std::clock_t start = std::clock();
    int randomMoves = st->getNumberOfVariables() / 5;
    //    std::cout << "Number of random moves " << randomMoves << std::endl;
    //    std::cout << "Timelimit " << timelimit << std::endl;
//        std::cout << "optimize" << std::endl;

    mv->first = st->getIntegerVariable(0);
//        std::cout << "optimize" << std::endl;

    mv->deltaValueFirst = 1 - mv->first->getCurrentValue() - mv->first->getCurrentValue();
    while (NE->bestImprovement(mv, st)) {
//        std::cout << __LINE__ << std::endl;


        iterations++;
        //        if (!st->recalculateAll()) {
        //            std::cout << "Line " << __LINE__ << std::endl;
        //            sleep(5);
        //        }
    }
//    std::cout << "in optimize" << std::endl;
    if (st->getObjectiveValue() < st->getSolutionValue() && st->numberOfViolations == 0) {
        st->saveSolution();
        std::cout << "improved solution value to: " << st->getSolutionValue() << " after " << iterations << " iterations" << std::endl;
    }
    //    if (!st->recalculateAll()) {
    //        std::cout << "Line " << __LINE__ << std::endl;
    //        sleep(5);
    //    }
    while (usedTime < timelimit) {
        for (int i = 0; i < randomMoves; i++) {
            NE->randomWalk(mv, st);
            iterations++;
            //            if (!st->recalculateAll()) {
            //                std::cout << "Line " << __LINE__ << std::endl;
            //                sleep(5);
            //            }
        }
        //        std::cout << "objective value after random moves " << st->getObjectiveValue() << std::endl;
        mv->first = st->getIntegerVariable(0);
        mv->deltaValueFirst = 1 - mv->first->getCurrentValue() - mv->first->getCurrentValue();
        while (NE->bestImprovement(mv, st)) {
            iterations++;
            //            if (!st->recalculateAll()) {
            //                std::cout << "Line " << __LINE__ << std::endl;
            //                sleep(5);
            //            }
        }
        //        std::cout << iterations << std::endl;
        if (st->getObjectiveValue() < st->getSolutionValue() && st->numberOfViolations == 0) {
            st->saveSolution();
            std::cout << "improved solution value to: " << st->getSolutionValue() << " after " << iterations << " iterations" << std::endl;
            //            st->recalculateAll();
        }
        //        if (!st->recalculateAll()) {
        //            std::cout << "Line " << __LINE__ << std::endl;
        //            sleep(5);
        //        }

        usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    }
    std::cout << "Time used " << usedTime << std::endl;


    st->recalculateAll();
    std::vector<int>* sol = st->getSolution();
    for (unsigned i = 0; i < sol->size(); i++) {
        IntegerVariable* var = st->getIntegerVariable(i);
        mv->first = var;
        mv->deltaValueFirst = sol->at(i) - var->getCurrentValue();
        NE->makeMove(mv, st);

    }


    std::cout << "Solution value: " << st->getObjectiveValue() << std::endl;
    std::cout << "Number of moves " << iterations << std::endl;
    delete mv;
    delete NE;
    std::cout << "O " << st->getObjectiveValue() << " ";
    //    st->setSolution();
    //
    //    std::cout << "Solution value: " << st->getObjectiveValue() << std::endl;
    //    std::cout << "Number of moves " << iterations << std::endl;
}


//    FlipMove mv = FlipMove(st->getIntegerVariables()->at(0));
//    Move bestMove = mv;
//    std::pair<int, int> delta = calculateDeltaValueOfMove(mv);
//    int violationChange = delta.first;
//    int objectiveChange = delta.second;
//bool LSSpace::bestImprovement() {
//    int violationChange = 0;
//    int objectiveChange = 0;
//    int bestVariable = -1;
//    for (unsigned i = 0; i < st->getIntegerVariables()->size(); i++) {
//        IntegerVariable* variable = st->getIntegerVariables()->at(i);
//        std::pair<int, int> delta = calculateDeltaValueOfVariableChange(i, 1 - variable->getCurrentValue());
//        if (delta.first <= violationChange) {
//            if (delta.second <= objectiveChange) {
//                violationChange = delta.first;
//                objectiveChange = delta.second;
//                bestVariable = variable->getID();
//            }
//        }
//    }
//    if (bestVariable != -1 && objectiveChange !=0) {
//        commitDeltaOfVariable(bestVariable);
//    } else {
//        std::cout << "no improving move" << std::endl;
//        return false;
////        sleep(1);
////        std::exit(1);
//
//    }
//    std::cout << "changed variable " << bestVariable << std::endl;
//    std::cout << "violation change " << violationChange << " objective change " << objectiveChange << std::endl;
//
//    return true;
//}

//void LSSpace::commitDeltaOfVariable(int changedVariable) {
//    IntegerVariable* var = st->getIntegerVariables()->at(changedVariable);
//    vector<int>* update = var->getUpdateVector();
//    for (unsigned i = 0; i < update->size(); i++) {
//        Invariant* invar = st->getInvariants()->at(update->at(i));
//        invar->updateValue();
//        if (invar->getUsedInConstraint() != -1) {
//            st->getConstraints()->at(invar->getUsedInConstraint())->updateViolation();
//        }
//
//        if (invar->getUsedInObjective() != -1) {
//            st->getObjectives()->at(invar->getUsedInObjective())->updateViolationDegree();
//        }
//    }
//    var->setCurrentValue(1 - var->getCurrentValue());
//
//}

//std::pair<int, int> LSSpace::calculateDeltaValueOfVariableChange(int variableNumber, int newValue) {
//
//    IntegerVariable* variable = st->getIntegerVariables()->at(variableNumber);
//    int oldValue = variable->getCurrentValue();
//    vector<int>* updateVector = variable->getUpdateVector();
//    int violationChange = 0;
//    int objectiveChange = 0;
//    //    std::cout << "Variable " << variableNumber << std::endl;
//    for (unsigned i = 0; i < updateVector->size(); i++) {
//        Invariant* invar = st->getInvariants()->at(updateVector->at(i));
//        invar->addChange(variableNumber, newValue - oldValue);
//        invar->calculateDeltaValue();
//        if (invar->getUsedInConstraint() != -1) {
//            //            std::cout << "Used in Constraint " << std::endl;
//            violationChange += st->getConstraints()->at(invar->getUsedInConstraint())->setDeltaViolation();
//        }
//        if (invar->getUsedInObjective() != -1) {
//            //            std::cout << "Used in objective " << std::endl;
//            objectiveChange += st->getObjectives()->at(invar->getUsedInObjective())->setDeltaViolationDegree();
//        }
//    }
//    //    std::cout << std::endl;
//    std::pair<int, int> change(violationChange, objectiveChange);
//    return change;
//    //    return std::pair<int,int> change = new pair<int,int>(violationChange,objectiveChange);
//    //        variable->setCurrentValue(newValue);
//}

//void LSSpace::initializeInvariants(shared_ptr<State> st) {
//    st->initializeInvariants();
//}
//
//void LSSpace::initializeConstraints(shared_ptr<State> st) {
//    st->initializeConstraints();
//}
//
//void LSSpace::initializeObjective(shared_ptr<State> st) {
//    initialValue = st->initializeObjective();
//}
//int LSSpace::getObjectiveValue(){
//    return st->getObjectiveValue();
//}

//void LSSpace::printCurrent() {
//
//    if (st->getIntegerVariables()->size() > 0) {
//        std::cout << "Integer Variables:" << std::endl;
//    }
//
//    for (unsigned i = 0; i < st->getIntegerVariables()->size(); i++) {
//        std::cout << st->getIntegerVariables()->at(i)->getCurrentValue() << " ";
//        //        std::cout << IntVarVector[i].VariablePointer << " ";
//
//    }
//    std::cout << std::endl;
//    if (BoolVarVector.size() > 0) {
//        std::cout << "Boolean Variables:" << std::endl;
//    }
//    for (unsigned i = 0; i < BoolVarVector.size(); i++) {
////        std::cout << BoolVarVector[i].getCurrentVal() << " ";
//    }
//    std::cout << std::endl;
//
//
//}

//LSSpace::~LSSpace() {
//}

