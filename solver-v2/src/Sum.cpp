#include "Sum.hpp"

//Sum::Sum(std::vector<IntegerVariable*>& vars, std::vector<int>& c, unsigned id)  {
//    type = SUM;
//    invariantID = id;
//    //    if(c->size()!=vars->size()){
//    //        std::cout << c->size() << " " <<vars->size() << std::endl;
//    //    }
//    assert(c.size() == vars.size());
//    for (unsigned i = 0; i < vars.size(); i++) {
//        //        coefficients.insert(std::make_pair(vars->at(i)->getID(), c->at(i)));
//        coefficients[vars.at(i)->getID()] = c.at(i);
//    }
//    VariablePointers = vars;
//    //        std::cout << std::endl;
//}

/// Construct that copies a Coefficient map. Size can be different when this sum contains invariants

Sum::Sum(std::vector<Variable*> vars, std::unordered_map<int, coefType> map) {//:IntVariables(vars),coefficients(c) {
    type = SUM;
    //    invariantID = id;
    //    assert(map.size() == vars.size());
    coefficients.insert(map.begin(), map.end());
    VariablePointers = vars;
    //
    //    //        std::cout << std::endl;
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
    //    invariantID = id;
    coefficients[-1] = 1;
    //    coefficients.insert(map.begin(), map.end());

}

//Sum::Sum(const Sum &a) : Invariant(a) {
//    //        std::cout << "copy constructor2" << std::endl;
//    this->coefficients = a.coefficients;
//    this->VariableChange = a.VariableChange;
//    //    this->VariablePointers = a.VariablePointers;
//    //        sleep(5000);
//}

Sum::~Sum() {
    //    delete &coefficients;
    //    delete &VariableChange;
    //    std::cout << "Delete Sum" << std::endl;
    //    VariablePointers->shrink_to_fit();
    //    VariablePointers->clear();
    //    std::vector<IntegerVariable*>().swap(VariablePointers);
    //    delete VariablePointers;
}

//Sum& Sum::operator=(const Sum &a) {
//    if (this != &a) {
//        this->coefficients = a.coefficients;
//
//        //        std::cout << "operator =" << std::endl;
//        //        sleep(5);
//        //        this->VariablePointers = a.VariablePointers;
//
//        this->VariableChange = a.VariableChange;
//    }
//    return *this;
//}

bool Sum::calculateDeltaValue() {
    //    int valueChange = 0;
    DeltaValue = 0;
    //    std::cout << "Change added " << changeAdd << std::endl;
    //    std::cout << "calc delta val. Size " << VariableChange.size() << std::endl;

    //    std::cout << startValue << " ";
    //    int id = variableID;
    //    if (id == 53552 || id == 110133 || id == 119734) {
    //        std::cout << "changed variables " << VariableChange.size() << std::endl;
    //    }
    //
    //    if (id == 53552) {
    //        while (!VariableChange.empty()) {
    //            //        std::cout << " + " << VariableChange.back();
    //            std::cout << VariableChange.back() << " ";
    //            valueChange += VariableChange.back();
    //            //        std::pair<int, int> currentPair = VariableChange.back();
    //            //        valueChange += coefficients.at(currentPair.first) * currentPair.second;
    //
    //            VariableChange.pop_back();
    //        }
    //        std::cout << std::endl;
    //        std::cout << "valueChange " << valueChange << std::endl;
    //    } else {
    while (!VariableChange.empty()) {
        //        std::cout << " + " << VariableChange.back();
        //        valueChange += VariableChange.back();
        DeltaValue += VariableChange.back();
        //        std::pair<int, int> currentPair = VariableChange.back();
        //        valueChange += coefficients.at(currentPair.first) * currentPair.second;
        //        std::cout << "value change " << valueChange << std::endl;
        VariableChange.pop_back();
    }
    //    if (valueChange != 0) {
    //        for (updateType invar : DDG->getInvariantUpdate(this->invariantID)) {
    //            invar->addChange(this->getVariabl eID(), valueChange);
    //        }
    //    }

    //    }
    //    if(invariantID == 253122){
    //        std::cout <<"changed delta of obj func " << valueChange << std::endl;
    //    }
    //        std::cout << valueChange << " ";
    //    std::cout << "Delta Value " << DeltaValue << " " << "Current value " << CurrentValue << std::endl;
    //    std::cout << "lowerbound " << lowerbound << " variable id  "<< variableID << std::endl;
    //    std::cout << "is used by constraint " << usedByConstraint << " ";
    //    if(variableID != -1){
    //        if(lowerbound < 0){
    //            std::cout << "forgot to define lowerbound" << std::endl;
    //        }
    //        assert(lowerbound >= 0);

    if (DeltaValue + CurrentValue < lowerbound) {
        //        DeltaValue = 0;
        //        if (DeltaValue < 0) {
        return false;
        //        }
        //            std::cout << "should not be a legal move" << std::endl;
        //            std::cout << "delta + currentvalue = " << DeltaValue + CurrentValue << std::endl; 
        //            std::cout << "variable id " << variableID << std::endl;
        //            
    }
    if (DeltaValue + CurrentValue > upperbound) {
        //        DeltaValue = 0;
        //        if (DeltaValue > 0) {
        return false;
        //        }
    }
    //    DeltaValue = valueChange;


    //        assert(DeltaValue+CurrentValue >= lowerbound);
    //    }


    //    if (CurrentValue + valueChange < lowerbound) {
    //        
    //        return lowerbound - CurrentValue;
    //    }
    return true;
}

