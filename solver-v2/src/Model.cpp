
#include "Model.hpp"
#include "Random.hpp"

Model::Model() {
    //    std::cout << "Create model " << std::endl;
    //    IntVarVector = new std::vector<IntegerVariable*>();
    //    original = new std::vector<IntegerVariable*>();
    //    BoolVarVector = new std::vector<Gecode::BoolVar>();
    //    Invariants = new std::vector<std::shared_ptr < Invariant >> ();
    //    Invariants = new std::vector<Invariant*>();
    //    Constraints = new std::vector<std::vector < std::shared_ptr < Constraint>>*>();
    //    SoftConstraints = new std::vector<Constraint*>();
    //    ObjectiveFunction = new std::vector<Constraint*>();
    //    solution = new std::vector<int>();
    //    solutionValue = std::numeric_limits<int>::max();
    //    mask = new std::vector<int>();
}

//Model::Model(const Model& orig) {
//    this->IntVarVector = orig.IntVarVector;
//    this->original = orig.original;
//    this->BoolVarVector = orig.BoolVarVector;
//    this->Invariants = orig.Invariants;
//    this->Constraints = orig.Constraints;
//    //    this->numberOfVariables = orig.numberOfVariables;
//    //    this->SoftConstraints = orig.SoftConstraints;
//    //    this->ObjectiveFunction = orig.ObjectiveFunction;
//    //    this->solution = orig.solution;
//    //    this->solutionValue = orig.solutionValue;
//    //    this->mask = orig.mask;
//}

Model::~Model() {
    //        std::cout << "Destructing Model" << std::endl;
    //    for (IntegerVariable* iv : *IntVarVector) {
    //        delete iv;
    //    }

    for (IntegerVariable* iv : original) {
        delete iv;
    }
    //    delete original;
    //    delete IntVarVector;


    //    for (std::shared_ptr<std::vector<std::shared_ptr < Constraint>>cons : Constraints) {
    //        delete cons;
    //    }
    //    delete Constraints;
    //    delete Invariants;

    //    delete mask;
    //    delete solution;
    //    delete BoolVarVector;

}

std::list<IntegerVariable*>& Model::getIntegerVariables() {
    return IntegerVariables;
}
//IntegerVariable* Model::addIntegerVariable(int lb, int ub) {

void Model::addBinaryVariable(int lb, int ub) {
    //    for (int i = 0; i < numberOfVariables; i++) {
    int id = original.size();
    IntegerVariable* v = new IntegerVariable(lb, ub, id);
    original.push_back(v);
//    binaryVariables.push_back(v);

}

void Model::addIntegerVariable(int lb, int ub) {
    //    std::cout << lb << " " << ub << std::endl;
    //    sleep(1);
    int id = original.size();
    IntegerVariable* v = new IntegerVariable(lb, ub, id);
    original.push_back(v);
    IntegerVariables.push_back(v);

}
//int Model::getNumberOfVariables(){
//    return numberOfVariables;
//}

void Model::nonFixedVariables(std::vector<IntegerVariable*>& nonFixed) {
    nonFixedBinaryVariables = nonFixed;
    mask = nonFixed;
    shuffleMask();
    //    std::cout << &IntVarVector << " vs " << nonFixed << std::endl;
}

void Model::addInvariantToDDG(invariant invar, variableContainer& variables) {
    DDG->addInvariant(invar, variables);
}

void Model::addInvariantToDDG(invariant invar, InvariantContainer& invariants) {
    DDG->addInvariant(invar, invariants);
}

void Model::addInvariantToDDG(invariant invar, variableContainer& variables, InvariantContainer& invariants) {
        DDG->addInvariant(invar, variables, invariants);
}

void Model::addVariablesToDDG() {
    DDG->addVariables(nonFixedBinaryVariables);

}

void Model::addVariablesToDDG(variableContainer& vars) {
    DDG->addVariables(vars);
}

void Model::createPropagationQueue() {
    DDG->createPropagationQueue();
}

propagation_queue& Model::getPropagationQueue(IntegerVariable* iv) {
    assert(DDG->propagationQueueHasBeenMade());
    return DDG->getPropagationQueue(iv);

}

updateVector& Model::getUpdate(invariant invar) {
    return DDG->getUpdate(invar);
}
updateVector& Model::getUpdate(IntegerVariable* iv) {
    return DDG->getUpdate(iv);
}
void Model::startUp(){
    id = original.size();
}

IntegerVariable* Model::getMaskAt(int i) {
    return mask.at(i);
}

std::vector<IntegerVariable*>& Model::getMask() {
    return mask;
}

void Model::shuffleMask() {
    std::random_shuffle(mask.begin(), mask.end());

}

void Model::updateIntegerVariable(int index, Gecode::IntVar& variable) {
    getNonFixedBinaryVariable(index)->setVariablePointer(variable);
}

variableContainer& Model::getNonFixedBinaryVariables() {
    return nonFixedBinaryVariables;
}

IntegerVariable* Model::getNonFixedBinaryVariable(int i) {
    return nonFixedBinaryVariables.at(i);
}

variableContainer& Model::getAllVariables() {
    return original;
}

