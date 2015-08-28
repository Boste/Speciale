
#include "State.hpp"
#include "Random.hpp"

State::State() {
    IntVarVector = new std::vector<IntegerVariable*>();
    BoolVarVector = new std::vector<Gecode::BoolVar>();
    Invariants = new std::vector<Invariant*>();
    Constraints = new std::vector<std::vector < Constraint*>*>();
    //    SoftConstraints = new std::vector<Constraint*>();
    //    ObjectiveFunction = new std::vector<Constraint*>();
    solution = new std::vector<int>();
    solutionValue = std::numeric_limits<int>::max();
    mask = new std::vector<int>();
}

State::State(const State& orig) {
    this->IntVarVector = orig.IntVarVector;
    this->BoolVarVector = orig.BoolVarVector;
    this->Invariants = orig.Invariants;
    this->Constraints = orig.Constraints;
    //    this->SoftConstraints = orig.SoftConstraints;
    //    this->ObjectiveFunction = orig.ObjectiveFunction;
    this->solution = orig.solution;
    this->solutionValue = orig.solutionValue;
    this->mask = orig.mask;
}

State::~State() {
//        std::cout << "Destructing State" << std::endl;
    for (unsigned i = IntVarVector->size(); i > 0; i--) {
        delete IntVarVector->at(i-1);
    }
//        std::cout << "Destructing IVV" << std::endl;
    delete IntVarVector;
    for (unsigned i = Invariants->size() ; i > 0; i--) {
        delete Invariants->at(i-1);
    }
    delete Invariants;
//    std::cout << "Destructing Invar" << std::endl;
    for (unsigned i = Constraints->size(); i > 0; i--) {
        std::vector<Constraint*>* cons = Constraints->at(i-1);
        for (unsigned j = cons->size(); j > 0; j--) {
            delete cons->at(j-1);
        }
        delete Constraints->at(i-1);
//        std::cout << "Destructing cons "<< i-1 << std::endl;
    }
    delete Constraints;
//    std::cout << "Destructing cons" << std::endl;
    //        delete HardConstraints;
    //        for (unsigned i = 0; i < SoftConstraints->size(); i++) {
    //            delete SoftConstraints->at(i);
    //        }
    //        delete SoftConstraints;
    //        for (unsigned i = 0; i < ObjectiveFunction->size(); i++) {
    //            delete ObjectiveFunction->at(i);
    //        }
    //        delete ObjectiveFunction;
    delete mask;
    delete solution;
    delete BoolVarVector;

}

std::vector<IntegerVariable*>* State::addIntegerVariable(int numberOfVariables, int lb, int ub) {
    for (int i = 0; i < numberOfVariables; i++) {
        int id = IntVarVector->size();
        IntegerVariable* v = new IntegerVariable(lb, ub, id);
        IntVarVector->push_back(v);
    }
    return IntVarVector;
}

//void State::addInvariantToIntVariable(int variableNumber, int invariantNumber) {
//    IntVarVector->at(variableNumber)->addToUpdate(invariantNumber);
//}

void State::initializeInvariants() {


    mask->resize(getNumberOfVariables());
    for (int i = 0; i < getNumberOfVariables(); i++) {
        mask->at(i) = i;
    }
    shuffleMask();
    //    std::random_shuffle(mask->begin(), mask->end());
    solution->resize(getNumberOfVariables());
    for (unsigned i = 0; i < IntVarVector->size(); i++) {
        IntegerVariable* current = IntVarVector->at(i);
        for (unsigned j = 0; j < current->getUpdateVector()->size(); j++) {
            int invariantNumber = current->getUpdateVector()->at(j);
            Invariants->at(invariantNumber)->addChange(current->getID(), current->getCurrentValue());
        }
    }
    for (unsigned i = 0; i < Invariants->size(); i++) {
        Invariants->at(i)->calculateDeltaValue();
        Invariants->at(i)->updateValue();
        //        Invariants->at(i)->test();
    }
}

void State::initializeConstraints() {

    int violations = 0;
        for (unsigned i = 1; i < Constraints->size(); i++) {
            std::vector<Constraint*>* prio = Constraints->at(i);
            for (unsigned j = 0; j < prio->size(); j++) {
                violations += prio->at(j)->updateViolation();
            }
        }
    if (violations != 0) {
        std::cout << "Initial solution not feasible? violations: " << violations << std::endl;
        numberOfViolations = violations;
        sleep(2);
    } else {
        assert(violations == 0);
        numberOfViolations = violations;
    }
}

