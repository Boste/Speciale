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
#include"Max.hpp"
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

        std::shared_ptr<Linear> LinearConstraint = std::make_shared<Linear>(coefficients, variables, ub, relation, priority);

        //        bool haveInt = false;
        int numberOfIntegerVariables = 0;
        for (IntegerVariable* iv : variables) {
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

        if (priority == OBJ) {
            if (model->getConstraints().size() == 0) {
                constraintContainer prioVector = std::make_shared<std::vector < constraint >> ();
                model->getConstraints().push_back(prioVector);
            }
            //            for (IntegerVariable* iv : variables) {
            //                if (iv->isInteger && LinearConstraint->getCoefficients().at(iv->getID()) != 0) {
            //                    std::cout << iv->getID() << std::endl;
            //                }
            //            }

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
            constraintContainer prio = model->getConstraintsWithPriority(priority);

            prio->push_back(LinearConstraint);

        }
    }

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


        //¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
        // Obj value after gecode
        //      ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
        //        int initialValue = 0;
        //        constraint obj = model->getObjectives()->at(0);
        //        auto coef = obj->getCoefficients();
        //        for (IntegerVariable* iv : obj->getVariables()) {
        //            double coeff = coef.at(iv->getID());
        //            if (coeff != 0) {
        //                std::cout << "id " << iv->getID() << " value " << iv->getCurrentValue() << " coeff "<< coeff << std::endl; 
        //                initialValue += coeff * iv->getCurrentValue();
        //            }
        //        }
        //        std::cout << "This should be initial value " << initialValue << std::endl;
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
            constraintContainer prio = model->getConstraintsWithPriority(i);
            //            for (unsigned j = 0; j < prio->size(); j++) {
            for (constraint cons : *prio) {
                //                std::shared_ptr<Invariant> invar = cons->getInvariant();
                //                Invariant* invar = cons->getInvariant();
                std::vector<IntegerVariable*> variables = cons->getVariables();
                if (cons->getType() == LINEAR) { // otherwise it is in objective function atm.
                    //                    Gecode::IntArgs c(integerVariables->size());
                    std::vector<int> c(variables.size());

                    //                    Gecode::IntVarArgs x(integerVariables->size());
                    for (unsigned j = 0; j < variables.size(); j++) {

                        c[j] = cons->getCoefficients().at(j); //                    std::cout << __LINE__ << std::endl;

                    }

                    int relation = cons->getArgument(0);
                    int ub = cons->getArgument(1);
                    GS->linear(c, variables, relation, ub);
                } else {
                    std::cout << "type should be LINEAR and assert should prevent this. Then type is set to " << cons->getType() << std::endl;
                }
            }
        }
        std::cout << "SimpleRelax made " << timesRelaxed + 1 << " times" << std::endl;
    }

    void initializeLS() {

        /// Sort constraints a variable is part of in decreasing order according to domain

        /// Sort integer variables decreasing order according to number of constraints they are involved
        //        for (IntegerVariable* vars : model->getAllVariables()) {
        //            std::vector<constraint> constraints = vars->usedInConstraints();
        //            std::sort(constraints.begin(), constraints.end(), Constraint::SortGreater());
        //        }
        //        std::cout << "Make alg to handle integer variables " << std::endl;
        //        std::cout << "opret Constraints, Invariants. Måske erstat pair med vector?" << std::endl;
        //        model->getConstraintsWithIntegerVariables();
        //        ConstraintSorter sorter();





        //#####################################################################################################################################
        // Making oneway looking at each integer variable. Harder to delete the constraints that have been made one-way
        //#####################################################################################################################################        
        std::list<IntegerVariable*> queue = model->getIntegerVariables();
        //        for (int i =queue.size()-1; i >= 0; i--){
        // Try to make Integer variables oneway
        //        unsigned numberOfIntegerVariables = queue.size();
        //        std::cout << numberOfIntegerVariables << std::endl;
        //        std::cout << "#####################################################################################################" << std::endl;
        //        std::cout << queue.size() << std::endl;
        //        unsigned layer = 1;
        //        int iter = 0;
        bool change = true;
        while (queue.size() != 0 && change) {
            //            iter++;
            //            std::cout << "iterations in while " << iter << std::endl;
            //            std::cout << " queue size " << queue.size() << std::endl;
            change = false;
            for (auto it = queue.begin(); it != queue.end(); ++it) {
                IntegerVariable* iv = *it;

                VariableInConstraints constraints = iv->usedInConstraints();
                for (constraint cons : constraints) {
                    //                iv->usedInConstraints()
                    if (canBeMadeOneway(iv, cons)) {
                        queue.erase(it);
                        it--;
                        change = true;
                        //                        model->getIntegerVariables().erase(it);
                        //                    madeOneway = true;
                        break;
                    }
                }
            }
            //            layer++;
        }

        if (queue.size() != 0) {
            std::cout << "Cannot make all integer variables oneway" << std::endl;
            exit(1);
        }
        std::cout << "All Integer variables can be made oneway" << std::endl;




        //        unsigned highestLayer;
        // Create the new model that is used in Local search

        for (constraint cons : *model->getObjectives()) {
            assert(!cons->isOneway());
        }
        for (unsigned i = 0; i < model->getConstraints().size(); i++) {
            for (constraint cons : *model->getConstraintsWithPriority(i)) {
                if (!cons->isOneway()) {
                    std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(cons->getCoefficients());
                    sumInvariant->invariantID = model->getInvariants().size();
                    for (IntegerVariable* iv : cons->getVariables()) {
                        if (!iv->isDefined) {
                            sumInvariant->VariablePointers.push_back(iv);
                            iv->addToUpdate(sumInvariant.get());
//                            iv->addToUpdate(sumInvariant);
                        } else {


                            sumInvariant->invariants.push_back(iv->oneway);
                            iv->oneway->addToUpdate(sumInvariant.get());
//                            iv->oneway->addToUpdate(sumInvariant);

                        }
                    }
                    model->addInvariant(sumInvariant);
                    cons->setInvariant(sumInvariant);
                    //                    std::cout << cons->getInvariant()->VariablePointers.size() << std::endl;


                    //                        std::shared_ptr<Linear> LinearConstraint = std::make_shared<Linear>(sumInvariant, cons->getArgument(1), cons->getArgument(0));
                    //                        model->getConstraintsWithPriority(i)->push_back(LinearConstraint);

                    //                    model->getInvariants().push_back(cons->getInvariant());
                }


                //                if (i == 0) {
                //                std::cout << cons->getInvariant()->VariablePointers.size() << std::endl;
                //                }
            }
        }
        /// Create propagate queue either from invariant or variables
        for (IntegerVariable* iv : model->getNonFixedBinaryVariables()) {
            //            propagation_queue& queue = iv->getPropagationQueue();
            //            updateVector allUpdates;
            propagation_queue& allUpdates = iv->getPropagationQueue();
            addToQueue(allUpdates, iv->getUpdateVector());
            //            addToQueue(allUpdates, iv->getUpdateVector());
            //            bool first = true;
            //            invariant lastInvar;
//            for (invariant invar : allUpdates) {
//                //                if(first){
//                //                    lastInvar = invar;
//                //                    first=false;
//                //                } else {
//                //                    if(invar == lastInvar){
//                //                        std::cout << invar->getID() << " " << lastInvar->getID() << std::endl;
//                //                    }
//                //                }
//                iv->addToPropagationQueue(invar);
//                //                
//            }
//            std::cout << iv->getPropagationQueue().size() << std::endl;

        }
        unsigned last = -1;
        for (invariant invar : model->getInvariants()) {
            unsigned current = invar->invariantID;
            if (current != last + 1) {
                std::cout << "last " << last << " current " << current << std::endl;
                sleep(10);

            }
            last = current;
        }

        //
        //            InvariantContainer invars = invar->getInvariants();
        //            if (invars.size() > 0) {
        //                std::cout << invar->getInvariants().size() << std::endl;
        //                std::cout << invars.size() << std::endl;
        //                invars.pop_back();
        //                std::cout << invar->getInvariants().size() << std::endl;
        //                std::cout << invars.size() << std::endl;
        //                sleep(5);
        //
        //            }
        //
        //        }
        //        model->numberOfLayers = highestLayer; // could there be an invariant with a layer one higher? 
        //        std::cout << "highest layer" << std::endl;

        //        constraint obj = model->getConstraintsWithPriority(0)->at(0);
        //        std::cout << obj->getInvariant()->VariablePointers.size() << std::endl;
        //        std::cout << "number of integer variable that could not be defined by oneway " << counter << std::endl;
        //        std::cout << "Handle the objetive func" << std::endl;
        //        std::vector<IntegerVariable*> mask = model->getMask();
        //        mask.resize(model->getNonFixedBinaryVariables().size());

        //        for (unsigned i = 0; i < model->getNonFixedBinaryVariables().size(); i++) {
        //            mask.at(i) = i;
        //        }
        //        shuffleMask();
        //        std::cout << "fill test" << std::endl;
        //        model->fillTest(10000000);
        //        std::cout << "done" << std::endl;
        //        std::vector<int>& ref = model->getTestRef();
        //        std::cout << "got test på ref " << &ref << std::endl;
        //        std::vector<int> val = model->getTestVal();
        //        model->incTest(67234);
        //        std::cout << "ref " << ref.at(67234) << std::endl;
        //        std::cout << "val " << val.at(67234) << std::endl;
        //        std::cout << "got test by val " << &val << std::endl;


        //        ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
        //        Random shuffle  Done when creating non fixed variables
        //        ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
        //        std::vector<IntegerVariable*>& mask = model->getNonFixedBinaryVariables();
        //        std::random_shuffle(mask.begin(), mask.end());


        //        bool fejl = false;
        //        for (invariant invar : model->getInvariants()) {
        //            Invariant* pointer = invar.get();
        //            //            std::cout << "Defines " << invar->getVariableID() << std::endl;
        //            variableContainer vars = invar->getVariables();
        //            for (IntegerVariable* iv : vars) {
        //                bool isInUpdate = false;
        //                for (invariant inv : iv->getUpdateVector()) {
        //                    if (inv.get() == pointer) {
        //                        isInUpdate = true;
        //                    }
        //                }
        //                if (iv->getUpdateVector().size() == 0) {
        //                    std::cout << "not in any invariant" << std::endl;
        //                }
        //                if (!isInUpdate) {
        //                    std::cout << "one variable not in update " << std::endl;
        //                    std::cout << iv->getID() << std::endl;
        //                    std::cout << iv->isInteger << std::endl;
        //                    fejl = true;
        //                    sleep(1);
        //                }
        //            }
        //        }
        //        if (fejl) {
        //            std::cout << "Done" << std::endl;
        //            sleep(5);
        //        }





        //        std::cout << "sorted" << std::endl;
        debug;
        st->initializeInvariants();

        // Kan check the obj func
        //        invariant inv = model->getObjectives()->at(0)->getInvariant();
        //        std::cout << "obj current value " << inv->CurrentValue << std::endl;
        //        int value = 0;
        //        for (IntegerVariable* iv : inv->getVariables()) {
        //            if (iv->isIntegerVariable()) {
        //                value += iv->getOneway()->CurrentValue;
        //            } else {
        //                value += inv->getCoefficients().at(iv->getID()) * iv->getCurrentValue();
        //            }
        //        }
        //        //        std::cout << "value for variables " << value << std::endl;
        //        for (invariant invar : inv->getInvariants()) {
        //            value += inv->getCoefficients().at(invar->getVariableID())*invar->getCurrentValue();
        //        }
        //        std::cout << "obj value check " << value << std::endl;



        //        std::cout << "invars" << std::endl;

        st->initializeConstraints();
        //        std::cout << "const" << std::endl;

        st->initializeObjective();
        //        std::cout << "objective" << std::endl;
        model->initialValue = st->getSolutionValue();

    }
    //        void addToQueue(propagation_queue& orgQueue, updateVector& queue, IntegerVariable* iv){

    void addToQueue(updateVector& orgQueue, updateVector& queue) {
        for (updateType invar : queue) {
            orgQueue.insert(invar);
            //                iv->addToPropagationQueue(invar);
            addToQueue(orgQueue, invar->getUpdateVector());
        }


    }

    // Not taking into account if the coefficient in objective function is negative

    bool canBeMadeOneway(IntegerVariable* iv, constraint cons) {
        if (cons->isOneway()) {
            //            std::cout << "Is one-way" << std::endl;
            return false;
        }
        int coeff = cons->getCoefficients().at(iv->getID());
        //        std::cout << "number of integer" << cons->getNumberOfIntegerVariables() << std::endl;
        //        std::cout << "Make oneway. constraint type " << cons->getArgument(0) << " number of intvars " << cons->getNumberOfIntegerVariables() << std::endl;
        if (cons->getNumberOfIntegerVariables() > 1) {
            //            unsigned highestLayer = 0;
            unsigned notDefined = 0;
            for (IntegerVariable* iv : cons->getVariables()) {
                if (iv->isIntegerVariable()) {
                    if (!iv->isDefined) {
                        notDefined++;
                        //                    } else {
                        //                        if (iv->oneway->layer > highestLayer) {
                        //                            highestLayer = iv->oneway->layer;
                        //                        }
                    }
                }
            }

            if (notDefined > 1) {
                //                std::cout << "Too many Integer variables not defined " << notDefined<< std::endl;
                return false;
            }
        }
        if (cons->getArgument(0) == EQ) {
            makeOneway(iv, cons, coeff);
            return true;
        }

        for (constraint cons : *model->getObjectives().get()) {
            int objCoef = cons->getCoefficients().at(iv->getID());
            if ((objCoef < 0 && coeff < 0) || (objCoef > 0 && coeff > 0)) {
                std::cout << "Coefficient fail" << std::endl;
                return false;
            }
        }

        makeOneway(iv, cons, coeff);
        return true;
    }

    void makeOneway(IntegerVariable* iv, constraint cons, int coeff) {
        // prob not from invariant 
        variableContainer& oldVars = cons->getVariables();
        //        std::vector<IntegerVariable*> variables;
        //        InvariantContainer invariants;
        //        for(IntegerVariable* iv : oldVars){
        //            std::cout << cons->getCoefficients().at(iv->getID()) << " ";//<< "x["<<iv->getCurrentValue()<<"] " ;
        //        }
        //        if(cons->getArgument(0)==0){
        //            std::cout << "= ";
        //        } else if(cons->getArgument(0)==1){
        //            std::cout << "<= ";
        //        }else {
        //            std::cout << "error" << std::endl;
        //            sleep(10);
        //        }
        //        std::cout << cons->getArgument(1) << std::endl;
        //        
        //        
        //        sleep(1);
        std::unordered_map<int, coefType> coefficients = cons->getCoefficients();
        std::unordered_map<int, coefType> newCoefficients;


        if (coeff == -1) {
            for (auto it = coefficients.begin(); it != coefficients.end(); ++it) {
                std::pair<int, coefType> coef(it->first, it->second);
                newCoefficients.insert(coef); //[it->first] = it->second;
            }
        } else {
            for (auto it = coefficients.begin(); it != coefficients.end(); ++it) {
                std::pair<int, coefType> coef(it->first, it->second / (-coeff));
                newCoefficients.insert(coef);
            }
            std::cout << "Changes coef " << coeff << std::endl;

        }
        newCoefficients.erase(iv->getID());

        // Check if this map goes out of scope
        //        std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(coefficients, model->getInvariants().size());
        std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(newCoefficients);

        //        if (cons->getNumberOfIntegerVariables() == 1) {
        //            for (IntegerVariable* oldiv : oldVars) {
        //                if (iv != oldiv) {
        //                    sumInvariant->VariablePointers.push_back(oldiv);
        //                    iv->addToUpdate(sumInvariant);
        //                }
        //            }
        //        } else {

        //        std::cout << sumInvariant->getVariableID() << std::endl;
        for (IntegerVariable* oldiv : oldVars) {

            if (oldiv != iv) {
                if (oldiv->isInteger) {
                    assert(oldiv->isDefined);
                    sumInvariant->addInvariant(oldiv->oneway);
                    oldiv->getOneway()->addToUpdate(sumInvariant.get());
//                    oldiv->getOneway()->addToUpdate(sumInvariant);



                } else {
                    sumInvariant->addVariable(oldiv);

                    //                    std::cout << sumInvariant.get() << std::endl;
                    oldiv->addToUpdate(sumInvariant.get());
//                    oldiv->addToUpdate(sumInvariant);
                    //                    sleep(1);
                }
            }
        }

        //        for (IntegerVariable* 
        //        }

        // Prob not from invariant i get coefficients

        //
        //        for (IntegerVariable* iv : variables) {
        //            iv->addToUpdate(sumInvariant);
        //        }
        //        for (invariant invar : invariants) {
        //            sumInvariant->invariants.push_back(invar);
        //            invar->addToUpdate(sumInvariant);
        //        }
        //        sumInvariant->layer = layer;
        //        sumInvariant->CurrentValue = -cons->getArgument(1);
        //        sumInvariant->startValue = -cons->getArgument(1);
        sumInvariant->invariantID = model->getInvariants().size();
        sumInvariant->setValue(-cons->getArgument(1));
        model->addInvariant(sumInvariant);

        if (cons->getArgument(0) == LQ) {

            std::shared_ptr<Max> maxInvariant = std::make_shared<Max>(sumInvariant, iv->getLowerBound(), iv->getID());
            sumInvariant->addToUpdate(maxInvariant.get());
//            sumInvariant->addToUpdate(maxInvariant);
            iv->setDefinedBy(maxInvariant, cons);
            cons->setInvariant(maxInvariant);
            maxInvariant->invariantID = model->getInvariants().size();
            model->addInvariant(maxInvariant);


        } else {

            iv->setDefinedBy(sumInvariant, cons);
            cons->setInvariant(sumInvariant);
            sumInvariant->variableID = iv->getID();

        }

        //        sumInvariant->setBounds(iv->lowerBound, iv->upperBound);
        cons->isOneway(true);
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

