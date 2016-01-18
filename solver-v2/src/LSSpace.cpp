//#include <c++/4.8/bits/stl_vector.h>

#include "LSSpace.hpp"

//#include "RowEcholonTransformer.hpp"


//using namespace Gecode;

LSSpace::LSSpace(std::shared_ptr<Model> model) {
    this->model = model;
    this->DDG = model->getDDG();
    //        std::cout << "constructed" << std::endl;

}

void LSSpace::createDDG(bool all) {
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


    //  return EXIT_SUCCESS;
    //    double M[3][4] = {
    //        { 1, 2, -1, -4},
    //        { 2, 3, -1, -11},
    //        { -2, 0, -3, 22}
    //    };
    //
    //    RowEcholonTransformer<double [3][4]> trans(M);
    //    for (int i = 0; i < 3; ++i) {
    //        for (int j = 0; j < 4; ++j)
    //            std::cout << M[i][j] << '\t';
    //        std::cout << "\n";
    //    }
    //    model->startUp();
    //    model->addVariablesToDDG(model->getAllVariables());
    DDG->addVariables(model->getAllVariables());
    //    model->getDDG()->printSizes();
    //    defining = std::vector<unsigned>(model->getAllVariables().size());


    //    std::vector<constraint> func2;
    //    for (unsigned i = 1; i < model->getConstraints().size(); i++) {
    //        for (constraint cons : *model->getConstraintsWithPriority(i)) {
    //            if (cons->isFunctional()) {
    //                func2.push_back(cons);
    //            }
    //        }
    //
    //    }
    ////    if (func2.size() > 0) {
    //        std::ofstream myfile;
    //        myfile.open("func.txt", std::ios::app);
    //        myfile << " " << func2.size() << "\n";
    //        myfile.close();
    ////    }
    //    std::cout << func2.size() << std::endl;
    //    exit(1);
    //    std::cout << "DELETE ABOVE" << std::endl;

    //#####################################################################################################################################
    // Making oneway looking at each integer variable. Harder to delete the constraints that have been made one-way
    //#####################################################################################################################################        
    //    std::list<IntegerVariable*> queue = model->getIntegerVariables();

    //    std::vector<IntegerVariable*>& queue = model->getPriorityVectorNr(0);
    //    std::sort(queue.begin(), queue.end(), IntegerVariable::SortGreater());
    //    for (IntegerVariable* iv : queue) {
    //        std::cout << iv->numberOfConstraints() << " ";
    //    } 
    //    std::cout << std::endl;

    //    for (IntegerVariable* iv : queue) {
    //        std::cout << iv->numberOfConstraints() << " ";
    //    }
    //    std::cout << std::endl;
    //        for (int i =queue.size()-1; i >= 0; i--){ 
    // Try to make Integer variables oneway
    //        unsigned numberOfIntegerVariables = queue.size();
    //        std::cout << numberOfIntegerVariables << std::endl;
    //        std::cout << "#####################################################################################################" << std::endl;
    //        std::cout << queue.size() << std::endl;
    //        unsigned layer = 1;
    //        int iter = 0;




    //    std::vector<IntegerVariable*>& queue = model->getPriorityVectorNr(0);
    //    std::sort(queue.begin(), queue.end(), IntegerVariable::SortGreater());
    /// Try to see if it is to expensive to prevent cycles. 

    std::vector<constraint> func;
    if (all) {

        for (unsigned i = 1; i < model->getConstraints().size(); i++) {
            for (constraint cons : *model->getConstraintsWithPriority(i)) {
                if (cons->isFunctional()) {
                    func.push_back(cons);
                }
            }

        }
    } else {
        func = model->getFeasibleFunctionalConstraints();
    }
    std::sort(func.begin(), func.end(), Constraint::Sortlower());
    unsigned totallyfixed = 0;
    int numberOfOneway = 0;
    //    unsigned lastsize = 0;
    for (constraint con : func) {
        //        if (canBeMadeOneway(con)) {

        if (con->canBeMadeOneway()) {
            invariant invar = con->makeOneway();
            model->addInvariant(invar);
            //            std::cout << "Add to DDG aswell " << std::endl;
            DDG->addInvariant(invar);

            numberOfOneway++;
            //            std::cout << numberOfOneway << " ";
            //            DDG->checkForCycles(model->getInvariants());

        }

        //        }

        if (!con->isOneway()) {
            bool gotNonfixed = false;
            for (Variable* iv : con->getVariables()) {
                if (!iv->isFixed()) {
                    gotNonfixed = true;
                }

            }

            if (gotNonfixed) {

            } else {
                totallyfixed++;
            }
            //            std::cout << std::endl;
        }
    }
    std::cout << "totally fixed " << totallyfixed << std::endl;

    std::cout << "number of invariants " << numberOfOneway << " out of " << func.size()
            << " (" << func.size() - totallyfixed << ") " << std::endl;
    DDG->cleanUpGraph(model->getNonFixedVariables());

    DDG->checkForCycles(model->getInvariants());

    //    exit(1);
    //    std::cout << "BUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    //    std::cout << "BUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    //    std::cout << "BUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    //    std::cout << "BUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    //    std::cout << "BUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    //    std::cout << "BUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    //    for (invariant inv : model->getInvariants()) {
    //        if (DDG->getBrokenInvariants().at(inv->getID())) {
    //            //            std::cout << inv->getID() << std::endl;
    //            continue;
    //        }
    //        //        std::cout << inv->getStartValue() << " ";
    //        assert(inv->getCurrentValue() == model->getAllVariables().at(inv->getVariableID())->getCurrentValue());
    //        for (invariant invar : DDG->getInvariantUpdate(inv->getID())) {
    //
    //
    //            for (invariant check : invar->getInvariantPointers()) {
    //                assert(check != inv);
    //            }
    //            for (Variable* check : invar->getVariablePointers()) {
    //                assert(check->getID() != invar->getVariableID());
    //                //                std::cout << check->getID() << " " <<   invar->getVariableID() << std::endl;
    //                assert(check != invar->getVariable());
    //            }
    //
    //
    //            invar->getInvariantPointers().push_back(inv);
    //        }
    //    }
    //
    //    invariant inv = model->getInvariants().at(127);
    //    int value = 0;
    //    auto coeff = inv->getCoefficients();
    //    for (Variable* var : inv->getVariablePointers()) {
    //        if (var->isDef()) {
    //            std::cout << DDG->getBrokenInvariants().at(var->getOneway()->getID()) << std::endl;
    //        }
    //        value += var->getCurrentValue() * coeff.at(var->getID());
    //    }
    //    for (invariant invar : inv->getInvariantPointers()) {
    //        value += invar->getCurrentValue() * coeff.at(invar->getVariableID());
    //    }
    //    std::cout << "value " << value << std::endl;
    //    value += inv->getStartValue();
    //    std::cout << "value " << value << std::endl;
    //
    //    invariant invar = model->getInvariants().at(127);
    //    std::cout << "Value of invariant 127 " << invar->getCurrentValue() << std::endl;
    //   
    //    std::cout << invar->getID() << std::endl;
    //    debug;
    //    for (Variable* var : invar->getVariablePointers()) {
    //        std::cout << var->getID() << " ";
    //    }
    //    for (Variable* var : invar->getVariablePointers()) {
    //        std::cout << invar->getCoefficients().at(var->getID()) << " ";
    //    }
    //    std::cout << std::endl;
    //    for (Variable* var : invar->getVariablePointers()) {
    //        std::cout << var->getCurrentValue() << " ";
    //    }
    //    std::cout << std::endl;
    //    for (invariant inv : invar->getInvariantPointers()) {
    //        std::cout << inv->getVariableID() << " ";
    //    }
    //    std::cout << std::endl;
    //    for (invariant inv : invar->getInvariantPointers()) {
    //        std::cout << inv->getVariable()->getCurrentValue() << " ";
    //    }
    //    std::cout << std::endl;
    //



}

