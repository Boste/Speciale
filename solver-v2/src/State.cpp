
#include "State.hpp"

State::State() {
}

State::State(const State& orig) {
}

State::~State() {
}

std::vector<IntegerVariable*>* State::addIntegerVariable(Gecode::IntVarArray* vars) {
    for (int i = 0; i < vars->size(); i++) {
        IntegerVariable* v = new IntegerVariable(&((*vars)[i]), vars, IntVarVector.size());
        IntVarVector.push_back(v);
    }
    return &IntVarVector;
}

//void State::addInvariantToIntVariable(int variableNumber, int invariantNumber) {
//    IntVarVector.at(variableNumber)->addToUpdate(invariantNumber);
//}

void State::initializeInvariants() {

    numberOfVariables = IntVarVector.size();
    solution.resize(numberOfVariables);
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
        //        Invariants.at(i)->test();
    }
}

void State::initializeConstraints() {

    int violations = 0;
    for (unsigned i = 0; i < Constraints.size(); i++) {
        violations += Constraints.at(i)->updateViolation();
    }
    if (violations != 0) {
        std::cout << "Initial solution not feasible? violations: " << violations << std::endl;
        numberOfViolations = violations;
        sleep(2);
    } else {
        assert(violations ==0);
        numberOfViolations = violations;
    }
}

void State::initializeObjective() {
    int violations = 0;
    for (unsigned i = 0; i < ObjectiveFunction.size(); i++) {
        violations += ObjectiveFunction.at(i)->setDeltaViolationDegree();
        ObjectiveFunction.at(i)->updateViolationDegree();
    }
    std::cout << "Initial solution value: " << violations << std::endl;

}

int State::getObjectiveValue() {
    int value = 0;
    for (unsigned i = 0; i < ObjectiveFunction.size(); i++) {
        value += ObjectiveFunction.at(i)->getViolationDegree();
    }
    return value;
}

std::vector<IntegerVariable*>* State::getIntegerVariables() {
    return &IntVarVector;
}

std::vector<Invariant*>* State::getInvariants() {
    return &Invariants;
}

std::vector<Constraint*>* State::getConstraints() {
    return &Constraints;
}

std::vector<Constraint*>* State::getObjectives() {
    return &ObjectiveFunction;
}

IntegerVariable* State::getIntegerVariable(int i) {
    return IntVarVector[i];
}

void State::saveSolution() {
    solutionValue = getObjectiveValue();
    for (int i = 0; i < numberOfVariables; i++) {
        solution.at(i) = IntVarVector.at(i)->getCurrentValue();
    }
}

std::vector<int>* State::getSolution() {
    return &solution;
}

int State::getSolutionValue() {
    return solutionValue;
}

void State::setSolution() {

    // setting invariants

    for (unsigned i = 0; i < solution.size(); i++) {

        IntegerVariable* current = IntVarVector.at(solution.at(i));
        for (unsigned j = 0; j < current->getUpdateVector()->size(); j++) {
            int invariantNumber = current->getUpdateVector()->at(j);
            Invariants.at(invariantNumber)->addChange(current->getID(), solution.at(i) - current->getCurrentValue());
        }
    }
    for (unsigned i = 0; i < Invariants.size(); i++) {
        Invariants.at(i)->calculateDeltaValue();
        Invariants.at(i)->updateValue();
    }
    // Setting constraints
    int violations = 0;
    for (unsigned i = 0; i < Constraints.size(); i++) {
        violations += Constraints.at(i)->updateViolation();
    }
    if (violations != 0) {
        std::cout << "Final solution not feasible? violations: " << violations << std::endl;
        sleep(2);
    }
    // setting objective value
    violations = 0;
    for (unsigned i = 0; i < ObjectiveFunction.size(); i++) {
        violations += ObjectiveFunction.at(i)->updateViolationDegree();
    }
    std::cout << "Final solution " << violations << " (" << solutionValue << ")" << std::endl;
    solutionValue = violations;

}

bool State::recalculateAll() {
//    std::cout << Invariants.at(0)->getUsedInConstraint() << std::endl;
//    std::cout << "lhs " << Invariants.at(0)->getCurrentValue() << std::endl;
//    for(int i = 0; i < numberOfVariables; i++ ){
//        std::cout << IntVarVector.at(i)->getCurrentValue() << " ";
//    }
//    std::cout << std::endl;
    bool success = true;
    for (int i = 0; i < Invariants.size(); i++) {
        Invariant* invar = Invariants.at(i);
        if (!invar->test()) {
            success = false;
        }
    }
    for (int i = 0; i < Constraints.size(); i++) {
        Constraint* cons = Constraints.at(i);
        if (!cons->testCons()) {
            success = false;
        }
    }
    for (int i = 0; i < ObjectiveFunction.size(); i++) {
        Constraint* obj = ObjectiveFunction.at(i);
        if(!obj->testObj()){
            success = false;
        }
    }
    return success;
}
