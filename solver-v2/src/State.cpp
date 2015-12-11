#include "State.hpp"
//#include "Model.hpp"

/// Model should be initialized before making a State (since State is only used in LS hence model should be made LS friendly)

State::State(std::shared_ptr<Model> model) {
    //    std::cout << "create state" << std::endl;
    this->model = model;

    for (IntegerVariable* iv : model->getAllVariables()) {
        solution.push_back(iv->getCurrentValue());
    }
    for (unsigned i = 0; i < model->getInitialEvaluation().size(); i++) {
        evaluation.push_back(model->getInitialEvaluation().at(i));
    }
    //    solutionValue = model->initialValue;
    //    numberOfViolations = model->initialViolations;

    //    mask = new std::vector<int>();

}
/// Careful using this, not sure how much is copied and how much references to same objects. (prob same model, copy of value and solution) 
/// Use copy instead

State::State(const State& orig) {
    this->solution = orig.solution;
    //    this->solutionValue = orig.solutionValue;
    //    this->mask = orig.mask;
    this->evaluation = orig.evaluation;
    //    std::cout << &evaluation[0] << " vs " &orig.evaluation[0] << std::endl;
    this->model = orig.model;
}

/// Only copies LSVariables 
void State::copy(std::shared_ptr<State> st) {
    this->saveSolution();
    //    for(unsigned i = 0; i< solution.size();i++){
    ////        solution[i] = st->getSolution()[i];
    //        std::cout << solution[i] << " ";
    //    }
    //    std::cout << std::endl;
    //    this->solutionValue = orig.solutionValue;
    //    this->mask = orig.mask;
    this->evaluation = st->evaluation;
    //    std::cout << &evaluation[0] << " vs " << &st->evaluation[0] << std::endl;
    this->model = st->model;
}

State::~State() {
    //    delete mask;
    //    delete solution;
}

/// Saves current value of all non-fixed binary variable and set solutionValue to the sum of obj functions. 

void State::saveSolution() {

//    for (IntegerVariable* iv : model->getNonFixedBinaryVariables()) {
//        solution.at(iv->getID()) = iv->getCurrentValue();
//    }
    for (IntegerVariable* iv : model->getMask()) {
        solution.at(iv->getID()) = iv->getCurrentValue();
    }

}
/// Returns a solution that is saved. 

std::vector<int>& State::getSolution() {
    return solution;
}
/// Get the solutionValue last saved. 

//int State::getSolutionValue() {
//    return solutionValue;
//}

std::vector<int>& State::getEvaluation() {
    return evaluation;

}

void State::updateEvaluation(std::vector<int>& changes) {
    for (unsigned i = 0; i < evaluation.size(); i++) {
        evaluation[i] += changes[i];
    }
}

bool State::isFeasible() {
    feasible = true;
    for (unsigned i = 1; i < evaluation.size(); i++) {
        if (evaluation.at(i) > 0) {
            feasible = false;
        }
    }
    return feasible;
}


/// Recomputes all invariant, constraints and obj func, from the last saved solution (expensive)
/// Not necessary for return the objective value

