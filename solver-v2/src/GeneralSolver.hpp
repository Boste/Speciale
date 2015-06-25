#ifndef GENERALSOLVER_HPP
#define	GENERALSOLVER_HPP
#include <cmath>
#include <algorithm>
#include "LSSpace.hpp"
#include <assert.h>
#include <gecode/driver.hh>
#include <gecode/int.hh>
//#include <gecode/minimodel.hh>
#include <limits>
#include "IntegerVariable.hpp"
//using namespace Gecode;

class GeneralSolver : public Gecode::Script, public LSSpace {
private:
    //    Gecode::IntVarArray* solutions;
    Gecode::IntVarArray IntVars;
public:

    GeneralSolver() {

    }

    GeneralSolver& operator=(const GeneralSolver &a) {
        this->IntVars = a.IntVars;
        //        std::cout << "hest " << std::endl;
    }

    void branch(Gecode::Home home, vector<IntegerVariable*>* vars, Gecode::IntVarBranch var, Gecode::IntValBranch val) {

        Gecode::branch(home, IntVars, var, val);
        //        std::cout << IntVars.size() << std::endl;

    }

    void linear(GeneralSolver& s, vector<int>* coefficients, vector<IntegerVariable*>* variables, Gecode::IntRelType irt, int ub, Gecode::IntConLevel icl, int type) { // 0 = implicit, 1 = soft, 2 = invariant 
        if (type == 1) {
            // add to obj fnc
            LSSpace::linear(coefficients, variables, 0, ub, type);

        } else {
            Gecode::IntArgs c(coefficients->size());
            Gecode::IntVarArgs x(coefficients->size());
            for (unsigned j = 0; j < coefficients->size(); j++) {
                c[j] = coefficients->at(j);
                x[j] = *(variables->at(j)->getVariablePointer());
            }
            //        std::cout << "før gecode" << std::endl;

            Gecode::linear(s, c, x, irt, ub, icl);

            // What should be given to LSSpace
            if (irt == Gecode::IRT_EQ) {
                LSSpace::linear(coefficients, variables, 0, ub, type);
            } else if (irt == Gecode::IRT_GQ) {
                LSSpace::linear(coefficients, variables, 1, ub, type);
            }
        }
    }

    void SetValues(Gecode::IntVarArray vars) {
        for (int i = 0; i < vars.size(); i++) {
            assert(vars[i].assigned());
            this->IntVarVector.at(i)->setCurrentValue(vars[i].val());
        }
    }

    std::vector<IntegerVariable*>* createIntVars(unsigned numberOfVariables, int lb, int ub) {
        // Given to gecode space
        Gecode::IntVarArray Vars(*this, numberOfVariables, lb, ub);
        IntVars = Vars;

        // Given to LS space


        return LSSpace::addIntVariablesToLS(IntVars);
        //        return LSSpace::addIntVariablesToLS(IntVars);
        //        return vars;
    }

    Gecode::BoolVarArray createBoolVars(int amount, int lb, int ub) {
        // Given to gecode space
        Gecode::BoolVarArray vars(*this, amount, lb, ub);

        // Given to LS space
        return vars;
    }

    /// Constructor for cloning s

    GeneralSolver(bool share, GeneralSolver & s) :
    Gecode::Script(share, s) {
        //        std::cout << "this is not a line " << IntVars.size() << std::endl;
        IntVars.update(*this, share, s.IntVars);
        // remember to update your main variables!
        //		model = s.model;

    }

    // Copy during cloning

    virtual Space * copy(bool share) {
        //        std::cout << "copy" << std::endl;
        return new GeneralSolver(share, *this);
    }

    void print(std::ostream & os) const {
        os << IntVars << std::endl;
    }

    void print(std::vector<IntegerVariable> IntegerVariables) {
        std::cout << "{";
        for (unsigned i = 0; i < IntegerVariables.size(); i++) {
            std::cout << *(IntegerVariables[i].getVariablePointer()) << ", ";
        }
        std::cout << std::endl;
    }


    //    void InitialSolution(Gecode::InstanceOptions opt) {

