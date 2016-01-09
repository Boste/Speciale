
#include "EQviolation.hpp"

EQviolation::EQviolation(invariant invar, int RHS) {
    LHS = invar;
    this->RHS = RHS;
    InvariantPointers.push_back(invar);
    type = EQVIO;
    representCons = true;
}

EQviolation::~EQviolation() {
}

bool EQviolation::calculateDeltaValue() {
    if(VariableChange.size() == 0){
        debug;
        return true;
    } 
//    if(VariableChange.size() > 1){
//        debug;
//    } 
    
    if (LHS->getCurrentValue() + VariableChange.back() == RHS) {

        DeltaValue = -CurrentValue;
    } else {
        //        DeltaValue = 
        int old = std::abs(LHS->getCurrentValue() - RHS);
        int ne = std::abs(LHS->getCurrentValue() + VariableChange.back() - RHS);
        DeltaValue = ne-old;
//                DeltaValue = 1 - CurrentValue;
    }
    VariableChange.pop_back();

    return true;
}

void EQviolation::proposeChange(int variableNumber, int changeInValue) {
    VariableChange.push_back(changeInValue);
}

void EQviolation::updateValue() {
    CurrentValue += DeltaValue;
    //    DeltaValue = 0;
}

bool EQviolation::test() {
    int value = 0;
    for (invariant inv : InvariantPointers) {
        value += inv->getCurrentValue();
    }
    if (value != LHS->getCurrentValue()) {
        std::cout << value << "  " << LHS->getCurrentValue() << std::endl;
        debug;
    }
    assert(value == LHS->getCurrentValue());
    if (value == RHS) {
        assert(CurrentValue == 0);
    } else {
        //        assert(CurrentValue == 1);
        assert(CurrentValue != 0);
    }
    return true;
}