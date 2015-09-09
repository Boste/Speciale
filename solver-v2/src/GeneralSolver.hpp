#ifndef GENERALSOLVER_HPP
#define	GENERALSOLVER_HPP
#include <cmath>
#include <algorithm>
#include "LSSpace.hpp"
#include <assert.h>
#include "Model.hpp"
#include <gecode/driver.hh>
#include <gecode/int.hh>
#include "Constants.hpp"
#include "GecodeSolver.hpp"
//#include <gecode/minimodel.hh>
#include <limits>
#include "IntegerVariable.hpp"
#include "Multistop.hpp"
#include "Sum.hpp"
#include <memory>
#include "State.hpp"
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

    // Should be unique
    //    std::unique_ptr<LSSpace> LS = std::make_shared<LSSpace>();

    std::shared_ptr<Model> model = std::make_shared<Model> ();
    std::shared_ptr<State> st = std::make_shared<State>(model);
    std::unique_ptr<LSSpace> LS = std::unique_ptr<LSSpace> (new LSSpace(model));

    // Should be unique
    //    std::shared_ptr<GecodeSolver> GS = std::make_shared<GecodeSolver>(st);
    std::unique_ptr<GecodeSolver> GS = std::unique_ptr<GecodeSolver> (new GecodeSolver(model));
    //    GecodeSolver* GS = new GecodeSolver(st);


    //    std::vector<Gecode::IntVarArgs*> test;
