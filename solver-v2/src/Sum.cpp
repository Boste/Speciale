#include "Sum.hpp"

Sum::Sum(vector<IntegerVariable*>* vars, vector<int>* c) : Invariant() {//:IntVariables(vars),coefficients(c) {
    VariablePointers = *vars;
    for (unsigned i = 0; i < vars->size(); i++) {
        coefficients.insert(std::make_pair(vars->at(i)->getID(), c->at(i)));
    }
    //        std::cout << std::endl;
}

Sum::Sum(const Sum &a) : Invariant(a) {
    //        std::cout << "copy constructor2" << std::endl;
    this->coefficients = a.coefficients;
    this->VariableChange = a.VariableChange;
    this->VariablePointers = a.VariablePointers;
    //        sleep(5000);
}

Sum::~Sum() {
    delete &coefficients;
    delete &VariableChange;
    delete &VariablePointers;
}

Sum& Sum::operator=(const Sum &a) {
    if (this != &a) {
        this->coefficients = a.coefficients;
        this->VariablePointers = a.VariablePointers;

        this->VariableChange = a.VariableChange;
    }
    return *this;
}

int Sum::calculateDeltaValue() {
    int valueChange = 0;
    while (!VariableChange.empty()) {
        std::pair<int, int> currentPair = VariableChange.back();
        valueChange += coefficients.at(currentPair.first) * currentPair.second;
        VariableChange.pop_back();
    }
    //        std::cout << valueChange << " ";
    DeltaValue = valueChange;
    return valueChange;
}

void Sum::addChange(int variableNumber, int changeInValue) {
    VariableChange.push_back(std::pair<int, int> (variableNumber, changeInValue));
    //        std::cout << "addChange in sum" << std::endl;
    //        sleep(5000);
}

void Sum::usedByConstraint(int constraint) {
    usedInConstraintNr = constraint;
}

void Sum::usedByObjective(int objective) {
    usedInObjectiveNr = objective;
    

}

//void Sum::addChange(std::vector<int> variableNumbers, std::vector<int> changes) {
//    std::cout << "Ikke implementeret" << std::endl;
//}
//
//void Sum::addChangeSwap(int variableNumber1, int variableNumber2) {
//    std::cout << "Ikke implementeret" << std::endl;
//
//}

double Sum::test() {
    double testValue = 0;
    for (unsigned i = 0; i < VariablePointers.size(); i++) {
        testValue += VariablePointers.at(i)->getCurrentValue() * coefficients.at(VariablePointers.at(i)->getID());
    }
    if (testValue != CurrentValue) {
        std::cout << "Failed test in sum" << std::endl;
        std::cout << "testValue: " << testValue << "  CurrentValue: " << CurrentValue << std::endl;
    }
    return testValue;
}