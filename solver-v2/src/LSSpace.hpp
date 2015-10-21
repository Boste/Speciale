//#include <../../../gecode/release-4.3.3/gecode/int.hh>
//#include <gecode/int.hh>
//#include "../include/Constraints.hpp"
#include "State.hpp"
#include "IntegerVariable.hpp"
#include "Invariant.hpp"
#include "Constraint.hpp"
#include "Sum.hpp"
#include "Linear.hpp"
#include "Constraint.hpp"
#include "IntegerVariable.hpp"
#include "Model.hpp"
#include "NeighborhoodExplorer.hpp"
#include "Constants.hpp"
#include <memory>
#include "Clock.hpp"
#include "Max.hpp"
#ifndef LSSPACE_HPP
#define	LSSPACE_HPP
//#include 

class LSSpace {
    friend class Test;

private:
    int Violations = 0;
//    int ObjectiveValue = 0;
    int iterations = 0;
    std::shared_ptr<Model> model;
    std::shared_ptr<State> currentState;
    std::shared_ptr<State> bestState;
    
//    void addInvariantToIntVariable(int variableNumber, int invariantNumber);
    
public:
    LSSpace(std::shared_ptr<Model> model);
    void printCurrent();

    ~LSSpace() {

    }
    void initializeLS();
    bool canBeMadeOneway(IntegerVariable* iv, constraint cons);
    void makeOneway(IntegerVariable* iv, constraint cons, int coef);
    void optimizeSolution(int time);
//    void simpleMove(int variabelNr);
    bool bestImprovement();

    // Assumes initial value is 0, hence can only be used to initialize once. 
//    void initializeInvariants(std::shared_ptr<State> st);

//    void initializeConstraints(std::shared_ptr<State> st);

//    void initializeObjective(std::shared_ptr<State> st);
//    int getObjectiveValue();

//    void commitDeltaOfVariable(int changedVariable);

//    std::pair<int, int> calculateDeltaValueOfVariableChange(int variableNumber, int newValue);

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

