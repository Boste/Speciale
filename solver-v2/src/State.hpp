#include "IntegerVariable.hpp"
#include "Invariant.hpp"
#include "Constraint.hpp"
#include "Constants.hpp"
#include <limits>
#ifndef STATE_HPP
#define	STATE_HPP

class State {
    //    friend class LSSpace;
private:
    std::vector<IntegerVariable*>* IntVarVector;
    std::vector<Gecode::BoolVar>* BoolVarVector;
    std::vector<Invariant*>* Invariants;
    std::vector<Constraint*>* Constraints;
    std::vector<Constraint*>* ObjectiveFunction;
    int numberOfVariables;
    std::vector<int>* solution;
    int solutionValue;
    std::vector<int>* mask;
public:

    int numberOfViolations;

    State();
    State(const State& orig);
    ~State();


    //        std::vector<IntegerVariable*> IntVarVector;
    //    std::vector<Gecode::BoolVar> BoolVarVector;
    //    std::vector<Invariant*> Invariants;
    //    std::vector<Constraint*> Constraints;
    //    std::vector<Constraint*> ObjectiveFunction;


    std::vector<IntegerVariable*>* addIntegerVariable(Gecode::IntVarArray* vars);
    void initializeInvariants();

    void initializeConstraints();

    int initializeObjective();

    //    void addInvariantToIntVariable(int variableNumber, int invariantNumber);
    std::vector<IntegerVariable*>* getIntegerVariables();
    std::vector<Invariant*>* getInvariants();
    std::vector<Constraint*>* getConstraints();
    std::vector<Constraint*>* getObjectives();
    IntegerVariable* getIntegerVariable(int i);
    int getObjectiveValue();

    int getNumberOfVariables() {
        return numberOfVariables;
    }
    void saveSolution();
    std::vector<int>* getSolution();
    int getSolutionValue();
    void setSolution();
    bool recalculateAll();
    int maskAt(int i);
    void shuffleMask();



};

#endif	/* STATE_HPP */

