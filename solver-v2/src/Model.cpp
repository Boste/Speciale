
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

std::shared_ptr<DependencyDigraph>& Model::getDDG() {
    return DDG;
}

propagation_queue& Model::getPropagationQueue(IntegerVariable* iv) {
    assert(DDG->propagationQueueHasBeenMade());
    return DDG->getPropagationQueue(iv);

}

updateVector& Model::getUpdate(invariant invar) {
    return DDG->getInvariantUpdate(invar->getID());
}

updateVector& Model::getUpdate(IntegerVariable* iv) {
    return DDG->getVariableUpdate(iv->getID());
}

void Model::startUp() {
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

IntegerVariable* Model::getVariable(unsigned id) {
    return original.at(id);
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
//    std::cout << "setting id " << id << std::endl;
//    sleep(1);
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

InvariantContainer& Model::getObjectiveInvariant() {
    return objectiveInvariant;
}

void Model::addToObjectiveInvariant(invariant invar) {
    objectiveInvariant.push_back(invar);
}

std::vector<int>& Model::getInitialEvaluation() {
    return initialEvaluation;
}

void Model::initialize() {
    //    std::cout << "ini cons" << std::endl;
    std::vector<int> violations(Constraints.size());
    for (unsigned i = 1; i < getConstraints().size(); i++) {
        constraintContainer prio = getConstraints().at(i);
        //        for (unsigned j = 0; j < prio->size(); j++) {
        //        std::cout << i << std::endl;
        for (std::shared_ptr<Constraint> cons : *prio) {
            if (!cons->isOneway()) {
                int change = cons->updateViolation();
                if (change != 0) {
                    //                    auto vars = cons->getVariables();
                    //                    auto coeff = cons->getCoefficients();
                    //                    assert(vars.size() == coeff.size());
                    //                    
                    //                    for (unsigned j = 0; j < vars.size(); j++) {
                    //
                    //                        if (vars.at(j)->isDef()) {
                    //                            invariant invar = vars.at(j)->getOneway();
                    //                            std::cout << coeff.at(vars.at(j)->getID()) << "*" << vars.at(j)->getCurrentValue()<<"("<<invar->getCurrentValue()<<")" << " + ";
                    //                        } else {
                    //                            std::cout << coeff.at(vars.at(j)->getID()) << "*" << vars.at(j)->getCurrentValue()<<"["<<vars.at(j)->getID()<<"]" << " + ";
                    //                        }
                    //                    }
                    //                    std::cout << " = 1" << std::endl;
                    //                    sleep(1);   
                    //                    std::cout << "change " << change << " invariant id " << cons->getInvariant()->getID() << " type " << cons->getInvariant()->getType() << " " << "     ";
                    //                    if(cons->getArgument(0) == EQ){
                    //                        std::cout << cons->getInvariant()->getCurrentValue() << " = " << cons->getArgument(1) << std::endl;
                    //                    } else {
                    //                        std::cout << cons->getInvariant()->getCurrentValue() << " <= " << cons->getArgument(1) << std::endl;
                    //                    }
                }
                violations.at(cons->getPriority()) += change;
            }
        }
    }
    //    if (violations != 0) {
    std::cout << "Initial solution not feasible? violations: " << violations.at(1) << std::endl;
    initialEvaluation = violations;
    //        sleep(2);
    //    } else {
    //        assert(violations == 0);
    //        initialEvaluation = violations;
    //    }

    for (invariant invar : getObjectiveInvariant()) {
        initialEvaluation.at(0) += invar->getCurrentValue();
    }
    //    for (IntegerVariable* iv : getAllVariables()) {
    //        //        if (iv->isDef()) {
    //
    //        //            iv->setCurrentValue(iv->getOneway()->getCurrentValue());
    //        solution[iv->getID()] = iv->getCurrentValue();
    //        //        } else {
    //        //            solution.at(iv->getID()) = iv->getCurrentValue();
    //        //        }
    //
    //    }
}

//void Model::initializeObjective() {
//    for (invariant invar : getObjectiveInvariant()) {
//        solutionValue = invar->getCurrentValue();
//    }
//    for (IntegerVariable* iv : getAllVariables()) {
////        if (iv->isDef()) {
//            
////            iv->setCurrentValue(iv->getOneway()->getCurrentValue());
//            solution[iv->getID()] = iv->getCurrentValue();
////        } else {
////            solution.at(iv->getID()) = iv->getCurrentValue();
////        }
//
//    }
//
//    for (constraint con : *getConstraintsWithPriority(0).get()) {
//        if (con->isOneway()) {
//            continue;
//        }
//        auto coefficients = con->getCoefficients();
//        auto variables = con->getVariables();
//        int lhs = 0;
//        for (IntegerVariable* iv : variables) {
//            if (coefficients.at(iv->getID()) != 0) {
//                std::cout << coefficients.at(iv->getID()) << "*" << iv->getCurrentValue() << " + ";
//            }
//            lhs += iv->getCurrentValue() * coefficients.at(iv->getID());
//        }
//        std::cout << " = " << lhs << std::endl;
////        std::cout << "obj val " << con->getViolationDegree() << std::endl;
//        invariant invar = getObjectiveInvariant().at(0);
//        int invarVal = invar->getCurrentValue();
//        auto rhs = con->getArgument(1);
//        auto rel = con->getArgument(0);
//        std::string all = std::to_string(lhs) + " " + std::to_string(rel) + " " + std::to_string(rhs) + "\n" + std::to_string(invarVal);
//        //            if (invarVal != lhs) {
//        std::cout << all << std::endl;
//        std::cout << invar->getType() << " " << invar->getID() << std::endl;
//    }
//
//
//
//    int violations = 0;
//    //    for (unsigned i = 0; i < Constraints->at(0)->size(); i++) {
//    for (std::shared_ptr < Constraint> obj : *getConstraints().at(0)) {
//
//        //        violations += obj->getViolationDegree();
//        obj->updateViolationDegree();
//        violations += obj->getViolationDegree();
//    }
//    solutionValue = violations;
//    std::cout << "Initial solution value: " << violations << std::endl;
//        return violations;
//
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
