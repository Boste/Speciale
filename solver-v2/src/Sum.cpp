#include "Sum.hpp"


Sum::Sum(std::vector<Variable*> vars, std::unordered_map<int, coefType> map) {//:IntVariables(vars),coefficients(c) {
    type = SUM;
    coefficients.insert(map.begin(), map.end());
    VariablePointers = vars;
}


Sum::Sum(std::unordered_map<int, coefType> map) {//:IntVariables(vars),coefficients(c) {
    type = SUM;
    coefficients.insert(map.begin(), map.end());
}
/// Used for objective and violation

Sum::Sum() {
    type = SUM;
    coefficients[-1] = 1;

}

Sum::~Sum() {
  
}

bool Sum::calculateDelta() {
    DeltaValue = 0;
    while (!VariableChange.empty()) {
        DeltaValue += VariableChange.back();
        VariableChange.pop_back();
    }
    if (DeltaValue + CurrentValue < lowerbound) {
        return false;
    } else if (DeltaValue + CurrentValue > upperbound) {
        return false;
    }
    return true;
}


void Sum::proposeChange(int variableNumber, coefType changeInValue) {
    coefType deltaChange = coefficients.at(variableNumber) * changeInValue;
    VariableChange.push_back(deltaChange);
}

void Sum::updateValue() {
    CurrentValue += DeltaValue;
    assert(CurrentValue >= lowerbound);
    assert(CurrentValue <= upperbound);

}

bool Sum::test() {
    double realValue = 0;
    for (Variable* iv : fixedVariablePointers) {
        unsigned id = iv->getID();
        double coef = coefficients.at(id);
//      
           int varValue = iv->getCurrentValue();
        realValue += varValue*coef;
    }
    
    
    for (Variable* iv : VariablePointers) {
        unsigned id = iv->getID();
        double coef = coefficients.at(id);
        int varValue;


        if (iv->isDef()) {
            varValue = iv->getOneway()->getCurrentValue();
            if (varValue < iv->getLowerBound()) {
                std::cout << "this should never happen, prob defined by wrong invariant (sum instead of max)" << std::endl;
                std::cout << "value of variable " << varValue << std::endl;
                debug;

            }
            if (iv->getCurrentValue() != varValue) {
            }
        } else {
            varValue = iv->getCurrentValue();
        }
        //        }
        realValue += varValue*coef;
    }
    for (invariant inv : InvariantPointers) {
        unsigned id = inv->getVariableID();

        double coef = coefficients.at(id);

        realValue += inv->getCurrentValue() * coef;
    }
    realValue += startValue;
    if (CurrentValue != realValue) {
        std::cout << "ID: " << getID() << " real value " << realValue << " current value " << CurrentValue << " variableID = "
                << variableID << " start value " << startValue << std::endl;
        std::cout << "coef size " << coefficients.size() << std::endl;
        std::cout << "variables/invariants " << VariablePointers.size() + InvariantPointers.size() + fixedVariablePointers.size()<< std::endl;
        debug;

    }

    if (realValue < 0 && this->getVariableID() != -1) {
        std::cout << "defining a variable but is negative" << std::endl;
        debug;
    }


    return true;
}