    void InitialSolution() {
        //	// This is to stop the search at the time limit imposed
        Gecode::Search::Options so;

        startTimer(so);

        printSpaceStatus();

        try {
            //            std::cout << "try" << std::endl;
            Gecode::DFS<GeneralSolver> e(this, so);
            GeneralSolver* s;
            int counter = 0;
            while (!e.stopped()) {
                std::cout << __LINE__ << std::endl;
                sleep(1);
                 s = e.next();
                 counter++;
                 if(counter>2){
                    std::cout << s->IntVars.size() << std::endl;
                 }
                 
            }
//                             s = e.next();

            //            std::cout << this->IntVars << std::endl;
//            GeneralSolver* s = e.next();
            //            std::cout << s->failed() << std::endl;
            //            s->print(std::cout);
            //            std::cout << "after print" << std::endl;
            s->compare(*this, std::cout);
            //            std::cout << __LINE__ << std::endl;
            //            std::cout << IntVars.size() << std::endl;
            //            std::cout << s->IntVars.size() << std::endl;
            assert(!e.stopped());
            this->SetValues(s->IntVars);

            //            for(int i = 0; i < IntVars.size(); i++ ){
            //                if(IntVarVector.at(i).getCurrentValue() != s->IntVars[i].val()){
            //                    std::cout << IntVarVector.at(i).getCurrentValue() << " vs " << s->IntVars[i] << std::endl;
            //                }
            //            }
            //            std::cout << __LINE__ << std::endl;

            initializeInvariants();
            //            std::cout << __LINE__ << std::endl;

            initializeConstraints();
            //            std::cout << __LINE__ << std::endl;

            initializeObjective();
            //            std::cout << __LINE__ << std::endl;

            printCurrent();



            //            for (int i = 0; i < Invariants.size(); i++) {
            //                std::cout << Invariants.at(i).getValue() << " " << LSSpace::testInvariant(i) << "  ";
            //                
            //            }

            if (e.stopped()) {
                cout << "WARNING: solver stopped, solution is not optimal!\n";
                if (so.stop->stop(e.statistics(), so)) {
                    cout << "\t Solver stopped because of TIME LIMIT!\n";
                }
            }
            delete s;

        } catch (Gecode::Exception e) {
            std::cerr << "Gecode exception: " << e.what() << std::endl;
            //        return 1;
        }
        //
        //        this->print(cout);

    }

    void optimizeSolution() {
        bestMove();
    }



private:



    //    void initializeInvariants() {
    //        for (int i = 0; i < LSSpace::Invariants.size(); i++) {
    //            LSSpace::Invariants.at(i).initialize();
    //        }
    //    }

    void printSpaceStatus() {

        Gecode::SpaceStatus status = this->status();

        if (status == Gecode::SS_FAILED) {
            cout << "Status: " << this->status() << " the space is failed at root."
                    << endl;
        } else if (status == Gecode::SS_SOLVED) {
            cout << "Status: " << this->status()
                    << " the space is not failed but the space has no brancher left."
                    << endl;
        } else if (status == Gecode::SS_BRANCH) {
            cout << "Status: " << this->status()
                    << " the space is not failed and we need to start branching."
                    << endl;
        }
    }

    void startTimer(Gecode::Search::Options so) {
        //        Gecode::Search::TimeStop* ts = new Gecode::Search::TimeStop(opt.time());
        Gecode::Search::TimeStop* ts = new Gecode::Search::TimeStop(7200000);
        so.stop = ts;
        Gecode::Search::Statistics stat;
        //
        // Let's start the Timer
        Gecode::Support::Timer t;
        t.start();
    }

    void print_stats(Gecode::Search::Statistics & stat) {
        cout << "\tfail: " << stat.fail << endl;
        cout << "\tnodes: " << stat.node << endl;
        cout << "\tpropagators: " << stat.propagate << endl; // see page 145 MPG
        cout << "\tdepth: " << stat.depth << endl;
        cout << "\trestarts: " << stat.restart << endl;
        cout << "\tnogoods: " << stat.nogood << endl;
    }


};

#endif	/* GENERALSOLVER_HPP */

