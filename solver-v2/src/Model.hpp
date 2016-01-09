#include "Variable.hpp"
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
    std::vector<Variable*> IntegerVariables;
    /// All constraints posted
    allConstraints Constraints;
    std::vector<std::set<Variable*,Variable::compare_variable>> inConstraintWith;
//    std::vector<std::vector<constraint>> constraintsWithIntegerVarsRelated;
    allConstraints constraintsWithIntegerVarsRelated;
//    std::vector<std::shared_ptr<std::vector<std::pair<IntegerVariable*,int>>>> IntegerVariablesAndChange;
    /// Variables for LS
//    variableContainer LSVariables;
//    InvariantContainer objectiveInvariant;
    InvariantContainer evaluationInvariants;
    
    std::vector<Variable*> evalVariables;
    std::vector<Variable*> mask;
    std::shared_ptr<DependencyDigraph> DDG = std::make_shared<DependencyDigraph>();
    /// The search priority vectors given by user when calling search(std::vector<IntegerVarible*> variables)
    std::vector<std::vector<Variable*>> priorityVectorsOfVariables;
    /// Only used to give invariants id
    unsigned id = 0;
//    std::vector<constraint> functionalConstraints;
    std::unordered_map<unsigned, invariant>  violatedConstraints;
    std::vector<constraint> feasibleFunctionalConstriants;
    
public:
    /// Should be moved to state
    std::vector<int> initialEvaluation;
    bool containsIntegerVariables = false;
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
//    std::vector<constraint>& getFunctionalConstraints();
    std::vector<std::vector<Variable*>>& getPriorityVectors();
    std::vector<Variable*>& getPriorityVectorNr(unsigned i);

    //    IntegerVariable* addIntegerVariable( int lb, int ub);
    Variable* addBinaryVariable(int lb, int ub);
    Variable* addIntegerVariable(int lb, int ub);
    //    void initializeInvariants();
    Variable* getMaskAt(int i);
    std::vector<Variable*>& getMask();
    void shuffleMask();
    std::shared_ptr<DependencyDigraph>& getDDG();
    std::set<Variable*,Variable::compare_variable>& getInConstraintWithAt(unsigned id);
    void addViolatedConstraint(invariant inv);
    void removeViolatedConstraint(invariant inv);
    std::unordered_map<unsigned, invariant>& getViolatedConstraints();
    // Was used aroung line 382 GS. Creating non oneway invariants
//    void addInvariantToDDG(invariant invar, variableContainer& variables);
//    void addInvariantToDDG(invariant invar, InvariantContainer& invariants);
//    void addInvariantToDDG(invariant invar, variableContainer& variables, InvariantContainer& invariants);
//    void addVariablesToDDG(variableContainer& vars);
//    /// Adds nonFixedBinaryVariables
//    void addVariablesToDDG();
//    void createPropagationQueue(variableContainer& vars,InvariantContainer& invars);
    propagation_queue& getPropagationQueue(Variable* iv);
    updateVector& getUpdate(Variable* iv);
    updateVector& getUpdate(invariant invar);
    //    void initializeConstraints();

    //    void initializeObjective();
    /// Returns all integer variables as a list. only used to make oneway constraints
    std::vector<Variable*>& getIntegerVariables();
    //    void addInvariantToIntVariable(int variableNumber, int invariantNumber);
    variableContainer& getNonFixedVariables();
    Variable* getVariable(unsigned id);
    variableContainer& getAllVariables();
    void setNonFixedVariables(std::vector<Variable*>& nonFixed);
//    variableContainer& getLSVariables();
    InvariantContainer& getInvariants();
    void addInvariant(invariant invar);
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
//    constraintContainer getObjectives();
    Variable* getNonFixedVariable(int i);
    //    int getObjectiveValue();
    //    void updateIntegerVariables(Gecode::IntVarArray& gecodeVars); 
    void updateIntegerVariable(int index, Gecode::IntVar& variable);
    std::vector<int>& getInitialEvaluation();
//    InvariantContainer& getObjectiveInvariant();
    InvariantContainer& getEvaluationInvariants();
    invariant getEvaluationInvariantNr(unsigned nr);
    std::vector<Variable*>& getEvaluationVariables();
    Variable* getEvaluationVariableNr(unsigned nr);
    std::vector<constraint>& getFeasibleFunctionalConstraints();
    void setFeasibleFunctionalConstraints(std::vector<constraint> funcCons);
//    void addToObjectiveInvariant(invariant invar);
    void addToEvaluationInvariants(invariant invar);
    void initialize();
    
    

    
};
struct compare_invariant;
//: public std::binary_function<invariant, invariant, bool> {
//
//    bool operator()(const invariant invar1, const invariant invar2) const {
//        //                std::cout << "is used"  << invar1 << " " << invar2 << " id1 "<< invar1->getID() <<" id2 "<< invar2->getID() << " compare " << (invar1 < invar2) << std::endl;
//        //                sleep(1);
//        return (invar1->getTimestamp() > invar2->getTimestamp());
//    }
//
//    bool operator()(invariant invar1, invariant invar2) {
//
//        //                std::cout << "is used123 " <<" id1 "<< invar1->getID() <<" id2 "<< invar2->getID() << " compare " << (invar1 < invar2) << std::endl;
//        //                sleep(1);
////        unsigned t1 = invariant_nodes.at(invar1->getID())->timestamp; 
//        return (invar1->getTimestamp() >     invar2->getTimestamp());
//    }
//   
//};
#endif	/* MODEL_HPP */