InvariantContainer& Model::getInvariants() {
    
    //std::vector<Invariant*>* Model::getInvariants() {
    return Invariants;
}

/// adds Invariant and gives it an id.

void Model::addInvariant(std::shared_ptr<Invariant> invar) {
    //    std::cout <<invar.get() << std::endl;
    //    invar->changeAdd  = true;
    invar->invariantID = id++;
    Invariants.push_back(invar);
}
//InvariantContainer& Model::getOrgInvariants() {
//    //std::vector<Invariant*>* Model::getInvariants() {
//    return orgInvariants;
//}

allConstraints& Model::getConstraints() {
    return Constraints;
}
//allConstraints& Model::getConstraints() {
//    return Constraints;
//}

constraintContainer Model::getConstraintsWithPriority(int prio) {
    return Constraints.at(prio);
}
//constraintContainer& Model::getConstraintsWithPriority(int prio) {
//    return Constraints.at(prio);
//}

//std::vector<Constraint*>* Model::getSoftConstraints() {
//    return SoftConstraints;
//}

constraintContainer Model::getObjectives() {
    return Constraints.at(0);
}
//constraintContainer& Model::getObjectives() {
//    return Constraints.at(0);
//}


//std::vector<std::shared_ptr<std::pair<int,int>>>& Model::getConstraintsWithIntegerVariables(){
//    return ConstraintsWithIntegerVariables;
//}
//void Model::saveSolution() {
//    solutionValue = getObjectiveValue();
//    for (int i = 0; i < getNumberOfVariables(); i++) {
//        solution->at(i) = IntVarVector->at(i)->getCurrentValue();
//    }
//}
//
//std::vector<int>* Model::getSolution() {
//    return solution;
//}
//
//int Model::getSolutionValue() {
//    return solutionValue;
//}
//
//void Model::setSolution() {
//
//    // setting invariants
//
//    for (unsigned i = 0; i < solution->size(); i++) {
//
//        IntegerVariable* current = IntVarVector->at(solution->at(i));
//        for (unsigned j = 0; j < current->getUpdateVector()->size(); j++) {
//            int invariantNumber = current->getUpdateVector()->at(j);
//            Invariants->at(invariantNumber)->addChange(current->getID(), solution->at(i) - current->getCurrentValue());
//        }
//    }
//    for (unsigned i = 0; i < Invariants->size(); i++) {
//        Invariants->at(i)->calculateDeltaValue();
//        Invariants->at(i)->updateValue();
//    }
//    // Setting constraints
//    int violations = 0;
//    for (unsigned i = 1; i < Constraints->size(); i++) {
//        std::vector<std::shared_ptr < Constraint>>*prio = getConstraintsWithPriority(i);
//        //        for (unsigned j = 0; j < prio->size(); j++) {
//        for (std::shared_ptr<Constraint> cons : *prio) {
//            violations += cons->updateViolation();
//        }
//    }
//    if (violations != 0) {
//        std::cout << "Final solution not feasible? violations: " << violations << std::endl;
//        sleep(2);
//    }
//    // setting objective value
//    violations = 0;
//    //    for (unsigned i = 0; i < getConstraintsWithPriority(0)->size(); i++) {
//    for (std::shared_ptr<Constraint> obj : *Constraints->at(0)) {
//        violations += obj->updateViolationDegree();
//        //        violations += getConstraintsWithPriority(0)->at(i)->updateViolationDegree();
//    }
//    std::cout << "Final solution " << violations << " (" << solutionValue << ")" << std::endl;
//    solutionValue = violations;
//
//}
//
//bool Model::recalculateAll() {
//    //    std::cout << Invariants->at(0)->getUsedInConstraint() << std::endl;
//    //    std::cout << "lhs " << Invariants->at(0)->getCurrentValue() << std::endl;
//    //    for(int i = 0; i < numberOfVariables; i++ ){
//    //        std::cout << IntVarVector->at(i)->getCurrentValue() << " ";
//    //    }
//    //    std::cout << std::endl;
//    bool success = true;
//    //    for (unsigned i = 0; i < Invariants->size(); i++) {
//    for (std::shared_ptr<Invariant> invar : *Invariants) {
//        //        std::shared_ptr<Invariant> invar = Invariants->at(i);
//        //        Invariant* invar = Invariants->at(i);
//        if (!invar->test()) {
//            success = false;
//        }
//    }
//    for (unsigned i = 1; i < Constraints->size(); i++) {
//        std::vector<std::shared_ptr < Constraint>>*prio = getConstraintsWithPriority(i);
//        for (unsigned j = 0; j < prio->size(); j++) {
//            std::shared_ptr<Constraint> cons = prio->at(i);
//
//            if (!cons->testCons()) {
//                success = false;
//            }
//        }
//    }
//    //    for (unsigned i = 0; i < getConstraintsWithPriority(0)->size(); i++) {
//    for (std::shared_ptr<Constraint> obj : *getConstraintsWithPriority(0)) {
//
//        //        std::shared_ptr<Constraint> obj = getConstraintsWithPriority(0)->at(i);
//        if (!obj->testObj()) {
//            success = false;
//        }
//    }
//    return success;
//}
