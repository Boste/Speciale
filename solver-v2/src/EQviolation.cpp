
#include "EQviolation.hpp"

EQviolation::EQviolation(invariant invar, int RHS) {
    LHS = invar;
    this->RHS = RHS;
    InvariantPointers.push_back(invar);
    type = EQVIO;

}

EQviolation::~EQviolation() {
}

bool EQviolation::calculateDeltaValue() {
    if (LHS->getCurrentValue() + VariableChange.back() == RHS) {
        DeltaValue = -CurrentValue;
    } else {
        DeltaValue = 1 - CurrentValue;
    }
    VariableChange.pop_back();

    return true;
}

void EQviolation::addChange(int variableNumber, int changeInValue) {
    VariableChange.push_back(changeInValue);
}

void EQviolation::updateValue() {
    CurrentValue += DeltaValue;

}

bool EQviolation::test() {
    int value = 0;
    for (invariant inv : InvariantPointers) {
        value += inv->getCurrentValue();
    }
    if (value != LHS->getCurrentValue()) {
        std::cout << value << "  " << LHS->getCurrentValue() << std::endl;
    }
    assert(value == LHS->getCurrentValue());
    if (value == RHS) {
        assert(CurrentValue == 0);
    } else {
        assert(CurrentValue == 1);
    }
    return true;
}