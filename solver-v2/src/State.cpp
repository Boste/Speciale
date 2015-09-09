#include "State.hpp"
#include "Model.hpp"

State::State(std::shared_ptr<Model> model) {
    //    std::cout << "create state" << std::endl;
    this->model = model;
    solution = new std::vector<int>();
    solutionValue = std::numeric_limits<int>::max();
    mask = new std::vector<int>();

}

State::State(const State& orig) {
    this->solution = orig.solution;
    this->solutionValue = orig.solutionValue;
    this->mask = orig.mask;
    this->model = orig.model;
}

State::~State() {
    delete mask;
    delete solution;
}

void State::initializeInvariants() {

    mask->resize(model->getIntegerVariables().size());
    for (unsigned i = 0; i < model->getIntegerVariables().size(); i++) {
        mask->at(i) = i;
    }
    shuffleMask();

    //    std::random_shuffle(mask->begin(), mask->end());
    solution->resize(model->getAllIntegerVariables().size());
    //    for (unsigned i = 0; i < IntVarVector->size(); i++) {
    for (IntegerVariable* current : model->getAllIntegerVariables()) {

        //        IntegerVariable* current = IntVarVector->at(i);
        //        for (unsigned j = 0; j < current->getUpdateVector()->size(); j++) {
        for (int invariantNumber : *current->getUpdateVector()) {
            //            int invariantNumber = current->getUpdateVector()->at(j);
            model->getInvariants().at(invariantNumber)->addChange(current->getID(), current->getCurrentValue());
        }
    }
    //    for (unsigned i = 0; i < Invariants->size(); i++) {
    for (std::shared_ptr<Invariant> invar : model->getInvariants()) {
        invar->calculateDeltaValue();
        invar->updateValue();
        //        Invariants->at(i)->calculateDeltaValue();
        //        Invariants->at(i)->updateValue();
        //        Invariants->at(i)->test();
    }
}

void State::initializeConstraints() {

    int violations = 0;
    for (unsigned i = 1; i < model->getConstraints().size(); i++) {
        std::shared_ptr<std::vector<std::shared_ptr < Constraint>>>prio = model->getConstraints().at(i);
        //        for (unsigned j = 0; j < prio->size(); j++) {
        for (std::shared_ptr<Constraint> cons : *prio) {
            violations += cons->updateViolation();
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
    for (IntegerVariable* iv : model->getIntegerVariables()) {


        solution->at(iv->getID()) = iv->getCurrentValue();

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

    //    for (unsigned i = 0; i < solution->size(); i++) {
    for (IntegerVariable* iv : model->getAllIntegerVariables()) {

        //        IntegerVariable* current = model->->getAllIntegerVariable(solution->at(i));
        for (unsigned j = 0; j < iv->getUpdateVector()->size(); j++) {
            int invariantNumber = iv->getUpdateVector()->at(j);
            model->getInvariants().at(invariantNumber)->addChange(iv->getID(), solution->at(iv->getID()) - iv->getCurrentValue());
        }
    }
    //    for (unsigned i = 0; i < model->getInvariants()->size(); i++) {
    for (std::shared_ptr<Invariant> invar : model->getInvariants()) {
        invar->calculateDeltaValue();
        invar->updateValue();
        //        model->getInvariants()->at(i)->calculateDeltaValue();
        //        model->getInvariants()->at(i)->updateValue();
    }
    // Setting constraints
    int violations = 0;
    for (unsigned i = 1; i < model->getConstraints().size(); i++) {
        std::shared_ptr<std::vector<std::shared_ptr < Constraint>>>prio = model->getConstraintsWithPriority(i);
        //        for (unsigned j = 0; j < prio->size(); j++) {
        for (std::shared_ptr<Constraint> cons : *prio) {
            violations += cons->updateViolation();
        }
    }
    if (violations != 0) {
        std::cout << "Final solution not feasible? violations: " << violations << std::endl;
        sleep(2);
    }
    // setting objective value
    violations = 0;
    //    for (unsigned i = 0; i < getConstraintsWithPriority(0)->size(); i++) {
    for (std::shared_ptr<Constraint> obj : *model->getConstraints().at(0)) {
        violations += obj->updateViolationDegree();
        //        violations += getConstraintsWithPriority(0)->at(i)->updateViolationDegree();
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

void State::shuffleMask() {
    std::random_shuffle(mask->begin(), mask->end());

}

int State::maskAt(int i) {
    return mask->at(i);
}

int State::getObjectiveValue() {
    int value = 0;
    //    for (unsigned i = 0; i < Constraints->at(0)->size(); i++) {
    for (std::shared_ptr < Constraint> obj : *(model->getConstraints().at(0))) {

        value += obj->getViolationDegree();
    }

    return value;
}
