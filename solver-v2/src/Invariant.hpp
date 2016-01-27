#ifndef INVARIANT_HPP
#define	INVARIANT_HPP
#include "Variable.hpp"
#include <unordered_map>
#include <iostream>
#include "Constants.hpp"
#include <limits>
#include <cassert>
//class Constraint;

class Invariant {
    //    friend class GeneralSolver;
    friend class Storage;

public:
    //    bool changeAdd = false;

    Invariant() {
        //                std::cout << "invariant" << std::endl;
    }

    Invariant(const Invariant &a) {
        std::cout << &a << std::endl;

    }
    // should be pointers instead of integers
    /// Tells this Invariant that deltavalue of invariant or variable with ID is changed by change

    virtual void proposeChange(int ID, int change) = 0;

    /// Computes the deltavalue of this Invariant based on the vector of changes (Maybe that vector should be moved here)

    virtual bool calculateDelta() =0;

    virtual void updateValue() =0;

    /// Return the posible change of value. Never resets but gets recomputed by CalculateDeltaValue() based on new changes and current value.

    int getDeltaValue() {
        return DeltaValue;
    }

    virtual bool test() {
        std::cout << "test in invariant" << std::endl;
        return false;
    }

    virtual ~Invariant() {
    }
    /// Return currnet value of this Invariant

    int getCurrentValue() {

        return CurrentValue;
    }

    void setObjective() {
        constraintPriority = 0;
//        usedByConstraint = true;


    }

    bool inViolatedConstraints(){
        return inViolated;
    }
    void setInViolatedConstraints(bool flag){
        inViolated = flag;
    }
    bool representConstraint(){
        return representCons;
    }
    /// The priority of the constraint this invariant is used by (if any otherwise fails assert) Not used

    unsigned getPriority() {
        return constraintPriority;
    }

    int getType() {
        return type;
    }

    /// not passing by value
    /// Should only be used for testing i think

    std::unordered_map<int, coefType>& getCoefficients() {
        return coefficients;
    }

    /// The id of the variable that this invariant defines

    void setVariableID(unsigned id) {
        variableID = id;
    }
    /// Only used when the invariant defines a variable through a oneway constraint

    int getVariableID() {
        return variableID;
    }
    Variable* getVariable() {
        return variable;
    }
    void setVariable(Variable* var) {
        variable = var;
    }
    

    /// Correspond to the priority the invariant should be updated. 
    //    unsigned getLayer() {
    //        return layer;
    //    }
    /// Not sure this is relevant after Max invariant is introduced.

    int getStartValue() {
        return startValue;
    }

    /// only for start value

    void setValue(double value) {
        CurrentValue = value;
    }

    void setStartValue(double value) {
        startValue = value;
        CurrentValue = value;
        //        }

    }
    /// Return the ID of this invariant (not the variable it defines if any)

    unsigned getID() {
        return invariantID;
    }

    ///Only when making invariants (if not called lb = integer min +1 and ub = integer max -1, (gecode limits))

    void setBounds(double lb, double ub) {
        lowerbound = lb;
        upperbound = ub;
    }
    std::pair<double,double> getBounds() {
        std::pair<double,double> bounds(lowerbound,upperbound);
        return bounds;
    }

    /// Only used for testing

    void setVariablePointers(std::vector<Variable*>& vars) {
        VariablePointers = vars;
    }

    std::vector<Variable*>& getVariablePointers() {
        return VariablePointers;
    }
    void setInvariantPointers(std::vector<invariant>& invars) {
        InvariantPointers = invars;
    }

    std::vector<invariant>& getInvariantPointers() {
        return InvariantPointers;
    }

    unsigned getTimestamp() {
        return timestamp;
    }

    void setTimestamp(unsigned time) {
        timestamp = time;
    }
    bool isBroken(){
        return broken;
    }
    void setBroken(bool flag){
        broken = flag;
    }
    void addToFixedVariables(Variable* var){
        fixedVariablePointers.push_back(var);
    }
    std::vector<Variable*>& getFixedVariables(){
        return fixedVariablePointers;
    }


protected:
    
    std::vector<Variable*> fixedVariablePointers;
    std::vector<Variable*> VariablePointers;
    std::vector<invariant> InvariantPointers;
    unsigned invariantID;
    coefType DeltaValue = 0;
    unsigned constraintPriority = 0;
    std::unordered_map<int, coefType> coefficients;
    int type;
    /// Should be defined when creating oneway constraints that define (integer)variables 
    double startValue = 0;
    int lowerbound = std::numeric_limits<int>::min() + 1;
    int upperbound = std::numeric_limits<int>::max() - 1;
    int CurrentValue = 0;
    unsigned timestamp;
    bool inViolated= false;
    bool representCons = false;
    int variableID = -1;
    Variable* variable;
    bool broken = false;
    //    InvariantContainer invariants;
    //    updateVector update;
    //    std::set<Invariant*, compare_invariant> update;
    //    bool changeAdd = false;

private:

};


#endif	/* INVARIANT_HPP */

