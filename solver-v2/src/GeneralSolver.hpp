#ifndef GENERALSOLVER_HPP
#define	GENERALSOLVER_HPP
#include <cmath>
#include <algorithm>
#include "LSSpace.hpp"
#include <assert.h>
#include <gecode/driver.hh>
#include <gecode/int.hh>
#include "Constants.hpp"
//#include <gecode/minimodel.hh>
#include <limits>
#include "IntegerVariable.hpp"
#include "Multistop.hpp"
//#include "Invariant.hpp"
//#include "Constraint.hpp"
#include "Clock.hpp"
//#include "getRSS.hpp"
//#include "Random.hpp"
//using namespace Gecode;

class GeneralSolver : public Gecode::Space, private LSSpace {
    friend class Test;
private:
    //    Gecode::IntVarArray* solutions;
    Gecode::IntVarArray IntVars;
    //    std::vector<Gecode::IntVarArgs*> test;
public:
    State* st = new State();

    GeneralSolver() {
        //        Random::Seed(SEED);



    }

    ~GeneralSolver() {
        //        std::cout << "Destructing GS" << std::endl;

        delete st;
    }

    GeneralSolver& operator=(const GeneralSolver &a) {
        this->IntVars = a.IntVars;
        //        std::cout << "hest " << std::endl;
        return *this;

    }

    void branch(Gecode::Home home, vector<IntegerVariable*>* vars, Gecode::IntVarBranch var, Gecode::IntValBranch val) {

        Gecode::branch(home, IntVars, var, val);
        //        std::cout << IntVars.size() << std::endl;

    }
    // Skal Gecode::IntConLevel icl være et argument?

    void linear(GeneralSolver& s, std::vector<int>* coefficients, vector<IntegerVariable*>* variables, int relation, int ub, int type) { // 0 = obj, 1 = soft, 2 = hard 
        Sum* sumInvariant = new Sum(variables, coefficients);

        //        std::cout << sumInvariant << std::endl;
        int variableNumber;
        int invariantNumber;

        for (unsigned i = 0; i < coefficients->size(); i++) {
            variableNumber = variables->at(i)->getID();
            invariantNumber = st->getInvariants()->size();
            st->getIntegerVariables()->at(variableNumber)->addToUpdate(invariantNumber);
        }
        st->getInvariants()->push_back(sumInvariant);
        //        std::cout << sumInvariant->VariablePointers << std::endl;
        //        std::cout << "first invariant size " << st->getInvariants()->at(0)->VariablePointers->size() << std::endl;
        //        sleep(1);
        //        std::cout << "Invariant size " << st->getInvariants()->size() << std::endl;
        //        std::cout << "invariant just added size " << st->getInvariants()->at(st->getInvariants()->size()-1)->VariablePointers->size() << " vs " << coefficients->size() << std::endl;
        Linear* LinearConstraint = new Linear(sumInvariant, ub, relation);
        // add to obj fnc
        if (type == OBJ) {
            sumInvariant->usedByObjective(st->getObjectives()->size());
            st->getObjectives()->push_back(LinearConstraint);
        } else if (type == SOFT) {
            // Implement a vector of vector of constraints that is used as priority of constraints to be satisfied. 
        } else {
            Gecode::IntArgs c(coefficients->size());
            Gecode::IntVarArgs x(coefficients->size());
            for (unsigned j = 0; j < coefficients->size(); j++) {
                c[j] = coefficients->at(j);
                x[j] = *(variables->at(j)->getVariablePointer());
            }
            //        std::cout << "før gecode" << std::endl;

            //            Gecode::ICL_DOM
            switch (relation) {
                case EQ:
                    Gecode::linear(s, c, x, Gecode::IRT_EQ, ub, Gecode::ICL_DOM);
                    break;
                case LQ:
                    Gecode::linear(s, c, x, Gecode::IRT_LQ, ub, Gecode::ICL_DOM);
                    break;
                case LE:
                    Gecode::linear(s, c, x, Gecode::IRT_LE, ub, Gecode::ICL_DOM);
                    break;
                case GQ:
                    Gecode::linear(s, c, x, Gecode::IRT_GQ, ub, Gecode::ICL_DOM);
                    break;
                case GR:
                    Gecode::linear(s, c, x, Gecode::IRT_GR, ub, Gecode::ICL_DOM);
                    break;
            }

            // What should be given to LSSpace
            sumInvariant->usedByConstraint(st->getHardConstraints()->size());
            st->getHardConstraints()->push_back(LinearConstraint);

        }
    }

    //    void SetValues(Gecode::IntVarArray vars) {
    //        for (int i = 0; i < vars.size(); i++) {
    //            assert(vars[i].assigned());
    //            this->IntVarVector.at(i)->setCurrentValue(vars[i].val());
    //        }
    //    }

