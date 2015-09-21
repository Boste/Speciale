#ifndef INVARIANT_HPP
#define	INVARIANT_HPP
#include "IntegerVariable.hpp"
#include <unordered_map>

class Invariant {
    friend class GeneralSolver;

public:

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
        CurrentValue += DeltaValue;
        for (invariant invar : invariants) {
            invar->addChange(invar->getVariableID(), DeltaValue);
        }
    }

    // should be pointer instead of integers
    void setUsedByConstraint(int constraint, int priority) {
        //        std::cout << "ehhh?" << std::endl;
        usedInConstraintNr = constraint;
        //        std::cout << "constraint" << std::endl;
        constraintPriority = priority;
    }
    // should be pointer instead of integers

    void setUsedByObjective(int objective) {
        usedInObjectiveNr = objective;
        constraintPriority = 0;


    }

    // should be pointer instead of integers

    int getConstraintNumber() {
        return usedInConstraintNr;
    }

    // should be pointer instead of integers

    int getUsedInObjective() {
        return usedInObjectiveNr;
    }

    unsigned getPriority() {
        return constraintPriority;
    }

    int getType() {
        return type;
    }

    InvariantContainer& getUpdateVector() {
        return update;
    }

    void addToUpdate(invariant invar) {
        update.push_back(invar);
    }

    std::vector<IntegerVariable*>& getVariables() {
        return VariablePointers;
    }
    
    /// Passing by value
    std::unordered_map<int, coefType> getCoefficients() {
        return coefficients;
    }
    
    /// Only used when the invariant defines a variable through a oneway constraint
    int getVariableID() {
        return variableID;
    }
    
    /// Correspond to the priority the invariant should be updated. 
    unsigned getLayer() {
        return layer;
    }

protected:
    std::vector<IntegerVariable*> VariablePointers;

    double CurrentValue = 0;
    double DeltaValue = 0;
    unsigned constraintPriority;
    int usedInConstraintNr;
    int usedInObjectiveNr;
    std::unordered_map<int, coefType> coefficients;
    int type;
    /// Should be defined when creating oneway constraints that define (integer)variables 
    unsigned layer = 0;
    int variableID;
    InvariantContainer invariants;
    InvariantContainer update;


private:

};


#endif	/* INVARIANT_HPP */

