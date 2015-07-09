#include <../../../gecode/release-4.3.3/gecode/int.hh>
//#include "../include/Constraints.hpp"
#include "IntegerVariable.hpp"
#include "Invariant.hpp"
#include "Constraint.hpp"
#include "Sum.hpp"
#include "Linear.hpp"
#include "State.hpp"
#include "NeighborhoodExplorer.hpp"
#include "Constants.hpp"
#ifndef LSSPACE_HPP
#define	LSSPACE_HPP
//#include 

class LSSpace {
    friend class Test;

private:
    int Violations = 0;
    int ObjectiveValue = 0;
    State* st;
    int iterations = 0;

    void addInvariantToIntVariable(int variableNumber, int invariantNumber);
public:
    LSSpace();
    void printCurrent();

    //    std::vector<IntegerVariable*> IntVarVector;
    std::vector<Gecode::BoolVar> BoolVarVector;
    //    std::vector<Invariant*> Invariants;
    //    std::vector<Constraint*> Constraints;
    //    std::vector<Constraint*> ObjectiveFunction;

    //        LSSpace(LSSpace &c) {
    //            this->IntVarVector = c.IntVarVector;
    //            this->BoolVarVector = c.BoolVarVector;
    //            this->Invariants = c.Invariants;
    //            this->Constraints = c.Constraints;
    //            this->ObjectiveFunction = c.ObjectiveFunction;
    //            this->Violations = c.Violations;
    //            this->ObjectiveValue = c.ObjectiveValue;
    //        }
    //    
    //        LSSpace& operator=(const LSSpace &c) {
    //            this->IntVarVector = c.IntVarVector;
    //            this->BoolVarVector = c.BoolVarVector;
    //            this->Invariants = c.Invariants;
    //            this->Constraints = c.Constraints;
    //            this->ObjectiveFunction = c.ObjectiveFunction;
    //            this->Violations = c.Violations;
    //            this->ObjectiveValue = c.ObjectiveValue;
    //            return *this;
    //        }
    //    

    ~LSSpace() {
//        std::cout << "Destructing LSSpace" << std::endl;
        //            delete &IntVarVector;
//        delete &BoolVarVector;
        //            delete &Invariants;
        //            delete &Constraints;
        //            delete &ObjectiveFunction;
        //            delete &Violations;
        //            delete &ObjectiveValue;
        delete  st;
    }


    //        IntVarVector.at(variableNumber)->addToUpdate(invariantNumber);
    //    }

protected:

    std::vector<IntegerVariable*>* addIntVariablesToState(Gecode::IntVarArray* vars);
    void SetValues(Gecode::IntVarArray vars);

    // Not able to add obj fnc. 
    void linear(std::vector<int>* coefficients, vector<IntegerVariable*>* variables, int relation, int upperbound, int type);
    void optimizeSolution();
    void simpleMove(int variabelNr);
    bool bestImprovement();

    // Assumes initial value is 0, hence can only be used to initialize once. 
    void initializeInvariants();

    void initializeConstraints();

    void initializeObjective();

    void commitDeltaOfVariable(int changedVariable);

    std::pair<int, int> calculateDeltaValueOfVariableChange(int variableNumber, int newValue);

    //    double testInvariant(int invariantNumber) {
    //        return Invariants.at(invariantNumber)->test();
    //    }

    //    void testObjective() {
    //        std::cout << "only works for cov1075" << std::endl;
    //        int objVal = 0;
    //        for (unsigned i = 0; i < IntVarVector.size(); i++) {
    //            if (IntVarVector.at(i)->getCurrentValue() == 1) {
    //                objVal++;
    //            }
    //        }
    //        std::cout << "objVal " << objVal << std::endl;
    //    }
};

#endif	/* LSSPACE_HPP */

