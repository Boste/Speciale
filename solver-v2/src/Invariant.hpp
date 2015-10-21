#ifndef INVARIANT_HPP
#define	INVARIANT_HPP
#include "IntegerVariable.hpp"
#include <unordered_map>
#include <iostream>
#include "Constants.hpp"
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
        //        std::cout << "invariant" << std::endl;
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

    virtual void addChange(int ID, int change) {
        std::cout << "addChange in invariant" << std::endl;
        std::cout << ID << " " << change << std::endl;
    }

    /// Computes the deltavalue of this Invariant based on the vector of changes (Maybe that vector should be moved here)

    virtual int calculateDeltaValue() {
        std::cout << "CalculateDeltaValue in  invariant" << std::endl;
        return -12000;
    }
    /// Not used but should maybe be used.

    virtual void initialize() {

    }
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

    virtual void updateValue() {
//        if (invariantID == 253122) {
//            std::cout << "Change value of obj func " << CurrentValue << " + " << DeltaValue << " = " << CurrentValue + DeltaValue << std::endl;
//        }
        std::cout << "in invariant" << std::endl;
//        CurrentValue += DeltaValue;
//        DeltaValue = 0;

        //        int realChange;
        //        if (value < lowerbound) {
        //            realChange = CurrentValue - lowerbound;
        //            CurrentValue = lowerbound;
        //
        //        } else {
        //            CurrentValue = value;
        //            realChange = DeltaValue;
        //        }
        //        CurrentValue += DeltaValue;
        //        for (updateType invar : update) {
        //            //            std::cout << invar->getVariableID() << std::endl;
        //
        //            //            std::cout << invar->coefficients.size() << std::endl;
        //            if (variableID == -1 && invar->type != MAX) {
        //                std::cout << "should not be here" << std::endl;
        //                //                std::cout << usedInObjectiveNr << std::endl;
        //                //                std::cout << usedInConstraintNr << std::endl;
        ////                std::cout << update.size() << std::endl;
        //                exit(1);
        //            }
        //            invar->updateValue();
        //            invar->addChange(variableID, DeltaValue);
        //        }
    }

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
    // should be pointer instead of integers

    //    void setUsedByObjective(int objective) {
    //        usedInObjectiveNr = objective;
    //        constraintPriority = 0;
    //  
    //
    //    }

    // should be pointer instead of integers

    //    int getConstraint() {
    //        return usedInConstraint;
    //    } 

    /// Return the constraint this invariant is used by

    constraint getConstraint() {
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
        }
        assert(usedByConstraint || constraintPriority == 0);
        return constraintPriority;
    }

    int getType() {
        return type;
    }

    //    updateVector& getUpdateVector() {
    //    std::set<Invariant*, compare_invariant>& getUpdateVector() {
    //        return update;
    //    }

    //    void addToUpdate(updateType invar) {
    //        update.insert(invar);
    //        //        update.push(invar);
    //        //        update.push_back(invar);
    //    }

    /// Varibles defining this invariant 

    //    std::vector<IntegerVariable*>& getVariables() {
    //        return VariablePointers;
    //    }
    /// Invariants defining this invariant

    //    InvariantContainer& getInvariants() {
    //        return invariants;
    //    }

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

    void setValue(int value) {
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
    /// add invariant that defines this invariant

    //    void addInvariant(invariant invar) {
    //        invariants.push_back(invar);
    //    }
    /// add variable that defines this invariant

    //    void addVariable(IntegerVariable* var) {
    //        VariablePointers.push_back(var);
    //    }
    ///Only when making invariants (if not called lb = integer min +1 and ub = integer max -1, (gecode limits))

    void setBounds(double lb, double ub) {
        lowerbound = lb;
        upperbound = ub;
    }

    //    bool operator<(const invariant invar) {
    //        std::cout << "used to sort1 <" << std::endl;
    //        debug;
    //        return (this->getID() < invar->getID());
    //    }
    //
    //    bool operator>(const invariant invar) {
    //        std::cout << "used to sort2 <" << std::endl;
    //        debug;
    //        return (this->getID() > invar->getID());
    //    }
    //
    //    bool operator<( const Invariant& invar) const {
    //        std::cout << "used to sort1 <" << std::endl;
    //        unsigned i1 = invariantID;
    //        unsigned i2 = invar.invariantID;
    //        debug;
    //        return i1<i2;
    ////        return (getID() < invar.getID());
    //    }
    //    bool operator>( const Invariant& invar) const {
    //        std::cout << "used to sort1 <" << std::endl;
    //        unsigned i1 = invariantID;
    //        unsigned i2 = invar.invariantID;
    //        debug;
    //        return i1>i2;
    ////        return (getID() < invar.getID());
    //    }
    //
    ////    bool operator>(const Invariant& invar) const {
    ////        std::cout << "used to sort2 <" << std::endl;
    ////        debug;
    ////        return (this->getID() > invar.getID());
    ////    }
    //
    //    bool operator==(const invariant invar) {
    //        std::cout << "used to sort3 <" << std::endl;
    //        debug;
    //        return (this == invar.get());
    //        //        return (this->getID() == invar->getID());
    //    }



protected:
    //    std::vector<IntegerVariable*> VariablePointers;
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
    int startValue = 0;
    int lowerbound = std::numeric_limits<int>::min() + 1;
    int upperbound = std::numeric_limits<int>::max() - 1;
    int CurrentValue = 0;

    int variableID = -1;
    //    InvariantContainer invariants;
    //    updateVector update;
    //    std::set<Invariant*, compare_invariant> update;
    //    bool changeAdd = false;

private:

};


#endif	/* INVARIANT_HPP */

