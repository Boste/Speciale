#include "IntegerVariable.hpp"
#include "Invariant.hpp"
#include "Constraint.hpp"
#ifndef STATE_HPP
#define	STATE_HPP

class State {
//    friend class LSSpace;
    private:
    std::vector<IntegerVariable*> IntVarVector;
    std::vector<Gecode::BoolVar> BoolVarVector;
    std::vector<Invariant*> Invariants;
    std::vector<Constraint*> Constraints;
    std::vector<Constraint*> ObjectiveFunction;
public:
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

    void initializeObjective();

//    void addInvariantToIntVariable(int variableNumber, int invariantNumber);
    std::vector<IntegerVariable*>* getIntegerVariables();
    std::vector<Invariant*>* getInvariants();
    std::vector<Constraint*>* getConstraints();
    std::vector<Constraint*>* getObjectives();
    


};

#endif	/* STATE_HPP */