void LSSpace::initializeLS(bool feasible) {



    //    if (feasible) {
    model->cleanUp();




    //    for (invariant inv : model->getInvariants()) {
    //        inv->calculateValue();
    //    }
    //    } else {
    //        debug;
    //        
    //        auto start = std::clock();
    //
    //        std::sort(model->getInvariants().begin(), model->getInvariants().end(), compare_invariant());
    //
    //        std::cout << "Backward invariant check" << std::endl;
    //        std::vector<bool> variableValueSet(model->getAllVariables().size(), false);
    //        std::vector<bool> invariantsFeasible(model->getInvariants().size(), false);
    //        for (invariant inv : model->getInvariants()) {
    //            std::cout << inv->getID() << " ";
    //        }
    //        for(Variable* var : model->getAllVariables()){
    //            if(var->isFixed() || var->isDef()){
    //                variableValueSet.at(var->getID()) = true;
    //            }
    //        }
    //        
    //        std::cout << std::endl;
    //        std::cout << model->getInvariants().size() << std::endl;
    //        //        for (invariant inv : model->getInvariants()) {
    //        //        int count = 0;
    //        //        for (InvariantContainer::reverse_iterator it = model->getInvariants().rbegin(); it != model->getInvariants().rend(); it++) {
    //        //        for (int i = model->getInvariants().size() - 1; i >= 0; i--) {
    //        for (int i = 0; i < model->getInvariants().size(); i++) {
    //            invariant inv = model->getInvariants().at(i);
    //            if (DDG->getBrokenInvariants().at(inv->getID())) {
    //                continue;
    //            }
    //            if (invariantsFeasible.at(inv->getID())) {
    //                continue;
    //            }
    //            std::vector<bool> variablesDefining(model->getAllVariables().size(), false);
    //            std::vector<bool> invariantsDefining(model->getInvariants().size(), false);
    //
    //            std::vector<Variable*> vars;
    //            std::vector<invariant> queue;
    //            //            for(invariant invar : inv->getInvariantPointers()){
    //            queue.push_back(inv);
    //            //                invariantsDefining.at(invar->getID())=true;
    //            std::cout << inv->getInvariantPointers().size() << std::endl;
    //            //            for (auto it = queue.begin(); it != queue.end(); it++) {
    //            for (unsigned i = 0; i < queue.size(); i++) {
    ////                if (queue.size() > 1) {
    ////                    std::cout << "inv id " << inv->getID() << std::endl;
    ////                    Variable* var = model->getAllVariables().at(inv->getVariableID());
    ////                    std::cout << "defining " << var->getDefining() << std::endl;
    ////
    ////                    Variable* var2 = model->getAllVariables().at(queue.at(1)->getVariableID());
    ////                    std::cout << "defining " << var2->getDefining() << std::endl;
    ////
    ////                    for (unsigned j = 0; i < queue.size(); j++) {
    ////                        std::cout << queue.at(j)->getVariableID() << std::endl;
    ////                        std::cout << "invar id " << queue.at(j)->getID() << std::endl;
    ////                        //                                std::cout << queue.at(i)
    ////                    }
    ////                }
    //
    //                for (invariant invar : queue.at(i)->getInvariantPointers()) {
    //
    //                    if (!invariantsDefining.at(invar->getID())) {
    //                        invariantsDefining.at(invar->getID()) = true;
    //                        queue.push_back(invar);
    //                    }
    //                }
    //            }
    //
    //
    //            std::cout << "queue size " << queue.size() << std::endl;
    //            for (invariant invar : queue) {
    //                for (Variable* var : invar->getVariablePointers()) {
    //                    if (!var->isDef() && !var->isFixed()) {
    //                        if (!variablesDefining.at(var->getID())) {
    //                            vars.push_back(var);
    //                            variablesDefining.at(var->getID()) = true;
    //                        }
    //                    }
    //                }
    //            }
    //            for (int i = queue.size() - 1; i >= 0; i--) {
    //                invariant invar = queue.at(i);
    //                auto coeffs = inv->getCoefficients();
    //
    //                std::pair<double, double> bounds = invar->getBounds();
    //                int value = 0;
    //                for (invariant inv : invar->getInvariantPointers()) {
    //                    if (!invariantsFeasible.at(inv->getID())) {
    //                        continue;
    //                    } else {
    //                        value += inv->getCurrentValue() * coeffs.at(inv->getVariableID());
    //                    }
    //                }
    //                std::vector<Variable*> notSet;
    //                for (Variable* var : invar->getVariablePointers()) {
    //                    if (variableValueSet.at(var->getID())) {
    //                        value += var->getCurrentValue() * coeffs.at(var->getID());
    //                    } else {
    //                        notSet.push_back(var);
    //                    }
    //                }
    //                bool feasible = (bounds.first <= value && value <= bounds.second);
    //                int newVal = value;
    //                for (Variable* var : notSet) {
    //                    var->setCurrentValue(var->getLowerBound());
    //                    newVal += var->getLowerBound() * coeffs.at(var->getID());
    //
    //                    //                    if (!variableValueSet.at(var->getID())) {
    //
    //                    //                        if (feasible) {
    //                    //                            int count = 0;
    //                    //                            while (count != 10) {
    //                    //                                int val = Random::Integer(var->getLowerBound(), var->getUpperBound()) * coeffs.at(var->getID());
    //                    //                                if (bounds.first <= val + value && val + value <= bounds.second) {
    //                    //                                    variableValueSet.at(var->getID()) = true;
    //                    //                                    value +=val;
    //                    //                                    
    //                    //                                } else {
    //                    //                                    feasible = false;
    //                    //                                    break;
    //                    //                                }
    //                    //                            }
    //                    //
    //                    //
    //                    //                        } 
    //
    //                }
    //
    //                if ((bounds.first <= newVal && newVal <= bounds.second)) {
    //
    //                    for (Variable* var : notSet) {
    //                        variableValueSet.at(var->getID()) = true;
    //                    }
    //                    invar->setValue(newVal);
    //                    invariantsDefining.at(invar->getID()) = true;
    //
    //                }
    //                std::cout << feasible << std::endl;
    //                //                debug;
    //
    //            }
    //
    //            std::cout << "vars size " << vars.size() << std::endl;
    //
    //            //            debug;
    //            //            (*it)->getInvariantPointers()
    //
    //        }
    //        std::cout << "took " << (std::clock() - start) / (double) CLOCKS_PER_SEC << " seconds" << std::endl;
    //        model->cleanUp();
    //
    //    }

    //    std::sort(model->getInvariants().begin(), model->getInvariants().end(), compare_invariant());
    //        exit(1);


    for (unsigned i = 0; i < model->getConstraints().size(); i++) {

        std::vector<invariant> collector;
        int value = 0;
        for (constraint cons : *model->getConstraintsWithPriority(i)) {
            if (!cons->isOneway()) {

                InvariantContainer invars = cons->createInvariants();
                //                for (invariant inv : invars) {
                //                InvariantContainer tmp;
                for (unsigned i = 0; i < invars.size(); i++) {
                    invariant inv = invars.at(i);
                    model->addInvariant(inv);


                    //                    model->addToObjectiveInvariant(inv);
                    DDG->addInvariant(inv, inv->getVariablePointers(), inv->getInvariantPointers());
                    //                    tmp.push_back(inv);


                }
                //                model->addInvariant(invars.back());

                //                DDG->addInvariant(invars.back(), tmp);
                collector.push_back(invars.back());
                value += invars.back()->getCurrentValue();
                if (invars.back()->representConstraint()) {
                    if (invars.back()->getCurrentValue() > 0) {
                        //                        std::cout << invars.back()->getCurrentValue() << std::endl;
                        //                        std::cout << "violated constraint " << std::endl;
                        model->addViolatedConstraint(invars.back()->getInvariantPointers().back());
                        //                        debug;
                    }
                }


            }
        }

        Sum* priorityResult = new Sum();
        priorityResult->setValue(value);
        priorityResult->setInvariantPointers(collector);
        model->addInvariant(priorityResult);
        DDG->addInvariant(priorityResult, collector);
        model->addToEvaluationInvariants(priorityResult);
    }

    if (model->getViolatedConstraints().size() != 0) {
        //        model->getViolatedConstraints().size();
        std::cout << "Not feasible " << std::endl;
        //        exit(1);
    }


    //    DDG->checkForCycles(model->getInvariants());
    DDG->checkForCycles(model->getInvariants());
    std::sort(model->getInvariants().begin(), model->getInvariants().end(), compare_invariant());

    std::cout << "Total time used so far " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << std::endl;
    std::cout << "Create propagator queues" << std::endl;
    DDG->createPropagationQueue(model->getAllVariables(), model->getInvariants());
    //    model->getDDG()->printSizes();
    std::cout << "Total time used so far " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << std::endl;
    model->initialize();
    if (model->getViolatedConstraints().size() != 0) {
        std::cout << "Not feasible " << std::endl;
        //        exit(1);
    }
    currentState = std::make_shared<State>(model);
    bestState = std::make_shared<State>(model);
    //    for (invariant invar : model->getInvariants()) {
    //        for (invariant inv : model->getUpdate(invar)) {
    //            if (invar->getID() > inv->getID()) {
    //                std::cout << "lower invariant id depends on higher id" << std::endl;
    //                
    //            }
    //        }
    //    }
    //    bool failed = false;
    //    for (unsigned i = 1; i < model->getConstraints().size(); i++) {
    //        std::shared_ptr<std::vector<std::shared_ptr < Constraint>>>prio = model->getConstraintsWithPriority(i);
    //        //        for (unsigned j = 0; j < prio->size(); j++) {
    //
    //        for (std::shared_ptr<Constraint> cons : *prio) {
    //            if (!cons->isOneway()) {
    //                if (!cons->testCons()) {
    //                    failed = true;
    //                }
    //            }
    //        }
    //    }
    //    if (failed) {
    //        std::cout << "constraints are violated? " << std::endl;
    //        exit(1);
    //    }
}