/// What if coef* change is double?

void Sum::proposeChange(int variableNumber, int changeInValue) {
    //    std::cout << variableNumber << " ";

    int deltaChange = coefficients.at(variableNumber) * changeInValue;

    //    if (deltaChange < -9 || deltaChange > 10) {
    //        std::cout << "Delta change in addChange " << deltaChange << std::endl;
    //        std::cout <<  coefficients.at(variableNumber) << "*" << changeInValue << std::endl;
    //        debug;
    //    }


    VariableChange.push_back(deltaChange);
    //    changeAdd = true;


    //    VariableChange.push_back(std::pair<int, int> (variableNumber, changeInValue));

    //        std::cout << "addChange in sum" << std::endl;
    //        sleep(5000);
}

void Sum::updateValue() {

    //    if (DeltaValue < -9 || DeltaValue > 10) {
    //        std::cout << "DeltaValue " << DeltaValue << std::endl;
    //    }
    CurrentValue += DeltaValue;
    assert(CurrentValue >= lowerbound);
    assert(CurrentValue <= upperbound);

    //    std::cout << "made move" << std::endl;
    //    test();

    //    DeltaValue = 0;
}

//bool Sum::calculateValue() {
//    double realValue = 0;
//    //    bool test = false;
//    //    if (variableID == 256) {
//    //        test = true;
//    //    }
//    for (Variable* iv : VariablePointers) {
//        unsigned id = iv->getID();
//        double coef = coefficients.at(id);
//        int varValue;
//
//
//        if (iv->isDef()) {
//            varValue = iv->getOneway()->getCurrentValue();
//            //            if(varValue <0){
//            if (varValue < iv->getLowerBound()) {
//                std::cout << "this should never happen, prob defined by wrong invariant (sum instead of max)" << std::endl;
//                std::cout << "is integer variable " << iv->isIntegerVariable() << std::endl;
//                std::cout << "value of variable " << varValue << std::endl;
//                debug;
//
//            }
//            if (iv->getCurrentValue() != varValue) {
//                //                std::cout << "should update the variable value according to the oneway defining it" << std::endl;
//            }
//        } else {
//            varValue = iv->getCurrentValue();
//        }
//        //        if (test && varValue!=0) {
//        //            std::cout << coef << "*"<< varValue  << " + ";
//        //        }
//        realValue += varValue*coef;
//    }
//    for (invariant inv : InvariantPointers) {
//        unsigned id = inv->getVariableID();
//
//        double coef = coefficients.at(id);
//
//        realValue += inv->getCurrentValue() * coef;
//        //        std::cout << inv->getCurrentValue() << std::endl;
//    }
//    //    if (test) {
//    //        std::cout << startValue << std::endl;
//    //        std::cout << "total value " << realValue << std::endl;
//    //    }
//    realValue += startValue;
//    return true;
//
//}
/// update currentValue by adding currentValue*coeff of all variables and invariants 

//void Sum::initialize() {
//    for (IntegerVariable* iv : VariablePointers) {
//        CurrentValue += iv->getCurrentValue()*coefficients.at(iv->getID());
//    }
//    for(invariant invar : invariants){
//        int id = invar->getVariableID();
//        CurrentValue += invar->getCurrentValue()*coefficients.at(id);
//    }
//}

//void Sum::setUsedByConstraint(int constraint) {
//    usedInConstraintNr = constraint;
//}
//
//void Sum::usedByObjective(int objective) {
//    usedInObjectiveNr = objective;
//
//
//}

//void Sum::addChange(std::vector<int> variableNumbers, std::vector<int> changes) {
//    std::cout << "Ikke implementeret" << std::endl;
//}
//
//void Sum::addChangeSwap(int variableNumber1, int variableNumber2) {
//    std::cout << "Ikke implementeret" << std::endl;
//
//}

bool Sum::test() {
    double realValue = 0;
    //    bool test = false;
    //    if (variableID == 256) {
    //        test = true;
    //    }
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
        std::cout << "variables/invariants " << VariablePointers.size() + InvariantPointers.size() << std::endl;
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