public:

    GeneralSolver() {
        //        Random::Seed(SEED);
        //        LS = std::make_shared<LSSpace>(new LSSpace());
        //        GS = std::make_shared<GecodeSolver>(new GecodeSolver());
        //        st = std::make_shared<Model> (new Model());


    }

    ~GeneralSolver() {
        //        std::cout << "Destructing GS" << std::endl;
        //        delete GS;
        //        delete st;
    }

    GeneralSolver& operator=(const GeneralSolver &a) {
        this->model = a.model;
        return *this;

    }


    // Skal Gecode::IntConLevel icl være et argument?
    // ingen shared i det her kald

    void linear(std::vector<int>& coefficients, std::vector<IntegerVariable*>& variables, int relation, int ub, unsigned priority) {
//    void linear(std::vector<int>& coefficients, std::vector<IntegerVariable*>& variables, int relation, int ub, unsigned priority) {
//    void linear(std::vector<int>& coefficients, std::array<IntegerVariable*>& variables, int relation, int ub, unsigned priority) {
        //        std::cout << "make linear" << std::endl;
        //        std::vector<IntegerVariable*>* vars = variables;
        //        std::vector<int> c = coefficients;
//        std::cout << variables.size() <<  " " << coefficients.size() << std::endl;
        
        if(variables.size()==0){
            std::cout << "Constraint nr " << model->getInvariants().size() << std::endl;
        }
//        std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(variables, coefficients);
        std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(variables, coefficients);
        //        Sum* sumInvariant = new Sum(vars,coefficients);


        //        std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(variables, coefficients);
        //        std::shared_ptr<Sum> sumInvariant = std::shared_ptr<Sum> (new Sum(variables, coefficients));
        //        Sum* sumInvariant = new Sum(variables,coefficients);
        //        std::unique_ptr<Sum> sumInvariant = std::unique_ptr<Sum> (new Sum (variables,coefficients));
        //        std::cout << sumInvariant << std::endl;
        int variableNumber;
        int invariantNumber;
//        std::cout << "for variable" << std::endl;
        for (IntegerVariable* iv : variables) {
//        for (IntegerVariable* iv : variables) {
            //        for (unsigned i = 0; i < coefficients.size(); i++) {

            variableNumber = iv->getID();
            invariantNumber = model->getInvariants().size();


            model->getAllIntegerVariables().at(variableNumber)->addToUpdate(invariantNumber);
        }
        model->getInvariants().push_back(sumInvariant);
        //        std::cout << sumInvariant->VariablePointers << std::endl;
        //        std::cout << "first invariant size " << st->getInvariants()->at(0)->VariablePointers->size() << std::endl;
        //        sleep(1);
        //        std::cout << "Invariant size " << st->getInvariants()->size() << std::endl;
        //        std::cout << "invariant just added size " << st->getInvariants()->at(st->getInvariants()->size()-1)->VariablePointers->size() << " vs " << coefficients->size() << std::endl;
        std::shared_ptr<Linear> LinearConstraint = std::make_shared<Linear>(sumInvariant, ub, relation);
        // add to obj fnc
        
        if (priority == OBJ) {
            if (model->getConstraints().size() == 0) {
                std::shared_ptr<std::vector<std::shared_ptr < Constraint>>> prioVector = std::make_shared<std::vector<std::shared_ptr < Constraint >>>();
                model->getConstraints().push_back(prioVector);
            }
            sumInvariant->setUsedByObjective(model->getConstraintsWithPriority(0)->size());
            model->getObjectives()->push_back(LinearConstraint);
        } else {
//            GS->linear(coefficients, &variables, relation, ub);
//            std::cout << "Skal Gecode have constraints? " << std::endl;
            GS->linear(coefficients, variables, relation, ub);
            //                        GS->print(std::cout );
            //                        sleep(1);
            //             What should be given to LSSpace
            if (model->getConstraints().size() <= priority) {
                for (unsigned i = model->getConstraints().size(); i <= priority; i++) {
                    std::shared_ptr<std::vector<std::shared_ptr < Constraint>>>prioVector = std::make_shared<std::vector<std::shared_ptr < Constraint >>> ();
                    model->getConstraints().push_back(prioVector);
                }
            }
//            std::cout << "så hvad nu? " << std::endl;
            std::shared_ptr<std::vector<std::shared_ptr < Constraint>>> prio = model->getConstraintsWithPriority(priority);
//            std::cout << "prio? " << std::endl;
//            std::cout << prio->size() << std::endl;
            sumInvariant->setUsedByConstraint(prio->size(), priority);
//            std::cout << "set used by? " << std::endl;
            prio->push_back(LinearConstraint);
//            std::cout <<  "tjaa det var så ikke linear" << std::endl;

        }
    }

    //    void SetValues(Gecode::IntVarArray vars) {
    //        for (int i = 0; i < vars.size(); i++) {
    //            assert(vars[i].assigned());
    //            this->IntVarVector.at(i)->setCurrentValue(vars[i].val());
    //        }
    //    }

    //    std::vector<IntegerVariable*>* createIntVars(unsigned numberOfVariables, int lb, int ub) {

    void createIntVars(unsigned numberOfVariables, int lb, int ub) {
        // Given to gecode space
        //        Gecode::IntVarArray vars(*this, numberOfVariables, lb, ub);


        for (unsigned i = 0; i < numberOfVariables; i++) {
            createIntVar(lb, ub);

        }

        // Looks bad but does not copy the variables. Points to two different places in memory but the variables in the two arrays have the same memory address. 
        //        IntVars = vars;
        // Given to LS space
        //        return st->addIntegerVariable(&vars);
        //        return model->getAllIntegerVariables();
        //        return LSSpace::addIntVariablesToLS(IntVars);
        //        return vars;
    }
    ///Create a single variable with given lower and upper bound

    void createIntVar(int lb, int ub) {
        model->addIntegerVariable(lb, ub);
        GS->createGecodeVariable(lb, ub);
        //        std::cout <<  "integer var pointer " << newVar << std::endl;
        //        std::cout << lb << " " << ub << std::endl;

    }

    std::vector<IntegerVariable*>& getBinaryVariables() {
        return model->getAllIntegerVariables();
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


    /// Only for testing, should be removed 

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


        //        GeneralSolver* s = GecodeSearch(so);
        //        GS->print(std::cout);
        //        fixVariables();
        GS->createArray();
//        GS->branch(true);
        if (GS->initialize(TimeForGecode,true)) {

        } else {
            std::cout << "Gecode did not find a solution within limits given (nodes,fail,time). Model will be relaxed according to priorities given to constraints. " << std::endl;
            int timesRelaxed = 0;
            bool solutionFound = false;
            while (!solutionFound && timesRelaxed != 5) {
                relax(timesRelaxed);
                timesRelaxed++;
//                GS->branch(false);
                solutionFound = GS->initialize(TimeForGecode,false);
            }
            if (!solutionFound) {
                std::cout << "Relaxation failed" << std::endl;
                exit(1);
            }
            //        assert(s != NULL);
            //        assert(!s->failed());
            //        return s;
            //
            //                this->print(cout);
        }
        //        fixVariables();
        initializeLS();

        //        delete ms;
        //        delete so;
    }

    /// relaxes the space (reduce the number of constraints). Used when Gecode cant find a solution in time.
    /// Only works for binary


    /// Different relaxation can be chosen (not atm) needs to create a new GecodeSolver (Space) and recreate some 
    /// of the calls the user made (those that should not be relaxed).

    void relax(int timesRelaxed) {
//        std::cout << "relax" << std::endl;

        GS = std::unique_ptr<GecodeSolver>(new GecodeSolver(model));

        for (IntegerVariable* iv : model->getAllIntegerVariables()) {
            int lb = iv->getLowerBound();
            int ub = iv->getUpperBound();


            GS->createGecodeVariable(lb, ub);
        }
        GS->createArray();
        simpleRelax(timesRelaxed);
        //        IntVars = relaxed->IntVars;

        // Remake hard constraints with new gecode variables. 
        //        for (Constraint* cons : st->getHardConstraints()) {
        //        for (unsigned i = 0; i < st->getHardConstraints()->size(); i++) {
        //            Constraint* cons = st->getHardConstraints()->at(i);
        //            std::vector<IntegerVariable*>* variables = new std::vector<IntegerVariable*>();
        //            //            std::vector<int>* coefficients = new std::vector<int>();
        //            if (cons->getType() == LINEAR) {
        //                vector<IntegerVariable*>* oldVars = cons->getInvariant()->VariablePointers;
        //                //                std::cout <<  oldVars->size() << std::endl;
        //                std::unordered_map<int, int> coefficients = cons->getInvariant()->coefficients;
        //                for (unsigned j = 0; j < oldVars->size(); j++) {
        //                    IntegerVariable* var = oldVars->at(j);
        //                    //                for (IntegerVariable* var : oldVars) {
        //                    variables->push_back(relaxed->st->getIntegerVariables()->at(var->getID()));
        //                    //                    coefficients->push_back()
        //                }
        //                //                std::cout << "Sum1" << std::endl;
        //                Sum* sumInvariant = new Sum(variables, coefficients);
        //                int variableNumber;
        //                int invariantNumber;
        //
        //                for (unsigned j = 0; j < oldVars->size(); j++) {
        //                    variableNumber = variables->at(j)->getID();
        //                    invariantNumber = relaxed->st->getInvariants()->size();
        //                    relaxed->st->getIntegerVariables()->at(variableNumber)->addToUpdate(invariantNumber);
        //                }
        //                relaxed->st->getInvariants()->push_back(sumInvariant);
        //                int relation = cons->getArgument(0);
        //                int ub = cons->getArgument(1);
        //                Linear* LinearConstraint = new Linear(sumInvariant, ub, relation);
        //                //                Gecode::IntArgs c(coefficients->size());
        //                //                Gecode::IntVarArgs x(coefficients->size());
        //                //                for (unsigned j = 0; j < coefficients->size(); j++) {
        //                //                    c[j] = coefficients->at(j);
        //                //                    x[j] = *(variables->at(j)->getVariablePointer());
        //                //                }
        //                //                switch (relation) {
        //                //                    case EQ:
        //                //                        Gecode::linear(relaxed, c, x, Gecode::IRT_EQ, ub, Gecode::ICL_VAL);
        //                //                        break;
        //                //                    case LQ:
        //                //                        Gecode::linear(relaxed, c, x, Gecode::IRT_LQ, ub, Gecode::ICL_VAL);
        //                //                        break;
        //                //                    case LE:
        //                //                        Gecode::linear(relaxed, c, x, Gecode::IRT_LE, ub, Gecode::ICL_VAL);
        //                //                        break;
        //                //                    case GQ:
        //                //                        Gecode::linear(relaxed, c, x, Gecode::IRT_GQ, ub, Gecode::ICL_VAL);
        //                //                        break;
        //                //                    case GR:
        //                //                        Gecode::linear(relaxed, c, x, Gecode::IRT_GR, ub, Gecode::ICL_VAL);
        //                //                        break;
        //                //                }
        //
        //                // What should be given to LSSpace
        //                sumInvariant->usedByConstraint(relaxed->st->getHardConstraints()->size());
        //                relaxed->st->getHardConstraints()->push_back(LinearConstraint);
        //            } else {
        //                std::cout << "It is not linear :S" << std::endl;
        //            }
        //        }
        //        // Remake objective func with new gecode variables. 
        //        //        for (Constraint* cons : st->getObjectives()) {
        //        for (int i = 0; i < st->getObjectives()->size(); i++) {
        //            Constraint* cons = st->getObjectives()->at(i);
        //            std::vector<IntegerVariable*>* variables = new std::vector<IntegerVariable*>();
        //            //            std::vector<int>* coefficients = new std::vector<int>();
        //            if (cons->getType() == LINEAR) {
        //                vector<IntegerVariable*>* oldVars = cons->getInvariant()->VariablePointers;
        //                std::unordered_map<int, int> coefficients = cons->getInvariant()->coefficients;
        //                //                for (IntegerVariable* var : oldVars) {
        //                for (unsigned j = 0; j < oldVars->size(); j++) {
        //                    IntegerVariable* var = oldVars->at(j);
        //                    variables->push_back(relaxed->st->getIntegerVariables()->at(var->getID()));
        //                    //                    coefficients->push_back()
        //                }
        //                //                std::cout << "sum2" << std::endl;
        //                Sum* sumInvariant = new Sum(variables, coefficients);
        //                int variableNumber;
        //                int invariantNumber;
        //
        //                for (unsigned j = 0; j < oldVars->size(); j++) {
        //                    variableNumber = variables->at(j)->getID();
        //                    invariantNumber = relaxed->st->getInvariants()->size();
        //                    relaxed->st->getIntegerVariables()->at(variableNumber)->addToUpdate(invariantNumber);
        //                }
        //                relaxed->st->getInvariants()->push_back(sumInvariant);
        //                int relation = cons->getArgument(0);
        //                int ub = cons->getArgument(1);
        //                Linear* LinearConstraint = new Linear(sumInvariant, ub, relation);
        //
        //                // What should be given to LSSpace
        //                sumInvariant->usedByObjective(relaxed->st->getObjectives()->size());
        //                relaxed->st->getObjectives()->push_back(LinearConstraint);
        //            } else {
        //                std::cout << "it is not linear :S" << std::endl;
        //            }
        //        }


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
        std::cout << "SimpleRelasadasdasdx" << std::endl;
        //        std::cout << "do nothing" << std::endl;
//        std::cout << model->getObjectives()->at(0)->getInvariant()->VariablePointers->size() << std::endl;
        for (unsigned i = 1; i < model->getConstraints().size()/*-((timesRelaxed+1)*100)*/; i++) {
            //        for ( std::vector<std::shared_ptr < Constraint>>* prio : *st->getConstraints()) {
            std::shared_ptr<std::vector<std::shared_ptr < Constraint>>> prio = model->getConstraintsWithPriority(i);
            //            for (unsigned j = 0; j < prio->size(); j++) {
            for (std::shared_ptr<Constraint> cons : *prio) {
//                std::cout << __LINE__ << std::endl;
                std::shared_ptr<Invariant> invar = cons->getInvariant();
                //                Invariant* invar = cons->getInvariant();
                std::vector<IntegerVariable*>& integerVariables = invar->VariablePointers;
                if (invar->getType() == SUM) { // otherwise it is in objective function atm.
                    //                    Gecode::IntArgs c(integerVariables->size());
//                    std::cout << __LINE__ << std::endl;
//                    std::cout << integerVariables->size() << std::endl;
                    std::vector<int> c(integerVariables.size());
//                    std::cout << __LINE__ << std::endl;

                    //                    Gecode::IntVarArgs x(integerVariables->size());
                    for (unsigned j = 0; j < integerVariables.size(); j++) {
//                        std::cout << __LINE__ << std::endl;

                        int variableID = integerVariables.at(j)->getID();
//                        std::cout << __LINE__ << std::endl;

                        //                        for(IntegerVariable* iv : *integerVariables){

                        //                            int variableID = iv->getID();
                        c[j] = invar->coefficients.at(variableID); //                    std::cout << __LINE__ << std::endl;

                    }

                    int relation = cons->getArgument(0);
                    int ub = cons->getArgument(1);
//                    std::cout << "posting linear" << std::endl;
                    GS->linear(c, integerVariables, relation, ub);
                } else {
                    std::cout << "type should be SUM and assert should prevent this. Then type is set to " << invar->getType() << std::endl;
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
        model->initialValue = st->getSolutionValue();
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
        return st->getObjectiveValue();
    }

    void optimizeSolution(int time) {
        LS->optimizeSolution(time, st);


    }
    // Only for testing

    //    Gecode::IntVarArray * getIntVars() {
    //        return &IntVars;
    //    }

    void printCurrent() {
        if (model->getIntegerVariables().size() > 0) {
            std::cout << "Integer Variables:" << std::endl;
        }

        for (unsigned i = 0; i < model->getIntegerVariables().size(); i++) {
            std::cout << model->getIntegerVariables().at(i)->getCurrentValue() << " ";
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
        std::cout << "\tfail: " << stat.fail << std::endl;
        std::cout << "\tnodes: " << stat.node << std::endl;
        std::cout << "\tpropagators: " << stat.propagate << std::endl; // see page 145 MPG
        std::cout << "\tdepth: " << stat.depth << std::endl;
        std::cout << "\trestarts: " << stat.restart << std::endl;
        std::cout << "\tnogoods: " << stat.nogood << std::endl;
    }



};

#endif	/* GENERALSOLVER_HPP */

