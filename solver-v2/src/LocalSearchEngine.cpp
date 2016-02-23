
#include "LocalSearchEngine.hpp"

LocalSearchEngine::LocalSearchEngine(std::shared_ptr<Storage> model) {
    this->model = model;
    this->DDG = model->getDDG();

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
    for (constraint con : func) {

        if (con->canBeMadeOneway()) {
            invariant invar = con->makeOneway();
            model->addInvariant(invar);
            DDG->addInvariant(invar);

            numberOfOneway++;

        }

    }

    std::cout << "number of invariants " << numberOfOneway << " out of " << func.size()
            << " (" << func.size() - totallyfixed << ") " << std::endl;
    DDG->cleanUpGraph(model->getNonFixedVariables());

    DDG->checkForCycles(model->getInvariants());

}

void LocalSearchEngine::initializeLS() {



    model->cleanUp();
    oneway = model->getInvariants().size();


    for (unsigned i = 0; i < model->getConstraints().size(); i++) {

        std::vector<invariant> collector;
        int value = 0;
        for (constraint cons : *model->getConstraintsWithPriority(i)) {
            if (!cons->isOneway()) {

                InvariantContainer invars = cons->createInvariants();
                for (unsigned i = 0; i < invars.size(); i++) {
                    invariant inv = invars.at(i);
                    model->addInvariant(inv);

                    DDG->addInvariant(inv, inv->getVariablePointers(), inv->getInvariantPointers());

                }
                collector.push_back(invars.back());
                value += invars.back()->getCurrentValue();
                if (invars.back()->representConstraint()) {
                    if (invars.back()->getCurrentValue() > 0) {
                        model->addViolatedConstraint(invars.back()->getInvariantPointers().back());
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
    DDG->checkForCycles(model->getInvariants());
    std::sort(model->getInvariants().begin(), model->getInvariants().end(), compare_invariant());

    std::cout << "Total time used so far " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << std::endl;
    std::cout << "Create propagator queues" << std::endl;
    DDG->createPropagationQueue(model->getAllVariables());
    std::cout << "Total time used so far " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << std::endl;
    model->initialize();
    if (model->getViolatedConstraints().size() != 0) {
        std::cout << "Not feasible " << std::endl;
    }
    currentState = std::make_shared<State>(model);
    bestState = std::make_shared<State>(model);
    for (invariant inv : model->getInvariants()) {
        for (auto it = inv->getVariablePointers().begin(); it != inv->getVariablePointers().end(); ++it) {

            if ((*it)->isFixed()) {
                inv->addToFixedVariables((*it));
                inv->getVariablePointers().erase(it);

                --it;
            }
        }

    }

}

void LocalSearchEngine::optimizeSolution(int time, int test) {






    std::cout << "Number of evaluation variables " << model->getEvaluationVariables().size() << std::endl;
    unsigned tabulistsize = 0;
    for (Variable* var : model->getMask()) {
        assert(!var->isFixed() && !var->isDef());
        if (var->getID() > tabulistsize) {
            tabulistsize = var->getID();
        }
    }
    tabulistsize++;
    std::vector<int> tabulist(tabulistsize, -tabulistsize);
    std::cout << "Number of variables used in local search " << model->getMask().size() << std::endl;
    double usedTime = (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC;

    FlipNeighborhood* FN = new FlipNeighborhood(model, currentState);
    FirstImprovement FI(FN);
    while (FI.Start()) {
        usedTime = (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC;
        if (usedTime >= time) {
            break;
        }
    }
    std::vector<double> atTime;
    std::vector<int> obj;
    std::vector<int> viol;
    std::vector<int> violDeg;
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
    double timelimit = (double) time;

    auto start = Clock::globalClock;
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
        TabuSearch TSRFN(NE);
        while (usedTime < timelimit) {


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

                }
                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;


            } else {
                TSRFN.Start(iterations, bestState, currentState, tabulist);

                iterations++;
                if (currentState->compare(bestState)) {
                    currentState->setViolation();
                    bestState->copy(currentState);
                    usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;

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
        delete CON;

        delete NE;
    }
    //######################################################
    // Test 2 RW 2 % + FI
    //######################################################



    if (test == 2) {
        std::cout << "# test 2" << std::endl;
        unsigned onePercent = model->getMask().size() / 100;
        unsigned randomMoves = std::min(onePercent, (unsigned) 10);
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

                }
                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;

            }

            RW.Start();
            iterations += randomMoves;
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
            usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
        }
        delete FN;

    }

    //######################################################
    // Test 3 MCH + TS Restricted neighborhood
    //######################################################
    if (test == 3) {

        std::cout << "# test 3" << std::endl;

        RandomConflictConNE* RCC = new RandomConflictConNE(model, currentState);
        BestImprovement BIRCC(RCC);
        bool alwaysCommit = true;
        Neighborhood* NE;
        if (model->getMask().size() > 5000) {
            NE = new RestrictedFlipNE(model, currentState);
        } else {
            NE = new FlipNeighborhood(model, currentState);
        }
        TabuSearch TSRFN(NE);
        while (usedTime < timelimit) {
            if (!currentState->isFeasible()) {
                BIRCC.Start(alwaysCommit);
                iterations++;
                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
                if (currentState->compare(bestState)) {
                    currentState->setViolation();
                    bestState->copy(currentState);
                    atTime.push_back(usedTime);
                    obj.push_back(bestState->getEvaluation().at(0));
                    violDeg.push_back(bestState->getEvaluation().at(1));
                    viol.push_back(bestState->getViolations());
                    std::cout << "# value: ";
                    for (int eval : bestState->getEvaluation()) {
                        std::cout << eval << " ";
                    }

                    std::cout << " # iter " << iterations << " # used " << usedTime << " # method BI RCC" << std::endl;

                }

            } else {
                TSRFN.Start(iterations, bestState, currentState, tabulist);
                iterations++;
                //                debug;
                usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;

                if (currentState->compare(bestState)) {
                    currentState->setViolation();
                    bestState->copy(currentState);
                    atTime.push_back(usedTime);
                    obj.push_back(bestState->getEvaluation().at(0));
                    violDeg.push_back(bestState->getEvaluation().at(1));
                    viol.push_back(bestState->getViolations());
                    std::cout << "# value: ";
                    for (int eval : bestState->getEvaluation()) {
                        std::cout << eval << " ";
                    }
                    std::cout << " # iter " << iterations << " # used " << usedTime << " # method TS RFN" << std::endl;
                }


            }
        }
        delete NE;
        delete RCC;
    }

    std::cout << iterations << " ";
    std::cout << bestState->getEvaluation().at(0) << " " << bestState->getViolations() << std::endl;
    setSolution(bestState);
    for (unsigned i = 0; i < atTime.size(); i++) {
        std::cout << "#conv ";
        std::cout << atTime.at(i) << " " << obj.at(i) << " " << viol.at(i) << " " << violDeg.at(i) << std::endl;
    }


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

    }


    InvariantContainer invs = model->getInvariants();
    std::sort(invs.begin(), invs.end(), compare_invariant());
    bool legal = true;
    for (updateType invar : invs) {
        legal = invar->calculateDelta();

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
                    model->removeViolatedConstraint(invar->getInvariantPointers().back());
                }
            } else {
                if (!invar->getInvariantPointers().back()->inViolatedConstraints()) {
                    model->addViolatedConstraint(invar->getInvariantPointers().back());
                }
            }
        }
    }

    std::cout << std::endl;
    for (invariant inv : model->getInvariants()) {
        inv->test();
    }
    for (Variable* var : model->getAllVariables()) {
        if (var->isDef()) {
            var->setCurrentValue(var->getOneway()->getCurrentValue());
        }
    }

}


