#ifndef INVARIANT_HPP
#define	INVARIANT_HPP
#include "IntegerVariable.hpp"
#include <unordered_map>
#include <iostream>
#include "Constants.hpp"
#include <limits>
#include <cassert>
//class Constraint;

//class Invariant;
//struct compare_invariant : public std::binary_function<Invariant*, Invariant*, bool> {
////
////   
////
//    bool operator<(const Invariant& invar) const {
//        std::cout << "used to sort1 <" << std::endl;
////        debug;
//        return (this->operator <(invar));
//    }
////
//    bool operator>(const Invariant& invar) const {
//        std::cout << "used to sort2 >" << std::endl;
////        debug;
//        return (this->operator >(invar));
//    }
////    bool operator<(const invariant invar) const {
////        std::cout << "used to sort1 <" << std::endl;
//////        debug;
////        return (this->operator <(invar));
////    }
//////
////    bool operator>(const invariant invar) const {
////        std::cout << "used to sort2 >" << std::endl;
//////        debug;
////        return (this->operator >(invar));
////    }
//     bool operator()(const Invariant* invar1, const Invariant* invar2) const {
////        std::cout << "compare " << (*invar1 < *invar2) << std::endl;
//        
//        return (invar1 < invar2);
//    }
//};

class Invariant {
    //    friend class GeneralSolver;
    friend class Model;

public:
    //    bool changeAdd = false;

    Invariant() {
        //                std::cout << "invariant" << std::endl;
    }

    Invariant(const Invariant &a) {
        std::cout << &a << std::endl;

    }
    //    virtual void commit() = 0;
    //    {
    //        std::cout << "commit in invariant" << std::endl;
    //    }


    // should be pointers instead of integers
    /// Tells this Invariant that deltavalue of invariant or variable with ID is changed by change

    virtual void addChange(int ID, int change) = 0;

    /// Computes the deltavalue of this Invariant based on the vector of changes (Maybe that vector should be moved here)

    virtual bool calculateDeltaValue() =0;
    /// Not used but should maybe be used.

//    virtual void initialize() =0;
    /// Return the posible change of value. Never resets but gets recomputed by CalculateDeltaValue() based on new changes and current value.

    int getDeltaValue() {
        return DeltaValue;
    }
    /// Not working

    virtual bool test() {
        std::cout << "test in invariant" << std::endl;
        return false;
    }

    virtual ~Invariant() {
        //        std::cout << "destruct invariant" << std::endl;

        //        for(IntegerVariable* IV : *VariablePointers){
        //            delete IV;
        //        }
        //        delete VariablePointers;
    }
    /// Return currnet value of this Invariant

    int getCurrentValue() {

        return CurrentValue;
    }
    /// Set Current value = Current Value + Delta value (should only be called after recomputing the delta value)

    virtual void updateValue() =0;

    // should be pointer instead of integers

    //    void setUsedByConstraint(int cons, int priority) {

    void setObjective() {
        constraintPriority = 0;
        usedByConstraint = true;


    }
    /// Set which constraints this Invariant is used by. Currently only possible to be used by one constraint

    void setUsedByConstraint(constraint cons, int priority) {
        //        std::cout << "ehhh?" << std::endl;
        usedInConstraint = cons;
        //        std::cout << "constraint" << std::endl;
        constraintPriority = priority;
        usedByConstraint = true;
    }
    /// Return wether this invariant is used by a constraint

    bool isUsedByConstraint() {
        return usedByConstraint;
    }


    /// Return the constraint this invariant is used by

    constraint& getConstraint() {
        return usedInConstraint;
    }

    // should be pointer instead of integers

    //    int getUsedInObjective() {
    //        return usedInObjectiveNr;
    //    }
    /// The priority of the constraint this invariant is used by (if any otherwise fails assert)

    unsigned getPriority() {
        if (!usedByConstraint) {
            std::cout << invariantID << " " << constraintPriority << std::endl;
            debug;
        }
        assert(usedByConstraint || constraintPriority == 0);
        return constraintPriority;
    }

    int getType() {
        return type;
    }

    /// not passing by value
    /// Should only be used for testing i think

    std::unordered_map<int, coefType>& getCoefficients() {
        return coefficients;
    }

    /// The id of the variable that this invariant defines

    void setVariableID(unsigned id) {
        variableID = id;
    }
    /// Only used when the invariant defines a variable through a oneway constraint

    int getVariableID() {
        return variableID;
    }

    /// Correspond to the priority the invariant should be updated. 
    //    unsigned getLayer() {
    //        return layer;
    //    }
    /// Not sure this is relevant after Max invariant is introduced.

    int getStartValue() {
        return startValue;
    }

    /// only for start value

    void setValue(double value) {
        CurrentValue = value;
    }

    void setStartValue(double value) {
        startValue = value;
        //        this->value = value;
        //        if (value < lowerbound) {
        //            CurrentValue = lowerbound;
        //        } else {
        CurrentValue = value;
        //        }

    }
    /// Return the ID of this invariant (not the variable it defines if any)

    unsigned getID() {
        return invariantID;
    }

    ///Only when making invariants (if not called lb = integer min +1 and ub = integer max -1, (gecode limits))

    void setBounds(double lb, double ub) {
        lowerbound = lb;
        upperbound = ub;
    }

    /// Only used for testing

    void setVariablePointers(std::vector<IntegerVariable*>& vars) {
        VariablePointers = vars;
    }

    std::vector<IntegerVariable*>& getVariablePointers() {
        return VariablePointers;
    }

    unsigned getTimestamp() {
        return timestamp;
    }

    void setTimestamp(unsigned time) {
        timestamp = time;
    }


protected:
    std::vector<IntegerVariable*> VariablePointers;
    //    int value = 0;
    unsigned invariantID;
    int DeltaValue = 0;
    unsigned constraintPriority = 0;
    //    int usedInConstraintNr;
    //    int usedInObjectiveNr;
    //    int usedInConstraint;
    constraint usedInConstraint;
    bool usedByConstraint = false;
    std::unordered_map<int, coefType> coefficients;
    int type;
    /// Should be defined when creating oneway constraints that define (integer)variables 
    double startValue = 0;
    int lowerbound = std::numeric_limits<int>::min() + 1;
    int upperbound = std::numeric_limits<int>::max() - 1;
    int CurrentValue = 0;
    unsigned timestamp;

    int variableID = -1;
    //    InvariantContainer invariants;
    //    updateVector update;
    //    std::set<Invariant*, compare_invariant> update;
    //    bool changeAdd = false;

private:

};


#endif	/* INVARIANT_HPP */

