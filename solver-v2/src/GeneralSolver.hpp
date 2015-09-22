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
#include <functional>
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
        //        int variableID;
        //        int invariantID;
        //        std::cout << "for variable" << std::endl;
        //        int numberOfIntegerVariables = 0;
        //        invariantID = model->getInvariants().size();
        std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(variables, coefficients);
        std::shared_ptr<Linear> LinearConstraint = std::make_shared<Linear>(sumInvariant, ub, relation);

        //        bool haveInt = false;
        int numberOfIntegerVariables = 0;
        for (IntegerVariable* iv : variables) {

            //        for (IntegerVariable* iv : variables) {
            //        for (unsigned i = 0; i < coefficients.size(); i++) {
            //            if (!haveInt && iv->isIntegerVariable()) {
            //            haveInt = true;
            //                numberOfIntegerVariables++;
            //                if (priority != OBJ && relation == EQ) {
            //                    iv->usedIn++;
            //                }
            //            }
            //            variableID = iv->getID();
            iv->addToUpdate(sumInvariant);
            if (priority != 0) {
                iv->addToUsedInConstraints(LinearConstraint);
            }
            if (iv->isInteger) {
                numberOfIntegerVariables++;
            }
        }
        if (priority == 0) {
            std::cout << "objective has " << numberOfIntegerVariables << " integer variables " << std::endl;
        }
        LinearConstraint->setNumberOfIntegerVariables(numberOfIntegerVariables);
        //        if(numberOfIntegerVariables == 1 && relation == LQ  && ub  ==0){
        //            
        //            std::cout << "Can be made oneway " << std::endl;
        //        }

        //        if(numberOfIntegerVariables > 1){
        //            std::cout << numberOfIntegerVariables << std::endl;
        //        }

        //        if(haveInt){
        //            std::cout <<  relation << std::endl;
        //        }
        //        if (numberOfIntegerVariables == 0) {
        model->getOrgInvariants().push_back(sumInvariant);
        //        std::cout << sumInvariant->VariablePointers << std::endl;
        //        std::cout << "first invariant size " << st->getInvariants()->at(0)->VariablePointers->size() << std::endl;
        //        sleep(1);
        //        std::cout << "Invariant size " << st->getInvariants()->size() << std::endl;
        //        std::cout << "invariant just added size " << st->getInvariants()->at(st->getInvariants()->size()-1)->VariablePointers->size() << " vs " << coefficients->size() << std::endl;
        // add to obj fnc


        if (priority == OBJ) {
            if (model->getOrgConstraints().size() == 0) {
                constraintContainer prioVector = std::make_shared<std::vector < constraint >> ();
                model->getOrgConstraints().push_back(prioVector);
            }
            int ConstraintID = model->getOrgConstraintsWithPriority(0)->size();
            sumInvariant->setUsedByObjective(ConstraintID);

            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            // Kig på ovenstående
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




            //            std::shared_ptr<pair<int, int>> p = std::make_shared<std::pair<int, int>>(0, ConstraintID);
            //            model->getConstraintsWithIntegerVariables().push_back(p);


            model->getOrgObjectives()->push_back(LinearConstraint);
        } else {
            //            GS->linear(coefficients, &variables, relation, ub);
            //            std::cout << "Skal Gecode have constraints? " << std::endl;
            GS->linear(coefficients, variables, relation, ub);
            //                        GS->print(std::cout );
            //                        sleep(1);
            //             What should be given to LSSpace
            if (model->getOrgConstraints().size() <= priority) {
                for (unsigned i = model->getOrgConstraints().size(); i <= priority; i++) {
                    std::shared_ptr<std::vector<std::shared_ptr < Constraint>>>prioVector = std::make_shared<std::vector<std::shared_ptr < Constraint >>> ();
                    model->getOrgConstraints().push_back(prioVector);
                }
            }
            //            std::cout << "så hvad nu? " << std::endl;
            constraintContainer prio = model->getOrgConstraintsWithPriority(priority);
            //            std::cout << "prio? " << std::endl;
            //            std::cout << prio->size() << std::endl;
            sumInvariant->setUsedByConstraint(prio->size(), priority);
            // Should be constraint* instead of numbers. 



            //            std::shared_ptr<pair<int, int>> p = std::make_shared<std::pair<int, int>>(priority, prio->size());
            //            model->getConstraintsWithIntegerVariables().push_back(p);

            //            std::cout << "set used by? " << std::endl;
            prio->push_back(LinearConstraint);
            //            std::cout <<  "tjaa det var så ikke linear" << std::endl;

        }
        //        } else if (numberOfIntegerVariables > 1) {
        ////            std::cout << "You can handle the truth!" << std::endl;
        ////            std::cout << numberOfIntegerVariables << std::endl;
        //        } else {
        //            //            std::cout << "Just a single Integer Variable" << std::endl;
        //        }

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

    }
    ///Create a single variable with given lower and upper bound

    void createIntVar(int lb, int ub) {
        if (ub == 1 && lb == 0) {
            model->addBinaryVariable(lb, ub);
        } else {
            model->addIntegerVariable(lb, ub);
        }
        GS->createGecodeVariable(lb, ub);
        //        std::cout <<  "integer var pointer " << newVar << std::endl;
        //        std::cout << lb << " " << ub << std::endl;

    }

    std::vector<IntegerVariable*>& getAllVariables() {
        return model->getAllVariables();
    }


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
        GS->createArray();
        //        int counter = 0;
        //        for (int id : model->getIntegerVariableID()) {
        //            IntegerVariable iv = *model->getAllVariables().at(id);
        //            if (iv.usedIn >= 1) {
        //                counter++;
        //                std::cout << iv.usedIn << std::endl;
        //            }
        //        }
        //        std::cout << "number of difficult vars " << counter << std::endl;
        //        GeneralSolver* s = GecodeSearch(so);
        //        GS->print(std::cout);
        //        fixVariables();



        //        GS->branch(true);
        if (GS->initialize(TimeForGecode, true)) {

        } else {
            std::cout << "Gecode did not find a solution within limits given (nodes,fail,time). Model will be relaxed according to priorities given to constraints. " << std::endl;
            int timesRelaxed = 0;
            bool solutionFound = false;
            while (!solutionFound && timesRelaxed != 5) {
                relax(timesRelaxed);
                timesRelaxed++;
                //                GS->branch(false);
                solutionFound = GS->initialize(TimeForGecode, false);
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

        for (IntegerVariable* iv : model->getAllVariables()) {
            int lb = iv->getLowerBound();
            int ub = iv->getUpperBound();


            GS->createGecodeVariable(lb, ub);
        }
        GS->createArray();
        simpleRelax(timesRelaxed);
        //        IntVars = relaxed->IntVars;
        // Brug operator = til at lave det gamle space til det nye? Ellers skal det hele løses med det nye og gamle pointer skal opdateres. 
        // Hvordan fungere det så med at slette? 


    }

    void simpleRelax(int timesRelaxed) {
        std::cout << "SimpleRelax" << std::endl;
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

        /// Sort constraints a variable is part of in decreasing order according to domain
//        for (IntegerVariable* vars : model->getAllVariables()) {
//            std::vector<constraint> constraints = vars->usedInConstraints();
//            std::sort(constraints.begin(), constraints.end(), Constraint::SortGreater());
//        }
        //        std::cout << "Make alg to handle integer variables " << std::endl;
        //        std::cout << "opret Constraints, Invariants. Måske erstat pair med vector?" << std::endl;
        //        model->getConstraintsWithIntegerVariables();
        //        ConstraintSorter sorter();
        std::list<IntegerVariable*> queue = model->getIntegerVariables();
        //        for (int i =queue.size()-1; i >= 0; i--){
        // Try to make Integer variables oneway
        unsigned numberOfIntegerVariables = queue.size();
        std::cout << "#####################################################################################################" << std::endl;
        std::cout << queue.size() << std::endl;
        unsigned layer = 1;
        while (queue.size() != 0 && layer <= numberOfIntegerVariables) { // kinda high limit but could in theory reach it.
            std::cout << "layer " << layer << " queue size " << queue.size() << std::endl;
            for (auto it = queue.begin(); it != queue.end(); ++it) {
                IntegerVariable* iv = *it;
                VariableInConstraints constraints = iv->usedInConstraints();
                for (constraint cons : constraints) {
                    //                iv->usedInConstraints()
                    if (canBeMadeOneway(iv, cons, layer)) {
                        queue.erase(it);
                        it--;
                        //                        model->getIntegerVariables().erase(it);
                        //                    madeOneway = true;
                        break;
                    }
                }
            }
            layer++;
        }

        if (queue.size() != 0) {
            std::cout << "Cannot make all integer variables oneway" << std::endl;
            exit(1);
        }
        std::cout << "All Integer variables can be made oneway" << std::endl;
        unsigned highestLayer;
        // Create the new model that is used in Local search
        for (unsigned i = 0; i < model->getOrgConstraints().size(); i++) {
            for (constraint cons : *model->getOrgConstraintsWithPriority(i)) {
                layer = 0; // if invariant got integer variable then layer will be at least one higher than the invariant corresponding to that int var.
                // oneway constraints are implicit
                if (!cons->isOneway()) {
                    // If there are integer variables then the invariant must be updated if no integer variables the constraint is hasn't changed 
                    if (cons->getNumberOfIntegerVariables() > 0) {
                        std::vector<IntegerVariable*> variables;
                        InvariantContainer invars;
                        invariant invar = cons->getInvariant();
                        for (IntegerVariable* iv : invar->getVariables()) {
                            if (!iv->isInteger) {
                                variables.push_back(iv);
                            } else {

                                invars.push_back(iv->oneway);
                                if (layer < iv->oneway->layer) {
                                    layer = iv->oneway->layer;
                                }
                            }
                        }
                        layer++;
                        std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(variables, invar->coefficients);
                        sumInvariant->layer = layer;
                        sumInvariant->invariants = invars;
                        model->getInvariants().push_back(sumInvariant);
                        std::shared_ptr<Linear> LinearConstraint = std::make_shared<Linear>(sumInvariant, cons->getArgument(1), cons->getArgument(0));
                        model->getConstraintsWithPriority(i)->push_back(LinearConstraint);

                        // Constraint does not have integer variables
                    } else {
                        if (model->getConstraints().size() <= i) {
                            constraintContainer tmp = std::make_shared<std::vector < constraint >> ();
                            model->getConstraints().push_back(tmp);
                        }
                        model->getConstraintsWithPriority(i)->push_back(cons);
                        model->getInvariants().push_back(cons->getInvariant());

                    }
                    if (highestLayer < layer) {
                        highestLayer = layer;
                    }

                } else {
                    model->getConstraintsWithPriority(i)->push_back(cons);
                    model->getInvariants().push_back(cons->getInvariant());
                }

            }
        }
        model->numberOfLayers = highestLayer; // could there be an invariant with a layer one higher? 
        std::cout << "highest layer" << std::endl;

        constraint obj = model->getOrgConstraintsWithPriority(0)->at(0);
        std::cout << obj->getInvariant()->VariablePointers.size() << std::endl;
        //        std::cout << "number of integer variable that could not be defined by oneway " << counter << std::endl;
        std::cout << "Handle the objetive func" << std::endl;
        //        std::vector<IntegerVariable*> mask = model->getMask();
        //        mask.resize(model->getNonFixedBinaryVariables().size());

        //        for (unsigned i = 0; i < model->getNonFixedBinaryVariables().size(); i++) {
        //            mask.at(i) = i;
        //        }
        //        shuffleMask();
        std::vector<IntegerVariable*> mask = model->getNonFixedBinaryVariables();
        std::random_shuffle(mask.begin(), mask.end());
        std::cout << "sorted" << std::endl;
        st->initializeInvariants();
        std::cout << "invars" << std::endl;

        st->initializeConstraints();
        std::cout << "const" << std::endl;

        st->initializeObjective();
        std::cout << "objective" << std::endl;
        model->initialValue = st->getSolutionValue();

    }


    // Not taking into account if the coefficient in objective function is negative

    bool canBeMadeOneway(IntegerVariable* iv, constraint cons, unsigned layer) {
        if (cons->isOneway()) {
            return false;
        }
        int coeff = cons->getInvariant()->coefficients.at(iv->getID());

        //        std::cout << "Make oneway. constraint type " << cons->getArgument(0) << " number of intvars " << cons->getNumberOfIntegerVariables() << std::endl;
        if (cons->getNumberOfIntegerVariables() > 1) {
            unsigned highestLayer = 0;
            unsigned notDefined = 0;
            for (IntegerVariable* iv : cons->getInvariant()->getVariables()) {
                if (iv->isIntegerVariable()) {
                    if (!iv->isDefined) {
                        notDefined++;
                    } else {
                        if (iv->oneway->layer > highestLayer) {
                            highestLayer = iv->oneway->layer;
                        }
                    }
                }
            }

            if (notDefined > 1 || layer == highestLayer) {
                return false;
            }
        }
        if (cons->getArgument(0) == EQ || coeff < 0) {
            makeOneway(iv, cons, coeff, layer);
            return true;
        }
        return false;
    }

    void makeOneway(IntegerVariable* iv, constraint cons, int coeff, unsigned layer) {
        auto oldVars = cons->getInvariant()->getVariables();
        std::vector<IntegerVariable*> variables;
        InvariantContainer invariants;
        if (cons->getNumberOfIntegerVariables() == 1) {
            for (IntegerVariable* oldiv : oldVars) {
                if (iv != oldiv) {
                    variables.push_back(oldiv);
                }
            }
        } else {
            for (IntegerVariable* oldiv : oldVars) {
                if (oldiv->isInteger) {
                    if (oldiv != iv) {
                        assert(oldiv->isDefined);
                        invariants.push_back(oldiv->oneway);
                    }
                } else {
                    variables.push_back(oldiv);
                }
            }
        }
        auto coefficients = cons->getInvariant()->getCoefficients();
        coefficients.erase(iv->getID());
        if (coeff != -1) {
            for (auto it = coefficients.begin(); it != coefficients.end(); ++it) {
                it->second = it->second / (-coeff);
            }
        }

        std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(variables, coefficients);
        //Need to update the updateVector, removing old invariant and adding the new one. 
        // Horrorible inefficient could save it in a map that would increase the construction time but reduce the update time
        for (IntegerVariable* iv : variables) {
            for (invariant inv : iv->getUpdateVector()) {
                if (inv == cons->getInvariant()) {
                    inv = sumInvariant;
                    break;
                }

            }
            iv->getUpdateVector().push_back(sumInvariant);
        }

        sumInvariant->layer = layer;
        sumInvariant->CurrentValue = -cons->getArgument(1);
        sumInvariant->variableID = iv->getID();
        for (invariant invar : invariants) {
            sumInvariant->invariants.push_back(invar);
            invar->getUpdateVector().push_back(sumInvariant);
        }
        iv->setDefinedBy(sumInvariant, cons);
        cons->isOneway(true);
        model->getInvariants().push_back(sumInvariant);

    }

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
        if (model->getNonFixedBinaryVariables().size() > 0) {
            std::cout << "Integer Variables:" << std::endl;
        }

        for (unsigned i = 0; i < model->getNonFixedBinaryVariables().size(); i++) {
            std::cout << model->getNonFixedBinaryVariables().at(i)->getCurrentValue() << " ";
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

