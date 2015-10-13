#include "State.hpp"
#include "Model.hpp"

State::State(std::shared_ptr<Model> model) {
    //    std::cout << "create state" << std::endl;
    this->model = model;
    
    solutionValue = std::numeric_limits<int>::max();
    //    mask = new std::vector<int>();

}

State::State(const State& orig) {
    this->solution = orig.solution;
    this->solutionValue = orig.solutionValue;
    //    this->mask = orig.mask;
    this->model = orig.model;
}

State::~State() {
    //    delete mask;
    //    delete solution;
}


/// Shoudl prob be in model instead

void State::initializeInvariants() {

    solution.resize(model->getAllVariables().size());

    for (IntegerVariable* current : model->getNonFixedBinaryVariables()) {

        assert(!current->isIntegerVariable());
        if (current->getCurrentValue() != 0) {
            for (updateType inv : model->getPropagationQueue(current)) {
                //            for (updateType inv : current->getPropagationQueue()) {
                deltaQueue.insert(inv);
                //                deltaQueue.push(inv);
            }
            for (updateType invar : model->getUpdate(current)) {
                //                current->getUpdateVector().
                invar->addChange(current->getID(), current->getCurrentValue());
            }
            //            for (updateType invariant : current->getUpdateVector()) {
            //                invariant->calculateDeltaValue();
            //
            //            }
            //            sleep(1);
        }

    }
    std::cout << "size " << deltaQueue.size() << std::endl;
    unsigned last = 0;
    for (updateType invar : deltaQueue) {
        if (invar->getID() <= last) {
            std::cout << "Wrong" << std::endl;
            exit(1);
        } else {
            last = invar->getID();
        }
        //        std::cout << invar->getID() << std::endl;
        //        sleep(1);
    }
    debug;
    //    std::cout << model->getInvariants().size() << std::endl;
    for (std::shared_ptr<Invariant> invar : deltaQueue) {
        //        std::cout << "Testing part" << std::endl;

        //        std::cout << invar.get() << std::endl;
        //        int id = invar->getVariables().at(0)->getID();
        //        invar->addChange(id,1000);
        //        invar->calculateDeltaValue();
        //        std::cout << "test part done " << std::endl;
        //        int id = invar->getVariableID();
        //        if (invar->getType() == MAX) {
        //            std::cout << "max " << std::endl;
        //        }
        //        if (id == 53552 || id == 110133 || id == 119734) {
        //            std::cout << "id " << id << std::endl;
        //            std::cout << "vars " << invar->getVariables().size() << std::endl;
        //            std::cout << "invars " << invar->getInvariants().size() << std::endl;
        //        }
        int delta = invar->calculateDeltaValue();
        for (updateType inv : model->getUpdate(invar)) {
            inv->addChange(invar->getID(),delta);
        }
        invar->updateValue();

    }
    for(invariant invar : model->getInvariants()){
        std::cout << invar->getCurrentValue() << " ";
    }
    std::cout << std::endl;
    debug;
//    for (std::shared_ptr<Invariant> invar : model->getInvariants()) {
//
//
//        //        std::cout << std::endl;
////        invar->updateValue();
//        //        sleep(1);
//        //        if(invar->changeAdd){
//        //            std::cout << "Some change added" << std::endl;
//        //        }
//        int value = invar->getCurrentValue();
//        auto coef = invar->getCoefficients();
////                if (invar->getVariableID() == 53552) {
////                    for (invariant inv : model->getUpdate(invar)) {
////                                        std::cout << coef.at(inv->getVariableID()) << " ";
////                    }
////                    std::cout << std::endl;
//////                    for(int i = model->getUpdate(invar).size()-1;i >=0; i--){
//////                        std::cout <<model->getUpdate(invar).at(i)->getCurrentValue() << " ";
//////                    }
////                    for (invariant inv : model->getUpdate(invar)) {
////                        std::cout << inv->getCurrentValue() << " ";
////                    }
////                    std::cout << std::endl;
////        
////                }
//                int check = 0;
//                for (IntegerVariable* iv : invar->getVariables()) {
//                    assert(!iv->isIntegerVariable());
//                    check += coef.at(iv->getID()) * iv->getCurrentValue();
//                    //            std::cout << coef.at(iv->getID()) * iv->getCurrentValue() << " + ";
//                    //            if (coef.at(iv->getID()) * iv->getCurrentValue() != 0) {
//                    //                std::cout << "id of this is " << iv->getID() << " is integer " << iv->isIntegerVariable() << " + ";
//                    //            }
//                }
//                //        //        std::cout << "my id " << invar->getVariableID() << std::endl;
//                //
//                for (invariant inv : invar->getInvariants()) {
//                    if (invar->getType() == SUM) {
//                        check += inv->getCurrentValue() * coef.at(inv->getVariableID());
//                    } else {
//                        check = inv->getCurrentValue();
//                    }
//                    //            std::cout << inv->getVariableID() << " ";
//                    //            std::cout << inv->getCurrentValue() << " + ";
//                }
//                //        //        std::cout << std::endl;
//                if (check + invar->getStartValue() < 0 && invar->getVariableID() != -1) {
//                    check = 0;
//                    //            std::cout <<" does this happen " << std::endl;
//                } else {
//                    check += invar->getStartValue();
//                }
//                //        //        std::cout << invar->getStartValue() << std::endl;
//                if (value != check) {
//                    std::cout << "Value " << value << "  vs  check " << check << " ini val " << invar->getStartValue() << " " << invar->getVariableID() << std::endl;
//                }
//
//    }


}

