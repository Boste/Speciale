#include "Variable.hpp"
#include "Invariant.hpp"
#include "Constraint.hpp"
#include "Constants.hpp"
#include <limits>
#include <memory>
#include <list>
#include "DependencyDigraph.hpp"
#include "Output.hpp"
#ifndef MODEL_HPP
#define	MODEL_HPP

class Storage {
        friend class GPSolver;
        
private:
    /// All variables given by user
    variableContainer original;
    /// Binary Variables that are not fixed by preprocessing 
    variableContainer nonFixedVariables;
    /// Invariants including oneway constraints but not the invariant from the constraints oneway is made
    InvariantContainer Invariants;
    /// All integer variables in the model
    std::vector<Variable*> IntegerVariables;
    /// All constraints posted
    allConstraints Constraints;
    std::vector<std::set<Variable*,Variable::compare_variable>> inConstraintWith;
    allConstraints constraintsWithIntegerVarsRelated;
    /// Variables for LS
    InvariantContainer evaluationInvariants;
    
    std::vector<Variable*> evalVariables;
    std::vector<Variable*> mask;
    std::shared_ptr<DependencyDigraph> DDG = std::make_shared<DependencyDigraph>();
    /// The search priority vectors given by user when calling search(std::vector<IntegerVarible*> variables)
    std::vector<std::vector<Variable*>> priorityVectorsOfVariables;
    /// Only used to give invariants id
    unsigned id = 0;
    std::unordered_map<unsigned, invariant>  violatedConstraints;
    std::vector<constraint> feasibleFunctionalConstriants;
    
public:
    /// Should be moved to state
    std::vector<int> initialEvaluation;
    bool containsIntegerVariables = false;

    Storage();
    Storage(const Storage& orig);
    ~Storage();
    std::vector<std::vector<Variable*>>& getPriorityVectors();
    std::vector<Variable*>& getPriorityVectorNr(unsigned i);
    Variable* addBinaryVariable(int lb, int ub);
    Variable* getMaskAt(int i);
    std::vector<Variable*>& getMask();
    void shuffleMask();
    std::shared_ptr<DependencyDigraph>& getDDG();
    std::set<Variable*,Variable::compare_variable>& getInConstraintWithAt(unsigned id);
    void addViolatedConstraint(invariant inv);
    void removeViolatedConstraint(invariant inv);
    std::unordered_map<unsigned, invariant>& getViolatedConstraints();
   
    propagation_queue& getPropagationQueue(Variable* iv);
    updateVector& getUpdate(Variable* iv);
    updateVector& getUpdate(invariant invar);
    std::vector<Variable*>& getIntegerVariables();
    variableContainer& getNonFixedVariables();
    Variable* getVariable(unsigned id);
    variableContainer& getAllVariables();
    void setNonFixedVariables(std::vector<Variable*>& nonFixed);
    InvariantContainer& getInvariants();
    void addInvariant(invariant invar);
    void cleanUp();
    constraintContainer getConstraintsWithPriority(int prio);
    allConstraints& getConstraints();
    Variable* getNonFixedVariable(int i);
    std::vector<int>& getInitialEvaluation();
    InvariantContainer& getEvaluationInvariants();
    invariant getEvaluationInvariantNr(unsigned nr);
    std::vector<Variable*>& getEvaluationVariables();
    Variable* getEvaluationVariableNr(unsigned nr);
    std::vector<constraint>& getFeasibleFunctionalConstraints();
    void setFeasibleFunctionalConstraints(std::vector<constraint> funcCons);
    void addToEvaluationInvariants(invariant invar);
    void initialize();
    
    

    
};
struct compare_invariant;
#endif	/* MODEL_HPP */

