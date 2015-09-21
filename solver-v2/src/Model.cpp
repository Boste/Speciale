
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

std::list<IntegerVariable*>& Model::getIntegerVariables(){
    return IntegerVariables;
}
//IntegerVariable* Model::addIntegerVariable(int lb, int ub) {

void Model::addBinaryVariable(int lb, int ub) {
    //    for (int i = 0; i < numberOfVariables; i++) {
    int id = original.size();
    IntegerVariable* v = new IntegerVariable(lb, ub, id);
    original.push_back(v);

}

void Model::addIntegerVariable(int lb, int ub) {
    int id = original.size();
    IntegerVariable* v = new IntegerVariable(lb, ub, id);
    original.push_back(v);
    IntegerVariables.push_back(v);
    
}
//int Model::getNumberOfVariables(){
//    return numberOfVariables;
//}

void Model::nonFixedVariables(std::vector<IntegerVariable*>* nonFixed) {
    nonFixedBinaryVariables = *nonFixed;
    //    std::cout << &IntVarVector << " vs " << nonFixed << std::endl;
}
//void Model::addInvariantToIntVariable(int variableNumber, int invariantNumber) {
//    IntVarVector->at(variableNumber)->addToUpdate(invariantNumber);
//}
//
//void Model::initializeInvariants() {
//
//
//    mask->resize(getNumberOfVariables());
//    for (int i = 0; i < getNumberOfVariables(); i++) {
//        mask->at(i) = i;
//    }
//    shuffleMask();
//    //    std::random_shuffle(mask->begin(), mask->end());
//    solution->resize(getNumberOfVariables());
//    //    for (unsigned i = 0; i < IntVarVector->size(); i++) {
//    for (IntegerVariable* current : *IntVarVector) {
//        //        IntegerVariable* current = IntVarVector->at(i);
//        //        for (unsigned j = 0; j < current->getUpdateVector()->size(); j++) {
//        for (int invariantNumber : *current->getUpdateVector()) {
//            //            int invariantNumber = current->getUpdateVector()->at(j);
//            Invariants->at(invariantNumber)->addChange(current->getID(), current->getCurrentValue());
//        }
//    }
//    //    for (unsigned i = 0; i < Invariants->size(); i++) {
//    for (std::shared_ptr<Invariant> invar : *Invariants) {
//        invar->calculateDeltaValue();
//        invar->updateValue();
//        //        Invariants->at(i)->calculateDeltaValue();
//        //        Invariants->at(i)->updateValue();
//        //        Invariants->at(i)->test();
//    }
//}
//
//void Model::initializeConstraints() {
//
//    int violations = 0;
//    for (unsigned i = 1; i < Constraints->size(); i++) {
//        std::vector<std::shared_ptr < Constraint>>*prio = Constraints->at(i);
//        //        for (unsigned j = 0; j < prio->size(); j++) {
//        for (std::shared_ptr<Constraint> cons : *prio) {
//            violations += cons->updateViolation();
//        }
//    }
//    if (violations != 0) {
//        std::cout << "Initial solution not feasible? violations: " << violations << std::endl;
//        numberOfViolations = violations;
//        sleep(2);
//    } else {
//        assert(violations == 0);
//        numberOfViolations = violations;
//    }
//}
//
//void Model::initializeObjective() {
//    int violations = 0;
//    //    for (unsigned i = 0; i < Constraints->at(0)->size(); i++) {
//    for (std::shared_ptr < Constraint> obj : *(Constraints->at(0))) {
//
//        violations += obj->setDeltaViolationDegree();
//        obj->updateViolationDegree();
//    }
//    solutionValue = violations;
//    std::cout << "Initial solution value: " << violations << std::endl;
//    //    return violations;
//}

//void Model::shuffleMask() {
//    std::random_shuffle(mask->begin(), mask->end());
//
//}
//
//int Model::maskAt(int i) {
//    return mask->at(i);
//}
//
//int Model::getObjectiveValue() {
//    int value = 0;
//    //    for (unsigned i = 0; i < Constraints->at(0)->size(); i++) {
//    for (std::shared_ptr < Constraint> obj : *(Constraints->at(0))) {
//
//        value += obj->getViolationDegree();
//    }
//    return value;
//}

//void Model::updateIntegerVariables(Gecode::IntVarArray& gecodeVars) {
////    std::cout << "update" << std::endl;
//    for(IntegerVariable* iv : *original){
////    for (int i = 0; i < original->size(); i++) {
////        IntegerVariable* iv = original->at(i);
//        iv->setVariablePointer(gecodeVars[iv->getID()]);
//    }
//}
IntegerVariable* Model::getMaskat(int i){
    return mask.at(i);
}
std::vector<IntegerVariable*>& Model::getMask(){
    return mask;
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
InvariantContainer& Model::getOrgInvariants() {
    //std::vector<Invariant*>* Model::getInvariants() {
    return orgInvariants;
}

allConstraints& Model::getConstraints() {
    return Constraints;
}
allConstraints& Model::getOrgConstraints() {
    return orgConstraints;
}

constraintContainer& Model::getConstraintsWithPriority(int prio) {
    return Constraints.at(prio);
}
constraintContainer& Model::getOrgConstraintsWithPriority(int prio) {
    return orgConstraints.at(prio);
}

//std::vector<Constraint*>* Model::getSoftConstraints() {
//    return SoftConstraints;
//}

constraintContainer& Model::getOrgObjectives() {
    return orgConstraints.at(0);
}
constraintContainer& Model::getObjectives() {
    return Constraints.at(0);
}


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
