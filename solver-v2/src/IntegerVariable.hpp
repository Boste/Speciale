#ifndef INTEGERVARIABLE_HPP
#define	INTEGERVARIABLE_HPP
//#include <gecode/driver.hh>
#include <gecode/int.hh>

#include "Constants.hpp"
#include "Constraint.hpp"
//#include "BP_Data.hpp"
//#include "../include/constraints/invariant.hpp"

//class Constraint;
class Invariant;

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
    bool isFix = false;
    VariableInConstraints constraints;
    invariant oneway;
    constraint definedByCons;
    unsigned searchPrio = 0;
    //    propagation_queue propagationQueue;
    //    invariant definedByInvar; 

    /// Current Value of variable

    operator int () {
        return CurrentValue;
    }



    //    bool operator <(int asd){
    //        return CurrentValue < asd;
    //    }
    //    updateVector update;
    //    Gecode::IntVarArray* ArrayPointer;
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
    unsigned usedIn = 0;

    VariableInConstraints& usedInConstraints() {
        usedIn++;
        return constraints;
    }
    unsigned getSerachPriority(){
        return searchPrio;
    }
    
    /// Just used for testing but the counter for constraints is used.
    void addToUsedInConstraints(std::shared_ptr<Constraint> constraint) {
        constraints.push_back(constraint);
    }

    invariant getOneway() {
        return oneway;
    }

    void setDefinedBy(invariant invar, constraint cons) {
        definedByCons = cons;
        oneway = invar;
        isDefined = true;
    }

    bool isDef() {
        return isDefined;
    }

    bool isFixed() {
        return isFix;
    }

    void setAsFixed() {
        isFix = true;
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

    //    void addToUpdate(updateType invariant) {
    ////        update.push(invariant);//push(invariant);
    //        update.insert(update.end(),invariant);
    //    }
    //    
    //    updateVector& getUpdateVector() {
    //        return update;
    //    }

    unsigned getID() {
        return vectorID;
    }
    void undefine(){
        isDefined = false;
        definedByCons->isOneway(false);
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
    //    propagation_queue& getPropagationQueue(){
    //        return propagationQueue;
    //    }
    //    void addToPropagationQueue(invariant& invar){
    ////        propagationQueue.push(invar);
    //        propagationQueue.insert(invar);
    //       
    //    }

    ~IntegerVariable() {
        //        delete VariablePointer;

    }

    
    /// Sort in order of decreasing domain
    struct SortGreater {

        bool operator()(const IntegerVariable* var1, const IntegerVariable* var2) const {
                //            std::cout << "sorter" << std::endl;
            int ds1 = var1->upperBound-var1->lowerBound;
            int ds2 = var2->upperBound-var2->lowerBound;
            if(ds1==ds2){
                return (var1->constraints.size() > var2->constraints.size());
            }
            return (var1->upperBound-var1->lowerBound  > var2->upperBound-var2->lowerBound);
        }
    };
    
    /// Just for testing
    unsigned numberOfConstraints(){
        return constraints.size();
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

