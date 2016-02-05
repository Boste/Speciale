#ifndef VARIABLE_HPP
#define	VARIABLE_HPP
//#include <gecode/driver.hh>
#include <gecode/int.hh>


#include "Constraint.hpp"
#include "Constants.hpp"

//#include "BP_Data.hpp"
//#include "../include/constraints/invariant.hpp"

//class Constraint;
class Invariant;

class Variable {
    friend class GPSolver;
protected:
    //    Gecode::IntVar* VariablePointer;
    //    Gecode::IntVarArray* ArrayPointer;
    //    int ArrayID;
    int lowerBound;
    int upperBound;
    int vectorID;
    unsigned defining = 0;
    int CurrentValue = 0;
//    bool isInteger = false;
    bool isDefined = false;
    bool isFix = false;
//    VariableInConstraints constraints;
    invariant oneway;
    //    constraint definedByCons;
    Constraint* definedByCons;
    unsigned searchPrio = 0;
    bool inObjective = false;

    operator int () {
        return CurrentValue;
    }

    Gecode::BoolVar* VariablePointer;
    unsigned usedIn = 0;


public:
    /// Only used for testing instances

    unsigned getDefining() {
        return defining;
    }

    void increaseDefining() {
        defining++;
    }

    void decreaseDefining() {
        defining--;
    }

//    VariableInConstraints& usedInConstraints() {
//        usedIn++;
//        return constraints;
//    }

    unsigned getSerachPriority() {
        return searchPrio;
    }

    /// Just used for testing but the counter for constraints is used.

//    void addToUsedInConstraints(std::shared_ptr<Constraint> constraint) {
//        constraints.push_back(constraint);
//    }
    void addToUsedInConstraints() {
        usedIn++;
    }

    invariant getOneway() {
        return oneway;
    }

    void setDefinedBy(invariant invar, Constraint* cons) {
        definedByCons = cons;
        oneway = invar;
        isDefined = true;
    }

    /// Only for testing

    Constraint* getDefinedByCon() {
        return definedByCons;
    }

    bool isDef() {
        return isDefined;
    }

    bool isFixed() {
        return isFix;
    }

    void setAsFixed() {
        isFix = true;
        lowerBound = VariablePointer->val();
        upperBound = VariablePointer->val();
        CurrentValue = VariablePointer->val();
    }

    Variable(int lowerbound, int upperbound, int id) { //: lb(lowerbound), ub(upperbound),vectorID(id),value(0) {
        //        VariablePointer = vp;
        //        ArrayPointer = ap;
        lowerBound = lowerbound;
        upperBound = upperbound;
//        if (upperbound > 1 || lowerbound < 0) {
//            isInteger = true;
//        }
        vectorID = id;
    }

    void setCurrentValue(int val) {
        CurrentValue = val;

    }

    int getCurrentValue() {
        return CurrentValue;
    }

    unsigned getID() {
        return vectorID;
    }

    void undefine() {

        isDefined = false;
        definedByCons->isOneway(false);
    }

    Gecode::BoolVar* getVariablePointer() {
        //    Gecode::IntVar* getVariablePointer() {
        return VariablePointer;
    }

    void setVariablePointer(Gecode::BoolVar& gecodeVar) {
        //    void setVariablePointer(Gecode::IntVar& gecodeVar) {
        VariablePointer = &gecodeVar;
    }

    int getLowerBound() {
        return lowerBound;
    }

    int getUpperBound() {
        return upperBound;
    }

    ~Variable() {
        //        delete VariablePointer;

    }

    bool isInObjective() {
        return inObjective;
    }


    /// Sort in order of decreasing domain
    //    struct SortGreater {
    //
    //        bool operator()(const Variable* var1, const Variable* var2) const {
    //                //            std::cout << "sorter" << std::endl;
    //            int ds1 = var1->upperBound-var1->lowerBound;
    //            int ds2 = var2->upperBound-var2->lowerBound;
    //            if(ds1==ds2){
    //                return (var1->constraints.size() > var2->constraints.size());
    //            }
    //            return (var1->upperBound-var1->lowerBound  > var2->upperBound-var2->lowerBound);
    //        }
    //    };

    struct compare_variable : public std::binary_function<Variable*, Variable*, bool> {

//        bool operator()(const IntegerVariable* var1, const IntegerVariable* var2) const {
//            //                std::cout << "is used"  << invar1 << " " << invar2 << " id1 "<< invar1->getID() <<" id2 "<< invar2->getID() << " compare " << (invar1 < invar2) << std::endl;
//            //                sleep(1);
//            return (var1->getID() > var2->getID());
//        }

        bool operator()(Variable* var1, Variable* var2) {

            //                std::cout << "is used123 " <<" id1 "<< invar1->getID() <<" id2 "<< invar2->getID() << " compare " << (invar1 < invar2) << std::endl;
            //                sleep(1);
            //        unsigned t1 = invariant_nodes.at(invar1->getID())->timestamp; 
            return (var1->getID() > var2->getID());
        }

    };


    /// Just for testing

    unsigned getDegree() {
        return usedIn;      
//        return constraints.size();
    }

    //    IntegerVariable(const IntegerVariable& orig);
    //    virtual ~IntegerVariable();
private:
    //    void updateInvariants(){
    //        for (int i = 0; i < update.size(); i++ ){
    //            int invariantNumber = update.at(i);
    //            // Make the change
    //        }
    //    }

};

#endif	/* VARIABLE_HPP */

