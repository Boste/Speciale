//#include <c++/4.8/bits/stl_vector.h>

#include "LocalSearchEngine.hpp"
#include "BP_Data.hpp"

//#include "RowEcholonTransformer.hpp"


//using namespace Gecode;

LocalSearchEngine::LocalSearchEngine(std::shared_ptr<Storage> model) {
    this->model = model;
    this->DDG = model->getDDG();
    //        std::cout << "constructed" << std::endl;

}

void LocalSearchEngine::createDDG(bool all) {

    DDG->addVariables(model->getAllVariables());
    
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

    }
    //    std::cout << "totally fixed " << totallyfixed << std::endl;

    std::cout << "number of invariants " << numberOfOneway << " out of " << func.size()
            << " (" << func.size() - totallyfixed << ") " << std::endl;
    DDG->cleanUpGraph(model->getNonFixedVariables());

    DDG->checkForCycles(model->getInvariants());

}

void LocalSearchEngine::initializeLS() {



    //    if (feasible) {
    model->cleanUp();
    std::cout << "##oneway " << model->getInvariants().size() << std::endl;
    //    model->out->oneway = model->getInvariants().size();
    oneway = model->getInvariants().size();


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
    //    if (!model->out->feasible) {
    //        if (model->getViolatedConstraints().size() == 0) {
    //            model->out->feasible = true;
    //            model->out->feasibleTime = model->out->solTime;
    //            model->out->feasibleVal = model->getEvaluationInvariantNr(0)->getCurrentValue();
    //
    //        }
    //    }

    //    if (model->getViolatedConstraints().size() != 0) {
    //        //        model->getViolatedConstraints().size();
    //        std::cout << "Not feasible " << std::endl;
    //        //        exit(1);
    //    }


    //    DDG->checkForCycles(model->getInvariants());
    DDG->checkForCycles(model->getInvariants());
    std::sort(model->getInvariants().begin(), model->getInvariants().end(), compare_invariant());

    std::cout << "Total time used so far " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << std::endl;
    std::cout << "Create propagator queues" << std::endl;
    //    DDG->createPropagationQueue(model->getAllVariables(), model->getInvariants());
    DDG->createPropagationQueue(model->getAllVariables());
    //    model->getDDG()->printSizes();
    std::cout << "Total time used so far " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << std::endl;
    model->initialize();
    if (model->getViolatedConstraints().size() != 0) {
        std::cout << "Not feasible " << std::endl;
        //        exit(1);
    }
    currentState = std::make_shared<State>(model);
    bestState = std::make_shared<State>(model);
    for (invariant inv : model->getInvariants()) {
        //        std::cout << "vp " << inv->getVariablePointers().size() << " coef " << inv->getCoefficients().size()<< " ip " << inv->getInvariantPointers().size() << std::endl;
        for (auto it = inv->getVariablePointers().begin(); it != inv->getVariablePointers().end(); ++it) {

            if ((*it)->isFixed()) {
                inv->addToFixedVariables((*it));
                inv->getVariablePointers().erase(it);

                --it;
            }
        }

    }

}

