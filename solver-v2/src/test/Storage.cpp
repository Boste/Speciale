
#include "Storage.hpp"
//#include "Random.hpp"

Storage::Storage() {

}

Storage::~Storage() {
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

std::vector<std::vector<Variable*>>&Storage::getPriorityVectors() {
    return priorityVectorsOfVariables;
}

std::vector<Variable*>& Storage::getPriorityVectorNr(unsigned i) {
    assert(i < priorityVectorsOfVariables.size());

    return priorityVectorsOfVariables.at(i);
}

std::vector<Variable*>& Storage::getIntegerVariables() {
    return IntegerVariables;
}
//IntegerVariable* Model::addIntegerVariable(int lb, int ub) {

Variable* Storage::addBinaryVariable(int lb, int ub) {
    //    for (int i = 0; i < numberOfVariables; i++) {
    int id = original.size();
    Variable* v = new Variable(lb, ub, id);
    original.push_back(v);
    return v;
    //    binaryVariables.push_back(v);

}

//Variable* Model::addIntegerVariable(int lb, int ub) {
//    //    std::cout << lb << " " << ub << std::endl;
//    //    sleep(1);
//    int id = original.size();
//    Variable* v = new Variable(lb, ub, id);
//    original.push_back(v);
//    IntegerVariables.push_back(v);
//    return v;
//
//}
//int Model::getNumberOfVariables(){
//    return numberOfVariables;
//}

void Storage::setNonFixedVariables(std::vector<Variable*>& nonFixed) {
    nonFixedVariables = nonFixed;
    //    mask = nonFixed;
    //    shuffleMask();
    //    std::cout << &IntVarVector << " vs " << nonFixed << std::endl;
}

void Storage::addViolatedConstraint(invariant inv) {
    //    assert(DDG->getInvariantUpdate(inv->getID()).size() == 1);
    //    assert(DDG->getInvariantUpdate(inv->getID()).at(0)->type == LEQVIO || DDG->getInvariantUpdate(inv->getID()).at(0)->type == EQVIO);
    violatedConstraints[inv->getID()] = inv;
    inv->setInViolatedConstraints(true);

}

void Storage::removeViolatedConstraint(invariant inv) {
    //    unsigned  size  = violatedConstraints.size();
    violatedConstraints.erase(inv->getID());
    //    assert(size-1 == violatedConstraints.size());
    //    debug;
    inv->setInViolatedConstraints(false);

}

std::unordered_map<unsigned, invariant>& Storage::getViolatedConstraints() {
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

std::shared_ptr<DependencyDigraph>& Storage::getDDG() {
    return DDG;
}

propagation_queue& Storage::getPropagationQueue(Variable* iv) {
    assert(DDG->propagationQueueHasBeenMade());
    assert(!iv->isDef());
    assert(!iv->isFixed());
    return DDG->getPropagationQueue(iv);

}

updateVector& Storage::getUpdate(invariant invar) {
    return DDG->getInvariantUpdate(invar->getID());
}

updateVector& Storage::getUpdate(Variable* iv) {
    return DDG->getVariableUpdate(iv->getID());
}

//void Model::startUp() {
//    id = original.size();
////    highestPriority = getConstraints().size();
//}

//std::vector<constraint>& Model::getFunctionalConstraints() {
//    return functionalConstraints;
//}

Variable* Storage::getMaskAt(int i) {
    return mask.at(i);
}

std::vector<Variable*>& Storage::getMask() {
    return mask;
}

void Storage::shuffleMask() {
    std::random_shuffle(mask.begin(), mask.end());

}

//void Model::updateIntegerVariable(int index, Gecode::BoolVar& variable) {
////void Model::updateIntegerVariable(int index, Gecode::IntVar& variable) {
//    getNonFixedVariable(index)->setVariablePointer(variable);
//}
/// Should only be used before propagation queue is made

variableContainer& Storage::getNonFixedVariables() {
    return nonFixedVariables;
}
/// Mask is just as useful

//variableContainer& Model::getLSVariables() {
//    return LSVariables;
//}

Variable* Storage::getNonFixedVariable(int i) {
    return nonFixedVariables.at(i);
}

Variable* Storage::getVariable(unsigned id) {
    return original.at(id);
}

variableContainer& Storage::getAllVariables() {
    return original;
}

InvariantContainer& Storage::getInvariants() {

    //std::vector<Invariant*>* Model::getInvariants() {
    return Invariants;
}

/// adds Invariant and gives it an id.

void Storage::addInvariant(invariant invar) {
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

allConstraints& Storage::getConstraints() {
    return Constraints;
}
//allConstraints& Model::getConstraints() {
//    return Constraints;
//}

constraintContainer Storage::getConstraintsWithPriority(int prio) {
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

InvariantContainer& Storage::getEvaluationInvariants() {
    return evaluationInvariants;
}

invariant Storage::getEvaluationInvariantNr(unsigned nr) {
    return evaluationInvariants.at(nr);
}

//void Model::addToObjectiveInvariant(invariant invar) {
//    objectiveInvariant.push_back(invar);
//}

void Storage::addToEvaluationInvariants(invariant invar) {
    evaluationInvariants.push_back(invar);
}

std::vector<int>& Storage::getInitialEvaluation() {
    return initialEvaluation;
}

std::vector<Variable*>& Storage::getEvaluationVariables() {
    return evalVariables;
}

Variable* Storage::getEvaluationVariableNr(unsigned nr) {
    return evalVariables.at(nr);
}

void Storage::initialize() {
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
    for (constraint cons : *getConstraintsWithPriority(0)) {
        for (Variable* var : cons->getVariables()) {

            if (!var->isDef() && !var->isFixed()) {
                evalVariables.push_back(var);
            }
        }
    }
    std::cout << "EvalVariables size " << evalVariables.size() << std::endl;
    //    for(int i : initialEvaluation){
    //        std::cout << i << std::endl;
    //    }
    for (Variable* iv : getNonFixedVariables()) {
        if (iv->isDef() || iv->isFixed()) { // || iv->isIntegerVariable()) {
            //            std::cout << iv->isDef() << " " <<  iv->isFixed() << " "<< iv->isIntegerVariable() << std::endl;
            continue;

        } else {
            //            LSVariables.push_back(iv);
            mask.push_back(iv);
        }
    }
    shuffleMask();



//    if (getIntegerVariables().size() == 0) {
////        std::cout << "Do stuff for binary swapping" << std::endl;
//    } else {
//        containsIntegerVariables = true;
//        auto start = std::clock();
//        std::cout << "Do stuff for binary <-> integer swap" << std::endl;
//        std::set<Variable*, Variable::compare_variable> newSet;
//        for (unsigned i = 0; i < original.size(); i++) {
//            inConstraintWith.push_back(newSet);
//        }
//        std::shared_ptr<std::vector < constraint>> newVector = std::make_shared<std::vector < constraint >> ();
//        for (unsigned i = 0; i < original.size(); i++) {
//            constraintsWithIntegerVarsRelated.push_back(newVector);
//        }
//        //        std::shared_ptr<std::vector<std::pair < IntegerVariable*, int>>> newVectorPair = std::make_shared<std::vector<std::pair < IntegerVariable*, int>>>();
//        //        for (unsigned i = 0; i < original.size(); i++) {
//        //            IntegerVariablesAndChange->push_back(newVectorPair);
//        //            constraintsWithIntegerVarsRelated.push_back(newVector);
//        //        }
//        //        inConstraintWith.reserve(original.size());
//
////        for (Variable* var : IntegerVariables) {
////            if (!(var->isFixed() || var->isDef())) {
////                for (constraint con : var->usedInConstraints()) {
////                    if (con->getPriority() != OBJ) {
////                        for (Variable* iv : con->getVariables()) {
////                            if (!(iv->isFixed() || iv->isDef())) { // || iv->isIntegerVariable())) {
////                                inConstraintWith[iv->getID()].insert(var);
////                            }
////                        }
////                    }
////                }
////            }
////        }
//        //        for (unsigned i = 1; i < Constraints.size(); i++) {
//        //            constraintContainer cons = Constraints.at(i);
//        //            for (constraint con : *cons) {
//        //                if (con->isOneway()) {
//        //                    continue;
//        //                }
//        //                if (con->getNumberOfIntegerVariables() > 0) {
//        //                    //                    std::unordered_map<int, coefType>& coefficients = con->getCoefficients();
//        //                    std::vector<Variable*>& intVars = con->getIntegerVariables();
//        //                    for (Variable* iv : con->getVariables()) {
//        //                        if (!(iv->isFixed() || iv->isDef() )) {//|| iv->isIntegerVariable())) {
//        //                            //                            std::vector<IntegerVariable*> newVector;
//        //                            //                            inConstraintWith[iv->getID()] = newVector;
//        //                            //                            int coef = coefficients.at(iv->getID());
//        //                            //                            for(IntegerVariable* intvar : intVars){
//        //                            //                                int intcoef = coefficients.at(intvar->getID());
//        //                            //                                int change = ++ 
//        //                            //                                std::pair<IntegerVariable*, int> intvarAndChange(intvar,) ;
//        //                            //                                IntegerVariablesAndChange->at(iv->getID())->push_back(intvarAndChange);
//        //                            //                            }
//        //                            //                            constraintsWithIntegerVarsRelated.at(iv->getID())->push_back(con);
//        //                            //                            std::cout << "Behøver ikke det efterfølgende, bare spørg con hvor mange int der skal ledes efter" << std::endl;
//        //                            //                            std::cout << "Check om de er fixed (integer variablene) " << std::endl;
//        //                            for (Variable* intvar : intVars) {
//        //                                if (intVars.size() > 1) {
//        //                                    std::cout << intVars.size() << std::endl;
//        //                                }
//        //                                assert(intvar->isIntegerVariable());
//        //                                if (intvar->isFixed() || intvar->isDef()) {
//        //
//        //                                } else {
//        //                                    inConstraintWith[iv->getID()].insert(intvar);
//        //
//        //                                }
//        //                            }
//        //                        }
//        //                    }
//        //                }
//        //            }
//        //        }
//        auto usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
//        std::cout << usedTime << std::endl;
//
//        unsigned count = 0;
//        std::cout << "Is this even working?" << std::endl;
//        std::vector<int> differentSizes(200, 0);
////        for (Variable* iv : getMask()) {
////            //            if (inConstraintWith.at(iv->getID()).size() > 0) {
////            differentSizes.at(inConstraintWith.at(iv->getID()).size())++;
////            if (inConstraintWith.at(iv->getID()).size() != 0) {
////                if (inConstraintWith.at(iv->getID()).size() != iv->usedInConstraints().size()) {
////                    //                    std::cout << inConstraintWith.at(iv->getID()).size() << "  vs  "<< iv->usedInConstraints().size() << std::endl;
////                    count++;
////                }
////            }
//            //                std::cout << inConstraintWith.at(iv->getID()).size() << " ";
//            //                std::cout << iv->getID() << " ";
//            //            } else if (inConstraintWith.at(iv->getID()).size() > 0) {
//            //                lolhest++;
//            //            }
//        }
//        std::cout << count << std::endl;
//        //        std::cout << std::endl;
//        debug;
//        unsigned total = 0;
//        std::cout << "Binary variables in constraint with: " << std::endl;
//        for (unsigned i = 0; i < differentSizes.size(); i++) {
//            if (differentSizes.at(i) != 0) {
//                std::cout << differentSizes.at(i) << " binary variables that are in same constraint with " << i << " different integer variables " << std::endl;
//                total += differentSizes.at(i);
//            }
//        }
//        std::cout << "Total number " << total << std::endl;
//        std::cout << "Number of nonfixed " << getMask().size() << std::endl;
//        //        for (constraint con : *Constraints.at(1)) {
//        //            if (con->getNumberOfIntegerVariables() > 1) {
//        //
//        //                std::cout << con->getNumberOfIntegerVariables() << " ";
//        //                std::cout << con->getVariables().size() << " ";
//        //            }
//        //        }
//        std::cout << std::endl;
//        debug;
//        //        for(IntegerVariable* iv : inConstraintWith.at(9779)){
//        //            assert(iv->isIntegerVariable());
//        //            std::cout << iv-> getID() << " ";
//        //        }
//
//        //        std::cout << std::endl;
//        //        std::cout << lolhest << std::endl;
//
//        debug;
//
//        debug;
//        exit(1);
//    }



}

std::set<Variable*, Variable::compare_variable>& Storage::getInConstraintWithAt(unsigned id) {

    return inConstraintWith.at(id);
}

std::vector<constraint>& Storage::getFeasibleFunctionalConstraints() {

    return feasibleFunctionalConstriants;
}

void Storage::setFeasibleFunctionalConstraints(std::vector<constraint> funcCons) {

    feasibleFunctionalConstriants = funcCons;
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

void Storage::cleanUp() {
    
    
    
    //    std::vector<bool>& brokenInvars = getDDG()->getBrokenInvariants();
    //        assert(Invariants.size() == brokenInvars.size());
    InvariantContainer tmp;
    //    InvariantContainer tmp2;
    //    for (unsigned i = 0; i < Invariants.size(); i++) {
    //        
    //        if (brokenInvars.size() <= i) {
    //            tmp.push_back(Invariants.at(i));
    //        } else if (!brokenInvars.at(i)) {
    //            tmp.push_back(Invariants.at(i));
    //        } else {
    //            brokenInvars.at(i) = false;
    //            delete Invariants.at(i);
    //
    //        }
    //
    //    }
    for (invariant invar : Invariants) {
        if (!invar->isBroken()) {
            tmp.push_back(invar);
        } else {

            delete invar;

        }
    }

    Invariants.swap(tmp);



    std::sort(Invariants.begin(), Invariants.end(), compare_invariant());
    for (invariant inv : Invariants) {
        //        std::cout << "vp " << inv->getVariablePointers().size() << " coef " << inv->getCoefficients().size()<< " ip " << inv->getInvariantPointers().size() << std::endl;

        for (invariant invar : inv->getInvariantPointers()) {
            assert(!invar->isBroken());
            assert(original[invar->getVariableID()]->isDef());
        }

        for (auto it = inv->getVariablePointers().begin(); it != inv->getVariablePointers().end(); ++it) {
            //            debug;
            //            std::cout << (*it)->getID() << std::endl;
            if ((*it)->isDef()) {
                inv->getInvariantPointers().push_back((*it)->getOneway());
                inv->VariablePointers.erase(it);
                --it;


                //                debug;
            }
//            if ((*it)->isFixed()) {
//                debug;
//                std::cout << (*it)->getID() << std::endl;
//                inv->VariablePointers.erase(it);
//                --it;
//            }


        }
        //        std::cout << inv->getVariablePointers().size() << std::endl;
        //        debug;


        if (inv->getInvariantPointers().size() + inv->getVariablePointers().size() != inv->getCoefficients().size()) {
            std::cout << inv->getInvariantPointers().size() << " + " << inv->getVariablePointers().size() << " != " << inv->getCoefficients().size();
            debug;

        }
    }

    //    std::cout << "Clean up pointers, only oneway pointers should be messy" << std::endl; 
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
