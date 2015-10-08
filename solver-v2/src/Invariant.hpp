#ifndef INVARIANT_HPP
#define	INVARIANT_HPP
#include "IntegerVariable.hpp"
#include <unordered_map>
#include <iostream>
#include "Constants.hpp"
//class Constraint;

class Invariant;
struct compare_invariant : public std::binary_function<Invariant*, Invariant*, bool> {
//
//   
//
    bool operator<(const Invariant& invar) const {
        std::cout << "used to sort1 <" << std::endl;
//        debug;
        return (this->operator <(invar));
    }
//
    bool operator>(const Invariant& invar) const {
        std::cout << "used to sort2 >" << std::endl;
//        debug;
        return (this->operator >(invar));
    }
//    bool operator<(const invariant invar) const {
//        std::cout << "used to sort1 <" << std::endl;
////        debug;
//        return (this->operator <(invar));
//    }
////
//    bool operator>(const invariant invar) const {
//        std::cout << "used to sort2 >" << std::endl;
////        debug;
//        return (this->operator >(invar));
//    }
     bool operator()(const Invariant* invar1, const Invariant* invar2) const {
//        std::cout << "compare " << (*invar1 < *invar2) << std::endl;
        
        return (invar1 < invar2);
    }
};
class Invariant {
    friend class GeneralSolver;

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

    virtual void addChange(int variableNumber, int change) {
        std::cout << "addChange in invariant" << std::endl;
        std::cout << variableNumber << " " << change << std::endl;
    }

    virtual int calculateDeltaValue() {
        std::cout << "CalculateDeltaValue in  invariant" << std::endl;
        return 0;
    }

    virtual void initialize() {

    }

    int getDeltaValue() {
        return DeltaValue;
    }

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

    int getCurrentValue() {

        return CurrentValue;
    }

    void updateValue() {
        //        std::cout << "current + delta " << CurrentValue << " + " << DeltaValue << " + " << " = " << CurrentValue + DeltaValue << std::endl;
        CurrentValue += DeltaValue;
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
        for (updateType invar : update) {
            //            std::cout << invar->getVariableID() << std::endl;

            //            std::cout << invar->coefficients.size() << std::endl;
            if (variableID == -1 && invar->type != MAX) {
                std::cout << "should not be here" << std::endl;
                //                std::cout << usedInObjectiveNr << std::endl;
                //                std::cout << usedInConstraintNr << std::endl;
                std::cout << update.size() << std::endl;
                exit(1);
            }
            invar->updateValue();
            //            invar->addChange(variableID, DeltaValue);
        }
    }

    // should be pointer instead of integers

    //    void setUsedByConstraint(int cons, int priority) {

    void setUsedByConstraint(constraint cons, int priority) {
        //        std::cout << "ehhh?" << std::endl;
        usedInConstraint = cons;
        //        std::cout << "constraint" << std::endl;
        constraintPriority = priority;
        usedByConstraint = true;
    }

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

    constraint getConstraint() {
        return usedInConstraint;
    }

    // should be pointer instead of integers

    //    int getUsedInObjective() {
    //        return usedInObjectiveNr;
    //    }

    unsigned getPriority() {
        return constraintPriority;
    }

    int getType() {
        return type;
    }

//    updateVector& getUpdateVector() {
    std::set<Invariant*, compare_invariant>& getUpdateVector() {
        return update;
    }

    void addToUpdate(updateType invar) {
        update.insert(invar);
        //        update.push(invar);
        //        update.push_back(invar);
    }

    /// Varibles defining this invariant 

    std::vector<IntegerVariable*>& getVariables() {
        return VariablePointers;
    }
    /// Invariants defining this invariant

    InvariantContainer& getInvariants() {
        return invariants;
    }

    /// not passing by value
    /// Should only be used for testing i think

    std::unordered_map<int, coefType>& getCoefficients() {
        return coefficients;
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

    unsigned getID() {
        return invariantID;
    }
    /// add invariant that defines this invariant

    void addInvariant(invariant invar) {
        invariants.push_back(invar);
    }
    /// add variable that defines this invariant

    void addVariable(IntegerVariable* var) {
        VariablePointers.push_back(var);
    }
    ///Only when making invariants

    void setBounds(double lb, double ub) {
        lowerbound = lb;
        upperbound = ub;
    }

    bool operator<(const invariant invar) {
        std::cout << "used to sort1 <" << std::endl;
        debug;
        return (this->getID() < invar->getID());
    }

    bool operator>(const invariant invar) {
        std::cout << "used to sort2 <" << std::endl;
        debug;
        return (this->getID() > invar->getID());
    }

    bool operator<( const Invariant& invar) const {
        std::cout << "used to sort1 <" << std::endl;
        unsigned i1 = invariantID;
        unsigned i2 = invar.invariantID;
        debug;
        return i1<i2;
//        return (getID() < invar.getID());
    }
    bool operator>( const Invariant& invar) const {
        std::cout << "used to sort1 <" << std::endl;
        unsigned i1 = invariantID;
        unsigned i2 = invar.invariantID;
        debug;
        return i1>i2;
//        return (getID() < invar.getID());
    }

//    bool operator>(const Invariant& invar) const {
//        std::cout << "used to sort2 <" << std::endl;
//        debug;
//        return (this->getID() > invar.getID());
//    }

    bool operator==(const invariant invar) {
        std::cout << "used to sort3 <" << std::endl;
        debug;
        return (this == invar.get());
        //        return (this->getID() == invar->getID());
    }



protected:
    std::vector<IntegerVariable*> VariablePointers;
    //    int value = 0;
    unsigned invariantID;
    int DeltaValue = 0;
    unsigned constraintPriority;
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
    InvariantContainer invariants;
//    updateVector update;
    std::set<Invariant*, compare_invariant> update;
    //    bool changeAdd = false;

private:

};


//struct compare_invariant : public std::binary_function<invariant, invariant, bool> {
//
//    bool operator()(const invariant invar1, const invariant invar2) const {
////        std::cout << "is used" << std::endl;
//        return (invar1 < invar2);
//    }
//
//    bool operator<(const invariant invar) const {
////        std::cout << "used to sort1 <" << std::endl;
//        return (this->operator <(invar));
//    }
//
//    bool operator>(const invariant invar) const {
////        std::cout << "used to sort2 >" << std::endl;
//        return (this->operator >(invar));
//    }
//};


//typedef std::set<invariant, compare_invariant> propagation_queue;
//typedef std::set<invariant, compare_invariant> updateVector;




//struct compare_invariant : public  std::binary_function<invariant,invariant,bool> {
//    bool operator()(invariant invar1, invariant invar2){
//        
//        return (invar1 > invar2); 
//    }
//};
//class compare_invariant
//{
//  bool reverse;
//public:
//  mycomparison(const bool& revparam=false)
//    {reverse=revparam;}
//  bool operator() (invariant& lhs, invariant&rhs) const
//  {
//    if (reverse) return (lhs->getID()>rhs->getID());
//    else return (lhs->getID()<rhs->getID());
//  }
//};

#endif	/* INVARIANT_HPP */

