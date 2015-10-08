#ifndef INTEGERVARIABLE_HPP
#define	INTEGERVARIABLE_HPP
//#include <gecode/driver.hh>
#include <gecode/int.hh>

#include "Constants.hpp"
//#include "../include/constraints/invariant.hpp"

//class Constraint;
//class Invariant;

class IntegerVariable {
    friend class GeneralSolver;
protected:
    //    Gecode::IntVar* VariablePointer;
    //    Gecode::IntVarArray* ArrayPointer;
    //    int ArrayID;
    int lowerBound;
    int upperBound;
    int vectorID;
    int CurrentValue = 0;
    bool isInteger = false;
    bool isDefined = false;
    VariableInConstraints constraints;
    invariant oneway;
    constraint definedByCons;
//    propagation_queue propagationQueue;
    std::set<Invariant*, compare_invariant> propagationQueue;
//    invariant definedByInvar;
    
    /// Current Value of variable
    operator int (){
        return CurrentValue;
    }
    
    
    
//    bool operator <(int asd){
//        return CurrentValue < asd;
//    }
//    updateVector update;
    std::set<Invariant*, compare_invariant> update;
    Gecode::IntVarArray* ArrayPointer;
    Gecode::IntVar* VariablePointer;

//    void clearUpdateVector() {
////        for(InvariantContainer invars : update){
////            invars.clear();
////            invars.resize(0);
////        }
//        
//        update.clear();
//        update.resize(0);
//    }


public:
    /// Only used for testing instances
    int usedIn = 0;
    VariableInConstraints& usedInConstraints(){
        return constraints;
    }
    void addToUsedInConstraints(std::shared_ptr<Constraint> constraint){
        constraints.push_back(constraint);
    }
    invariant getOneway(){
        return oneway;
    }
    void setDefinedBy(invariant invar, constraint cons){
        definedByCons = cons;
        oneway = invar;
        isDefined = true;
    }

    IntegerVariable(int lowerbound, int upperbound, int id) { //: lb(lowerbound), ub(upperbound),vectorID(id),value(0) {
        //        VariablePointer = vp;
        //        ArrayPointer = ap;
        lowerBound = lowerbound;
        upperBound = upperbound;
        if (upperbound > 1 || lowerbound < 0) {
            isInteger = true;
        }
        vectorID = id;
    }

    void setCurrentValue(int val) {
        CurrentValue = val;

    }

    bool isIntegerVariable() {
        return isInteger;
    }

    int getCurrentValue() {
        return CurrentValue;
    }

    void addToUpdate(updateType invariant) {
//        update.push(invariant);//push(invariant);
//        update.insert(update.end(),invariant);
        update.insert(update.end(),invariant);
    }
    
    updateVector& getUpdateVector() {
        return update;
    }

    int getID() {
        return vectorID;
    }

    Gecode::IntVar* getVariablePointer() {
        return VariablePointer;
    }

    void setVariablePointer(Gecode::IntVar& gecodeVar) {
        VariablePointer = &gecodeVar;
    }

//    Gecode::IntVarArray* getArrayPointer() {
//        return ArrayPointer;
//    }

    int getLowerBound() {
        return lowerBound;
    }

    int getUpperBound() {
        return upperBound;
    }
    propagation_queue& getPropagationQueue(){
        return propagationQueue;
    }
    void addToPropagationQueue(invariant& invar){
//        propagationQueue.push(invar);
//        propagationQueue.insert(invar);
        propagationQueue.insert(invar.get());
       
    }
    ~IntegerVariable() {
        //        delete VariablePointer;

    }

    //    IntegerVariable(const IntegerVariable& orig);
    //    virtual ~IntegerVariable();
private:
    //    void updateInvariants(){
    //        for (int i = 0; i < update.size(); i++ ){
    //            int invariantNumber = update.at(i);
    //            // Make the change
    //        }
    //    }

};

#endif	/* INTEGERVARIABLE_HPP */