void State::initializeConstraints() {
    std::cout << "ini cons" << std::endl;
    int violations = 0;
    for (unsigned i = 1; i < model->getConstraints().size(); i++) {
        constraintContainer prio = model->getConstraints().at(i);
        //        for (unsigned j = 0; j < prio->size(); j++) {
        std::cout << i << std::endl;
        for (std::shared_ptr<Constraint> cons : *prio) {
            if (!cons->isOneway()) {
                violations += cons->updateViolation();
            }
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
    //    for (unsigned i = 0; i < Constraints->at(0)->size(); i++) {
    for (std::shared_ptr < Constraint> obj : *model->getConstraints().at(0)) {

        violations += obj->setDeltaViolationDegree();
        obj->updateViolationDegree();
    }
    solutionValue = violations;
    std::cout << "Initial solution value: " << violations << std::endl;
    //        return violations;

}

void State::saveSolution() {
    solutionValue = getObjectiveValue();

    //     for (int i = 0; i < model->getIntegerVariables()->size(); i++) {
    for (IntegerVariable* iv : model->getNonFixedBinaryVariables()) {


        solution.at(iv->getID()) = iv->getCurrentValue();

    }


}

std::vector<int>& State::getSolution() {
    return solution;
}

int State::getSolutionValue() {
    return solutionValue;
}

void State::setSolution() {
    //
    //    // setting invariants
    //
    //    //    for (unsigned i = 0; i < solution->size(); i++) {
    //    for (IntegerVariable* iv : model->getAllVariables()) {
    //
    //        //        IntegerVariable* current = model->->getAllIntegerVariable(solution->at(i));
    //        for (unsigned j = 0; j < iv->getUpdateVector().size(); j++) { // FIX ME !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //            updateType invariant = iv->getUpdateVector().at(j);
    //            invariant->addChange(iv->getID(), solution.at(iv->getID()) - iv->getCurrentValue());
    //        }
    //    }
    //    //    for (unsigned i = 0; i < model->getInvariants()->size(); i++) {
    //    for (std::shared_ptr<Invariant> invar : model->getInvariants()) {
    //        invar->calculateDeltaValue();
    //        invar->updateValue();
    //        //        model->getInvariants()->at(i)->calculateDeltaValue();
    //        //        model->getInvariants()->at(i)->updateValue();
    //    }
    //    // Setting constraints
    //    int violations = 0;
    //    for (unsigned i = 1; i < model->getConstraints().size(); i++) {
    //        std::shared_ptr<std::vector<std::shared_ptr < Constraint>>>prio = model->getConstraintsWithPriority(i);
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
    //    for (std::shared_ptr<Constraint> obj : *model->getConstraints().at(0)) {
    //        violations += obj->updateViolationDegree();
    //        //        violations += getConstraintsWithPriority(0)->at(i)->updateViolationDegree();
    //    }
    //    std::cout << "Final solution " << violations << " (" << solutionValue << ")" << std::endl;
    //    solutionValue = violations;

}

bool State::recalculateAll() {
    //    std::cout << Invariants->at(0)->getUsedInConstraint() << std::endl;
    //    std::cout << "lhs " << Invariants->at(0)->getCurrentValue() << std::endl;
    //    for(int i = 0; i < numberOfVariables; i++ ){
    //        std::cout << IntVarVector->at(i)->getCurrentValue() << " ";
    //    }
    //    std::cout << std::endl;
    bool success = true;
    //    for (unsigned i = 0; i < Invariants->size(); i++) {
    for (std::shared_ptr<Invariant> invar : model->getInvariants()) {
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
    for (std::shared_ptr<Constraint> obj : *model->getConstraintsWithPriority(0)) {

        //        std::shared_ptr<Constraint> obj = getConstraintsWithPriority(0)->at(i);
        if (!obj->testObj()) {
            success = false;
        }
    }
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

int State::getObjectiveValue() {
    int value = 0;
    //    for (unsigned i = 0; i < Constraints->at(0)->size(); i++) {
    for (std::shared_ptr < Constraint> obj : *(model->getConstraints().at(0))) {

        value += obj->getViolationDegree();
    }

    return value;
}
