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

    virtual void addChange(int variableNumber, int change) {
        std::cout << "addChange in invariant" << std::endl;
        std::cout << variableNumber << " " << change << std::endl;
    }

    virtual int calculateDeltaValue() {
        std::cout << "CalculateDeltaValue in  invariant" << std::endl;
        return 0;
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
    }

    void setUsedByConstraint(int constraint, int priority) {
        usedInConstraintNr = constraint;
        constraintPriority = priority;
    }

    void setUsedByObjective(int objective) {
        usedInObjectiveNr = objective;
        constraintPriority = 0;


    }

    int getConstraintNumber() {
        return usedInConstraintNr;
    }

    int getUsedInObjective() {
        return usedInObjectiveNr;
    }

    unsigned getPriority() {
        return constraintPriority;
    }

    int getType() {
        return type;
    }
    std::vector<IntegerVariable*>* VariablePointers;

protected:
    double CurrentValue = 0;
    double DeltaValue = 0;
    unsigned constraintPriority;
    int usedInConstraintNr;
    int usedInObjectiveNr;
    std::unordered_map<int, int> coefficients;
    int type;

private:

};


#endif	/* INVARIANT_HPP */

