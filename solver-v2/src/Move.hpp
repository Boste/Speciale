#include "State.hpp"
#include "IntegerVariable.hpp"

#ifndef MOVE_HPP
#define	MOVE_HPP


// maybe it should be templated for other variable types

class Move {
protected:
    std::vector<IntegerVariable*> variables;
    std::vector<int> oldValue;
    std::vector<int> newValue;
    std::vector<int> deltaValue;

public:

    Move() {

    }

    ~Move() {
        delete variables;
        delete deltaValue;
        delete newValue;
        delete oldValue;
    }

    Move::Move(const Move& orig) {
        this->variables = orig.variables;
        this->oldValue = orig.oldValue;
        this->newValue = orig.newValue;
        this->deltaValue = orig.deltaValue;
    }

    std::vector<IntegerVariable*> getVariables() {
        return variables;
    }

    std::vector<int> getOldValue() {
        return oldValue;
    }

    std::vector<int> getNewValue() {
        return newValue;
    }

    std::vector<int> getDeltaValue() {
        return deltaValue;
    }


};


#endif	/* MOVE_HPP */

