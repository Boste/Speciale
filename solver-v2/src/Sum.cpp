#include "Sum.hpp"


Sum::Sum(std::vector<Variable*> vars, std::unordered_map<int, coefType> map) {//:IntVariables(vars),coefficients(c) {
    type = SUM;
    coefficients.insert(map.begin(), map.end());
    VariablePointers = vars;
}

//Sum::Sum(std::unordered_map<int, coefType> map, unsigned id)  {//:IntVariables(vars),coefficients(c) {

Sum::Sum(std::unordered_map<int, coefType> map) {//:IntVariables(vars),coefficients(c) {
    type = SUM;
    //    invariantID = id;
    coefficients.insert(map.begin(), map.end());
}
/// Used for objective and violation

Sum::Sum() {//:IntVariables(vars),coefficients(c) {
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

/// What if coef* change is double?

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
//        int varValue;


//        if (iv->isDef()) {
//            varValue = iv->getOneway()->getCurrentValue();
//            //            if(varValue <0){
//            if (varValue < iv->getLowerBound()) {
//                std::cout << "this should never happen, prob defined by wrong invariant (sum instead of max)" << std::endl;
////                std::cout << "is integer variable " << iv->isIntegerVariable() << std::endl;
//                std::cout << "value of variable " << varValue << std::endl;
//                debug;
//
//            }
//            if (iv->getCurrentValue() != varValue) {
//                //                std::cout << "should update the variable value according to the oneway defining it" << std::endl;
//            }
//        } else {
           int varValue = iv->getCurrentValue();
//        }
        //        if (test && varValue!=0) {
        //            std::cout << coef << "*"<< varValue  << " + ";
        //        }
        realValue += varValue*coef;
    }
    
    
    for (Variable* iv : VariablePointers) {
        unsigned id = iv->getID();
        double coef = coefficients.at(id);
        int varValue;


        if (iv->isDef()) {
            varValue = iv->getOneway()->getCurrentValue();
            //            if(varValue <0){
            if (varValue < iv->getLowerBound()) {
                std::cout << "this should never happen, prob defined by wrong invariant (sum instead of max)" << std::endl;
//                std::cout << "is integer variable " << iv->isIntegerVariable() << std::endl;
                std::cout << "value of variable " << varValue << std::endl;
                debug;

            }
            if (iv->getCurrentValue() != varValue) {
                //                std::cout << "should update the variable value according to the oneway defining it" << std::endl;
            }
        } else {
            varValue = iv->getCurrentValue();
        }
        //        if (test && varValue!=0) {
        //            std::cout << coef << "*"<< varValue  << " + ";
        //        }
        realValue += varValue*coef;
    }
    for (invariant inv : InvariantPointers) {
        unsigned id = inv->getVariableID();

        double coef = coefficients.at(id);

        realValue += inv->getCurrentValue() * coef;
        //        std::cout << inv->getCurrentValue() << std::endl;
    }
    //    if (test) {
    //        std::cout << startValue << std::endl;
    //        std::cout << "total value " << realValue << std::endl;
    //    }
    realValue += startValue;
    if (CurrentValue != realValue) {
        std::cout << "ID: " << getID() << " real value " << realValue << " current value " << CurrentValue << " variableID = "
                << variableID << " start value " << startValue << std::endl;
        
//        bool gotInt = false;
//        for (auto iv : VariablePointers) {
//            //            if(iv->isDef()){
//            //                std::cout << coefficients.at(iv->getID()) << "*" << iv->getOneway()->getCurrentValue() << " ";
//            //            } else {
//            //                std::cout << coefficients.at(iv->getID()) << "*" << iv->getCurrentValue() << " ";
//            //            }
//
////            if (iv->isIntegerVariable()) {
////                gotInt = true;
////            }
//        }
        //        std::cout << startValue << " = " << realValue << " current " << CurrentValue << std::endl; 
//        if (gotInt) {
//            std::cout << "Got integer variable" << std::endl;
//        }
        std::cout << "coef size " << coefficients.size() << std::endl;
        std::cout << "variables/invariants " << VariablePointers.size() + InvariantPointers.size() + fixedVariablePointers.size()<< std::endl;
        debug;

    }

    if (realValue < 0 && this->getVariableID() != -1) {
        std::cout << "defining a variable but is negative" << std::endl;
        debug;
    }


    //    double testValue = 0;

    //    for (unsigned i = 0; i < VariablePointers.size(); i++) {
    //        testValue += VariablePointers.at(i)->getCurrentValue() * coefficients.at(VariablePointers.at(i)->getID());
    //        testValue += VariablePointers.at(i)->getCurrentValue() * 1;
    //        std::cout << "Variable " << VariablePointers.at(i)->getID() << " val " << VariablePointers.at(i)->getCurrentValue() << " coef " << coefficientsBUG.at(VariablePointers.at(i)->getID()) << std::endl;
    //        std::cout << &coefficientsBUG.at(VariablePointers.at(i)->getID()) << std::endl;
    //    }
    //    for (auto it = coefficients.begin(); it != coefficients.end(); ++it)

    //        std::cout << " " << it->first << ":" << it->second;
    //    std::cout << "testValue: " << testValue << std::endl;

    //    sleep(5);
    //    if (testValue != CurrentValue) {
    //        std::cout << "Failed test in sum" << std::endl;
    //        std::cout << "testValue: " << testValue << "  CurrentValue: " << CurrentValue << std::endl;
    //        return false;
    //
    //    }
    return true;
}