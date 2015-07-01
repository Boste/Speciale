//#include <c++/4.8/bits/stl_vector.h>

#include "LSSpace.hpp"
#include "Sum.hpp"
//using namespace Gecode;

LSSpace::LSSpace() {


}

std::vector<IntegerVariable*>* LSSpace::addIntVariablesToLS(Gecode::IntVarArray& vars) {
    for (int i = 0; i < vars.size(); i++) {
        IntegerVariable* v = new IntegerVariable(&vars[i], &vars, IntVarVector.size());
        IntVarVector.push_back(v);
    }
    return &IntVarVector;
}

void LSSpace::linear(std::vector<int>* coefficients, vector<IntegerVariable*>* variables, int relation, int upperbound, int type) {
    //        std::cout << variables << std::endl;
    Sum* sumInvariant = new Sum(variables, coefficients);
    //        std::cout << sumInvariant << std::endl;
    for (unsigned i = 0; i < coefficients->size(); i++) {
        addInvariantToIntVariable(variables->at(i)->getID(), Invariants.size());
    }
    Invariants.push_back(sumInvariant);
    Linear* LinearConstraint = new Linear(sumInvariant, upperbound, relation);
    if (type == 1) {
        sumInvariant->usedByObjective(ObjectiveFunction.size());
        ObjectiveFunction.push_back(LinearConstraint);
    } else {
        sumInvariant->usedByConstraint(Constraints.size());
        Constraints.push_back(LinearConstraint);
    }
}

void LSSpace::initializeInvariants() {
    for (unsigned i = 0; i < IntVarVector.size(); i++) {
        IntegerVariable* current = IntVarVector.at(i);
        for (unsigned j = 0; j < current->getUpdateVector()->size(); j++) {
            int invariantNumber = current->getUpdateVector()->at(j);
            Invariants.at(invariantNumber)->addChange(current->getID(), current->getCurrentValue());
        }
    }
    for (unsigned i = 0; i < Invariants.size(); i++) {
        Invariants.at(i)->calculateDeltaValue();
        Invariants.at(i)->updateValue();
        Invariants.at(i)->test();
    }

}

void LSSpace::initializeConstraints() {
    int violations = 0;
    for (unsigned i = 0; i < Constraints.size(); i++) {
        violations += Constraints.at(i)->updateViolation();
    }
    if (violations != 0) {
        std::cout << "Initial solution not feasible? violations: " << violations << std::endl;
        sleep(2);
    }
}

void LSSpace::initializeObjective() {
    int violations = 0;
    for (unsigned i = 0; i < ObjectiveFunction.size(); i++) {
        violations += ObjectiveFunction.at(i)->updateViolationDegree();
    }
    if (violations != 0) {
        std::cout << "Initial solution value: " << violations << std::endl;
    }
}
    
int LSSpace::bestMove() {
    int violationChange = 0;
    int objectiveChange = 0;
    int bestVariable = -1;
    for (unsigned i = 0; i < IntVarVector.size(); i++) {
        IntegerVariable* variable = IntVarVector.at(i);
        std::pair<int, int> delta = calculateDeltaValueOfVariableChange(i, 1 - variable->getCurrentValue());
        if (delta.first <= violationChange) {
            if (delta.second <= objectiveChange) {
                violationChange = delta.first;
                objectiveChange = delta.second;
                bestVariable = variable->getID();
            }
        }
    }
    if(bestVariable !=-1){
        commitDeltaOfVariable(bestVariable);
    } else {
        std::cout << "no improving move" << std::endl;
        sleep(1);
        std::exit(1);
                
    }
    std::cout << "changed variable " << bestVariable << std::endl;
    std::cout << "violation change " << violationChange << " objective change " << objectiveChange << std::endl;
    
    return bestVariable;
}

void LSSpace::commitDeltaOfVariable(int changedVariable) {
    IntegerVariable* var = IntVarVector.at(changedVariable);
    vector<int>* update = var->getUpdateVector();
    for (unsigned i = 0; i < update->size(); i++) {
        Invariant* invar = Invariants.at(update->at(i));
        invar->updateValue();
        if (invar->getUsedInConstraint() != -1) {
            Constraints.at(invar->getUsedInConstraint())->updateViolation();
        }

        if (invar->getUsedInObjective() != -1) {
            ObjectiveFunction.at(invar->getUsedInObjective())->updateViolationDegree();
        }
    }
    var->setCurrentValue(1-var->getCurrentValue());
    
}

std::pair<int, int> LSSpace::calculateDeltaValueOfVariableChange(int variableNumber, int newValue) {

    IntegerVariable* variable = IntVarVector.at(variableNumber);
    int oldValue = variable->getCurrentValue();
    vector<int>* updateVector = variable->getUpdateVector();
    int violationChange = 0;
    int objectiveChange = 0;
//    std::cout << "Variable " << variableNumber << std::endl;
    for (unsigned i = 0; i < updateVector->size(); i++) {
        Invariant* invar = Invariants.at(updateVector->at(i));
        invar->addChange(variableNumber, newValue - oldValue);
        invar->calculateDeltaValue();
        if (invar->getUsedInConstraint() != -1) {
//            std::cout << "Used in Constraint " << std::endl;
            violationChange += Constraints.at(invar->getUsedInConstraint())->setDeltaViolation();
        }
        if (invar->getUsedInObjective() != -1) {
//            std::cout << "Used in objective " << std::endl;
            objectiveChange += ObjectiveFunction.at(invar->getUsedInObjective())->setDeltaViolationDegree();
        }
    }
//    std::cout << std::endl;
    std::pair<int, int> change(violationChange, objectiveChange);
    return change;
    //    return std::pair<int,int> change = new pair<int,int>(violationChange,objectiveChange);


    //        variable->setCurrentValue(newValue);
}

void LSSpace::printCurrent() {

    if (IntVarVector.size() > 0) {
        std::cout << "Integer Variables:" << std::endl;
    }

    for (unsigned i = 0; i < IntVarVector.size(); i++) {
        std::cout << IntVarVector[i]->getCurrentValue() << " ";
        //        std::cout << IntVarVector[i].VariablePointer << " ";

    }
    std::cout << std::endl;
    if (BoolVarVector.size() > 0) {
        std::cout << "Boolean Variables:" << std::endl;
    }
    for (unsigned i = 0; i < BoolVarVector.size(); i++) {
        std::cout << BoolVarVector[i].getCurrentVal() << " ";
    }
    std::cout << std::endl;


}

//LSSpace::~LSSpace() {
//}

