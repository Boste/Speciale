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
    variableContainer nonFixedBinaryVariables;
    //    variableContainer binaryVariables;
    //    std::vector<Gecode::BoolVar> BoolVarVector;
    /// Invariants for original model, before oneway constraints are made. 
    //    InvariantContainer orgInvariants;
    /// Invariants including oneway constraints but not the invariant from the constraints oneway is made
    InvariantContainer Invariants;
    //    variableContainer IntegerVariables;
    /// All integer variables in the model
    std::list<IntegerVariable*> IntegerVariables;
    //    std::vector<std::shared_ptr<std::pair<int, int>>> ConstraintsWithIntegerVariables;
    //    std::vector<Invariant*>* Invariants;
    // All constraints posted
    allConstraints Constraints;
    InvariantContainer objectiveInvariant;
    /// All Constraints after some has been made oneway (oneway is not included, oneway is invariants)
    //    allConstraints Constraints;
    std::vector<IntegerVariable*> mask;
    //    int numberOfVariables =0;;
    //    std::vector<Constraint*>* SoftConstraints;
    //    std::vector<Constraint*>* ObjectiveFunction;
    //    std::vector<int>* solution;
    //    int solutionValue;
    //    std::vector<int>* mask;
    std::shared_ptr<DependencyDigraph> DDG = std::make_shared<DependencyDigraph>();
    /// Only used to give invariants id
    unsigned id;
public:
    /// Should be moved to state
    std::vector<int> initialEvaluation;
//    int initialValue;
//    unsigned initialViolations;
//    std::vector<int> test;
    //    unsigned numberOfLayers = 0;
    //    int numberOfViolations;

    Model();
    Model(const Model& orig);
    ~Model();

    //    std::vector<int> getTestVal(){
    //        return test;
    //    }
    //    std::vector<int>& getTestRef(){
    //        return test;
    //    }
    //    void fillTest(int elements){
    //        for(int i = 0; i < elements; i++){
    //            test.push_back(i);
    //        }
    //        std::cout << "test " << &test << std::endl;
    //    }
    //    void incTest(int nr){
    //        test.at(nr) = nr+1;
    //        std::cout << test.at(nr) << std::endl;
    //    }

    //        std::vector<IntegerVariable*> IntVarVector;
    //    std::vector<Gecode::BoolVar> BoolVarVector;
    //    std::vector<std::shared_ptr<Invariant>> Invariants;
    //    std::vector<Constraint*> Constraints;
    //    std::vector<Constraint*> ObjectiveFunction;


    //    IntegerVariable* addIntegerVariable( int lb, int ub);
    void startUp();
    void addBinaryVariable(int lb, int ub);
    void addIntegerVariable(int lb, int ub);
    //    void initializeInvariants();
    IntegerVariable* getMaskAt(int i);
    std::vector<IntegerVariable*>& getMask();
    void shuffleMask();
    std::shared_ptr<DependencyDigraph>& getDDG();

    // Was used aroung line 382 GS. Creating non oneway invariants
    void addInvariantToDDG(invariant invar, variableContainer& variables);
    void addInvariantToDDG(invariant invar, InvariantContainer& invariants);
    void addInvariantToDDG(invariant invar, variableContainer& variables, InvariantContainer& invariants);
    void addVariablesToDDG(variableContainer& vars);
    /// Adds nonFixedBinaryVariables
    void addVariablesToDDG();
    void createPropagationQueue();
    propagation_queue& getPropagationQueue(IntegerVariable* iv);
    updateVector& getUpdate(IntegerVariable* iv);
    updateVector& getUpdate(invariant invar);
    //    void initializeConstraints();

    //    void initializeObjective();
    /// Returns all integer variables as a list. only used to make oneway constraints
    std::list<IntegerVariable*>& getIntegerVariables();
    //    void addInvariantToIntVariable(int variableNumber, int invariantNumber);
    variableContainer& getNonFixedBinaryVariables();
    IntegerVariable* getVariable(unsigned id);
    variableContainer& getAllVariables();
    void nonFixedVariables(std::vector<IntegerVariable*>& nonFixed);

    InvariantContainer& getInvariants();
    void addInvariant(std::shared_ptr<Invariant> invar);
    //    InvariantContainer& getOrgInvariants();
    //    std::vector<Invariant*>* getInvariants();

    //    std::vector<std::shared_ptr<std::pair<int, int>>>& getConstraintsWithIntegerVariables();
    //    constraintContainer& getConstraintsWithPriority(int prio);
    constraintContainer getConstraintsWithPriority(int prio);
    allConstraints& getConstraints();
    //    allConstraints& getConstraints();
    //    std::vector<Constraint*>* getSoftConstraints();
    //    constraintContainer& getObjectives();
    constraintContainer getObjectives();
    IntegerVariable* getNonFixedBinaryVariable(int i);
    //    int getObjectiveValue();
    //    void updateIntegerVariables(Gecode::IntVarArray& gecodeVars); 
    void updateIntegerVariable(int index, Gecode::IntVar& variable);
    std::vector<int>& getInitialEvaluation();
    InvariantContainer& getObjectiveInvariant();
    void addToObjectiveInvariant(invariant invar);
    void initialize();
    


};

#endif	/* MODEL_HPP */

