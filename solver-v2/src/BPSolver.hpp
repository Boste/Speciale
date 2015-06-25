#ifndef BPSOLVER_HPP
#define	BPSOLVER_HPP

#include "BP_Data.hpp"
#include <cmath>
#include <algorithm>
#include "GeneralSolver.hpp"
#include "IntegerVariable.hpp"
#include "boost/make_shared.hpp"
#include "boost/shared_ptr.hpp"
//using namespace Gecode;

class BPSolver : public GeneralSolver {
protected:
    BP_Input *p_in;
    int model;
    //    FloatVarArray varFloat;
    //    IntVarArray varInt;
    //    FloatVar cost;
    //    BoolVarArray varInt2;
    //    vector<IntegerVariable>* varInt;
    // Evt udnødt det som en pointer og man kan ligge til pointeren for at få specifikke pladser.


public:

    BPSolver(/*const Gecode::InstanceOptions& opt, */BP_Input *in) :
    p_in(in)/*, varInt(*this, (int) in->getNvars(), 0, 1) /*, varInt2(*this,(int) in->getNvars(), 0, 1) */ {

        vector<IntegerVariable*>* varInt = GeneralSolver::createIntVars(in->getNvars(), 0, 1);

        for (unsigned i = 0; i < in->getNcons(); i++) {
            //            std::cout << varInt->size() << std::endl;
            const vector<elem> leftside = in->getMatcoeff(i);
            bounds b = in->getBterms(i);
            vector<int>* c = new vector<int>(leftside.size());
            vector<IntegerVariable*>* x = new vector<IntegerVariable*>();

            for (unsigned j = 0; j < leftside.size(); j++) {
                elem e = leftside[j];
                
                //                c[j] = e.coeff;
                //                x[j] = varFloat[e.index];
                //            c[j] = static_cast<int>(e.coeff*scale);
                c->at(j) = static_cast<int> (e.coeff);
//                boost::shared_ptr<IntegerVariable> tmp = boost::make_shared<IntegerVariable>();
               
                x->push_back(varInt->at(e.index));
//                x.push_back(&varInt->at(e.index));

            }

            int upperbound = static_cast<int> (b.ub);
            //        int lb = static_cast<int>(b.lb*scale);
            if (b.type == 5) {
                GeneralSolver::linear(*this, c, x, Gecode::IRT_EQ, upperbound, Gecode::ICL_DOM, 2);
            } else {
                GeneralSolver::linear(*this, c, x, Gecode::IRT_LQ, upperbound, Gecode::ICL_DOM, 2);
            }

        }
        
        // Add objective function
        vector<int>* c = new vector<int>(varInt->size());
//        std::cout << "objective function " << std::endl;
        for(unsigned i = 0; i < varInt->size(); i++ ){
            c->at(i) = static_cast<int> (in->getVars(i).objcoeff);      
//            std::cout << c->at(i) << " + ";
        }
        std::cout << std::endl;
        GeneralSolver::linear(*this,c, varInt, Gecode::IRT_LQ, 0, Gecode::ICL_DOM, 1 );
        //        std::cout << "BPS after linear" << std::endl;
        //        std::cout << varInt->back().ArrayPointer << std::endl;
            
        GeneralSolver::branch(*this, varInt, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());
        //        std::cout << "efter branch BPS" << std::endl;

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
        LSSpace::printCurrent();
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

