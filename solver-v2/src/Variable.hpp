#ifndef VARIABLE_HPP
#define	VARIABLE_HPP
#include <gecode/int.hh>


#include "Constraint.hpp"
#include "Constants.hpp"

class Invariant;

class Variable {
    friend class GPSolver;
protected:
    int lowerBound;
    int upperBound;
    int vectorID;
    unsigned defining = 0;
    int CurrentValue = 0;
    bool isDefined = false;
    bool isFix = false;
    invariant oneway;
    Constraint* definedByCons;
    unsigned searchPrio = 0;
    bool inObjective = false;

    operator int () {
        return CurrentValue;
    }

    Gecode::BoolVar* VariablePointer;
    unsigned usedIn = 0;


public:

    unsigned getDefining() {
        return defining;
    }

    void increaseDefining() {
        defining++;
    }

    void decreaseDefining() {
        defining--;
    }

    unsigned getSerachPriority() {
        return searchPrio;
    }

    void addToUsedInConstraints() {
        usedIn++;
    }

    invariant getOneway() {
        return oneway;
    }

    void setDefinedBy(invariant invar, Constraint* cons) {
        definedByCons = cons;
        oneway = invar;
        isDefined = true;
    }

    /// Only for testing

    Constraint* getDefinedByCon() {
        return definedByCons;
    }

    bool isDef() {
        return isDefined;
    }

    bool isFixed() {
        return isFix;
    }

    void setAsFixed() {
        isFix = true;
        lowerBound = VariablePointer->val();
        upperBound = VariablePointer->val();
        CurrentValue = VariablePointer->val();
    }

    Variable(int lowerbound, int upperbound, int id) { //: lb(lowerbound), ub(upperbound),vectorID(id),value(0) {
        lowerBound = lowerbound;
        upperBound = upperbound;
        vectorID = id;
    }

    void setCurrentValue(int val) {
        CurrentValue = val;

    }

    int getCurrentValue() {
        return CurrentValue;
    }

    unsigned getID() {
        return vectorID;
    }

    void undefine() {

        isDefined = false;
        definedByCons->isOneway(false);
    }

    Gecode::BoolVar* getVariablePointer() {
        return VariablePointer;
    }

    void setVariablePointer(Gecode::BoolVar& gecodeVar) {
        VariablePointer = &gecodeVar;
    }

    int getLowerBound() {
        return lowerBound;
    }

    int getUpperBound() {
        return upperBound;
    }

    ~Variable() {

    }

    bool isInObjective() {
        return inObjective;
    }


    struct compare_variable : public std::binary_function<Variable*, Variable*, bool> {

        bool operator()(Variable* var1, Variable* var2) {

            return (var1->getID() > var2->getID());
        }

    };


    /// Just for testing

    unsigned getDegree() {
        return usedIn;      
    }
private:

};

#endif	/* VARIABLE_HPP */