//void State::setSolution() {
//
//    int change;
//    //    for (unsigned i = 0; i < solution->size(); i++) {
//    for (IntegerVariable* iv : model->getNonFixedBinaryVariables()) {
//
//        //        IntegerVariable* current = model->->getAllIntegerVariable(solution->at(i));
//        //            for (unsigned j = 0; j < model->getUpdate(iv); j++) {
//        change = solution.at(iv->getID()) - iv->getCurrentValue();
//        if (change != 0) {
//            iv->setCurrentValue(solution.at(iv->getID()));
//
//            for (updateType invar : model->getUpdate(iv)) {
//                //                updateType invariant = iv->getUpdateVector().at(j);
//                invar->addChange(iv->getID(), change);
//            }
//        }
//
//
//    }
////    std::cout << "value of 20340 " << solution.at(20340) << std::endl;
//    //    for (unsigned i = 0; i < model->getInvariants()->size(); i++) {
//    for (std::shared_ptr<Invariant> invar : model->getInvariants()) {
//
//        int change = invar->calculateDeltaValue();
//        if (change != 0) {
//            for (invariant inv : model->getUpdate(invar)) {
//                assert(invar != inv);
//                inv->addChange(invar->getVariableID(), change);
//            }
//        }
//        invar->updateValue();
//        if (invar->getVariableID() != -1) {
//            IntegerVariable* iv = model->getVariable(invar->getVariableID());
//            assert(iv->getOneway() == invar);
//            if (invar->getCurrentValue() < iv->getLowerBound()) {
//                std::cout << "FML!!!!!!!!!!!" << std::endl;
//                debug;
//                exit(1);
//
//            }
//            assert(invar->getCurrentValue() >= iv->getLowerBound());
//            iv->setCurrentValue(invar->getCurrentValue());
//            //            assert(solution.at(iv->getID())== iv->getCurrentValue());
//            //            if(solution.at(iv->getID())!= iv->getCurrentValue()){
//            //                std::cout << solution.at(iv->getID()) << " vs " << iv->getCurrentValue() << " type " << invar->getType() << std::endl;
//            //            }
//            solution.at(iv->getID()) = iv->getCurrentValue();
//        }
//        //        model->getInvariants()->at(i)->calculateDeltaValue();
//        //        model->getInvariants()->at(i)->updateValue();
//    }
//
//    std::cout << std::endl;
//    // Setting constraints
//    int violations = 0;
//    for (unsigned i = 1; i < model->getConstraints().size(); i++) {
//        std::shared_ptr<std::vector<std::shared_ptr < Constraint>>>prio = model->getConstraintsWithPriority(i);
//        //        for (unsigned j = 0; j < prio->size(); j++) {
//
//        for (std::shared_ptr<Constraint> cons : *prio) {
//            if (!cons->isOneway()) {
//                cons->updateViolation();
//                violations += cons->getViolation();
////                int violation = cons->updateViolation();
////                if(violation != 0){
////                    std::cout << "Violation " << violation << std::endl;
////                }
////                cons->testCons();
////                if (violation == 1) {
////                    std::cout << "invar id " << cons->getInvariant()->getID() << " value " << cons->getInvariant()->getCurrentValue() << " rhs " << cons->getArgument(1) << std::endl;
////                    violations += violation;
////                }
//            }
//        }
//    }
//    evaluation.at(1) = violations;
//    if (violations != 0) {
//        std::cout << "Final solution not feasible? violations: " << violations << std::endl;
//        sleep(2);
//    }
//    // setting objective value
//    int value = 0;
//    for (invariant invar : model->getObjectiveInvariant()) {
//        value += invar->getCurrentValue();
//
//    }
//    std::cout << std::endl;
//    //    for (unsigned i = 0; i < getConstraintsWithPriority(0)->size(); i++) {
//    //    for (std::shared_ptr<Constraint> obj : *model->getConstraints().at(0)) {
//    //        value += obj->updateViolationDegree();
//    //        //        violations += getConstraintsWithPriority(0)->at(i)->updateViolationDegree();
//    //    }
//    std::cout << "Final solution " << value << " (" << violations << ")" << std::endl;
//    evaluation.at(0) = value;
//
//}
/// Test all invariants, constraints and obj funcs according to their test() 

bool State::recalculateAll() {
    debug;
    //    std::cout << Invariants->at(0)->getUsedInConstraint() << std::endl;
    //    std::cout << "lhs " << Invariants->at(0)->getCurrentValue() << std::endl;
    //    for(int i = 0; i < numberOfVariables; i++ ){
    //        std::cout << IntVarVector->at(i)->getCurrentValue() << " ";
    //    }
    //    std::cout << std::endl;
    bool success = true;
    //    for (unsigned i = 0; i < Invariants->size(); i++) {
    for (invariant invar : model->getInvariants()) {
        //        std::shared_ptr<Invariant> invar = Invariants->at(i);
        //        Invariant* invar = Invariants->at(i);
        if (!invar->test()) {
            success = false;
        }
    }
    for (unsigned i = 1; i < model->getConstraints().size(); i++) {
        std::shared_ptr<std::vector<std::shared_ptr < Constraint>>> prio = model->getConstraintsWithPriority(i);
        for (unsigned j = 0; j < prio->size(); j++) {
            std::shared_ptr<Constraint> cons = prio->at(i);

            if (!cons->testCons()) {
                success = false;
            }
        }
    }
    //    for (unsigned i = 0; i < getConstraintsWithPriority(0)->size(); i++) {
    //    for (std::shared_ptr<Constraint> obj : *model->getConstraintsWithPriority(0)) {
    //
    //        //        std::shared_ptr<Constraint> obj = getConstraintsWithPriority(0)->at(i);
    //        if (!obj->testObj()) {
    //            success = false;
    //        }
    //    }
    return success;
}

/// Move this to model

//void State::shuffleMask() {
//    std::random_shuffle(mask->begin(), mask->end());
//
//}
/// Move this to model

//int State::maskAt(int i) {
//    return mask->at(i);
//}


/// Returns the sum of obj functions violation Degree (They are implemented as constraints still)

//int State::getObjectiveValue() {
//    int value = 0;
//    //    for (unsigned i = 0; i < Constraints->at(0)->size(); i++) {
//    for (invariant invar : model->getObjectiveInvariant()) {
//
//        value += invar->getCurrentValue();
//    }
//    //    std::cout << value << std::endl;
//    return value;
//}