    std::vector<IntegerVariable*>* createIntVars(unsigned numberOfVariables, int lb, int ub) {
        // Given to gecode space
        Gecode::IntVarArray vars(*this, numberOfVariables, lb, ub);

        // Looks bad but does not copy the variables. Points to two different places in memory but the variables in the two arrays have the same memory address. 
        IntVars = vars;
        // Given to LS space
        return st->addIntegerVariable(&vars);

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
    Gecode::Space(share, s) {
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

    /// Uses Gecode to find initial solution
    /// returns a new GeneralSolver with the initial solution the old (the one this method is called from) is not updated with new solution. 

    //    GeneralSolver* InitialSolution(Gecode::Search::Options* so) {

    GeneralSolver* InitialSolution(Gecode::Search::Options* so) {

        std::cout << this << std::endl;
        GeneralSolver* s = GecodeSearch(so);
        int timesRelaxed = 0;
        while (s == NULL && timesRelaxed != 5 ) {
            s = relax(so, this, timesRelaxed);
            timesRelaxed++;
        }

        assert(s != NULL);
        assert(!s->failed());
        return s;
        //
        //        this->print(cout);

    }

    GeneralSolver* GecodeSearch(Gecode::Search::Options* so) {
        printSpaceStatus();

        GeneralSolver* s;
        try {
            std::clock_t GecodeClock = std::clock();
            Gecode::DFS<GeneralSolver> e(this, *so);
            std::cout << "Still searching for solution" << std::endl;
            s = e.next();
            if (e.stopped()) {
                cout << "WARNING: solver stopped, solution is not optimal!\n";

                if (so->stop->stop(e.statistics(), *so)) {
                    //                    cout << "\t Solver stopped because of TIME LIMIT!\n";
                    //                    cout << "\t Solver stopped because of  NODE LIMIT!\n";
                    std::cout << "\t Number of nodes expanded: " << e.statistics().node << std::endl;
                    std::cout << "\t Number of failed nodes: " << e.statistics().fail << std::endl;
                    std::cout << "\t Number of restarts: " << e.statistics().restart << std::endl;
                    double time = (std::clock() - GecodeClock) / (double) CLOCKS_PER_SEC;
                    std::cout << "\t Time spend searching for solution: " << time << " seconds" << std::endl;
                }
            } else {
                std::cout << "Gecode found solution after " << (std::clock() - GecodeClock) / (double) CLOCKS_PER_SEC << std::endl;
                std::cout << "Total time used so far " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << std::endl;
                Gecode::Search::Statistics stat = e.statistics();
                print_stats(stat);

            }

        } catch (Gecode::Exception e) {
            std::cerr << "Gecode exception: " << e.what() << std::endl;
            //        return 1;
        }
        return s;
    }


    /// relaxes the space (reduce the number of constraints). Used when Gecode cant find a solution in time.
    /// Only works for binary

    GeneralSolver* relax(Gecode::Search::Options* so, GeneralSolver* GS, int timesRelaxed) {
                std::cout << this << std::endl;

        GeneralSolver* relaxed = new GeneralSolver();
        relaxed.st = this->st;
        //        relaxed->st = this->st;
        //        std::cout << relaxed->st << " vs " << this->st << std::endl;
        //        std::cout << "first invariant size " << st->getInvariants()->at(0)->VariablePointers->size() << std::endl;
        //        std::cout << "first invariant size " << relaxed->st->getInvariants()->at(0)->VariablePointers->size() << std::endl;
        //        relaxed->st->getIntegerVariables()->clear();
        
        
        /*
        relaxed->createIntVars(IntVars.size(), 0, 1);
        IntVars = relaxed->IntVars;

        // Remake hard constraints with new gecode variables. 
        //        for (Constraint* cons : st->getHardConstraints()) {
        for (unsigned i = 0; i < st->getHardConstraints()->size(); i++) {
            Constraint* cons = st->getHardConstraints()->at(i);
            std::vector<IntegerVariable*>* variables = new std::vector<IntegerVariable*>();
            //            std::vector<int>* coefficients = new std::vector<int>();
            if (cons->getType() == LINEAR) {
                vector<IntegerVariable*>* oldVars = cons->getInvariant()->VariablePointers;
                //                std::cout <<  oldVars->size() << std::endl;
                std::unordered_map<int, int> coefficients = cons->getInvariant()->coefficients;
                for (unsigned j = 0; j < oldVars->size(); j++) {
                    IntegerVariable* var = oldVars->at(j);
                    //                for (IntegerVariable* var : oldVars) {
                    variables->push_back(relaxed->st->getIntegerVariables()->at(var->getID()));
                    //                    coefficients->push_back()
                }
                //                std::cout << "Sum1" << std::endl;
                Sum* sumInvariant = new Sum(variables, coefficients);
                int variableNumber;
                int invariantNumber;

                for (unsigned j = 0; j < oldVars->size(); j++) {
                    variableNumber = variables->at(j)->getID();
                    invariantNumber = relaxed->st->getInvariants()->size();
                    relaxed->st->getIntegerVariables()->at(variableNumber)->addToUpdate(invariantNumber);
                }
                relaxed->st->getInvariants()->push_back(sumInvariant);
                int relation = cons->getArgument(0);
                int ub = cons->getArgument(1);
                Linear* LinearConstraint = new Linear(sumInvariant, ub, relation);
                //                Gecode::IntArgs c(coefficients->size());
                //                Gecode::IntVarArgs x(coefficients->size());
                //                for (unsigned j = 0; j < coefficients->size(); j++) {
                //                    c[j] = coefficients->at(j);
                //                    x[j] = *(variables->at(j)->getVariablePointer());
                //                }
                //                switch (relation) {
                //                    case EQ:
                //                        Gecode::linear(relaxed, c, x, Gecode::IRT_EQ, ub, Gecode::ICL_DOM);
                //                        break;
                //                    case LQ:
                //                        Gecode::linear(relaxed, c, x, Gecode::IRT_LQ, ub, Gecode::ICL_DOM);
                //                        break;
                //                    case LE:
                //                        Gecode::linear(relaxed, c, x, Gecode::IRT_LE, ub, Gecode::ICL_DOM);
                //                        break;
                //                    case GQ:
                //                        Gecode::linear(relaxed, c, x, Gecode::IRT_GQ, ub, Gecode::ICL_DOM);
                //                        break;
                //                    case GR:
                //                        Gecode::linear(relaxed, c, x, Gecode::IRT_GR, ub, Gecode::ICL_DOM);
                //                        break;
                //                }

                // What should be given to LSSpace
                sumInvariant->usedByConstraint(relaxed->st->getHardConstraints()->size());
                relaxed->st->getHardConstraints()->push_back(LinearConstraint);
            } else {
                std::cout << "It is not linear :S" << std::endl;
            }
        }
        // Remake objective func with new gecode variables. 
        //        for (Constraint* cons : st->getObjectives()) {
        for (int i = 0; i < st->getObjectives()->size(); i++) {
            Constraint* cons = st->getObjectives()->at(i);
            std::vector<IntegerVariable*>* variables = new std::vector<IntegerVariable*>();
            //            std::vector<int>* coefficients = new std::vector<int>();
            if (cons->getType() == LINEAR) {
                vector<IntegerVariable*>* oldVars = cons->getInvariant()->VariablePointers;
                std::unordered_map<int, int> coefficients = cons->getInvariant()->coefficients;
                //                for (IntegerVariable* var : oldVars) {
                for (unsigned j = 0; j < oldVars->size(); j++) {
                    IntegerVariable* var = oldVars->at(j);
                    variables->push_back(relaxed->st->getIntegerVariables()->at(var->getID()));
                    //                    coefficients->push_back()
                }
                //                std::cout << "sum2" << std::endl;
                Sum* sumInvariant = new Sum(variables, coefficients);
                int variableNumber;
                int invariantNumber;

                for (unsigned j = 0; j < oldVars->size(); j++) {
                    variableNumber = variables->at(j)->getID();
                    invariantNumber = relaxed->st->getInvariants()->size();
                    relaxed->st->getIntegerVariables()->at(variableNumber)->addToUpdate(invariantNumber);
                }
                relaxed->st->getInvariants()->push_back(sumInvariant);
                int relation = cons->getArgument(0);
                int ub = cons->getArgument(1);
                Linear* LinearConstraint = new Linear(sumInvariant, ub, relation);

                // What should be given to LSSpace
                sumInvariant->usedByObjective(relaxed->st->getObjectives()->size());
                relaxed->st->getObjectives()->push_back(LinearConstraint);
            } else {
                std::cout << "it is not linear :S" << std::endl;
            }
        }
         */ 
        
        GeneralSolver* s;
        if (timesRelaxed > 0 || st->getSoftConstraints()->size() == 0) {
            // Maybe split this into more methods. eg relax half of const, relax top ten with most variables, relax in topological order. 
            relaxed->simpleRelax(timesRelaxed);

            s = relaxed->GecodeSearch(so);
        } else {
            // Do not give soft to gecode
        }


        return s;
        // Brug operator = til at lave det gamle space til det nye? Ellers skal det hele løses med det nye og gamle pointer skal opdateres. 
        // Hvordan fungere det så med at slette? 


    }

    void simpleRelax(int timesRelaxed) {
//        std::cout << "do nothing" << std::endl;
        for (unsigned i = 0; i < st->getHardConstraints()->size()/*-((timesRelaxed+1)*100); i++) {
            Constraint* cons = st->getHardConstraints()->at(i);
            Invariant* invar = cons->getInvariant();
            std::vector<IntegerVariable*>* integerVariables = invar->VariablePointers;
            if (invar->getType() == SUM) { // otherwise it is in objective function atm.
                Gecode::IntArgs c(integerVariables->size());
                Gecode::IntVarArgs x(integerVariables->size());
                for (unsigned j = 0; j < integerVariables->size(); j++) {
                    int variableID = integerVariables->at(j)->getID();
                    c[j] = invar->coefficients.at(variableID);
                    x[j] = *(integerVariables->at(j)->getVariablePointer());
                    //                    std::cout << __LINE__ << std::endl;

                }
                //                std::cout << __LINE__ << std::endl;

                //                Constraint* cons = hardConstraints->at(invar->getUsedInConstraint());
                //                std::cout << __LINE__ << std::endl;
                //                std::cout << "give to gecode" << std::endl;
                int relation = cons->getArgument(0);
                int ub = cons->getArgument(1);
                switch (relation) {
                    case EQ:
                        Gecode::linear(*this, c, x, Gecode::IRT_EQ, ub, Gecode::ICL_DOM);
                        break;
                    case LQ:
                        Gecode::linear(*this, c, x, Gecode::IRT_LQ, ub, Gecode::ICL_DOM);
                        break;
                    case LE:
                        Gecode::linear(*this, c, x, Gecode::IRT_LE, ub, Gecode::ICL_DOM);
                        break;
                    case GQ:
                        Gecode::linear(*this, c, x, Gecode::IRT_GQ, ub, Gecode::ICL_DOM);
                        break;
                    case GR:
                        Gecode::linear(*this, c, x, Gecode::IRT_GR, ub, Gecode::ICL_DOM);
                        break;
                }
                //            Constraint* cons = hardConstraints->at(invar->usedInObjectiveNr);
                // irt (eq, lq), ub, og IntegerConsisdencyLevel. Hvor skal jeg få dem fra?



                //        std::cout << "før gecode" << std::endl;

                //            Gecode::linear(relaxed, c, x, irt, ub, icl);
                //            st->getInvariants()->at(0).
            } else {
                std::cout << "type should be SUM and assert should prevent this. Then type must be set to other than 1" << std::endl;
            }
        }
        std::cout << "SimpleRelax made " << timesRelaxed+1 << " times" << std::endl;
    }

    void initializeLS(GeneralSolver * GS) {
        SetValues(GS->IntVars);

        initializeInvariants(st);

        initializeConstraints(st);

        initializeObjective(st);
        delete GS;
    }

    void SetValues(Gecode::IntVarArray vars) {
        for (int i = 0; i < vars.size(); i++) {
            assert(vars[i].assigned());
            st->getIntegerVariables()->at(i)->setCurrentValue(vars[i].val());

        }
    }

    int getInitialValue() {
        return this->initialValue;
    }

    void optimizeSolution(int time) {
        LSSpace::optimizeSolution(time, st);

    }
    // Only for testing

    Gecode::IntVarArray * getIntVars() {
        return &IntVars;
    }

    void printCurrent() {
        if (st->getIntegerVariables()->size() > 0) {
            std::cout << "Integer Variables:" << std::endl;
        }

        for (unsigned i = 0; i < st->getIntegerVariables()->size(); i++) {
            std::cout << st->getIntegerVariables()->at(i)->getCurrentValue() << " ";
            //        std::cout << IntVarVector[i].VariablePointer << " ";

        }
        std::cout << std::endl;
        //        if (BoolVarVector.size() > 0) {
        //            std::cout << "Boolean Variables:" << std::endl;
        //        }
        //        for (unsigned i = 0; i < BoolVarVector.size(); i++) {
        //            //        std::cout << BoolVarVector[i].getCurrentVal() << " ";
        //        }
        //        std::cout << std::endl;
    }


private:



    //    void initializeInvariants() {
    //        for (int i = 0; i < LSSpace::Invariants.size(); i++) {
    //            LSSpace::Invariants.at(i).initialize();
    //        }
    //    }

    void printSpaceStatus() {
        std::cout << "in status" << std::endl;
        Gecode::SpaceStatus status = this->status();
        if (status == Gecode::SS_FAILED) {
            cout << "Status: " << this->status() << " the space is failed at root."
                    << endl;
            std::cout << "No feasible solution" << std::endl;
            //            exit(1);
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

    //    void startTimer(Gecode::Search::Options so) {
    //        //        Gecode::Search::TimeStop* ts = new Gecode::Search::TimeStop(opt.time());
    //        Gecode::Search::TimeStop* ts = new Gecode::Search::TimeStop(1);
    //        so.stop = ts;
    //        Gecode::Search::Statistics stat;
    //        //
    //        // Let's start the Timer
    //        Gecode::Support::Timer t;
    //        t.start();
    //        
    //        //        delete ts;
    //    }

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

