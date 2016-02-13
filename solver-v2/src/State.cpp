#include "State.hpp"

State::State(std::shared_ptr<Storage> model) {
    this->model = model;

    for (Variable* iv : model->getMask()) {
        solution.push_back(iv->getCurrentValue());
    }
    for (unsigned i = 0; i < model->getInitialEvaluation().size(); i++) {
        evaluation.push_back(model->getInitialEvaluation().at(i));
    }
    violation = model->getViolatedConstraints().size();
}
/// Careful using this, not sure how much is copied and how much references to same objects. (prob same model, copy of value and solution) 
/// Use copy instead

State::State(const State& orig) {
    this->solution = orig.solution;
    this->evaluation = orig.evaluation;
    this->model = orig.model;
}

/// Only copies LSVariables (Mask) and set violations

void State::copy(std::shared_ptr<State> st) {
    for(unsigned i = 0; i< model->getMask().size();i++){
        solution[i] = model->getMaskAt(i)->getCurrentValue();
    }
    this->evaluation = st->evaluation;
    violation = st->violation;
}

State::~State() {
}
/// Saves current value of all non-fixed binary variable and set solutionValue to the sum of obj functions. 

void State::setViolation() {
    
    violation = model->getViolatedConstraints().size();
}

unsigned State::getViolations() {
    return violation;
}

std::vector<int>& State::getSolution() {
    return solution;
}

std::vector<int>& State::getEvaluation() {
    return evaluation;

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

bool State::compare(std::shared_ptr<State>& st) {
    for (int i = this->evaluation.size() - 1; i >= 0; i--) {

        if (this->evaluation.at(i) > st->getEvaluation().at(i)) {
            return false;
        } else if (this->evaluation.at(i) < st->getEvaluation().at(i)) {
            return true;
        }
    }
    return (this->evaluation.at(0) < st->getEvaluation().at(0));
}

/// Only used for testing
bool State::recalculateAll() {
    bool success = true;
    for (invariant invar : model->getInvariants()) {
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
    return success;
}
