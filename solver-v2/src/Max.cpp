#include "Max.hpp"

Max::Max(Variable* var, int second, unsigned id) {
    type = MAX;
    variableID = id;
    variableValue = var->getCurrentValue();
    this->second = second;
    //    this->DDG = DDG;
    //    VariablePointers.push_back(var);
}

Max::Max(invariant invar, int bound, unsigned id) {
    type = MAX;
    variableID = id;
    variableValue = invar->getCurrentValue();
    second = bound;
    this->invar = invar;
}

Max::~Max() {
}

void Max::proposeChange(int id, int change) {
    VariableChange.push_back(change);

}

bool Max::calculateDelta() {
    firstChange = 0;
    DeltaValue = 0;
    while (!VariableChange.empty()) {
        firstChange += VariableChange.back();
        VariableChange.pop_back();
    }
    if (firstChange + variableValue > second) {
        DeltaValue = firstChange + variableValue - CurrentValue;
    } else {
        DeltaValue = second - CurrentValue;
    }
    return true;
}

void Max::updateValue() {
    CurrentValue += DeltaValue;

    variableValue += firstChange;
}

bool Max::test() {
    if (CurrentValue == std::max(0, invar->getCurrentValue())) {
        return true;
    } else {
        return false;
    }
}