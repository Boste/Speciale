#include "Max.hpp"


//template<class T1, class T2>
//Max<T1, T2>::Max(T1 first, T2 sec, unsigned id) {
//Max<T1,T2>::Max(T1& first, T2& sec, int variableid) {
//    type=MAX;
//    variableID = variableid;
//    this->firstID = (int) first;
//    this->secondID = (int) sec;
//    this->firstValue 
//    if(this->first < second){
//        CurrentValue = second;
//    } else {
//        CurrentValue = this->first;
//    }
////    invariantID = id;
//}

//Max::Max(IntegerVariable* var, int second, unsigned id, std::shared_ptr<DependencyDigraph> DDG) {

Max::Max(IntegerVariable* var, int second, unsigned id) {
    type = MAX;
    variableID = id;
    variableValue = var->getCurrentValue();
    this->second = second;
    //    this->DDG = DDG;
    //    VariablePointers.push_back(var);
}

//Max::Max(invariant invar, int integer, int id) {

//Max::Max(invariant invar, int bound, unsigned id, std::shared_ptr<DependencyDigraph> DDG) {

Max::Max(invariant invar, int bound, unsigned id) {
    type = MAX;
    variableID = id;
    variableValue = invar->getCurrentValue();
    second = bound;
    this->invar = invar;
    //    this->DDG = DDG;
    //    invariants.push_back(invar);
}
//Max::Max(const Max& orig) {
//}

Max::~Max() {
}

void Max::addChange(int id, int change) {
    VariableChange.push_back(change);
//    assert(id == -1);
    //    if (id != -1) {
    //        std::cout << "id should be -1" << std::endl;
    //    }

}

bool Max::calculateDeltaValue() {
    firstChange = 0;
    DeltaValue = 0;
    //    std::cout << "lowerbound " << lowerbound << std::endl;
    //    debug;
    //        int tmp2 = secondValue;
    while (!VariableChange.empty()) {
        firstChange += VariableChange.back();
        //        assert(change.first == T1 || change.second == T2);
        //        if (change.first == T1) {
        VariableChange.pop_back();
        //        } else {
        //            tmp2 += change.second;
        //        }
        //        std::cout << "change for max " << change << std::endl;
    }
    if (firstChange + variableValue > second) {
        DeltaValue = firstChange + variableValue - CurrentValue;
    } else {
        DeltaValue = second - CurrentValue;
    }
    //    if (DeltaValue != 0) {
    //        for (updateType invar : DDG->getInvariantUpdate(this->invariantID)) {
    //            invar->addChange(this->getVariableID(), DeltaValue);
    //        }
    //    }
//    if (DeltaValue + CurrentValue < lowerbound) {
//        std::cout << "never the case i think" << std::endl;
//        debug;
//        return false;
//
//    }
//    if (DeltaValue + CurrentValue > upperbound) {
//        std::cout << "never the case i think" << std::endl;
//        debug;
//        return false;
//    }
//    if(variableValue != invar->getCurrentValue()){
//        std::cout << "this timestamp " << timestamp << " invar timestamp " << invar->getTimestamp() <<
//                " values: this " << variableValue << " invars " << invar->getCurrentValue() << std::endl;
//    }
//    assert(variableValue == invar->getCurrentValue());

//    assert(DeltaValue == std::max(firstChange + variableValue, second) - CurrentValue);
    //    if(DeltaValue == -CurrentValue || DeltaValue == firstChange){
    //        
    //    } else {
    //        std::cout << "DeltaValue " << DeltaValue << " currentValue " << CurrentValue << " firstChange " << firstChange << std::endl;
    //        debug;
    //    }
    return true;
}

void Max::updateValue() {
    CurrentValue += DeltaValue;
//    assert(CurrentValue >= 0);

    variableValue += firstChange;
//    test();
//    assert(CurrentValue == std::max(variableValue, 0));
//    assert(CurrentValue == std::max(0, invar->getCurrentValue()));
//    assert(variableValue <= CurrentValue);
//    assert(variableValue == invar->getCurrentValue());

    //    firstChange = 0;
    //    DeltaValue = 0;
}

bool Max::test() {
    if (CurrentValue == std::max(0, invar->getCurrentValue())) {
        return true;
    } else {
        return false;
    }
}
//template<class T1, class T2>
//Max<T1, T2>::~Max() {
//}
//
//template<class T1, class T2>
//void Max<T1, T2>::addChange(int id, int change) {
//    VariableChange.push_back(std::pair<int,int>(id,change));
//        
//
//}
//
//template<class T1, class T2>
//int Max<T1, T2>::calculateDeltaValue() {
//    int tmp1 = (int) T1 first;
//    int tmp2 = (int) T2 second;
//    while(!VariableChange.empty()){
//        std::pair<int,int> change = VariableChange.back();
//        assert(change.first == T1 || change.second == T2);
//        if(change.first == T1){
//            tmp1 += change.second;
//        } else {
//            tmp2 += change.second;
//        }
//    }
//    if(tmp1 > tmp2){
//        DeltaValue = tmp1 - CurrentValue;
//    } else {
//        DeltaValue = tmp2 - CurrentValue;
//    }
//    
//    return DeltaValue;
//}