#ifndef BPSOLVER_HPP
#define	BPSOLVER_HPP

#include <cmath>
#include <algorithm>

#include <iostream>
#include "BP_Data.hpp"
#include "GPSolver.hpp"
#include "Variable.hpp"
//#include "boost/make_shared.hpp"
//#include "boost/shared_ptr.hpp"
//using namespace Gecode;

class BPSolver : public GPSolver {
protected:
public:

    BPSolver(BP_Input *in) {
        std::vector<Variable*> integerVariables;
        std::vector<Variable*> binaryVariables;
        for (var variable : in->getVars()) {
            Variable* iv = createVariable(variable.lb, variable.ub);

            if (variable.ub == 1) {
                binaryVariables.push_back(iv);
            } else {
                integerVariables.push_back(iv);
            }

        }
        vector<Variable*>& varInt = getAllVariables();
        for (unsigned i = 0; i < in->getNcons(); i++) {
            //            std::cout << "creating linear " << i << std::endl;
            const std::vector<elem> leftside = in->getMatcoeff(i);
            bounds b = in->getBterms(i);
            vector<int> c(leftside.size());
            //            if (leftside.size() == 1) {
            //                counter++;
            //            }
            vector<Variable*> x(leftside.size());
            //                        vector<IntegerVariable*>* x = new vector<IntegerVariable*>(leftside.size());
            for (unsigned j = 0; j < leftside.size(); j++) {
                elem e = leftside[j];
                c.at(j) = (e.coeff);
                assert(e.coeff == (double) c.at(j));
                x.at(j) = varInt.at(e.index);
            }
            double upperbound = (b.ub);
            if (x.size() != 0) {
                if (b.type == 5) {
                    GPSolver::linear(c, x, EQ, upperbound, HARD);
                } else {
                    GPSolver::linear(c, x, LQ, upperbound, HARD);
                }
            }
        }
        // Add objective function
        std::vector<int> c;
        std::vector<Variable*> x;
        std::vector<unsigned> obj = in->getObjVars();
        for (unsigned i : obj) {
            c.push_back((int) in->getVar(i).objcoeff);
            assert(in->getVar(i).objcoeff == (coefType) c.back());
            x.push_back(varInt.at(i));
        }
        //        GPSolver::addObjective(c,x);
        GPSolver::linear(c, x, LQ, 0, OBJ);
        Search(binaryVariables);

    }

    ~BPSolver() {
    }

    void printCurrent() {
        GPSolver::printVariableValues();
    }


};


#endif	/* BPSOLVER_HPP */

