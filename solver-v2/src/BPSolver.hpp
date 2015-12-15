#ifndef BPSOLVER_HPP
#define	BPSOLVER_HPP

#include <cmath>
#include <algorithm>

#include <iostream>
#include "BP_Data.hpp"
#include "GeneralSolver.hpp"
#include "IntegerVariable.hpp"
//#include "boost/make_shared.hpp"
//#include "boost/shared_ptr.hpp"
//using namespace Gecode;

class BPSolver : public GeneralSolver {
protected:
    //    BP_Input *p_in;
    //    int model;
    //    FloatVarArray varFloat;
    //    IntVarArray varInt;
    //    FloatVar cost;
    //    BoolVarArray varInt2;
    //    vector<IntegerVariable>* varInt;
    // Evt udnødt det som en pointer og man kan ligge til pointeren for at få specifikke pladser.


public:

    //    BPSolver(/*const Gecode::InstanceOptions& opt, */BP_Input *in) :

    BPSolver(BP_Input *in) {
        //        std::vector<IntegerVariable*>* varInt;
        int counter = 0;
        std::vector<IntegerVariable*> integerVariables;
        std::vector<IntegerVariable*> binaryVariables;
        for (var intVar : in->getVars()) {
            //            counter++;
            //            std::cout << "adding variable number " << counter << std::endl;
            IntegerVariable* iv = createIntVar(intVar.lb, intVar.ub);

            if (intVar.ub == 1) {
                counter++;
                binaryVariables.push_back(iv);
            } else {
                integerVariables.push_back(iv);
            }

        }
        std::cout << "Number of binary variables " << counter << std::endl;

        //        std::cout << "Variables created" << std::endl;
        vector<IntegerVariable*>& varInt = getAllVariables();
        //        for (unsigned i = 0; i < varInt->size(); i++) {
        //            assert(varInt->at(i)->getID() == i);
        //        }
        //        std::vector<IntegerVariable*>* varInt = GeneralSolver::createIntVars(in->getNvars(), 0, 1);
        counter = 0;
        for (unsigned i = 0; i < in->getNcons(); i++) {
            //            std::cout << "creating linear " << i << std::endl;
            const std::vector<elem> leftside = in->getMatcoeff(i);
            bounds b = in->getBterms(i);
            vector<double> c(leftside.size());
            if (leftside.size() == 1) {
                counter++;
            }
            vector<IntegerVariable*> x(leftside.size());
//                        vector<IntegerVariable*>* x = new vector<IntegerVariable*>(leftside.size());
            for (unsigned j = 0; j < leftside.size(); j++) {
                elem e = leftside[j];
                c.at(j) =  (e.coeff);
//                assert(e.coeff == (double) c.at(j));
                //                x->push_back(varInt.at(e.index));
//                x->at(j) = varInt.at(e.index);
                x.at(j) = varInt.at(e.index);
                //                x.at(j) = varInt->at(e.index);
            }
            double upperbound = (b.ub);
//            int upperbound2 = upperbound;
            //            std::cout << "posting" << std::endl;
            //            std::cout << "constraint nr " << i << std::endl;
//            if (x->size() != 0) {
            if (x.size() != 0) {
                if (b.type == 5) {
                    //                GeneralSolver::linear(*this, c, x, EQ, upperbound, Gecode::ICL_DOM, HARD);
                    GeneralSolver::linear(c, x, EQ, upperbound, HARD);
                } else {
                    //                GeneralSolver::linear(*this, c, x, LQ, upperbound, Gecode::ICL_DOM, HARD);
                    GeneralSolver::linear(c, x, LQ, upperbound, HARD);
                }
            }
//            delete x;
            // deleter den også pointer inden i vector?
            //            delete x;
            //            delete c;
            //            this->print(std::cout);
            //            std::cout << std::endl;
            //            sleep(1);
        }
        std::cout << "Number of Singleton constraints " << counter << std::endl;
        //        std::cout << "Constraints posted" << std::endl;
        // Add objective function
        std::vector<double> c(varInt.size());
//        std::vector<IntegerVariable*>* x = new std::vector<IntegerVariable*>(varInt.size());
        std::vector<IntegerVariable*> x (varInt.size());

        for (unsigned i = 0; i < varInt.size(); i++) {
            
            c.at(i) =  (in->getVar(i).objcoeff);
//            x->at(i) = varInt.at(i);
            x.at(i) = varInt.at(i);
        }

        GeneralSolver::linear(c, x, LQ, 0, OBJ);
//        delete x;

        //        Search(getAllVariables());
        Search(binaryVariables);
        Search(integerVariables);


        //        std::cout << "Model posted" << std::endl;
        //        std::cout << "done " << std::endl;
        //        GeneralSolver::linear(*this, c, varInt, LQ, 0, Gecode::ICL_DOM, OBJ);
        //        std::cout << "obj posted" << std::endl;
        // Branch
        //        GeneralSolver::branch(*this, varInt, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());

    }

    ~BPSolver() {
        //        delete p_in;
    }

    /// Constructor for cloning s

    //    BPSolver(bool share, BPSolver& s) :
    //    GeneralSolver(share, s), p_in(s.p_in) {
    //        varInt.update(*this, share, s.varInt);
    ////        std::cout << "Array committed " << &varInt << std::endl;
    ////        std::cout << "size of copy " << sizeof(*this) << std::endl;
    //        commit(&varInt);
    //        // remember to update your main variables!
    //        //		model = s.model;
    //    }
    //    // Copy during cloning
    //    virtual Space* copy(bool share) {
    ////        std::cout << "copy" << std::endl;
    ////        commit(&this->varInt);
    //        return new BPSolver(share, *this);
    //    }
    //
    //    /// Print solution
    //
    //    void print(std::ostream& os) const {
    ////        std::cout << "Is first variable assigned? " << varInt[0].assigned() << std::endl;
    ////        std::cout << "Address of first variable " << &varInt[0] << std::endl;
    ////        os << "This will crash after values are set " << std::endl;
    ////        std::cout << "in print" << std::endl;
    //        os << varInt << std::endl;
    ////        std::cout << "Address of array " << &varInt << std::endl;
    //        //    std::cout << &varInt << std::endl;
    //        //    std::cout << LSSpace::IntVarVector.at(0).ArrayPointer << std::endl;
    //
    //    }

    void printCurrent() {
        GeneralSolver::printCurrent();
    }

    //void print_stats(Search::Statistics &stat) {
    //    cout << "\tfail: " << stat.fail << endl;
    //    cout << "\tnodes: " << stat.node << endl;
    //    cout << "\tpropagators: " << stat.propagate << endl; // see page 145 MPG
    //    cout << "\tdepth: " << stat.depth << endl;
    //    cout << "\trestarts: " << stat.restart << endl;
    //    cout << "\tnogoods: " << stat.nogood << endl;
    //}

};


#endif	/* BPSOLVER_HPP */

