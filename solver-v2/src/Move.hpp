#include "Variable.hpp"

#ifndef MOVE_HPP
#define	MOVE_HPP



class Move {
protected:


public:
    std::vector<int> deltaVector;

    Variable* var;
    int variableChange;
    std::vector<Variable*> variables;
    std::vector<int> variableChanges;
    unsigned id;

    void setID(unsigned id){
        this->id = id;
    }
    unsigned getID(){
        return id;
    }

    void setDeltaVector(std::vector<int>& evaluationChanges) {
        deltaVector = evaluationChanges;
    }
    /// Return a vector of changes (obj, violations1, violations2, ..) by reference

    std::vector<int>& getDeltaVector() {
        return deltaVector;
    }
    /// Empty Move (use neighborhood.next())

    Move() {

    }

    Move(Variable* iv, int change) {
        var = iv;
        this->variableChange = change;
    }

    Move(std::vector<Variable*> vars, std::vector<int> changes) {
        variables = vars;
        this->variableChanges = changes;
    }

    Variable* getVar() {
        return var;
    }

    std::vector<Variable*>& getVars() {
        return variables;
    }

    int getVariableChange() {
        return variableChange;
    }

    std::vector<int>& getVariableChanges() {
        return variableChanges;
    }


    ~Move() {
    }

    Move(const Move& a) {

        if (this != &a) {
            this->var = a.var;
            this->variableChange = a.variableChange;
            this->variableChanges = a.variableChanges;
            this->variables = a.variables;
            this->deltaVector = a.deltaVector;
        }
    }

    Move& operator=(const Move& a) {
        if (this != &a) {
            this->var = a.var;
            this->variableChange = a.variableChange;
            this->variableChanges = a.variableChanges;
            this->variables = a.variables;
            this->deltaVector = a.deltaVector;

        }
        return *this;
    }

    void copy(Move* mv) {
        if (this != mv) {
            this->var = mv->var;
            this->variableChange = mv->variableChange;
            this->variableChanges = mv->variableChanges;
            this->variables = mv->variables;
            this->deltaVector = mv->deltaVector;
        }
    }

};


#endif	/* MOVE_HPP */

