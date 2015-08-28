#ifndef GENERALSOLVER_HPP
#define	GENERALSOLVER_HPP
#include <cmath>
#include <algorithm>
#include "LSSpace.hpp"
#include <assert.h>
#include "State.hpp"
#include <gecode/driver.hh>
#include <gecode/int.hh>
#include "Constants.hpp"
#include "GecodeSolver.hpp"
//#include <gecode/minimodel.hh>
#include <limits>
#include "IntegerVariable.hpp"
#include "Multistop.hpp"
#include "Sum.hpp"

//#include "Invariant.hpp"
//#include "Constraint.hpp"
#include "Clock.hpp"
//#include "getRSS.hpp"
//#include "Random.hpp"
//using namespace Gecode;

//class GeneralSolver : public Gecode::Space, private LSSpace {

class GeneralSolver {
    friend class Test;
private:
    //    Gecode::IntVarArray* solutions;
    //    std::vector<IntegerVariable*> IntVars;

    //    shared_ptr<Gecode::Space> gecode(new Gecode::Space());
    //    LSSpace* LS = new LSSpace();
    //    std::shared_ptr<LSSpace> LS;
    //    shared_ptr<LSSpace> LS(new LSSpace());
    std::shared_ptr<LSSpace> LS = std::make_shared<LSSpace>();
    //      std::shared_ptr<LSSpace> LS (new LSSpace);

    shared_ptr<State> st = std::make_shared<State> ();

//    std::shared_ptr<GecodeSolver> GS = std::make_shared<GecodeSolver>(st);
    GecodeSolver* GS = new GecodeSolver(st);


    //    std::vector<Gecode::IntVarArgs*> test;
public:

    GeneralSolver() {
        //        Random::Seed(SEED);
        //        LS = std::make_shared<LSSpace>(new LSSpace());
        //        GS = std::make_shared<GecodeSolver>(new GecodeSolver());
        //        st = std::make_shared<State> (new State());


    }

    ~GeneralSolver() {
        //        std::cout << "Destructing GS" << std::endl;
        delete GS;
        //        delete st;
    }

    GeneralSolver& operator=(const GeneralSolver &a) {
        this->st = a.st;
        //        std::cout << "hest " << std::endl;
        return *this;

    }


    // Skal Gecode::IntConLevel icl være et argument?
    // ingen shared i det her kald

