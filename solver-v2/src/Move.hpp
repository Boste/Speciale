//#include "Model.hpp"
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
    std::vector<int> deltaVector;

    IntegerVariable* var;
    int variableChange;
    std::vector<IntegerVariable*> variables;
    std::vector<int> variableChanges;
    //    IntegerVariable* second;
    //    int deltaValueSecond;
    //    IntegerVariable* third;
    //    int deltaValueThird;
    //    int moveType;

    //    void updateDelta(std::vector<int>& delta) {
    //        assert(deltaChanges.size() == delta.size());
    //        for (unsigned i = 0; i < delta.size(); i++) {
    //            deltaChanges[i] = delta[i];
    //        }
    //    }

    /// Set size of delta vector;

    void setDeltaVector(std::vector<int>& evaluationChanges) {
        //        for (unsigned i = deltaVector.size(); i < size; i++) {
        deltaVector = evaluationChanges;
        //        }
    }

    /// Return a vector of changes (obj, violations1, violations2, ..) by reference

    std::vector<int>& getDeltaVector() {
        return deltaVector;
    }
    /// Empty Move (use neighborhood.next())

    Move() {

    }

    Move(IntegerVariable* iv, int change) {
        var = iv;
        this->variableChange = change;
    }

    Move(std::vector<IntegerVariable*> vars, std::vector<int> changes) {
        variables = vars;
        this->variableChanges = changes;
    }

    IntegerVariable* getVar() {
        return var;
    }

    std::vector<IntegerVariable*>& getVars() {
        return variables;
    }

    int getVariableChange() {
        return variableChange;
    }

    std::vector<int>& getVariableChanges() {
        return variableChanges;
    }


    //
    //    Move(int deltaVectorSize, int moveType) :    first(NULL), deltaValueFirst(0), second(NULL), deltaValueSecond(0), third(NULL), deltaValueThird(0), moveType(moveType) {
    //        setDeltaChangeSize(deltaVectorSize);
    //    }
    //    Move() :    first(NULL), deltaValueFirst(0), second(NULL), deltaValueSecond(0), third(NULL), deltaValueThird(0), moveType(0) {
    //
    //    }
    //
    //    Move(IntegerVariable* var1, int delta1, int type) :
    //    first(var1), deltaValueFirst(delta1), second(NULL), deltaValueSecond(0), third(NULL), deltaValueThird(0), moveType(type) {
    //        assert(type < 4);
    //        assert(0 < type);
    //    }
    //
    //    Move(IntegerVariable* var1, int delta1, IntegerVariable* var2, int delta2, int type) :
    //    first(var1), deltaValueFirst(delta1), second(var2), deltaValueSecond(delta2), third(NULL), deltaValueThird(0), moveType(type) {
    //        assert(type < 4);
    //        assert(0 < type);
    //        assert(var1 != var2);
    //    }
    //
    //    Move(IntegerVariable* var1, int delta1, IntegerVariable* var2, int delta2, IntegerVariable* var3, int delta3, int type) :
    //    first(var1), deltaValueFirst(delta1), second(var2), deltaValueSecond(delta2), third(var3), deltaValueThird(delta3), moveType(type) {
    //        assert(type < 4);
    //        assert(0 < type);
    //        assert(var1 != var2);
    //        assert(var2 != var3);
    //        assert(var1 != var3);
    //    }
    //
    //    void flip() {
    //        assert(moveType == FLIP);
    //        deltaValueFirst = 1 - first->getCurrentValue() - first->getCurrentValue();
    //    }

    ~Move() {
        //        first = NULL;
        //        second = NULL;
        //        third = NULL;
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
            //            this->second = a.second;
            //            this->third = a.third;
            this->var = a.var;
            this->variableChange = a.variableChange;
            this->variableChanges = a.variableChanges;
            this->variables = a.variables;
            this->deltaVector = a.deltaVector;
            //            this->moveType = a.moveType;
        }
    }

    Move& operator=(const Move& a) {
        if (this != &a) {
            this->var = a.var;
            this->variableChange = a.variableChange;
            this->variableChanges = a.variableChanges;
            this->variables = a.variables;
            this->deltaVector = a.deltaVector;


            //            this->first = a.first;
            //            this->second = a.second;
            //            this->third = a.third;
            //            this->deltaValueFirst = a.deltaValueFirst;
            //            this->deltaValueSecond = a.deltaValueSecond;
            //            this->deltaValueThird = a.deltaValueThird;
            //            this->moveType = a.moveType;
        }
        return *this;
    }

    void copy(Move* mv) {
        if (this != mv) {
            this->var = mv->var;
            this->variableChange = mv->variableChange;
//            this->variableChanges = mv->variableChanges;
//            this->variables = mv->variables;
            this->deltaVector = mv->deltaVector;
//    void copy(Move* mv) {
//        if (this != mv) {
//            this->var = mv->var;
//            this->variableChange = mv->variableChange;
////            this->variableChanges = mv->variableChanges;
////            this->variables = mv->variables;
//            this->deltaVector = mv->deltaVector;
            //            this->first = mv->first;
            //            this->second = mv->second;
            //            this->third = mv->third;
            //            this->deltaValueFirst = mv->deltaValueFirst;
            //            this->deltaValueSecond = mv->deltaValueSecond;
            //            this->deltaValueThird = mv->deltaValueThird;
            //            //            this->moveType = mv->moveType;
            //            //            std::cout << "copy move ";
            //            for (unsigned i = 0; i < mv->deltaVector.size(); i++) {
            //
            //                this->deltaVector[i] = mv->deltaVector[i];
            //                //                    std::cout << deltaChanges[i] << " ";
            //
            //            }
            //            std::cout << std::endl; 
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

