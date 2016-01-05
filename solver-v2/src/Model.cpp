
#include "Model.hpp"
//#include "Random.hpp"

Model::Model() {


}

Model::~Model() {
    //        std::cout << "Destructing Model" << std::endl;
    //    for (IntegerVariable* iv : *IntVarVector) {
    //        delete iv;
    //    }

    for (Variable* iv : original) {
        delete iv;
    }
    for (invariant invar : Invariants) {
        delete invar;
    }

}

std::vector<std::vector<Variable*>>&Model::getPriorityVectors() {
    return priorityVectorsOfVariables;
}

std::vector<Variable*>& Model::getPriorityVectorNr(unsigned i) {
    assert(i < priorityVectorsOfVariables.size());
    return priorityVectorsOfVariables.at(i);
}

std::vector<Variable*>& Model::getIntegerVariables() {
    return IntegerVariables;
}
//IntegerVariable* Model::addIntegerVariable(int lb, int ub) {

Variable* Model::addBinaryVariable(int lb, int ub) {
    //    for (int i = 0; i < numberOfVariables; i++) {
    int id = original.size();
    Variable* v = new Variable(lb, ub, id);
    original.push_back(v);
    return v;
    //    binaryVariables.push_back(v);

}

Variable* Model::addIntegerVariable(int lb, int ub) {
    //    std::cout << lb << " " << ub << std::endl;
    //    sleep(1);
    int id = original.size();
    Variable* v = new Variable(lb, ub, id);
    original.push_back(v);
    IntegerVariables.push_back(v);
    return v;

}
//int Model::getNumberOfVariables(){
//    return numberOfVariables;
//}

void Model::setNonFixedVariables(std::vector<Variable*>& nonFixed) {
    nonFixedVariables = nonFixed;
    //    mask = nonFixed;
    //    shuffleMask();
    //    std::cout << &IntVarVector << " vs " << nonFixed << std::endl;
}
void Model::addViolatedConstraint(invariant inv){
    violatedConstraints.insert(inv);
    
}
std::set<invariant,compare_invariant>& Model::getViolatedConstraints(){
    return violatedConstraints;
            
}
//void Model::addInvariantToDDG(invariant invar, variableContainer& variables) {
//    DDG->addInvariant(invar, variables);
//}
//
//void Model::addInvariantToDDG(invariant invar, InvariantContainer& invariants) {
//    DDG->addInvariant(invar, invariants);
//}
//
//void Model::addInvariantToDDG(invariant invar, variableContainer& variables, InvariantContainer& invariants) {
//    //    debug;
//    DDG->addInvariant(invar, variables, invariants);
//}
//
//void Model::addVariablesToDDG() {
//    DDG->addVariables(nonFixedVariables);
//
//}
//
//void Model::addVariablesToDDG(variableContainer& vars) {
//    DDG->addVariables(vars);
//}
//
//void Model::createPropagationQueue(variableContainer& vars) {
//    DDG->createPropagationQueue(vars);
//}

std::shared_ptr<DependencyDigraph>& Model::getDDG() {
    return DDG;
}

propagation_queue& Model::getPropagationQueue(Variable* iv) {
    assert(DDG->propagationQueueHasBeenMade());
    return DDG->getPropagationQueue(iv);

}

updateVector& Model::getUpdate(invariant invar) {
    return DDG->getInvariantUpdate(invar->getID());
}

updateVector& Model::getUpdate(Variable* iv) {
    return DDG->getVariableUpdate(iv->getID());
}

//void Model::startUp() {
//    id = original.size();
////    highestPriority = getConstraints().size();
//}

//std::vector<constraint>& Model::getFunctionalConstraints() {
//    return functionalConstraints;
//}

Variable* Model::getMaskAt(int i) {
    return mask.at(i);
}

std::vector<Variable*>& Model::getMask() {
    return mask;
}

void Model::shuffleMask() {
    std::random_shuffle(mask.begin(), mask.end());

}

