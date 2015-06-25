#include <../../../gecode/release-4.3.3/gecode/int.hh>
//#include "../include/Constraints.hpp"
#include "IntegerVariable.hpp"
#include "Invariant.hpp"
#include "Constraint.hpp"
#include "Sum.hpp"
#include "Linear.hpp"
#ifndef LSSPACE_HPP
#define	LSSPACE_HPP
//#include 

class LSSpace {
public:
    LSSpace();
    void printCurrent();

    std::vector<IntegerVariable*> IntVarVector;
    std::vector<Gecode::BoolVar> BoolVarVector;
    std::vector<Invariant*> Invariants;
    std::vector<Constraint*> Constraints;
    std::vector<Constraint*> ObjectiveFunction;


private:

    void addInvariantToIntVariable(int variableNumber, int invariantNumber) {
        IntVarVector.at(variableNumber)->addToUpdate(invariantNumber);
    }
protected:

    std::vector<IntegerVariable*>* addIntVariablesToLS(Gecode::IntVarArray& vars);

    // Not able to add obj fnc. 
    void linear(std::vector<int>* coefficients, vector<IntegerVariable*>* variables, int relation, int upperbound, int type);

    void simpleMove(int variabelNr);
    int bestMove();

    // Assumes initial value is 0, hence can only be used to initialize once. 
    void initializeInvariants();

    void initializeConstraints();

    void initializeObjective();
    
    void commitDeltaOfVariable(int changedVariable);

    std::pair<int,int> calculateDeltaValueOfVariableChange(int variableNumber, int newValue);

    double testInvariant(int invariantNumber) {
        return Invariants.at(invariantNumber)->test();
    }

    void testObjective() {
        std::cout << "only works for cov1075" << std::endl;
        int objVal = 0;
        for (unsigned i = 0; i < IntVarVector.size(); i++) {
            if (IntVarVector.at(i)->getCurrentValue() == 1) {
                objVal++;
            }
        }
        std::cout << "objVal " << objVal << std::endl;
    }
};

#endif	/* LSSPACE_HPP */

