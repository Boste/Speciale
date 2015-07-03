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

void State::initializeConstraints() {
    int violations = 0;
    for (unsigned i = 0; i < Constraints.size(); i++) {
        violations += Constraints.at(i)->updateViolation();
    }
    if (violations != 0) {
        std::cout << "Initial solution not feasible? violations: " << violations << std::endl;
        sleep(2);
    }
}

void State::initializeObjective() {
    int violations = 0;
    for (unsigned i = 0; i < ObjectiveFunction.size(); i++) {
        violations += ObjectiveFunction.at(i)->updateViolationDegree();
    }
    if (violations != 0) {
        std::cout << "Initial solution value: " << violations << std::endl;
    }
}
    std::vector<IntegerVariable*>* State::getIntegerVariables(){
        return &IntVarVector;
    }
    std::vector<Invariant*>* State::getInvariants(){
        return &Invariants;
    }
    std::vector<Constraint*>* State::getConstraints(){
        return &Constraints;
    }
    std::vector<Constraint*>* State::getObjectives(){
        return &ObjectiveFunction;
    }