void LSSpace::optimizeSolution(int time, int test) {







    //#####################################################################################
    //    should test all invariants
    //#####################################################################################
    //    for (invariant inv : model->getInvariants()) {
    //        if (!inv->test()) {
    //            std::cout << "Wrong before optimization " << std::endl;
    //
    //            exit(1);
    //        }
    //
    //    }
    //
    //
    //
    //    for (Variable* iv : model->getNonFixedVariables()) {
    //        if (iv->isDef()) {
    //            assert(iv->getOneway()->getCurrentValue() == iv->getCurrentValue());
    //        } else {
    //            //            unsigned prev = 0;
    //            //            for (invariant invar : model->getPropagationQueue(iv)) {
    //            //                if (invar->getTimestamp() > prev) {
    //            //                    std::cout << "pointing to higher timestamp" << std::endl;
    //            //                    
    //            //                } else {
    //            //                    prev = invar->getTimestamp();
    //            //                }
    //            //            }
    //            for (unsigned i = 0; i < model->getUpdate(iv).size(); i++) {
    //                bool found = false;
    //                for (auto inva : model->getPropagationQueue(iv)) {
    //                    if (model->getUpdate(iv).at(i) == inva) {
    //                        found = true;
    //                        break;
    //                    }
    //                }
    //                if (!found) {
    //                    std::cout << "one from update is not in propagation queue" << std::endl;
    //
    //                }
    //            }
    //            for (auto inva : model->getPropagationQueue(iv)) {
    //                unsigned times = inva->getTimestamp();
    //                bool found = false;
    //                for (auto invas : model->getUpdate(inva)) {
    //                    if (invas->getTimestamp() > times) {
    //                        std::cout << "wrong order of timestamp " << std::endl;
    //
    //                    }
    //                    for (auto inva2 : model->getPropagationQueue(iv)) {
    //
    //                        if (invas == inva2) {
    //                            found = true;
    //                            break;
    //                        }
    //                    }
    //                    if (!found) {
    //                        std::cout << "one from update is not in propagation queue" << std::endl;
    //
    //                    }
    //
    //                }
    //
    //
    //
    //            }
    //        }
    //    }
    //    std::cout << DDG->getVariableUpdate(1).size() << std::endl;
    //    for (invariant inv : DDG->getPropagationQueue(model->getVariable(1))) {
    //        std::cout << inv->getCurrentValue() << " ";
    //    }
    //    std::cout << std::endl;
    //    std::cout << DDG->getPropagationQueue(model->getVariable(1)).size() << std::endl;
    //    debug;
    //    debug;
    //    debug;
    //    debug;
    //    debug;
    //    for (invariant inv : model->getInvariants()) {
    //        std::cout << inv->getType() << ": " << inv->getCurrentValue() << "\t ";
    //
    //    }
    //    for(Variable* var : model->getAllVariables()){
    //        std::cout << var->getCurrentValue() << " ";
    //    }
    //    std::cout << std::endl;
    //    std::cout << "Eval ";
    //    for (invariant inv : model->getEvaluationInvariants()) {
    //        std::cout << inv->getType() << ": " << inv->getCurrentValue() << "\t";
    //    }
    //    std::cout << std::endl;
    //    debug;
    //    if (model->getViolatedConstraints().size() != 0) {
    //        //        model->getViolatedConstraints().size();
    //        std::cout << "violation " << model->getViolatedConstraints().size() << std::endl;
    //        //        exit(1);
    //    }

    for (Variable* var : model->getNonFixedVariables()) {
        if (var->isDef()) {
            Constraint * con = var->getDefinedByCon();
            invariant inv = var->getOneway();
            assert(con->getVariables().size() - 1 == inv->getVariablePointers().size() + inv->getInvariantPointers().size());
            for (Variable* var : inv->getVariablePointers()) {
                if (var->isDef()) {
                    std::cout << "Defined but in variable pointers" << std::endl;
                    debug;
                }
                bool found = false;
                for (invariant invar : DDG->getVariableUpdate(var->getID())) {
                    if (invar == inv) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    debug;
                    exit(1);
                }

            }
            for (invariant invar : inv->getInvariantPointers()) {
                bool found = false;
                for (invariant inva : DDG->getInvariantUpdate(invar->getID())) {
                    if (inva == inv) {
                        found = true;
                        break;
                    }
                }


                if (!found) {
                    debug;
                    exit(1);
                }
            }
        }
    }


    int counter = 0;

    for (Variable* var : model->getNonFixedVariables()) {
        //        std::cout << "Var id " << var->getID() << std::endl;
        if (var->isDef()) {

            //            debug;
            if (var->isInObjective()) {
                if (var->numberOfConstraints() != DDG->getInvariantUpdate(var->getOneway()->getID()).size()) {
                    std::cout << var->numberOfConstraints() + 1 << " " << DDG->getInvariantUpdate(var->getOneway()->getID()).size() << std::endl;

                } else if (var->numberOfConstraints() != DDG->getInvariantUpdate(var->getOneway()->getID()).size()) {
                    std::cout << var->numberOfConstraints() << " " << DDG->getInvariantUpdate(var->getOneway()->getID()).size() << std::endl;
                    counter++;
                }
            }
        } else {
            if (var->isInObjective()) {
                if (var->numberOfConstraints() + 1 != DDG->getVariableUpdate(var->getID()).size()) {
                    std::cout << var->numberOfConstraints() + 1 << " " << DDG->getVariableUpdate(var->getID()).size() << std::endl;
                }
            } else {

                if (var->numberOfConstraints() != DDG->getVariableUpdate(var->getID()).size()) {
                    std::cout << var->numberOfConstraints() << " " << DDG->getVariableUpdate(var->getID()).size() << std::endl;
                    counter++;
                }
            }
        }
        //        assert(var->numberOfConstraints() == DDG->getVariableUpdate(var->getID()).size());
    }
    //    
    std::cout << "Number of evaluation variables " << model->getEvaluationVariables().size() << std::endl;
    std::cout << "counter " << counter << " variables " << model->getNonFixedVariables().size() << std::endl;
    //    debug;
    //    debug;
    //    debug;
    //    debug;
    //    assert(model->getEvaluationVariables().size() + counter == model->getAllVariables().size());
    //        exit(1);


    //###########################################################################################
    // Check pointers in DDG and backwards in invariants
    //###########################################################################################
    //        for (invariant inv : model->getInvariants()) {
    //    
    //            //        Variable* var = model->getVariable(inv->getVariableID());
    //            //        assert(var->getOneway() == inv);
    //            //        assert(var->getID() == inv->getVariableID());
    //            //        assert(var->isDef());
    //            for (Variable* var : inv->getVariablePointers()) {
    //                assert(!var->isDef());
    //                if (!var->isFixed()) {
    //                    bool foundInv = false;
    //                    for (invariant upd : DDG->getVariableUpdate(var->getID())) {
    //                        if (upd == inv) {
    //                            foundInv = true;
    //                            break;
    //                        }
    //                    }
    //                    assert(foundInv);
    //                }
    //            }
    //            for (invariant invar : inv->getInvariantPointers()) {
    //                bool foundInv = false;
    //                for (invariant upd : DDG->getInvariantUpdate(invar->getID())) {
    //                    if (upd == inv) {
    //                        foundInv = true;
    //                        break;
    //                    }
    //                }
    //                assert(foundInv);
    //            }
    //            for (invariant upd : DDG->getInvariantUpdate(inv->getID())) {
    //                bool foundInv = false;
    //    
    //                for (invariant backward : upd->getInvariantPointers()) {
    //                    if (backward == inv) {
    //                        foundInv = true;
    //                        break;
    //                    }
    //                }
    //                assert(foundInv);
    //    
    //            }
    //        }
    //        for (Variable* var : model->getAllVariables()) {
    //            assert(var->getLowerBound() <= var->getCurrentValue() && var->getCurrentValue() <= var->getUpperBound());
    //    
    //            if (var->isDef()) {
    //                assert(var->getCurrentValue() == var->getOneway()->getCurrentValue());
    //                assert(var->getLowerBound() <= var->getCurrentValue() && var->getCurrentValue() <= var->getUpperBound());
    //            } else if (var->isFixed()) {
    //    
    //            } else {
    //                for (invariant inv : DDG->getVariableUpdate(var->getID())) {
    //                    bool foundVar = false;
    //                    for (Variable* iv : inv->getVariablePointers()) {
    //                        if (iv == var) {
    //                            foundVar = true;
    //                            break;
    //                        }
    //                    }
    //                    assert(foundVar);
    //                }
    //            }
    //        }
    //        std::cout << "No error in pointers" << std::endl;
    //        debug;
    //        exit(1);
    //    



    //        currentState->saveSolution();
    //    std::cout << "ini obj val " << currentState->getEvaluation().at(0) << std::endl;
    //    std::cout << "ini sol val " << st->getSolutionValue() << std::endl;
    //    unsigned fourPercent = model->getNonFixedVariables().size() / 25;
    //    unsigned twoPercent = model->getMask().size() / 50;
    //    std::cout << "two % " << twoPercent << " " << model->getNonFixedVariables().size() << "/" << 50 << std::endl;
    //    randomMoves = 6;
    //    for (unsigned i = 0; i < currentState->getSolution().size(); i++) {
    //        assert(currentState->getSolution().at(i) == bestState->getSolution().at(i));
    //    }
    //    for (unsigned i = 0; i < model->getEvaluationInvariants().size(); i++) {
    //        int val = model->getEvaluationInvariantNr(i)->getCurrentValue();
    //        currentState->getEvaluation().at(i) = val;
    //        bestState->getEvaluation().at(i) = val;
    //        std::cout << val << std::endl;
    //
    //    }

    //        setSolution(bestState);
    //    std::cout << "solution set" << std::endl;
    //    std::vector<Variable*> swapVars;
    //    if (model->containsIntegerVariables) {
    //
    //        for (Variable* iv : model->getMask()) {
    //            if (model->getInConstraintWithAt(iv->getID()).size() > 0) {
    //                swapVars.push_back(iv);
    //            }
    //        }
    //    }
    unsigned tabulistsize = 0;
    for (Variable* var : model->getMask()) {
        if (var->getID() > tabulistsize) {
            tabulistsize = var->getID();
        }
    }
    //    for (invariant inv : model->getInvariants()) {
    //        std::cout << inv->getDeltaValue() << " " << inv->getCurrentValue() << " ";
    //    }
    //    std::cout << std::endl;
    tabulistsize++;
    std::vector<int> tabulist(tabulistsize, -tabulistsize);
    //    std::cout << "Number of randomMoves " << randomMoves << std::endl;
    std::cout << "Number of variables used in local search " << model->getMask().size() << std::endl;
    //    FlipNeighborhood* FN = new FlipNeighborhood(model, currentState);
    //    RestrictedFlipNE* RFN = new RestrictedFlipNE(model, currentState);
    //    EvalFlipNE* FON = new EvalFlipNE(model, currentState);
    //    ConflictOnlyNE* CON = new ConflictOnlyNE(model, currentState);
    //    RestrictedFlipNE* RFN = new RestrictedFlipNE(model, currentState);
    //    Flip2Neighborhood* F2N = new Flip2Neighborhood(model, currentState);
    //    SwapNeighborhood* SN = new SwapNeighborhood(model, currentState);
    //    TabuSearch TSFN(model, FN);
    //    TabuSearch TSRFN(model, RFN);
    //    TabuSearch TSFON(model, FON);
    //    TabuSearch TSMCN(model, MCN);

    //    TabuSearchFI TSFIFON(model, FON);
    //    TabuSearchFI TSFIMCN(model, MCN);
    //    TabuSearchFI TSFIFN(model, FN);
    //    RandomWalk RW(model, FN, randomMoves);
    //    BestImprovement BI(model, FN);
    //    BestImprovement BIFN(model, FN);
    //    FirstImprovement FI(model, FN);
    //    FirstImprovement FIMCN(model, MCN);
    //    FirstImprovement FIFON(model, FON);
    //    unsigned loopCounter = 0;
    //    unsigned impCounter = 0;
    //    std::cout << model->getMask().size() << std::endl;
    double timelimit = (double) time - (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC;
    //    std::cout << "time left for local search " << timelimit << std::endl;
    std::cout << "## LSLimitLeft " << timelimit << std::endl;
    double usedTime = 0;
    std::clock_t start = std::clock();

    //    std::cout << "Number of evaluation variables " << model->getEvaluationVariables().size() << std::endl;
    //    debug;

    //######################################################
    // Test 1 TS conflict only + TS Restricted neighborhood
    //######################################################
    if (test == 1) {
        std::cout << "# test 1" << std::endl;

        ConflictOnlyNE* CON = new ConflictOnlyNE(model, currentState);
        TabuSearch TSCON(CON);
        //        EvalFlipNE* EFN = new EvalFlipNE(model, currentState);
        FlipNeighborhood* FN = new FlipNeighborhood(model, currentState);

        RestrictedFlipNE* RFN = new RestrictedFlipNE(model, currentState);
        TabuSearch TSRFN(RFN);
        while (usedTime < timelimit) {
            while (!currentState->isFeasible() && usedTime < timelimit) {
                TSCON.Start(iterations, bestState, currentState, tabulist);
                iterations++;
                if (currentState->compare(bestState)) {
                    currentState->setViolation();
                    bestState->copy(currentState);
                    usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
                    std::cout << "# value: ";
                    for (int eval : bestState->getEvaluation()) {
                        std::cout << eval << " ";
                    }
                    std::cout << " # iter " << iterations << " # used " << usedTime << " # method TS CON" << std::endl;
                    //                std::cout << "Number of violated constraints " << model->getViolatedConstraints().size() << std::endl;                }

                }
                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;


            }
            //            unsigned counter = 0;
            //            while (counter < randomMoves&& usedTime < timelimit) {
            while (currentState->isFeasible() && usedTime < timelimit) {
//                if(iterations > 23){
//                    break;
//                }
                TSRFN.Start(iterations, bestState, currentState, tabulist);

                iterations++;
                if (currentState->compare(bestState)) {
                    currentState->setViolation();
//                    assert(model->getViolatedConstraints().size() == 0);
                    bestState->copy(currentState);
                    usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
                    std::cout << "# value: ";
                    for (int eval : bestState->getEvaluation()) {
                        std::cout << eval << " ";
                    }
                    std::cout << " # iter " << iterations << " # used " << usedTime << " # method TS RFN" << std::endl;
                }
                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;


            }
//                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;

        }
        delete CON;
        delete RFN;
        delete FN;
    }
    //######################################################
    // Test 2 RW 2 % + FI
    //######################################################



    if (test == 2) {
        std::cout << "# test 2" << std::endl;
        unsigned twoPercent = model->getMask().size() / 50;
        unsigned onePercent = model->getMask().size() / 100;
        //        unsigned randomMoves = std::min(twoPercent, (unsigned) 10);
        unsigned randomMoves = std::min(twoPercent, (unsigned) 10);
        //        unsigned randomMoves = twoPercent;
        std::cout << "Number of randomMoves " << randomMoves << std::endl;
        FlipNeighborhood* FN = new FlipNeighborhood(model, currentState);
        RandomWalk RW(FN, randomMoves);
        FirstImprovement FIFN(FN);
        bool imp = true;
        while (usedTime < timelimit) {
            imp = true;
            while (imp && usedTime < timelimit) {
                imp = FIFN.Start();
                iterations++;
                if (currentState->compare(bestState)) {
                    currentState->setViolation();
                    bestState->copy(currentState);
                    usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
                    std::cout << "# value: ";
                    for (int eval : bestState->getEvaluation()) {
                        std::cout << eval << " ";
                    }
                    std::cout << " # iter " << iterations << " # used " << usedTime << " # method FI FN" << std::endl;
                    //                std::cout << "Number of violated constraints " << model->getViolatedConstraints().size() << std::endl;                }

                }
                //                std::cout << "# value: ";
                //                for (int eval : currentState->getEvaluation()) {
                //                    std::cout << eval << " ";
                //                }
                //                std::cout << " # iter " << iterations << " # used " << usedTime << " # method FI FN" << std::endl;
                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;

            }

            //            unsigned counter = 0;
            //            while (counter < randomMoves&& usedTime < timelimit) {
            RW.Start();
            iterations += randomMoves;
            //                iterations++;
            //                counter++;
            if (currentState->compare(bestState)) {
                currentState->setViolation();
                bestState->copy(currentState);
                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
                std::cout << "# value: ";
                for (int eval : bestState->getEvaluation()) {
                    std::cout << eval << " ";
                }
                std::cout << " # iter " << iterations << " # used " << usedTime << " # method RW FN" << std::endl;
            }
            //            std::cout << "# value: ";
            //            for (int eval : currentState->getEvaluation()) {
            //                std::cout << eval << " ";
            //            }
            //            std::cout << " # iter " << iterations << " # used " << usedTime << " # method RW FN" << std::endl;
            usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;

        }
        delete FN;

    }

    //######################################################
    // Test 3 MCH + TS Restricted neighborhood
    //######################################################
    if (test == 3) {
        //        FlipNeighborhood* FN = new FlipNeighborhood(model, currentState);

        std::cout << "# test 3" << std::endl;
        RandomConflictConNE* RCC = new RandomConflictConNE(model, currentState);
        BestImprovement BIRCC(RCC);
        bool alwaysCommit = true;
        RestrictedFlipNE* RFN = new RestrictedFlipNE(model, currentState);
        //        TabuSearch TSRFN(model, FN);
        TabuSearch TSRFN(RFN);
        while (usedTime < timelimit) {
            while (!currentState->isFeasible() && usedTime < timelimit) {
                BIRCC.Start(alwaysCommit);
                iterations++;
                if (currentState->compare(bestState)) {
                    currentState->setViolation();
                    bestState->copy(currentState);
                    usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
                    std::cout << "# value: ";
                    for (int eval : bestState->getEvaluation()) {
                        std::cout << eval << " ";
                    }
                    std::cout << " # iter " << iterations << " # used " << usedTime << " # method BI RCC" << std::endl;
                    //                std::cout << "Number of violated constraints " << model->getViolatedConstraints().size() << std::endl;                }

                }
                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;



            }
            //            std::cout << usedTime << std::endl;
            //            while (usedTime < timelimit) {
            while (currentState->isFeasible() && usedTime < timelimit) {
                //                debug;
                TSRFN.Start(iterations, bestState, currentState, tabulist);
                iterations++;
                //                debug;
                if (currentState->compare(bestState)) {
                    currentState->setViolation();
                    bestState->copy(currentState);
                    usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
                    std::cout << "# value: ";
                    for (int eval : bestState->getEvaluation()) {
                        std::cout << eval << " ";
                    }
                    std::cout << " # iter " << iterations << " # used " << usedTime << " # method TS RFN" << std::endl;
                }
                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;


            }
        }
        delete RFN;
        delete RCC;
    }

    //    exit(1);
    //    timelimit = -1;
    //    debug;
    //        while (usedTime < timelimit) {
    //        loopCounter++;


    //                while (FI.Start()) {
    //                    iterations++;
    //                }
    //                if (currentState->getEvaluation().at(0) < bestState->getEvaluation().at(0) && currentState->isFeasible()) {
    //                    bestState->copy(currentState);
    //                    //            setSolution(bestState);
    //                    usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    //                    std::cout << "improved solution value to: " << bestState->getEvaluation().at(0) << " after " << iterations << " iterations and " << usedTime << " seconds using FI" << std::endl;
    //        
    //                }
    //                RW.Start();
    //                iterations += randomMoves;
    //        
    //         for (invariant inv : model->getInvariants()) {
    //            if (!inv->test()) {
    //                std::cout << "Wrong before optimization " << std::endl;
    //                
    //                exit(1);
    //            }
    //    
    //        }
    //        for (auto it = model->getViolatedConstraints().begin(); it != model->getViolatedConstraints().end(); it++) {
    //
    //            std::cout << model->getViolatedConstraints().size() << std::endl;
    //            unsigned id = it->first;
    //            std::cout << "id " << id << std::endl;
    //            std::cout << "Size of pointing to " << DDG->getInvariantUpdate(id).size() << std::endl;
    //            std::cout << DDG->getInvariantUpdate(id).at(0)->getCurrentValue() << std::endl;
    //            std::cout << "is in violated constraints " << model->getViolatedConstraints().begin()->second->inViolatedConstraints() << std::endl;
    //            std::cout << "Number of variables " << model->getViolatedConstraints().at(id)->getVariablePointers().size() << std::endl;
    //        }


    //        while (!currentState->isFeasible()) {
    //
    //            imp = TS2.Start(iterations, bestState, currentState, tabulist);
    //            //                    imp = BI.Start();
    //            iterations++;
    //            imp ? impCounter = 0 : impCounter++;
    //
    //
    //            //            std::cout << "infes" << std::endl;
    //            //            std::cout << "Feasible? " << currentState->isFeasible() << std::endl;
    //            if (currentState->compare(bestState)) {
    //                //        if ((currentState->getEvaluation().at(0) < bestState->getEvaluation().at(0) && currentState->isFeasible()) || !bestState->isFeasible()) {
    //                bestState->copy(currentState);
    //                //            std::cout << "Implement state comparison, which infeasible is best?" << std::endl;
    //                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    //                std::cout << "improved solution value to: " << bestState->getEvaluation().at(0) << " " << bestState->getEvaluation().at(1) << " after " << iterations << " iterations and " << usedTime << " seconds using TS BI with MCN" << std::endl;
    //
    //            }
    //            if (impCounter > 100) {
    //                std::cout << "break" << std::endl;
    //                break;
    //            }
    //            //            std::cout << "in 1" << std::endl;
    //            std::cout << currentState->getEvaluation().at(0) << " " << currentState->getEvaluation().at(1) << " after " << iterations << " iterations and " << usedTime << " seconds using TS BI with MCN" << std::endl;
    //            std::cout << "Number of violated constraints " << model->getViolatedConstraints().size() << std::endl;
    //            //            std::cout << model->getViolatedConstraints().size() << std::endl;
    //
    //
    //            //            std::cout << "used time " << (std::clock() - start) / (double) CLOCKS_PER_SEC << std::endl;
    //            //                                for (auto it = model->getViolatedConstraints().begin(); it != model->getViolatedConstraints().end(); it++) {
    //            //                    
    //            //                                    std::cout << "Size of violated cons  " << model->getViolatedConstraints().size() << std::endl;
    //            //                                    unsigned id = it->first;
    //            //                                    std::cout << "id " << id << std::endl;
    //            //                                    std::cout << "Size of pointing to " << DDG->getInvariantUpdate(id).size() << std::endl;
    //            //                                    std::cout << "Value " << DDG->getInvariantUpdate(id).at(0)->getCurrentValue() << std::endl;
    //            //                                    std::cout << "is in violated constraints " << model->getViolatedConstraints().begin()->second->inViolatedConstraints() << std::endl;
    //            //                                    std::cout << "Number of variables " << it->second->getVariablePointers().size() << std::endl;
    //            //                                    std::cout <<  currentState->getEvaluation().at(1) << std::endl;
    //            //                                }
    //        }

    //        imp = true;
    //        //        while (imp) {
    //        while (!currentState->isFeasible() && usedTime < timelimit) {
    //            //            imp = TSMCN.Start(iterations, bestState, currentState, tabulist);
    //            //            imp = FIMCN.Start();
    //            imp = BIMCN.Start();
    //            //            imp = TSFN.Start(iterations, bestState, currentState, tabulist);
    //            imp ? impCounter = 0 : impCounter++;
    //
    //            iterations++;
    //            if (currentState->compare(bestState)) {
    //                //        if ((currentState->getEvaluation().at(0) < bestState->getEvaluation().at(0) && currentState->isFeasible()) || !bestState->isFeasible()) {
    //                bestState->copy(currentState);
    //                //            std::cout << "Implement state comparison, which infeasible is best?" << std::endl;
    //                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    //                std::cout << "improved solution value to: ";
    //                for (int eval : bestState->getEvaluation()) {
    //                    std::cout << eval << " ";
    //                }
    //                std::cout << "after " << iterations << " iterations and " << usedTime << " seconds using BI MCN" << std::endl;
    //                std::cout << "Number of violated constraints " << model->getViolatedConstraints().size() << std::endl;
    //
    //
    //            }
    //            //            if (impCounter > model->getViolatedConstraints().size() ) {
    //            //                std::cout << "RW ";
    //            //                RW.Start();
    //            //                iterations += randomMoves;
    //            //                impCounter = 0;
    //            //            }
    //            usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    //
    //        }
    //        //        std::cout << currentState->getEvaluation().at(0) << " " << currentState->getEvaluation().at(1) << " after " << iterations << " iterations and " << usedTime << " seconds using BI " << std::endl;
    //        //        std::cout << tabulist.at(708) << " " << tabulist.at(2086) << " " << tabulist.at(698) << " " << tabulist.at(2066) << " " << tabulist.at(1619) << " " << tabulist.at(1613) << " " << tabulist.at(1204) << " " << std::endl;
    //        //        imp ? impCounter = 0 : impCounter++;
    //
    //        imp = true;
    //        //        while (TS.Start()) {
    //        //            if (imp) {
    //        unsigned counter = 0;
    //        //        while (counter < FN->getSize()/10 && usedTime < timelimit) {
    //        while (imp) {
    //            //            if(model->getViolatedConstraints().size()>0){
    //            //                std::cout <<model->getViolatedConstraints().size() << std::endl;
    //            //                debug;
    //            //            }
    //            counter++;
    //            //            imp = FIFON.Start();
    //            //            imp = TSFIFON.Start(iterations, bestState, currentState, tabulist);
    //            imp = TSFON.Start(iterations, bestState, currentState, tabulist);
    //            //        std::cout << currentState->getEvaluation().at(0) << " " << currentState->getEvaluation().at(1) << " after " << iterations << " iterations and " << usedTime << " seconds using TS BI FN" << std::endl;
    //
    //            iterations++;
    //
    //            if (currentState->compare(bestState)) {
    //                //        if ((currentState->getEvaluation().at(0) < bestState->getEvaluation().at(0) && currentState->isFeasible()) || !bestState->isFeasible()) {
    //                bestState->copy(currentState);
    //                //            std::cout << "Implement state comparison, which infeasible is best?" << std::endl;
    //                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    //                std::cout << "improved solution value to: ";
    //                for (int eval : bestState->getEvaluation()) {
    //                    std::cout << eval << " ";
    //                }
    //                std::cout << "after " << iterations << " iterations and " << usedTime << " seconds using TS FI FON" << std::endl;
    //
    //            }
    //            usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    //
    //            //            for (int eval : bestState->getEvaluation()) {
    //            //                std::cout << eval << " ";
    //            //            }
    //            //            std::cout << "after " << iterations << " iterations and " << usedTime << " seconds using TS" << std::endl;
    //        }
    //            }
    //        }

    //            usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;

    //        std::cout << currentState->getEvaluation().at(0) << " " << currentState->getEvaluation().at(1) << " after " << iterations << " iterations and " << usedTime << " seconds using TS BI FN" << std::endl;
    //    }

    //        std::cout << "feas -> infeas" <<   std::endl;

    //                        while (!currentState->isFeasible()) {
    //                
    //                            //            if(model->getViolatedConstraints().size() != currentState->getEvaluation().at(1)){
    //                            //                std::cout <<model->getViolatedConstraints().size() << " vs "  << currentState->getEvaluation().at(1) << std::endl;
    //                            //            }
    //                            BI.Start();
    //                            iterations++;
    //                
    //                            //            std::cout << currentState->getEvaluation().at(0) << " " << currentState->getEvaluation().at(1) << " after " << iterations << " iterations and " << usedTime << " seconds using BI with MCN" << std::endl;
    //                
    //     
    //    if (test == 1) {
    //        unsigned twoPercent = model->getMask().size() / 50;
    //        unsigned randomMoves = std::min(twoPercent, (unsigned) 10);
    //        FlipNeighborhood* FN = new FlipNeighborhood(model, currentState);
    //        BestImprovement BIFN(model, FN);
    //        FirstImprovement FIFN(model, FN);
    //        RandomWalk RW(model, FN, randomMoves);
    //
    //        while (usedTime < timelimit) {
    //
    //            //            while (FIFN.Start()) {
    //            while (BIFN.Start()) {
    //                iterations++;
    //            }
    //            if (currentState->compare(bestState)) {
    //                //        if ((currentState->getEvaluation().at(0) < bestState->getEvaluation().at(0) && currentState->isFeasible()) || !bestState->isFeasible()) {
    //                bestState->copy(currentState);
    //                //            std::cout << "Implement state comparison, which infeasible is best?" << std::endl;
    //                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    //                std::cout << "improved solution value to: ";
    //                for (int eval : bestState->getEvaluation()) {
    //                    std::cout << eval << " ";
    //                }
    //                std::cout << "after " << iterations << " iterations and " << usedTime << " seconds using BI" << std::endl;
    //
    //            }
    //            //        std::cout << currentState->getEvaluation().at(0) << " " << currentState->getEvaluation().at(1) << " after " << iterations << " iterations and " << usedTime << " seconds using BI" << std::endl;
    //            //
    //            RW.Start();
    //            iterations += randomMoves;
    //            //
    //            usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    //        }
    //    }
    //    for (invariant inv : model->getInvariants()) {
    //        if (!inv->test()) {
    //            std::cout << "Wrong before optimization " << std::endl;
    //
    //            exit(1);
    //        }
    //
    //    }

    //    std::cout << "Number of Moves delta calculated " << NE->testCounter << std::endl;
    //    std::cout << "number of loops " << loopCounter << std::endl;
    std::cout << "## LSTimeUsed " << usedTime << std::endl;
    //    std::cout << "obj val " << currentState->getEvaluation().at(0) << " violations ";
    //    for (unsigned i = 1; i < currentState->getEvaluation().size(); i++) {
    //        std::cout << currentState->getEvaluation().at(i) << " ";
    //    }
    //    std::cout << std::endl;
    //    std::cout << "obj val " << bestState->getEvaluation().at(0) << std::endl;
    //    std::cout << "Set soluton " << std::endl;
    //    setSolution(bestState);
    //    if (testInvariant()) {
    //        std::cout << "failed test of invariant after setSolution" << std::endl;
    //        exit(1);
    //    }
    //    sol2 = bestState->getSolution();
    //    for (unsigned i = 0; i < sol1.size(); i++) {
    //        if(sol1[i] != sol2[i]){
    //            std::cout << sol1[i] << "  " << sol2[i] << std::endl;
    //            auto iv =  model->getAllVariables().at(i);
    //            std::cout << "id " << iv->getID() << std::endl;
    //            if( iv->isDef()){
    //                std::cout << "defined by invariant " << iv->getOneway()->getID() <<" type " <<  iv->getOneway()->getType() << std::endl;
    //            } else {
    //                std::cout << "fixed? " << iv->isFixed() << std::endl;
    //            }
    //        }
    ////        assert(sol1[i] == sol2[i]);
    //    }
    std::cout << "## quality ";
    for (int eval : bestState->getEvaluation()) {

        std::cout << eval << " ";
    }
    std::cout << std::endl;
    std::cout << "## violations " << bestState->getViolations() << std::endl;
    std::cout << "## iterations " << iterations << std::endl;
    setSolution(bestState);
    //    delete mv;
    //    delete bestMove;
    //    delete SN;
    //    delete F2N;
    //    delete FN;
    //    delete RFN;
    //    delete RCC;
    //    std::cout << "O " << bestState->getEvaluation().at(0) << " ";
    //
    //    std::cout << "Solution value: " << st->getObjectiveValue() << std::endl;
    //    std::cout << "Number of moves " << iterations << std::endl;
}

bool LSSpace::testInvariant() {
    bool failed = false;
    for (invariant invar : model->getInvariants()) {
        //        if (invar->isUsedByConstraint()) {
        //            continue;
        //        }
        if (!invar->test()) {

            failed = true;
        }
    }
    return failed;
}

void LSSpace::setSolution(std::shared_ptr<State> st) {
    std::vector<int>& solution = st->getSolution();
    for (unsigned i = 0; i < model->getAllVariables().size(); i++) {
        model->getVariable(i)->setCurrentValue(solution.at(i));
    }
    //    std::cout << "need a remake" << std::endl;
    //    debug;


    //
    //    int change;
    //    //    for (unsigned i = 0; i < solution->size(); i++) {
    //    for (Variable* iv : model->getMask()) {
    //
    //        //        IntegerVariable* current = model->->getAllIntegerVariable(solution->at(i));
    //        //            for (unsigned j = 0; j < model->getUpdate(iv); j++) {
    //        assert(!iv->isDef() && !iv->isFixed());
    //        change = st->getSolution().at(iv->getID()) - iv->getCurrentValue();
    //        if (change != 0) {
    //            iv->setCurrentValue(st->getSolution().at(iv->getID()));
    //
    //            for (updateType invar : model->getUpdate(iv)) {
    //                //                updateType invariant = iv->getUpdateVector().at(j);
    //                invar->addChange(iv->getID(), change);
    //            }
    //
    //
    //            //            // Think this should be right, but expensive
    //            //            propagation_queue queue = model->getPropagationQueue(iv);
    //            //            for (updateType invar : queue) {
    //            //                //        for (unsigned i = 0; i < update->size(); i++) {
    //            //
    //            //                //            std::shared_ptr<Invariant> invar = update->at(i);
    //            //                //            Invariant* invar = st->getInvariants()->at(update->at(i));
    //            //                invar->calculateDeltaValue();
    //            //                invar->updateValue();
    //            //
    //            ////                if (invar->isUsedByConstraint()) {
    //            ////
    //            ////                    if (invar->getPriority() > 0) {
    //            ////                        std::shared_ptr<Constraint> cons = invar->getConstraint(); // model->getConstraintsWithPriority(invar->getPriority())->at(invar->getConstraintNumber());
    //            ////                        evaluation.at(cons->getPriority()) += cons->updateViolation();
    //            ////
    //            ////
    //            ////                    } else {
    //            ////                        evaluation.at(0) += invar->getDeltaValue();
    //            ////                    }
    //            ////                }
    //            //            }
    //
    //        }
    //
    //
    //    }
    //    //    std::cout << "value of 20340 " << solution.at(20340) << std::endl;
    //    //    for (unsigned i = 0; i < model->getInvariants()->size(); i++) {
    //    for (invariant invar : model->getInvariants()) {
    //        invar->calculateDeltaValue();
    //        //        bool legal = invar->calculateDeltaValue();
    //        //        if (!legal) {
    //        //            std::cout << "not a legal move while setting previous solution" << std::endl;
    //        //            exit(1);
    //        //        }
    //        change = invar->getDeltaValue();
    //        if (change != 0) {
    //            for (invariant inv : model->getUpdate(invar)) {
    //                assert(invar != inv);
    //                inv->addChange(invar->getVariableID(), change);
    //            }
    //        }
    //        invar->updateValue();
    //        if (invar->getVariableID() != -1) {
    //            Variable* iv = model->getVariable(invar->getVariableID());
    //            assert(iv->getOneway() == invar);
    //            if (invar->getCurrentValue() < iv->getLowerBound()) {
    //                std::cout << "FML!!!!!!!!!!!" << std::endl;
    //
    //                exit(1);
    //
    //            }
    //            assert(invar->getCurrentValue() >= iv->getLowerBound());
    //            iv->setCurrentValue(invar->getCurrentValue());
    //            //            assert(solution.at(iv->getID())== iv->getCurrentValue());
    //            //            if(solution.at(iv->getID())!= iv->getCurrentValue()){
    //            //                std::cout << solution.at(iv->getID()) << " vs " << iv->getCurrentValue() << " type " << invar->getType() << std::endl;
    //            //            }
    //            st->getSolution().at(iv->getID()) = iv->getCurrentValue();
    //
    //        }
    //        //        model->getInvariants()->at(i)->calculateDeltaValue();
    //        //        model->getInvariants()->at(i)->updateValue();
    //    }
    //
    //    std::cout << std::endl;
    //    // Setting constraints
    //    int violations = 0;
    //    for (unsigned i = 1; i < model->getConstraints().size(); i++) {
    //        std::shared_ptr<std::vector<std::shared_ptr < Constraint>>>prio = model->getConstraintsWithPriority(i);
    //        //        for (unsigned j = 0; j < prio->size(); j++) {
    //
    //        for (std::shared_ptr<Constraint> cons : *prio) {
    //            if (!cons->isOneway()) {
    //                cons->updateViolation();
    //                violations += cons->getViolation();
    //                //                int violation = cons->updateViolation();
    //                //                if(violation != 0){
    //                //                    std::cout << "Violation " << violation << std::endl;
    //                //                }
    //                //                cons->testCons();
    //                //                if (violation == 1) {
    //                //                    std::cout << "invar id " << cons->getInvariant()->getID() << " value " << cons->getInvariant()->getCurrentValue() << " rhs " << cons->getArgument(1) << std::endl;
    //                //                    violations += violation;
    //                //                }
    //            }
    //        }
    //    }
    //    st->getEvaluation().at(1) = violations;
    //    if (violations != 0) {
    //        std::cout << "Final solution not feasible? violations: " << violations << std::endl;
    //        sleep(2);
    //    }
    //    // setting objective value
    //    int value = 0;
    //    for (invariant invar : model->getObjectiveInvariant()) {
    //        value += invar->getCurrentValue();
    //
    //    }
    //    std::cout << std::endl;
    //    //    for (unsigned i = 0; i < getConstraintsWithPriority(0)->size(); i++) {
    //    //    for (std::shared_ptr<Constraint> obj : *model->getConstraints().at(0)) {
    //    //        value += obj->updateViolationDegree();
    //    //        //        violations += getConstraintsWithPriority(0)->at(i)->updateViolationDegree();
    //    //    }
    //    std::cout << "Final solution " << value << " (" << violations << ")" << std::endl;
    //    st->getEvaluation().at(0) = value;

}

//    FlipMove mv = FlipMove(st->getIntegerVariables()->at(0));
//    Move bestMove = mv;
//    std::pair<int, int> delta = calculateDeltaValueOfMove(mv);
//    int violationChange = delta.first;
//    int objectiveChange = delta.second;
//bool LSSpace::bestImprovement() {
//    int violationChange = 0;
//    int objectiveChange = 0;
//    int bestVariable = -1;
//    for (unsigned i = 0; i < st->getIntegerVariables()->size(); i++) {
//        IntegerVariable* variable = st->getIntegerVariables()->at(i);
//        std::pair<int, int> delta = calculateDeltaValueOfVariableChange(i, 1 - variable->getCurrentValue());
//        if (delta.first <= violationChange) {
//            if (delta.second <= objectiveChange) {
//                violationChange = delta.first;
//                objectiveChange = delta.second;
//                bestVariable = variable->getID();
//            }
//        }
//    }
//    if (bestVariable != -1 && objectiveChange !=0) {
//        commitDeltaOfVariable(bestVariable);
//    } else {
//        std::cout << "no improving move" << std::endl;
//        return false;
////        sleep(1);
//
//    }
//    std::cout << "changed variable " << bestVariable << std::endl;
//    std::cout << "violation change " << violationChange << " objective change " << objectiveChange << std::endl;
//
//    return true;
//}

//void LSSpace::commitDeltaOfVariable(int changedVariable) {
//    IntegerVariable* var = st->getIntegerVariables()->at(changedVariable);
//    vector<int>* update = var->getUpdateVector();
//    for (unsigned i = 0; i < update->size(); i++) {
//        Invariant* invar = st->getInvariants()->at(update->at(i));
//        invar->updateValue();
//        if (invar->getUsedInConstraint() != -1) {
//            st->getConstraints()->at(invar->getUsedInConstraint())->updateViolation();
//        }
//
//        if (invar->getUsedInObjective() != -1) {
//            st->getObjectives()->at(invar->getUsedInObjective())->updateViolationDegree();
//        }
//    }
//    var->setCurrentValue(1 - var->getCurrentValue());
//
//}

//std::pair<int, int> LSSpace::calculateDeltaValueOfVariableChange(int variableNumber, int newValue) {
//
//    IntegerVariable* variable = st->getIntegerVariables()->at(variableNumber);
//    int oldValue = variable->getCurrentValue();
//    vector<int>* updateVector = variable->getUpdateVector();
//    int violationChange = 0;
//    int objectiveChange = 0;
//    //    std::cout << "Variable " << variableNumber << std::endl;
//    for (unsigned i = 0; i < updateVector->size(); i++) {
//        Invariant* invar = st->getInvariants()->at(updateVector->at(i));
//        invar->addChange(variableNumber, newValue - oldValue);
//        invar->calculateDeltaValue();
//        if (invar->getUsedInConstraint() != -1) {
//            //            std::cout << "Used in Constraint " << std::endl;
//            violationChange += st->getConstraints()->at(invar->getUsedInConstraint())->setDeltaViolation();
//        }
//        if (invar->getUsedInObjective() != -1) {
//            //            std::cout << "Used in objective " << std::endl;
//            objectiveChange += st->getObjectives()->at(invar->getUsedInObjective())->setDeltaViolationDegree();
//        }
//    }
//    //    std::cout << std::endl;
//    std::pair<int, int> change(violationChange, objectiveChange);
//    return change;
//    //    return std::pair<int,int> change = new pair<int,int>(violationChange,objectiveChange);
//    //        variable->setCurrentValue(newValue);
//}

//void LSSpace::initializeInvariants(shared_ptr<Model> st) {
//    st->initializeInvariants();
//}
//
//void LSSpace::initializeConstraints(shared_ptr<Model> st) {
//    st->initializeConstraints();
//}
//
//void LSSpace::initializeObjective(shared_ptr<Model> st) {
//    initialValue = st->initializeObjective();
//}
//int LSSpace::getObjectiveValue(){
//    return st->getObjectiveValue();
//}

//void LSSpace::printCurrent() {
//
//    if (st->getIntegerVariables()->size() > 0) {
//        std::cout << "Integer Variables:" << std::endl;
//    }
//
//    for (unsigned i = 0; i < st->getIntegerVariables()->size(); i++) {
//        std::cout << st->getIntegerVariables()->at(i)->getCurrentValue() << " ";
//        //        std::cout << IntVarVector[i].VariablePointer << " ";
//
//    }
//    std::cout << std::endl;
//    if (BoolVarVector.size() > 0) {
//        std::cout << "Boolean Variables:" << std::endl;
//    }
//    for (unsigned i = 0; i < BoolVarVector.size(); i++) {
////        std::cout << BoolVarVector[i].getCurrentVal() << " ";
//    }
//    std::cout << std::endl;
//
//
//}

//LSSpace::~LSSpace() {
//}
//template<typename MatrixType> struct matrix_traits
//{
//  typedef typename MatrixType::index_type index_type;
//  typedef typename MatrixType::value_typ value_type;
//  static index_type min_row(MatrixType const& A)
//  { return A.min_row(); }
//  static index_type max_row(MatrixType const& A)
//  { return A.max_row(); }
//  static index_type min_column(MatrixType const& A)
//  { return A.min_column(); }
//  static index_type max_column(MatrixType const& A)
//  { return A.max_column(); }
//  static value_type& element(MatrixType& A, index_type i, index_type k)
//  { return A(i,k); }
//  static value_type element(MatrixType const& A, index_type i, index_type k)
//  { return A(i,k); }
//};
// 
//// specialization of the matrix traits for built-in two-dimensional
//// arrays
//template<typename T, std::size_t rows, std::size_t columns>
// struct matrix_traits<T[rows][columns]>
//{
//  typedef std::size_t index_type;
//  typedef T value_type;
//  static index_type min_row(T const (&)[rows][columns])
//  { return 0; }
//  static index_type max_row(T const (&)[rows][columns])
//  { return rows-1; }
//  static index_type min_column(T const (&)[rows][columns])
//  { return 0; }
//  static index_type max_column(T const (&)[rows][columns])
//  { return columns-1; }
//  static value_type& element(T (&A)[rows][columns],
//                             index_type i, index_type k)
//  { return A[i][k]; }
//  static value_type element(T const (&A)[rows][columns],
//                            index_type i, index_type k)
//  { return A[i][k]; }
//};
// 
//// Swap rows i and k of a matrix A
//// Note that due to the reference, both dimensions are preserved for
//// built-in arrays
//template<typename MatrixType>
// void swap_rows(MatrixType& A,
//                 typename matrix_traits<MatrixType>::index_type i,
//                 typename matrix_traits<MatrixType>::index_type k)
//{
//  matrix_traits<MatrixType> mt;
//  typedef typename matrix_traits<MatrixType>::index_type index_type;
// 
//  // check indices
//  assert(mt.min_row(A) <= i);
//  assert(i <= mt.max_row(A));
// 
//  assert(mt.min_row(A) <= k);
//  assert(k <= mt.max_row(A));
// 
//  for (index_type col = mt.min_column(A); col <= mt.max_column(A); ++col)
//    std::swap(mt.element(A, i, col), mt.element(A, k, col));
//}
// 
//// divide row i of matrix A by v
//template<typename MatrixType>
// void divide_row(MatrixType& A,
//                  typename matrix_traits<MatrixType>::index_type i,
//                  typename matrix_traits<MatrixType>::value_type v)
//{
//  matrix_traits<MatrixType> mt;
//  typedef typename matrix_traits<MatrixType>::index_type index_type;
// 
//  assert(mt.min_row(A) <= i);
//  assert(i <= mt.max_row(A));
// 
//  assert(v != 0);
// 
//  for (index_type col = mt.min_column(A); col <= mt.max_column(A); ++col)
//    mt.element(A, i, col) /= v;
//}
// 
//// in matrix A, add v times row k to row i
//template<typename MatrixType>
// void add_multiple_row(MatrixType& A,
//                  typename matrix_traits<MatrixType>::index_type i,
//                  typename matrix_traits<MatrixType>::index_type k,
//                  typename matrix_traits<MatrixType>::value_type v)
//{
//  matrix_traits<MatrixType> mt;
//  typedef typename matrix_traits<MatrixType>::index_type index_type;
// 
//  assert(mt.min_row(A) <= i);
//  assert(i <= mt.max_row(A));
// 
//  assert(mt.min_row(A) <= k);
//  assert(k <= mt.max_row(A));
// 
//  for (index_type col = mt.min_column(A); col <= mt.max_column(A); ++col)
//    mt.element(A, i, col) += v * mt.element(A, k, col);
//}
// 
//// convert A to reduced row echelon form
//template<typename MatrixType>
// void to_reduced_row_echelon_form(MatrixType& A)
//{
//  matrix_traits<MatrixType> mt;
//  typedef typename matrix_traits<MatrixType>::index_type index_type;
// 
//  index_type lead = mt.min_row(A);
// 
//  for (index_type row = mt.min_row(A); row <= mt.max_row(A); ++row)
//  {
//    if (lead > mt.max_column(A))
//      return;
//    index_type i = row;
//    while (mt.element(A, i, lead) == 0)
//    {
//      ++i;
//      if (i > mt.max_row(A))
//      {
//        i = row;
//        ++lead;
//        if (lead > mt.max_column(A))
//          return;
//      }
//    }
//    swap_rows(A, i, row);
//    divide_row(A, row, mt.element(A, row, lead));
//    for (i = mt.min_row(A); i <= mt.max_row(A); ++i)
//    {
//      if (i != row)
//        add_multiple_row(A, i, row, -mt.element(A, i, lead));
//    }
//  }
//}

//// test code
//#include <iostream>
// 
//int main()
//{
//  double M[3][4] = { {  1, 2, -1,  -4 },
//                     {  2, 3, -1, -11 },
//                     { -2, 0, -3,  22 } };
// 
//  to_reduced_row_echelon_form(M);
//  for (int i = 0; i < 3; ++i)
//  {
//    for (int j = 0; j < 4; ++j)
//      std::cout << M[i][j] << '\t';
//    std::cout << "\n";
//  }
// 
//  return EXIT_SUCCESS;
//}