std::pair<int,int> LocalSearchEngine::optimizeSolution(int time, int test) {






    std::cout << "Number of evaluation variables " << model->getEvaluationVariables().size() << std::endl;
    //    std::cout << "counter " << counter << " variables " << model->getNonFixedVariables().size() << std::endl;
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
        assert(!var->isFixed() && !var->isDef());
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
    double usedTime = (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC;



    //    if (!currentState->isFeasible()) {
    FlipNeighborhood* FN = new FlipNeighborhood(model, currentState);
    FirstImprovement FI(FN);
    while (FI.Start()) {

        usedTime = (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC;
        if (usedTime >= time) {
            break;
        }
    }

    delete FN;

    std::cout << "Suggestion of all variable flips has been made " << std::endl;
    if (currentState->compare(bestState)) {
        currentState->setViolation();
        bestState->copy(currentState);
        std::cout << "# value: ";
        for (int eval : bestState->getEvaluation()) {
            std::cout << eval << " ";
        }
        std::cout << " # iter " << 0 << " # used " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << " # method TS FI" << std::endl;
    }

    //    }
    //    double timelimit = (double) time - (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC;
    double timelimit = (double) time;
    //    std::cout << "time left for local search " << timelimit << std::endl;
    std::cout << "## LSLimitLeft " << timelimit << std::endl;

    auto start = Clock::globalClock;
    //    std::clock_t start = std::clock();

    //    std::cout << "Number of evaluation variables " << model->getEvaluationVariables().size() << std::endl;
    //    debug;
    //    std::cout << "##beforels " << model->out->name << " " << currentState->getEvaluation().at(0) << " " << model->getViolatedConstraints().size() << " " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << " " << std::endl;
    //     << model->out->relaxp << std::endl;
    //######################################################
    // Test 1 TS conflict only + TS Restricted neighborhood
    //######################################################
    if (test == 1) {
        std::cout << "# test 1" << std::endl;

        ConflictOnlyNE* CON = new ConflictOnlyNE(model, currentState);
        TabuSearch TSCON(CON);
        //        EvalFlipNE* EFN = new EvalFlipNE(model, currentState);
        Neighborhood* NE;
        if (model->getMask().size() > 5000) {
            NE = new RestrictedFlipNE(model, currentState);
        } else {
            NE = new FlipNeighborhood(model, currentState);
        }
        //        std::cout << "\t evaluation \t violation 1 \t iterations \t time \t method \n";




        //        RestrictedFlipNE* RFN = new RestrictedFlipNE(model, currentState);
        TabuSearch TSRFN(NE);
        while (usedTime < timelimit) {


            //            while (!currentState->isFeasible() && us  edTime < timelimit) {
            if (!currentState->isFeasible()) {
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


            } else {
                //            unsigned counter = 0;
                //            while (counter < randomMoves&& usedTime < timelimit) {
                //            while (currentState->isFeasible() && usedTime < timelimit) {
                //            if (currentState->isFeasible()) {
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

                    //                    for (int eval : bestState->getEvaluation()) {
                    //                        std::cout << "\t" << eval;
                    //                    }
                    //                    std::cout << "\t" << iterations << "\t" << usedTime << "\t" << "TS-RFN" << std::endl;



                    usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
                    std::cout << "# value: ";
                    for (int eval : bestState->getEvaluation()) {
                        std::cout << eval << " ";
                    }
                    std::cout << " # iter " << iterations << " # used " << usedTime << " # method TS RFN" << std::endl;
//                    for(Variable* var : model->getMask()){
//                        std::cout << var->getCurrentValue() << " ";
//                    }
//                    std::cout << std::endl;
//                    for(int val : bestState->getSolution()){
//                        std::cout << val << " ";
//                    }
//                    std::cout << std::endl;
                }
                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;


            }
            //                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;

        }
        delete CON;

        delete NE;
    }
    //######################################################
    // Test 2 RW 2 % + FI
    //######################################################



    if (test == 2) {
        std::cout << "# test 2" << std::endl;
        //        unsigned twoPercent = model->getMask().size() / 50;
        unsigned onePercent = model->getMask().size() / 100;
        unsigned randomMoves = std::min(onePercent, (unsigned) 10);
        //        unsigned randomMoves = std::min(twoPercent, (unsigned) 10);
        //        unsigned randomMoves = twoPercent;
        std::cout << "Number of randomMoves " << randomMoves << std::endl;
        //        std::cout << "\t value \t evaluation \t priority 1 \t iterations \t time \t method \n";

        FlipNeighborhood* FN = new FlipNeighborhood(model, currentState);
        RandomWalk RW(FN, randomMoves);
        FirstImprovement FIFN(FN);
        bool imp = true;
        //                unsigned loopCounter =0;
        while (usedTime < timelimit) {
            imp = true;
            //                        loopCounter++;
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

            //            if(loopCounter > 20){
            //                std::random_shuffle(model->getMask().begin(),model->getMask().end());
            //                loopCounter = 0;
            //            }
            //            std::cout << loopCounter << std::endl;
        }
        delete FN;

    }

    //######################################################
    // Test 3 MCH + TS Restricted neighborhood
    //######################################################
    if (test == 3) {
        //        FlipNeighborhood* FN = new FlipNeighborhood(model, currentState);

        std::cout << "# test 3" << std::endl;
        //        std::cout << "\t value \t evaluation \t priority 1 \t iterations \t time \t method \n";

        RandomConflictConNE* RCC = new RandomConflictConNE(model, currentState);
        BestImprovement BIRCC(RCC);
        bool alwaysCommit = true;
        Neighborhood* NE;
        if (model->getMask().size() > 5000) {
            NE = new RestrictedFlipNE(model, currentState);
        } else {
            NE = new FlipNeighborhood(model, currentState);
        } //        TabuSearch TSRFN(model, FN);
        TabuSearch TSRFN(NE);
        while (usedTime < timelimit) {
            //                        while (!currentState->isFeasible() && usedTime < timelimit) {
            if (!currentState->isFeasible()) {
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



            } else {
                //            std::cout << usedTime << std::endl;
                //            while (usedTime < timelimit) {
                //                        while (currentState->isFeasible() && usedTime < timelimit) {
                //            if (currentState->isFeasible()) {
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
        delete NE;
        delete RCC;
    }
    //    if (bestState->isFeasible()) {
    //        model->out->addToTable1(std::to_string(model->out->feasibleVal));
    //        model->out->addToTable1(std::to_string(iterations));
    //    } else {
    //        assert(!model->out->feasible);
    //        model->out->addToTable1("NA");
    //        model->out->addToTable1("NA");
    //        model->out->addToTable1(std::to_string(iterations));
    //    }


    //    std::cout << "name, oneway, iterations, eval[0], eval[1]" << std::endl;
    //    std::cout << "##invar " << model->out->name << " " << oneway << " ";
    std::cout << iterations << " ";
    std::cout << bestState->getEvaluation().at(0) << " " << bestState->getViolations() << std::endl;
    
   
    
    //    
    //    std::cout << "Number of Moves delta calculated " << NE->testCounter << std::endl;
    //    std::cout << "number of loops " << loopCounter << std::endl;
    //    std::cout << "## LSTimeUsed " << usedTime << std::endl;

    //    std::cout << "## quality ";
    //    for (int eval : bestState->getEvaluation()) {
    //
    //        std::cout << eval << " ";
    //        model->out->addToTable1(std::to_string(eval));
    //    }
    //    model->out->addToTable1(std::to_string(bestState->getViolations()));
    //    std::cout << std::endl;
    //    std::cout << "## violations " << bestState->getViolations() << std::endl;
    //    std::cout << "## iterations " << iterations << std::endl;
    setSolution(bestState);
    std::pair<int,int> sol(bestState->getEvaluation().at(0), bestState->getViolations());
    return sol;
//    for (Variable* var : model->getAllVariables()) {
//        if (var->isDef()) {
//            invariant inv = var->getOneway();
//
//        }
//    }
    //    std::cout << "##table1 " << model->out->getToTable1() << std::endl;
    //    std::cout << "instance name, variables, constraints, gecode percentage of constraints,"
    //            << " time to initial solution, time to feasible solution, value of first feasible,"
    //            << " #iterations, final solution val, number of violations" << std::endl;

}

bool LocalSearchEngine::testInvariant() {
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

void LocalSearchEngine::setSolution(std::shared_ptr<State> st) {
  
    std::vector<int>& solution = st->getSolution();
    for (unsigned i = 0; i < model->getMask().size(); i++) {
        Variable* var = model->getMaskAt(i);
        if (solution.at(i) != var->getCurrentValue()) {

            for (invariant inv : model->getUpdate(var)) {
                inv->proposeChange(var->getID(), solution.at(i) - var->getCurrentValue());
            }
        }

        var->setCurrentValue(solution.at(i));

        // Send change and create big propqueue. Run that
    }
     
    
    InvariantContainer invs = model->getInvariants();
    std::sort(invs.begin(), invs.end(), compare_invariant());
    bool legal = true;
    for (updateType invar : invs) {
        legal = invar->calculateDelta();
        //        std::cout << "calcul " << invar->getDeltaValue() << " ";

        if (!legal) {
            break;
        }
        if (invar->getDeltaValue() != 0) {
            for (updateType inv : model->getUpdate(invar)) {
                inv->proposeChange(invar->getVariableID(), invar->getDeltaValue());
            }
        }
    }
    assert(legal);
    for (updateType invar : invs) {

        invar->updateValue();
        if (invar->representConstraint()) {
            if (invar->getCurrentValue() == 0) {
                if (invar->getInvariantPointers().back()->inViolatedConstraints()) {
                    //                    std::unordered_map<unsigned, invariant>& vioCons = model->getViolatedConstraints();
                    model->removeViolatedConstraint(invar->getInvariantPointers().back());
                }
            } else {
                if (!invar->getInvariantPointers().back()->inViolatedConstraints()) {
                    //                    std::unordered_map<unsigned, invariant>& vioCons = model->getViolatedConstraints();
                    model->addViolatedConstraint(invar->getInvariantPointers().back());
                }
            }
        }
    }

    std::cout << std::endl;
    for(invariant inv : model->getInvariants()){
        inv->test();
    }
    for(Variable* var : model->getAllVariables()){
       if(var->isDef()){
           var->setCurrentValue(var->getOneway()->getCurrentValue());
       } 
    }
    
}



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

//    for (Variable* var : model->getNonFixedVariables()) {
//        if (var->isDef()) {
//            Constraint * con = var->getDefinedByCon();
//            invariant inv = var->getOneway();
//            assert(con->getVariables().size() - 1 == inv->getVariablePointers().size() + inv->getInvariantPointers().size() + inv->getFixedVariables().size());
//            for (Variable* var : inv->getVariablePointers()) {
//                if (var->isDef()) {
//                    std::cout << "Defined but in variable pointers" << std::endl;
//                    debug;
//                }
//                if (var->isFixed()) {
//                    std::cout << "Fixed but in variable pointers" << std::endl;
//                    debug;
//                }
//
//                bool found = false;
//                for (invariant invar : DDG->getVariableUpdate(var->getID())) {
//                    if (invar == inv) {
//                        found = true;
//                        break;
//                    }
//                }
//                if (!found) {
//                    debug;
//                    exit(1);
//                }
//                //
//            }
//            for (invariant invar : inv->getInvariantPointers()) {
//                bool found = false;
//                for (invariant inva : DDG->getInvariantUpdate(invar->getID())) {
//                    if (inva == inv) {
//                        found = true;
//                        break;
//                    }
//                }
//
//
//                if (!found) {
//                    debug;
//                    exit(1);
//                }
//            }
//        }
//    }
//
//    int counter = 0;
//
//    for (Variable* var : model->getNonFixedVariables()) {
//        //        std::cout << "Var id " << var->getID() << std::endl;
//        if (var->isDef()) {
//
//            //            debug;
//            if (var->isInObjective()) {
//                if (var->numberOfConstraints() != DDG->getInvariantUpdate(var->getOneway()->getID()).size()) {
//                    std::cout << var->numberOfConstraints() + 1 << " " << DDG->getInvariantUpdate(var->getOneway()->getID()).size() << std::endl;
//
//                } else if (var->numberOfConstraints() != DDG->getInvariantUpdate(var->getOneway()->getID()).size()) {
//                    std::cout << var->numberOfConstraints() << " " << DDG->getInvariantUpdate(var->getOneway()->getID()).size() << std::endl;
//                    counter++;
//                }
//            }
//        } else {
//            if (var->isInObjective()) {
//                if (var->numberOfConstraints() + 1 != DDG->getVariableUpdate(var->getID()).size()) {
//                    std::cout << var->numberOfConstraints() + 1 << " " << DDG->getVariableUpdate(var->getID()).size() << std::endl;
//                }
//            } else {
//
//                if (var->numberOfConstraints() != DDG->getVariableUpdate(var->getID()).size()) {
//                    std::cout << var->numberOfConstraints() << " " << DDG->getVariableUpdate(var->getID()).size() << std::endl;
//                    counter++;
//                }
//            }
//        }
//        //        assert(var->numberOfConstraints() == DDG->getVariableUpdate(var->getID()).size());
//    }

//    
