#include "IntegerVariable.hpp"
#include "Invariant.hpp"
#include "Constraint.hpp"
#include "Constants.hpp"
#include <limits>
#include <memory>
#include <list>
#ifndef MODEL_HPP
#define	MODEL_HPP

class Model {
    //    friend class LSSpace;
private:
    /// All variables given by user
    variableContainer original;
    /// Binary Variables that are not fixed by preprocessing 
    variableContainer nonFixedBinaryVariables;
    //    std::vector<Gecode::BoolVar> BoolVarVector;
    /// Invariants for original model, before oneway constraints are made. 
    InvariantContainer orgInvariants;
    /// Invariants including oneway constraints but not the invariant from the constraints oneway is made
    InvariantContainer Invariants;
//    variableContainer IntegerVariables;
    /// All integer variables in the model
    std::list<IntegerVariable*> IntegerVariables;
    //    std::vector<std::shared_ptr<std::pair<int, int>>> ConstraintsWithIntegerVariables;
    //    std::vector<Invariant*>* Invariants;
    // All constraints posted
    allConstraints orgConstraints;
    /// All Constraints after some has been made oneway (oneway is not included, oneway is invariants)
    allConstraints Constraints;
    std::vector<IntegerVariable*> mask;
    //    int numberOfVariables =0;;
    //    std::vector<Constraint*>* SoftConstraints;
    //    std::vector<Constraint*>* ObjectiveFunction;
    //    std::vector<int>* solution;
    //    int solutionValue;
    //    std::vector<int>* mask;
public:
    /// Should be moved to state
    int initialValue;
//    unsigned numberOfLayers = 0;
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
    void addBinaryVariable(int lb, int ub);
    void addIntegerVariable(int lb, int ub);
    //    void initializeInvariants();
    IntegerVariable* getMaskat(int i);
    std::vector<IntegerVariable*>& getMask();
      
    
    //    void initializeConstraints();

    //    void initializeObjective();
    /// Returns all integer variables as a list. only used to make oneway constraints
    std::list<IntegerVariable*>& getIntegerVariables();
    //    void addInvariantToIntVariable(int variableNumber, int invariantNumber);
    variableContainer& getNonFixedBinaryVariables();
    variableContainer& getAllVariables();
    InvariantContainer& getInvariants();
    InvariantContainer& getOrgInvariants();
    //    std::vector<Invariant*>* getInvariants();

    //    std::vector<std::shared_ptr<std::pair<int, int>>>& getConstraintsWithIntegerVariables();
    constraintContainer& getConstraintsWithPriority(int prio);
    constraintContainer& getOrgConstraintsWithPriority(int prio);
    allConstraints& getOrgConstraints();
    allConstraints& getConstraints();
    //    std::vector<Constraint*>* getSoftConstraints();
    constraintContainer& getObjectives();
    constraintContainer& getOrgObjectives();
    IntegerVariable* getNonFixedBinaryVariable(int i);
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

