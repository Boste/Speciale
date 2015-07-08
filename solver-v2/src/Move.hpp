#include "State.hpp"
#include "IntegerVariable.hpp"

#ifndef MOVE_HPP
#define	MOVE_HPP


// maybe it should be templated for other variable types

class Move {
protected:
    //    std::vector<IntegerVariable*> variables;
    //    std::vector<int> oldValue;
    //    std::vector<int> newValue;
    //    std::vector<int> deltaValue;



public:
    IntegerVariable* first;
    int deltaValueFirst;
    IntegerVariable* second;
    int deltaValueSecond;
    IntegerVariable* third;
    int deltaValueThird;
    int moveType;
    
    

    Move() :
    first(NULL), deltaValueFirst(0), second(NULL), deltaValueSecond(0), third(NULL), deltaValueThird(0), moveType(0) {
        
    }
    Move(IntegerVariable* var1, int delta1, int type) :
    first(var1), deltaValueFirst(delta1), second(NULL), deltaValueSecond(0), third(NULL), deltaValueThird(0), moveType(type) {
        assert(type < 4);
        assert(0 < type);
    }

    Move(IntegerVariable* var1, int delta1, IntegerVariable* var2, int delta2, int type) :
    first(var1), deltaValueFirst(delta1), second(var2), deltaValueSecond(delta2), third(NULL), deltaValueThird(0), moveType(type) {
        assert(type < 4);
        assert(0 < type);
        assert(var1 != var2);
    }

    Move(IntegerVariable* var1, int delta1, IntegerVariable* var2, int delta2, IntegerVariable* var3, int delta3, int type) :
    first(var1), deltaValueFirst(delta1), second(var2), deltaValueSecond(delta2), third(var3), deltaValueThird(delta3), moveType(type) {
        assert(type < 4);
        assert(0 < type);
        assert(var1 != var2);
        assert(var2 != var3);
        assert(var1 != var3);
    }
    void flip(){
        assert(moveType == FLIP);
        deltaValueFirst = 1-first->getCurrentValue()-first->getCurrentValue();
    }

    ~Move() {
        //        delete &variables;
        //        delete &deltaValue;
        //        delete &newValue;
        //        delete &oldValue;
    }

    Move(const Move& a) {

        //        this->variables = orig.variables;
        //        this->oldValue = orig.oldValue;
        //        this->newValue = orig.newValue;
        //        this->deltaValue = orig.deltaValue;
        if (this != &a) {
            this->first = a.first;
            this->second = a.second;
            this->third = a.third;
            this->deltaValueFirst = a.deltaValueFirst;
            this->deltaValueSecond = a.deltaValueSecond;
            this->deltaValueThird = a.deltaValueThird;
            this->moveType = a.moveType;
        }
    }

    Move& operator=(const Move& a) {
        if (this != &a) {
            this->first = a.first;
            this->second = a.second;
            this->third = a.third;
            this->deltaValueFirst = a.deltaValueFirst;
            this->deltaValueSecond = a.deltaValueSecond;
            this->deltaValueThird = a.deltaValueThird;
            this->moveType = a.moveType;
        }
        return *this;
    }
    void copy(Move* mv){
        if (this != mv) {
            this->first = mv->first;
            this->second = mv->second;
            this->third = mv->third;
            this->deltaValueFirst = mv->deltaValueFirst;
            this->deltaValueSecond = mv->deltaValueSecond;
            this->deltaValueThird = mv->deltaValueThird;
            this->moveType = mv->moveType;
        }
    } 

    //    std::vector<IntegerVariable*>* getVariables() {
    //        return &variables;
    //    }
    //
    //    std::vector<int>* getOldValue() {
    //        return &oldValue;
    //    }
    //
    //    std::vector<int>* getNewValue() {
    //        return &newValue;
    //    }
    //
    //    std::vector<int>* getDeltaValue() {
    //        return &deltaValue;
    //    }


};


#endif	/* MOVE_HPP */