void Model::updateIntegerVariable(int index, Gecode::IntVar& variable) {
    getNonFixedVariable(index)->setVariablePointer(variable);
}
/// Should only be used before propagation queue is made

variableContainer& Model::getNonFixedVariables() {
    return nonFixedVariables;
}
/// Mask is just as useful

//variableContainer& Model::getLSVariables() {
//    return LSVariables;
//}

Variable* Model::getNonFixedVariable(int i) {
    return nonFixedVariables.at(i);
}

Variable* Model::getVariable(unsigned id) {
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

void Model::addInvariant(invariant invar) {
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

//constraintContainer Model::getObjectives() {
//    return Constraints.at(0);
//}
//constraintContainer& Model::getObjectives() {
//    return Constraints.at(0);
//}

//InvariantContainer& Model::getObjectiveInvariant() {
//    return objectiveInvariant;
//}

InvariantContainer& Model::getEvaluationInvariants() {
    return evaluationInvariants;
}

invariant Model::getEvaluationInvariantNr(unsigned nr) {
    return evaluationInvariants.at(nr);
}

//void Model::addToObjectiveInvariant(invariant invar) {
//    objectiveInvariant.push_back(invar);
//}

void Model::addToEvaluationInvariants(invariant invar) {
    evaluationInvariants.push_back(invar);
}

std::vector<int>& Model::getInitialEvaluation() {
    return initialEvaluation;
}

void Model::initialize() {
    //    std::cout << "ini cons" << std::endl;
    //    std::vector<int> violations(Constraints.size());
    //    for (unsigned i = 1; i < getConstraints().size(); i++) {
    //        constraintContainer prio = getConstraints().at(i);
    //        //        for (unsigned j = 0; j < prio->size(); j++) {
    //        //        std::cout << i << std::endl;
    //        for (std::shared_ptr<Constraint> cons : *prio) {
    //            if (!cons->isOneway()) {
    //                int change = cons->updateViolation();
    //                if (change != 0) {
    //                    //                    auto vars = cons->getVariables();
    //                    //                    auto coeff = cons->getCoefficients();
    //                    //                    assert(vars.size() == coeff.size());
    //                    //                    
    //                    //                    for (unsigned j = 0; j < vars.size(); j++) {
    //                    //
    //                    //                        if (vars.at(j)->isDef()) {
    //                    //                            invariant invar = vars.at(j)->getOneway();
    //                    //                            std::cout << coeff.at(vars.at(j)->getID()) << "*" << vars.at(j)->getCurrentValue()<<"("<<invar->getCurrentValue()<<")" << " + ";
    //                    //                        } else {
    //                    //                            std::cout << coeff.at(vars.at(j)->getID()) << "*" << vars.at(j)->getCurrentValue()<<"["<<vars.at(j)->getID()<<"]" << " + ";
    //                    //                        }
    //                    //                    }
    //                    //                    std::cout << " = 1" << std::endl;
    //                    //                    sleep(1);   
    //                    //                    std::cout << "change " << change << " invariant id " << cons->getInvariant()->getID() << " type " << cons->getInvariant()->getType() << " " << "     ";
    //                    //                    if(cons->getArgument(0) == EQ){
    //                    //                        std::cout << cons->getInvariant()->getCurrentValue() << " = " << cons->getArgument(1) << std::endl;
    //                    //                    } else {
    //                    //                        std::cout << cons->getInvariant()->getCurrentValue() << " <= " << cons->getArgument(1) << std::endl;
    //                    //                    }
    //                }
    //                violations.at(cons->getPriority()) += change;
    //            }
    //        }
    //    }
    //
    //    if (violations.at(1) != 0) {
    //        std::cout << "Initial solution not feasible? violations: " << violations.at(1) << std::endl;
    //        initialEvaluation = violations;
    //        //        sleep(2);
    //    } else {
    //        assert(violations.at(1) == 0);
    //        initialEvaluation = violations;
    //    }
    //
    //    for (invariant invar : getObjectiveInvariant()) {
    //        initialEvaluation.at(0) += invar->getCurrentValue();
    //    }
    for (invariant inv : Invariants) {
        if (!inv->test()) {
            debug;
        }
    }



    for (unsigned int i = 0; i < getEvaluationInvariants().size(); i++) {
        initialEvaluation.push_back(getEvaluationInvariants().at(i)->CurrentValue);
    }
    //    for(int i : initialEvaluation){
    //        std::cout << i << std::endl;
    //    }
    for (Variable* iv : getNonFixedVariables()) {
        if (iv->isDef() || iv->isFixed() || iv->isIntegerVariable()) {
            //            std::cout << iv->isDef() << " " <<  iv->isFixed() << " "<< iv->isIntegerVariable() << std::endl;
            continue;

        } else {
            //            LSVariables.push_back(iv);
            mask.push_back(iv);
        }
    }
    shuffleMask();



    if (getIntegerVariables().size() == 0) {
        std::cout << "Do stuff for binary swapping" << std::endl;
    } else {
        containsIntegerVariables = true;
        auto start = std::clock();
        std::cout << "Do stuff for binary <-> integer swap" << std::endl;
        std::set<Variable*, Variable::compare_variable> newSet;
        for (unsigned i = 0; i < original.size(); i++) {
            inConstraintWith.push_back(newSet);
        }
        std::shared_ptr<std::vector < constraint>> newVector = std::make_shared<std::vector < constraint >> ();
        for (unsigned i = 0; i < original.size(); i++) {
            constraintsWithIntegerVarsRelated.push_back(newVector);
        }
        //        std::shared_ptr<std::vector<std::pair < IntegerVariable*, int>>> newVectorPair = std::make_shared<std::vector<std::pair < IntegerVariable*, int>>>();
        //        for (unsigned i = 0; i < original.size(); i++) {
        //            IntegerVariablesAndChange->push_back(newVectorPair);
        //            constraintsWithIntegerVarsRelated.push_back(newVector);
        //        }
        //        inConstraintWith.reserve(original.size());

        for (Variable* intvar : IntegerVariables) {
            if (!(intvar->isFixed() || intvar->isDef())) {
                for (constraint con : intvar->usedInConstraints()) {
                    if (con->getPriority() != OBJ) {
                        for (Variable* iv : con->getVariables()) {
                            if (!(iv->isFixed() || iv->isDef() || iv->isIntegerVariable())) {
                                inConstraintWith[iv->getID()].insert(intvar);
                            }
                        }
                    }
                }
            }
        }
        for (unsigned i = 1; i < Constraints.size(); i++) {
            constraintContainer cons = Constraints.at(i);
            for (constraint con : *cons) {
                //                if (con->isOneway()) {
                //                    continue;
                //                }
                if (con->getNumberOfIntegerVariables() > 0) {
                    //                    std::unordered_map<int, coefType>& coefficients = con->getCoefficients();
                    std::vector<Variable*>& intVars = con->getIntegerVariables();
                    for (Variable* iv : con->getVariables()) {
                        if (!(iv->isFixed() || iv->isDef() || iv->isIntegerVariable())) {
                            //                            std::vector<IntegerVariable*> newVector;
                            //                            inConstraintWith[iv->getID()] = newVector;
                            //                            int coef = coefficients.at(iv->getID());
                            //                            for(IntegerVariable* intvar : intVars){
                            //                                int intcoef = coefficients.at(intvar->getID());
                            //                                int change = ++ 
                            //                                std::pair<IntegerVariable*, int> intvarAndChange(intvar,) ;
                            //                                IntegerVariablesAndChange->at(iv->getID())->push_back(intvarAndChange);
                            //                            }
                            //                            constraintsWithIntegerVarsRelated.at(iv->getID())->push_back(con);
                            //                            std::cout << "Behøver ikke det efterfølgende, bare spørg con hvor mange int der skal ledes efter" << std::endl;
                            //                            std::cout << "Check om de er fixed (integer variablene) " << std::endl;
                            for (Variable* intvar : intVars) {
                                if (intVars.size() > 1) {
                                    std::cout << intVars.size() << std::endl;
                                }
                                assert(intvar->isIntegerVariable());
                                if (intvar->isFixed() || intvar->isDef()) {

                                } else {
                                    inConstraintWith[iv->getID()].insert(intvar);

                                }
                            }
                        }
                    }
                }
            }
        }
        auto usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
        std::cout << usedTime << std::endl;

                unsigned count = 0;
        std::cout << "Is this even working?" << std::endl;
        std::vector<int> differentSizes(200, 0);
        for (Variable* iv : getMask()) {
            //            if (inConstraintWith.at(iv->getID()).size() > 0) {
            differentSizes.at(inConstraintWith.at(iv->getID()).size())++;
            if (inConstraintWith.at(iv->getID()).size() != 0) {
                if (inConstraintWith.at(iv->getID()).size() != iv->usedInConstraints().size()) {
                    //                    std::cout << inConstraintWith.at(iv->getID()).size() << "  vs  "<< iv->usedInConstraints().size() << std::endl;
                                        count++;
                }
            }
            //                std::cout << inConstraintWith.at(iv->getID()).size() << " ";
            //                std::cout << iv->getID() << " ";
            //            } else if (inConstraintWith.at(iv->getID()).size() > 0) {
            //                lolhest++;
            //            }
        }
        std::cout << count << std::endl;
        //        std::cout << std::endl;
        debug;
        unsigned total = 0;
        std::cout << "Binary variables in constraint with: " << std::endl;
        for (unsigned i = 0; i < differentSizes.size(); i++) {
            if (differentSizes.at(i) != 0) {
                std::cout << differentSizes.at(i) << " binary variables that are in same constraint with " << i << " different integer variables " << std::endl;
                total += differentSizes.at(i);
            }
        }
        std::cout << "Total number " << total << std::endl;
        std::cout << "Number of nonfixed " << getMask().size() << std::endl;
        for (constraint con : *Constraints.at(1)) {
            if (con->getNumberOfIntegerVariables() > 1) {
                std::cout << con->getNumberOfIntegerVariables() << " ";
                std::cout << con->getVariables().size() << " ";
            }
        }
        std::cout << std::endl;
        debug;
        //        for(IntegerVariable* iv : inConstraintWith.at(9779)){
        //            assert(iv->isIntegerVariable());
        //            std::cout << iv-> getID() << " ";
        //        }

        //        std::cout << std::endl;
        //        std::cout << lolhest << std::endl;

        debug;

        debug;
        exit(1);
    }



}

std::set<Variable*, Variable::compare_variable>& Model::getInConstraintWithAt(unsigned id) {
    return inConstraintWith.at(id);
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

void Model::cleanUp() {
    std::vector<bool>& brokenInvars = getDDG()->getBrokenInvariants();
    //    assert(Invariants.size() == brokenInvars.size());
    InvariantContainer tmp;
    for (unsigned i = 0; i < Invariants.size(); i++) {
        if (brokenInvars.size() <= i || !brokenInvars.at(i)) {
            tmp.push_back(Invariants.at(i));
        }

    }
    Invariants.swap(tmp);
    
    std::sort(Invariants.begin(), Invariants.end(), compare_invariant());
    std::cout << "Clean up pointers, only oneway pointers should be messy" << std::endl; 
    //    unsigned id = Invariants.size()*2;
    //    unsigned timstamp = 600000000;
    //    for(invariant inv : Invariants){
    //        if(inv->timestamp > timstamp){
    //            std::cout <<"timestamp " << inv->timestamp << std::endl;
    //            debug;
    //        }
    //        if(inv->getID() > id ){
    //            std::cout << "id " << inv->getID() << " prev id " << id << std::endl;
    //            debug;
    //        }
    //        timstamp = inv->timestamp;
    //        id = inv->getID();
    //        std::cout << "timestamp, id " << inv->timestamp << ", " << inv->getID() << std::endl;
    //        debug;
    //    }
    //    std::cout << "broken size " << brokenInvars.size() << " Invariants size " << Invariants.size() << std::endl;
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