void State::initializeObjective() {
    int violations = 0;
    for (unsigned i = 0; i < Constraints->at(0)->size(); i++) {
        violations += Constraints->at(0)->at(i)->setDeltaViolationDegree();
        Constraints->at(0)->at(i)->updateViolationDegree();
    }
    solutionValue = violations;
    std::cout << "Initial solution value: " << violations << std::endl;
//    return violations;
}

void State::shuffleMask() {
    std::random_shuffle(mask->begin(), mask->end());

}

int State::maskAt(int i) {
    return mask->at(i);
}

int State::getObjectiveValue() {
    int value = 0;
    for (unsigned i = 0; i < Constraints->at(0)->size(); i++) {
        value += Constraints->at(0)->at(i)->getViolationDegree();
    }
    return value;
}

void State::updateIntegerVariables(Gecode::IntVarArray& gecodeVars) {
    for (int i = 0; i < IntVarVector->size(); i++) {
        IntegerVariable* iv = IntVarVector->at(i);
        iv->setVariablePointer(&gecodeVars[i]);
    }
}

std::vector<IntegerVariable*>* State::getIntegerVariables() {
    return IntVarVector;
}

std::vector<Invariant*>* State::getInvariants() {
    return Invariants;
}

std::vector<std::vector<Constraint*>*>* State::getConstraints() {
    return Constraints;
}

std::vector<Constraint*>* State::getConstraintsWithPriority(int prio) {

    return Constraints->at(prio);
}

//std::vector<Constraint*>* State::getSoftConstraints() {
//    return SoftConstraints;
//}

std::vector<Constraint*>* State::getObjectives() {
    return Constraints->at(0);
}

IntegerVariable* State::getIntegerVariable(int i) {
    return IntVarVector->at(i);
}

void State::saveSolution() {
    solutionValue = getObjectiveValue();
    for (int i = 0; i < getNumberOfVariables(); i++) {
        solution->at(i) = IntVarVector->at(i)->getCurrentValue();
    }
}

std::vector<int>* State::getSolution() {
    return solution;
}

int State::getSolutionValue() {
    return solutionValue;
}

void State::setSolution() {

    // setting invariants

    for (unsigned i = 0; i < solution->size(); i++) {

        IntegerVariable* current = IntVarVector->at(solution->at(i));
        for (unsigned j = 0; j < current->getUpdateVector()->size(); j++) {
            int invariantNumber = current->getUpdateVector()->at(j);
            Invariants->at(invariantNumber)->addChange(current->getID(), solution->at(i) - current->getCurrentValue());
        }
    }
    for (unsigned i = 0; i < Invariants->size(); i++) {
        Invariants->at(i)->calculateDeltaValue();
        Invariants->at(i)->updateValue();
    }
    // Setting constraints
    int violations = 0;
    for (unsigned i = 1; i < Constraints->size(); i++) {
        std::vector<Constraint*>* prio = getConstraintsWithPriority(i);
        for (unsigned j = 0; j < prio->size(); j++) {

            violations += prio->at(i)->updateViolation();
        }
    }
    if (violations != 0) {
        std::cout << "Final solution not feasible? violations: " << violations << std::endl;
        sleep(2);
    }
    // setting objective value
    violations = 0;
    for (unsigned i = 0; i < getConstraintsWithPriority(0)->size(); i++) {
        violations += getConstraintsWithPriority(0)->at(i)->updateViolationDegree();
    }
    std::cout << "Final solution " << violations << " (" << solutionValue << ")" << std::endl;
    solutionValue = violations;

}

bool State::recalculateAll() {
    //    std::cout << Invariants->at(0)->getUsedInConstraint() << std::endl;
    //    std::cout << "lhs " << Invariants->at(0)->getCurrentValue() << std::endl;
    //    for(int i = 0; i < numberOfVariables; i++ ){
    //        std::cout << IntVarVector->at(i)->getCurrentValue() << " ";
    //    }
    //    std::cout << std::endl;
    bool success = true;
    for (unsigned i = 0; i < Invariants->size(); i++) {
        Invariant* invar = Invariants->at(i);
        if (!invar->test()) {
            success = false;
        }
    }
    for (unsigned i = 1; i < Constraints->size(); i++) {
        std::vector<Constraint*>* prio = getConstraintsWithPriority(i);
        for (unsigned j = 0; j < prio->size(); j++) {
            Constraint* cons = prio->at(i);

            if (!cons->testCons()) {
                success = false;
            }
        }
    }
    for (unsigned i = 0; i < getConstraintsWithPriority(0)->size(); i++) {
        Constraint* obj = getConstraintsWithPriority(0)->at(i);
        if (!obj->testObj()) {
            success = false;
        }
    }
    return success;
}
