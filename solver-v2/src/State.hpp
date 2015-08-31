#include "IntegerVariable.hpp"
#include "Invariant.hpp"
#include "Constraint.hpp"
#include "Constants.hpp"
#include <limits>
#include <memory>
#ifndef STATE_HPP
#define	STATE_HPP

class State {
    //    friend class LSSpace;
private:
    std::vector<IntegerVariable*>* IntVarVector;
    std::vector<Gecode::BoolVar>* BoolVarVector;
    std::vector<std::shared_ptr<Invariant>>* Invariants;
//    std::vector<Invariant*>* Invariants;
    std::vector<std::vector<std::shared_ptr<Constraint>>*>* Constraints;
//    std::vector<Constraint*>* SoftConstraints;
//    std::vector<Constraint*>* ObjectiveFunction;
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
    //    std::vector<std::shared_ptr<Invariant>> Invariants;
    //    std::vector<Constraint*> Constraints;
    //    std::vector<Constraint*> ObjectiveFunction;


    std::vector<IntegerVariable*>* addIntegerVariable(int numberOfVariables, int lb, int ub);
    void initializeInvariants();

    void initializeConstraints();

    void initializeObjective();

    //    void addInvariantToIntVariable(int variableNumber, int invariantNumber);
    std::vector<IntegerVariable*>* getIntegerVariables();
    std::vector<std::shared_ptr<Invariant>>* getInvariants();
//    std::vector<Invariant*>* getInvariants();
    std::vector<std::shared_ptr<Constraint>>* getConstraintsWithPriority(int prio);
    std::vector<std::vector<std::shared_ptr<Constraint>>*>* getConstraints();
//    std::vector<Constraint*>* getSoftConstraints();
    std::vector<std::shared_ptr<Constraint>>* getObjectives();
    IntegerVariable* getIntegerVariable(int i);
    int getObjectiveValue();
    void updateIntegerVariables(Gecode::IntVarArray& gecodeVars);

    int getNumberOfVariables() {
        return IntVarVector->size();
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

