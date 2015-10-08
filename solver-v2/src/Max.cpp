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

Max::Max(IntegerVariable* var, int integer, int id) {
    type = MAX;
    variableID = id;
    variableValue = var->getCurrentValue();
    Integer = integer;
    VariablePointers.push_back(var);
}

Max::Max(invariant invar, int integer, int id) {
    type = MAX;
    variableID = id;
    variableValue = invar->getCurrentValue();
    Integer = integer;
    invariants.push_back(invar);
}
//Max::Max(const Max& orig) {
//}

Max::~Max() {
}

void Max::addChange(int id, int change) {
    VariableChange.push_back(change);
    if (id != -1) {
        std::cout << "id should be -1" << std::endl;
    }

}

int Max::calculateDeltaValue() {
    int tmp1 = variableValue;
    //    int tmp2 = secondValue;
    while (!VariableChange.empty()) {
        int change = VariableChange.back();
        //        assert(change.first == T1 || change.second == T2);
        //        if (change.first == T1) {
        tmp1 += change;
        VariableChange.pop_back();
                //        } else {
                //            tmp2 += change.second;
                //        }
    }
    if (tmp1 > Integer) {
        DeltaValue = tmp1 - CurrentValue;
    } else {
        DeltaValue = Integer - CurrentValue;
    }

    return DeltaValue;
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