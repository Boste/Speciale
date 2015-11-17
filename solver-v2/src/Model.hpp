#include "IntegerVariable.hpp"
#include "Invariant.hpp"
#include "Constraint.hpp"
#include "Constants.hpp"
#include <limits>
#include <memory>
#include <list>
#include "DependencyDigraph.hpp"
#ifndef MODEL_HPP
#define	MODEL_HPP

class Model {
        friend class GeneralSolver;
        
private:
    /// All variables given by user
    variableContainer original;
    /// Binary Variables that are not fixed by preprocessing 
    variableContainer nonFixedVariables;
    /// Invariants including oneway constraints but not the invariant from the constraints oneway is made
    InvariantContainer Invariants;
    //    variableContainer IntegerVariables;
    /// All integer variables in the model
    std::vector<IntegerVariable*> IntegerVariables;
    /// All constraints posted
    allConstraints Constraints;
    /// Variables for LS
    variableContainer LSVariables;
    InvariantContainer objectiveInvariant;
    std::vector<IntegerVariable*> mask;
    std::shared_ptr<DependencyDigraph> DDG = std::make_shared<DependencyDigraph>();
    /// The search priority vectors given by user when calling search(std::vector<IntegerVarible*> variables)
    std::vector<std::vector<IntegerVariable*>> priorityVectorsOfVariables;
    /// Only used to give invariants id
    unsigned id = 0;
    std::vector<constraint> functionalConstraints;
    
public:
    /// Should be moved to state
    std::vector<int> initialEvaluation;
//    int initialValue;
    /// Test if it improves bestImprovement
//    unsigned highestPriority = 0;
//    unsigned initialViolations;
//    std::vector<int> test;
    //    unsigned numberOfLayers = 0;
    //    int numberOfViolations;

    Model();
    Model(const Model& orig);
    ~Model();
    std::vector<constraint>& getFunctionalConstraints();
    std::vector<std::vector<IntegerVariable*>>& getPriorityVectors();
    std::vector<IntegerVariable*>& getPriorityVectorNr(unsigned i);

    //    IntegerVariable* addIntegerVariable( int lb, int ub);
    void startUp();
    IntegerVariable* addBinaryVariable(int lb, int ub);
    IntegerVariable* addIntegerVariable(int lb, int ub);
    //    void initializeInvariants();
    IntegerVariable* getMaskAt(int i);
    std::vector<IntegerVariable*>& getMask();
    void shuffleMask();
    std::shared_ptr<DependencyDigraph>& getDDG();

    // Was used aroung line 382 GS. Creating non oneway invariants
//    void addInvariantToDDG(invariant invar, variableContainer& variables);
//    void addInvariantToDDG(invariant invar, InvariantContainer& invariants);
//    void addInvariantToDDG(invariant invar, variableContainer& variables, InvariantContainer& invariants);
//    void addVariablesToDDG(variableContainer& vars);
//    /// Adds nonFixedBinaryVariables
//    void addVariablesToDDG();
//    void createPropagationQueue(variableContainer& vars,InvariantContainer& invars);
    propagation_queue& getPropagationQueue(IntegerVariable* iv);
    updateVector& getUpdate(IntegerVariable* iv);
    updateVector& getUpdate(invariant invar);
    //    void initializeConstraints();

    //    void initializeObjective();
    /// Returns all integer variables as a list. only used to make oneway constraints
    std::vector<IntegerVariable*>& getIntegerVariables();
    //    void addInvariantToIntVariable(int variableNumber, int invariantNumber);
    variableContainer& getNonFixedVariables();
    IntegerVariable* getVariable(unsigned id);
    variableContainer& getAllVariables();
    void setNonFixedVariables(std::vector<IntegerVariable*>& nonFixed);
    variableContainer& getLSVariables();
    InvariantContainer& getInvariants();
    void addInvariant(std::shared_ptr<Invariant> invar);
    //    InvariantContainer& getOrgInvariants();
    //    std::vector<Invariant*>* getInvariants();
    void cleanUp();
    //    std::vector<std::shared_ptr<std::pair<int, int>>>& getConstraintsWithIntegerVariables();
    //    constraintContainer& getConstraintsWithPriority(int prio);
    constraintContainer getConstraintsWithPriority(int prio);
    allConstraints& getConstraints();
    //    allConstraints& getConstraints();
    //    std::vector<Constraint*>* getSoftConstraints();
    //    constraintContainer& getObjectives();
    constraintContainer getObjectives();
    IntegerVariable* getNonFixedVariable(int i);
    //    int getObjectiveValue();
    //    void updateIntegerVariables(Gecode::IntVarArray& gecodeVars); 
    void updateIntegerVariable(int index, Gecode::IntVar& variable);
    std::vector<int>& getInitialEvaluation();
    InvariantContainer& getObjectiveInvariant();
    void addToObjectiveInvariant(invariant invar);
    void initialize();
    


};

#endif	/* MODEL_HPP */

