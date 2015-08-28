#ifndef INTEGERVARIABLE_HPP
#define	INTEGERVARIABLE_HPP
//#include <gecode/driver.hh>
#include <gecode/int.hh>
//#include "../include/constraints/invariant.hpp"

class IntegerVariable {
protected:
    //    Gecode::IntVar* VariablePointer;
    //    Gecode::IntVarArray* ArrayPointer;
    //    int ArrayID;
    int lb;
    int ub;
    int value=0;
    std::vector<int> update;
    Gecode::IntVarArray* ArrayPointer;
    Gecode::IntVar* VariablePointer;

    int vectorID;
public:
    IntegerVariable(int lowerbound, int upperbound,int id) : lb(lowerbound), ub(upperbound),vectorID(id),value(0) {
//        VariablePointer = vp;
//        ArrayPointer = ap;
        vectorID = id;
        value = 0;
    }
    void setCurrentValue(int val) {
        value = val;
        
    }

    int getCurrentValue() {
        return value;
    }

    void addToUpdate(int invariantNumber) {
        update.push_back(invariantNumber);
    }

    std::vector<int>* getUpdateVector() {
        return &update;
    }

    int getID() {
        return vectorID;
    }

    Gecode::IntVar* getVariablePointer() {
        return VariablePointer;
    }
    void setVariablePointer(Gecode::IntVar* gecodeVar) {
        VariablePointer = gecodeVar;
    }

    Gecode::IntVarArray* getArrayPointer() {
        return ArrayPointer;
    }
    ~IntegerVariable(){
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