    void linear(std::vector<int>& coefficients, std::vector<IntegerVariable*>* variables, int relation, int ub, unsigned priority) { // 0 = obj, 1 = soft, 2 = hard 
        Sum* sumInvariant = new Sum(variables, coefficients);

        //        std::cout << sumInvariant << std::endl;
        int variableNumber;
        int invariantNumber;
        for (unsigned i = 0; i < coefficients.size(); i++) {

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
        if (priority == OBJ) {
            if (st->getConstraints()->size() == 0) {
                std::vector<Constraint*>* prioVector = new std::vector<Constraint*>();
                st->getConstraints()->push_back(prioVector);
            }
            sumInvariant->setUsedByObjective(st->getConstraintsWithPriority(0)->size());
            st->getObjectives()->push_back(LinearConstraint);
        } else {

            GS->linear(coefficients, variables, relation, ub);
            // What should be given to LSSpace
            if (st->getConstraints()->size() <= priority) {
                for (unsigned i = st->getConstraints()->size(); i <= priority; i++) {
                    std::vector<Constraint*>*prioVector = new std::vector<Constraint*>();
                    st->getConstraints()->push_back(prioVector);
                }
            }
            std::vector<Constraint*>* prio = st->getConstraintsWithPriority(priority);
            sumInvariant->setUsedByConstraint(st->getConstraintsWithPriority(priority)->size(), priority);
            st->getConstraintsWithPriority(priority)->push_back(LinearConstraint);

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
        //        Gecode::IntVarArray vars(*this, numberOfVariables, lb, ub);
        st->addIntegerVariable(numberOfVariables, lb, ub);

        GS->createGecodeVariables(st);

        // Looks bad but does not copy the variables. Points to two different places in memory but the variables in the two arrays have the same memory address. 
        //        IntVars = vars;
        // Given to LS space
        //        return st->addIntegerVariable(&vars);
        return st->getIntegerVariables();


        //        return LSSpace::addIntVariablesToLS(IntVars);
        //        return vars;
    }

    //    Gecode::BoolVarArray createBoolVars(int amount, int lb, int ub) {
    //        // Given to gecode space
    //        Gecode::BoolVarArray vars(*this, amount, lb, ub);
    //
    //        // Given to LS space
    //        return vars;
    //    }

    /// Constructor for cloning s

    //    GeneralSolver(bool share, GeneralSolver & s) :
    //    Gecode::Space(share, s) {
    //        //        std::cout << "this is not a line " << IntVars.size() << std::endl;
    //        IntVars.update(*this, share, s.IntVars);
    //        // remember to update your main variables!
    //        //		model = s.model;
    //
    //    }

    // Copy during cloning

    //    virtual Space * copy(bool share) {
    //        //        std::cout << "copy" << std::endl;create
    //        return new GeneralSolver(share, *this);
    //    }

    void print(std::vector<IntegerVariable>& IntegerVariables) {
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

    void InitialSolution(int TimeForGecode) {

        Multistop* ms = new Multistop(0, 10, TimeForGecode * 1000);
        Gecode::Search::Options* so = new Gecode::Search::Options();
        so->stop = ms;
        //        GeneralSolver* s = GecodeSearch(so);
        GS->branch(true);
        if (GS->initialize()) {
            //            
        } else {
            std::cout << "Gecode did not find a solution within limits given (nodes,fail,time). Model will be relaxed according to priorities given to constraints. " << std::endl;
            int timesRelaxed = 0;
            bool failed = true;
            while (failed && timesRelaxed != 5) {
                relax(so, timesRelaxed);
                timesRelaxed++;
                GS->branch(false);
                failed = GS->initialize();
            }
            //        assert(s != NULL);
            //        assert(!s->failed());
            //        return s;
            //
            //                this->print(cout);
        }
    }

    /// relaxes the space (reduce the number of constraints). Used when Gecode cant find a solution in time.
    /// Only works for binary

    GeneralSolver* relax(Gecode::Search::Options* so, int timesRelaxed) {

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

        //        GeneralSolver* s;
        //        if (timesRelaxed > 0 || st->getSoftConstraints()->size() == 0) {
        //            // Maybe split this into more methods. eg relax half of const, relax top ten with most variables, relax in topological order. 
        //            relaxed->simpleRelax(timesRelaxed);
        //
        //            //            s = relaxed->GecodeSearch(so);
        //        } else {
        //            // Do not give soft to gecode
        //        }


        //        return s;
        // Brug operator = til at lave det gamle space til det nye? Ellers skal det hele løses med det nye og gamle pointer skal opdateres. 
        // Hvordan fungere det så med at slette? 


    }

    void simpleRelax(int timesRelaxed) {
        //        std::cout << "do nothing" << std::endl;
        for (unsigned i = 1; i < st->getConstraints()->size()/*-((timesRelaxed+1)*100)*/; i++) {
            std::vector<Constraint*>* prio = st->getConstraintsWithPriority(i);
            for (unsigned j = 0; j < prio->size(); j++) {
                Constraint* cons = prio->at(i);
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
                    //                switch (relation) {
                    //                    case EQ:
                    //                        Gecode::linear(*this, c, x, Gecode::IRT_EQ, ub, Gecode::ICL_DOM);
                    //                        break;
                    //                    case LQ:
                    //                        Gecode::linear(*this, c, x, Gecode::IRT_LQ, ub, Gecode::ICL_DOM);
                    //                        break;
                    //                    case LE:
                    //                        Gecode::linear(*this, c, x, Gecode::IRT_LE, ub, Gecode::ICL_DOM);
                    //                        break;
                    //                    case GQ:
                    //                        Gecode::linear(*this, c, x, Gecode::IRT_GQ, ub, Gecode::ICL_DOM);
                    //                        break;
                    //                    case GR:
                    //                        Gecode::linear(*this, c, x, Gecode::IRT_GR, ub, Gecode::ICL_DOM);
                    //                        break;
                    //                }
                    //            Constraint* cons = hardConstraints->at(invar->usedInObjectiveNr);
                    // irt (eq, lq), ub, og IntegerConsisdencyLevel. Hvor skal jeg få dem fra?



                    //        std::cout << "før gecode" << std::endl;

                    //            Gecode::linear(relaxed, c, x, irt, ub, icl);
                    //            st->getInvariants()->at(0).
                } else {
                    std::cout << "type should be SUM and assert should prevent this. Then type must be set to other than 1" << std::endl;
                }
            }
        }
        std::cout << "SimpleRelax made " << timesRelaxed + 1 << " times" << std::endl;
    }

    void initializeLS() {
        //        SetValues(GS->IntVars);

            st->initializeInvariants();
        

            st->initializeConstraints();
        

            st->initializeObjective();
//        LS->initializeInvariants(st);
//        std::cout << __LINE__ << std::endl;
//
//        LS->initializeConstraints(st);
//        std::cout << __LINE__ << std::endl;
//
//        LS->initializeObjective(st);
//        std::cout << __LINE__ << std::endl;

        //        delete GS;
    }

    //    void SetValues() {
    //        for (int i = 0; i < st->getNumberOfVariables(); i++) {
    //            assert(vars[i].assigned());
    //            
    //            st->getIntegerVariables()->at(i)->setCurrentValue(vars[i].val());
    //
    //        }
    //    }

    int getInitialValue() {
        return LS->initialValue;
    }

    void optimizeSolution(int time) {
        LS->optimizeSolution(time, st);


    }
    // Only for testing

    //    Gecode::IntVarArray * getIntVars() {
    //        return &IntVars;
    //    }

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

