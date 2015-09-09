#include "IntegerVariable.hpp"
#include "Invariant.hpp"
#include "Constraint.hpp"
#include "Constants.hpp"
#include <limits>
#include <memory>
#ifndef MODEL_HPP
#define	MODEL_HPP

class Model {
    //    friend class LSSpace;
private:
    std::vector<IntegerVariable*> original;
    std::vector<IntegerVariable*> IntVarVector;
    std::vector<Gecode::BoolVar> BoolVarVector;
    std::vector<std::shared_ptr<Invariant>> Invariants;
//    std::vector<Invariant*>* Invariants;
    std::vector<std::shared_ptr<std::vector<std::shared_ptr<Constraint>>>> Constraints;
//    int numberOfVariables =0;;
//    std::vector<Constraint*>* SoftConstraints;
//    std::vector<Constraint*>* ObjectiveFunction;
//    std::vector<int>* solution;
//    int solutionValue;
//    std::vector<int>* mask;
public:
    int initialValue;

//    int numberOfViolations;

    Model();
    Model(const Model& orig);
    ~Model();


    //        std::vector<IntegerVariable*> IntVarVector;
    //    std::vector<Gecode::BoolVar> BoolVarVector;
    //    std::vector<std::shared_ptr<Invariant>> Invariants;
    //    std::vector<Constraint*> Constraints;
    //    std::vector<Constraint*> ObjectiveFunction;


//    IntegerVariable* addIntegerVariable( int lb, int ub);
    void addIntegerVariable( int lb, int ub);
//    void initializeInvariants();

//    void initializeConstraints();

//    void initializeObjective();

    //    void addInvariantToIntVariable(int variableNumber, int invariantNumber);
    std::vector<IntegerVariable*>& getIntegerVariables();
    std::vector<IntegerVariable*>& getAllIntegerVariables();
    std::vector<std::shared_ptr<Invariant>>& getInvariants();
//    std::vector<Invariant*>* getInvariants();
    std::shared_ptr<std::vector<std::shared_ptr<Constraint>>> getConstraintsWithPriority(int prio);
    std::vector<std::shared_ptr<std::vector<std::shared_ptr<Constraint>>>>& getConstraints();
//    std::vector<Constraint*>* getSoftConstraints();
    std::shared_ptr<std::vector<std::shared_ptr<Constraint>>> getObjectives();
    IntegerVariable* getIntegerVariable(int i);
//    int getObjectiveValue();
//    void updateIntegerVariables(Gecode::IntVarArray& gecodeVars); 
    void updateIntegerVariable(int index, Gecode::IntVar& variable); 
//    int getNumberOfVariables();
    void nonFixedVariables(std::vector<IntegerVariable*>* nonFixed);
//        return IntVarVector->size();
////    }
//    void saveSolution();
//    std::vector<int>* getSolution();
//    int getSolutionValue();
//    void setSolution();
//    bool recalculateAll();
//    int maskAt(int i);
//    void shuffleMask();



};

#endif	/* MODEL_HPP */

