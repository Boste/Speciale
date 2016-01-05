
#include "LEQviolation.hpp"


LEQviolation::LEQviolation(invariant invar, int RHS) {
    LHS = invar;
    this->RHS = RHS;
    InvariantPointers.push_back(invar);
    type = LEQVIO;
    representCons = true;
    

}



LEQviolation::~LEQviolation() {
}

bool LEQviolation::calculateDeltaValue() {
    if(LHS->getCurrentValue() + VariableChange.back() <= RHS){
        DeltaValue = -CurrentValue;
    } else {
        DeltaValue = 1-CurrentValue;
    }
    VariableChange.pop_back();
    
    return true;
}

void LEQviolation::proposeChange(int variableNumber, int changeInValue) {
    VariableChange.push_back(changeInValue);
}

void LEQviolation::updateValue() {
    CurrentValue += DeltaValue;
    
}
bool LEQviolation::test(){
    int value = 0;
    for(invariant inv : InvariantPointers){
        value +=inv->getCurrentValue();
    }
    assert(value == LHS->getCurrentValue());
    if(value <= RHS){
        assert(CurrentValue == 0);
    } else {
        assert(CurrentValue == 1);
    }
    return true;
}