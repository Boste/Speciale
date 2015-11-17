//#include <c++/4.8/bits/stl_vector.h>

#include "LSSpace.hpp"
//#include "RowEcholonTransformer.hpp"


//using namespace Gecode;

LSSpace::LSSpace(std::shared_ptr<Model> model) {
    this->model = model;
    this->DDG = model->getDDG();
    //        std::cout << "constructed" << std::endl;

}

void LSSpace::initializeLS() {
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
    defining = std::vector<unsigned>(model->getAllVariables().size());

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

    // ################################################################################################
    // Make oneway for integer vars
    // ################################################################################################
    std::vector<IntegerVariable*> queue = model->getIntegerVariables();
    std::sort(queue.begin(), queue.end(), IntegerVariable::SortGreater());

    bool change = true;
    while (queue.size() != 0 && change) {
        //        std::cout << queue.size() << std::endl;
        //        debug;
        //            iter++;
        //            std::cout << "iterations in while " << iter << std::endl;
        //            std::cout << " queue size " << queue.size() << std::endl;
        change = false;
        for (auto it = queue.begin(); it != queue.end(); ++it) {
            IntegerVariable* iv = *it;
            if (iv->isFixed()) {
                continue;
            }
            VariableInConstraints constraints = iv->usedInConstraints();
            for (constraint cons : constraints) {
                //                iv->usedInConstraints()
                if (intVarCanBeMadeOneway(iv, cons)) {
                    makeIntVarOneway(iv, cons);
                    //                    numberOfOneway++;
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

    //    std::cout << queue.size() << std::endl;
    //    debug;


    //    std::vector<IntegerVariable*>& queue = model->getPriorityVectorNr(0);
    //    std::sort(queue.begin(), queue.end(), IntegerVariable::SortGreater());
    /// Try to see if it is to expensive to prevent cycles. 
    std::vector<constraint> func = model->getFunctionalConstraints();
    std::sort(func.begin(), func.end(), Constraint::Sortlower());
    unsigned totallyfixed = 0;
    int numberOfOneway = 0;

    //    unsigned lastsize = 0;
    for (constraint con : func) {
        if (canBeMadeOneway(con)) {
            numberOfOneway++;

        }
        //        }

        if (!con->isOneway()) {
            bool gotNonfixed = false;
            for (IntegerVariable* iv : con->getVariables()) {
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

    std::cout << "number of invariants " << numberOfOneway << " out of " << model->getFunctionalConstraints().size()
            << " (" << model->getFunctionalConstraints().size() - totallyfixed << ") " << std::endl;
    bool allIntegerVariablesDefined = true;
    for (IntegerVariable* iv : model->getIntegerVariables()) {
        if (!iv->isDef()) {
            allIntegerVariablesDefined = false;
            break;
        }
    }
    if (allIntegerVariablesDefined) {
        std::cout << "All integer variables could be defined by a oneway" << std::endl;
    } else {
        std::cout << "All integer variables could NOT be defined by a oneway" << std::endl;
    }
    //    int sum = 0;
    //    int number = 0;
    //    for (int i : defining) {
    //        assert(i>-1);
    //        sum += i;
    //        if (i > 0) {
    //            number++;
    //        }
    //    }
    //
    //    std::cout << "sum " << sum << std::endl;
    //    std::cout << "Number " << number << std::endl;
    model->getDDG()->cleanUpGraph(model->getAllVariables());
    model->getDDG()->checkForCycles(model->getInvariants());
    model->cleanUp();
    for (constraint cons : *model->getObjectives()) {
        assert(!cons->isOneway());
    }
    for (unsigned i = 0; i < model->getConstraints().size(); i++) {
        for (constraint cons : *model->getConstraintsWithPriority(i)) {
            if (!cons->isOneway()) {
                int value = 0;
                std::unordered_map<int, coefType>& coef = cons->getCoefficients();
                //                std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(coef, model->getDDG());
                std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(cons->getVariables(), coef);
                //                    sumInvariant->invariantID = model->getInvariants().size();
                variableContainer variables;
                InvariantContainer invars;
                for (IntegerVariable* iv : cons->getVariables()) {
                    unsigned id = iv->getID();
                    if (!iv->isDef()) {

                        value += coef.at(id) * iv->getCurrentValue();
                        variables.push_back(iv);
                    } else {
                        value += coef.at(id) * iv->getOneway()->getCurrentValue();
                        invars.push_back(iv->getOneway());
                    }
                }
                sumInvariant->setValue(value);
                model->addInvariant(sumInvariant);
                //                model->addInvariantToDDG(sumInvariant, variables, invars);
                DDG->addInvariant(sumInvariant, variables, invars);
                //                model->addInvariantToDDG(sumInvariant, variables);
                cons->setInvariant(sumInvariant);
                if (cons->getPriority() == OBJ) {
                    model->addToObjectiveInvariant(sumInvariant);
                    sumInvariant->setObjective();
                } else {
                    sumInvariant->setUsedByConstraint(cons, cons->getPriority());

                }

            }
        }

    }
    std::cout << "Total time used so far " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << std::endl;
    std::cout << "Create propagator queues" << std::endl;
    DDG->createPropagationQueue(model->getAllVariables(), model->getInvariants());
    //    model->getDDG()->printSizes();
    std::cout << "Total time used so far " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << std::endl;


    model->initialize();
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
    bool failed = false;
    for (unsigned i = 1; i < model->getConstraints().size(); i++) {
        std::shared_ptr<std::vector<std::shared_ptr < Constraint>>>prio = model->getConstraintsWithPriority(i);
        //        for (unsigned j = 0; j < prio->size(); j++) {

        for (std::shared_ptr<Constraint> cons : *prio) {
            if (!cons->isOneway()) {
                if (!cons->testCons()) {
                    failed = true;
                }
            }
        }
    }
    if (failed) {
        std::cout << "constraints are violated? " << std::endl;
        exit(1);
    }
}

// Not taking into account if the coefficient in objective function is negative

//bool LSSpace::canBeMadeOneway(IntegerVariable* iv, constraint cons) {

bool LSSpace::canBeMadeOneway(constraint cons) {
    if (cons->isOneway()) {
        return false;
    }
    if (cons->getPriority() == OBJ) {
        return false;
    }
    if (cons->getArgument(0) == LQ) {
        return false;
    }
    // Find best iv to define (currently the one not defining other variables)
    bool canBeMadeOneway = false;
    IntegerVariable* bestVar;
    unsigned prevEqual = 0;
    unsigned prio = 0;
    unsigned equalCounter = 0;
    unsigned defined = std::numeric_limits<unsigned int>::max();
    unsigned constraintsApllyingToiv = std::numeric_limits<unsigned>::max();
    for (IntegerVariable* iv : cons->getVariables()) {
        if (iv->isFixed()) {
            continue;
        }
        if (iv->isDef()) {
            continue;
        }

        // Will not make cycles 
        //        if(defining.at(iv->getID())>0){
        //            continue;
        //        }
        if (defined > defining.at(iv->getID())) {
            bestVar = iv;
            canBeMadeOneway = true;
            constraintsApllyingToiv = iv->usedIn;
            prio = iv->getSerachPriority();
            prevEqual = 1;
            defined = defining.at(iv->getID());
            continue;

        } else if (defined < defining.at(iv->getID())) {
            continue;
        }
        if (iv->getSerachPriority() > prio) {
            bestVar = iv;
            canBeMadeOneway = true;
            constraintsApllyingToiv = iv->usedIn;
            prio = iv->getSerachPriority();
            prevEqual = 1;
            defined = defining.at(iv->getID());
            continue;
        } else if (iv->getSerachPriority() < prio) {
            continue;
        }
        if (iv->usedIn < constraintsApllyingToiv) {
            bestVar = iv;
            canBeMadeOneway = true;
            constraintsApllyingToiv = iv->usedIn;
            prio = iv->getSerachPriority();
            prevEqual = 1;
            defined = defining.at(iv->getID());
        } else if (iv->usedIn == constraintsApllyingToiv) {
            int choose = Random::Integer(prevEqual);
            prevEqual++;
            //            ties++;

            equalCounter++;
            if (choose == 0) {
                bestVar = iv;
                canBeMadeOneway = true;
                constraintsApllyingToiv = iv->usedIn;
                prio = iv->getSerachPriority();
                defined = defining.at(iv->getID());
            }
        }

        //        if (defining.at(iv->getID()) > 0) {
        //            return false;
        //        }
    }
    if (canBeMadeOneway) {
        makeOneway(bestVar, cons);
        return true;
    }

    return false;
}

bool LSSpace::intVarCanBeMadeOneway(IntegerVariable* iv, constraint cons) {
    if (cons->isOneway()) {
        return false;
    }
    if (cons->getPriority() == OBJ) {
        return false;
    }
    unsigned notDefined = 0;
    for (IntegerVariable* iv : cons->getVariables()) {
        if (iv->isIntegerVariable()) {
            if (!iv->isDef()) {
                notDefined++;
            }
        }
    }

    if (notDefined > 1) {
        return false;
    }
    if (cons->getArgument(0) == LQ) {
        double coeff = cons->getCoefficients().at(iv->getID());
        for (constraint cons : *model->getObjectives().get()) {
            int objCoef = cons->getCoefficients().at(iv->getID());
            if ((objCoef < 0 || coeff > 0)) {
                std::cout << "Coefficient fail" << std::endl;
                return false;
            }
        }
    } else {
        //        std::cout << "equal constraint" << std::endl;
        return true;
    }
    ////                makeOneway(iv, cons, coeff);
    return true;
    //            }

}

void LSSpace::makeIntVarOneway(IntegerVariable* iv, constraint cons) {
    variableContainer& oldVars = cons->getVariables();
    std::unordered_map<int, coefType> coefficients = cons->getCoefficients();
    std::unordered_map<int, coefType> newCoefficients;
    double coeff = coefficients.at(iv->getID());
    //    std::cout << cons->getArgument(1) << "  "<< coeff<< std::endl;
    double value = -cons->getArgument(1);
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
        value = value / (-coeff);
    }
    newCoefficients.erase(iv->getID());
    std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(newCoefficients);
    variableContainer vars;
    sumInvariant->setStartValue(value);
    for (IntegerVariable* oldiv : oldVars) {
        if (oldiv != iv) {
            //            varsAndInvars.push_back(oldiv);
            unsigned id = oldiv->getID();
            value += newCoefficients.at(id) * (oldiv->getCurrentValue()*1.0);
            //                std::cout << newCoefficients.at(id) << oldiv->getCurrentValue() << " + ";
            defining.at(oldiv->getID())++;
            vars.push_back(oldiv);
            //            }
        }
    }

    sumInvariant->setVariablePointers(vars);
    //        sumInvariant->invariantID = model->getInvariants().size();
    model->addInvariant(sumInvariant);
    //    model->addInvariantToDDG(sumInvariant, vars, invars);
    //    model->addInvariantToDDG(sumInvariant, vars);
    DDG->addInvariant(sumInvariant, vars);
    assert(value == (int) value);
    assert(iv->getCurrentValue() >= 0);
    //    if(iv->getID() == 256 ){
    //        std::cout << value << std::endl;
    //    }
    sumInvariant->setValue(value);
    if (cons->getArgument(0) == LQ) {

        //        std::shared_ptr<Max> maxInvariant = std::make_shared<Max>(sumInvariant, iv->getLowerBound(), iv->getID(), model->getDDG());
        std::shared_ptr<Max> maxInvariant = std::make_shared<Max>(sumInvariant, iv->getLowerBound(), iv->getID());
        InvariantContainer invars;
        invars.push_back(sumInvariant);
        model->addInvariant(maxInvariant);
        DDG->addInvariant(maxInvariant, invars);

        //            sumInvariant->addToUpdate(maxInvariant.get());
        //            sumInvariant->addToUpdate(maxInvariant);
        iv->setDefinedBy(maxInvariant, cons);
        maxInvariant->setBounds(iv->getLowerBound(), iv->getUpperBound());
        maxInvariant->setVariableID(iv->getID());
        cons->setInvariant(maxInvariant);
        //            iv->getLowerBound(), 
        if (value < iv->getLowerBound()) {
            maxInvariant->setValue(iv->getLowerBound());
        } else {
            maxInvariant->setValue(value);
        }

        if (iv->getCurrentValue() != maxInvariant->getCurrentValue()) {
            std::cout << iv->getCurrentValue() << " vs " << maxInvariant->getCurrentValue() << std::endl;
        }
        maxInvariant->test();
    } else {
        if (value != iv->getCurrentValue()) {
            std::cout << value << " vs " << iv->getCurrentValue() << " ";
        }
        iv->setDefinedBy(sumInvariant, cons);
        cons->setInvariant(sumInvariant);
        sumInvariant->setVariableID(iv->getID());
        sumInvariant->setBounds(iv->getLowerBound(), iv->getUpperBound());
        if (!sumInvariant->test()) {

        }
    }
    cons->isOneway(true);
}

//void LSSpace::makeOneway(IntegerVariable* iv, constraint cons, double coeff) {

void LSSpace::makeOneway(IntegerVariable* iv, constraint cons) {
    // prob not from invariant 
    variableContainer& oldVars = cons->getVariables();
    std::unordered_map<int, coefType> coefficients = cons->getCoefficients();
    std::unordered_map<int, coefType> newCoefficients;
    double coeff = coefficients.at(iv->getID());
    //    std::cout << cons->getArgument(1) << "  "<< coeff<< std::endl;
    double value = -cons->getArgument(1);
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

        value = value / (-coeff);
    }
    newCoefficients.erase(iv->getID());
    std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(newCoefficients);
    //    InvariantContainer invars;
    variableContainer vars;
    //    variableContainer varsAndInvars;
    sumInvariant->setStartValue(value);
    //    std::cout << value << " ";
    for (IntegerVariable* oldiv : oldVars) {
        if (oldiv != iv) {
            unsigned id = oldiv->getID();
            value += newCoefficients.at(id) * (oldiv->getCurrentValue()*1.0);
            defining.at(oldiv->getID())++;
            vars.push_back(oldiv);
            //            }
        }
    }

    sumInvariant->setVariablePointers(vars);
    //        sumInvariant->invariantID = model->getInvariants().size();
    model->addInvariant(sumInvariant);
    //    model->addInvariantToDDG(sumInvariant, vars, invars);
    //    model->addInvariantToDDG(sumInvariant, vars);
    DDG->addInvariant(sumInvariant, vars);
    assert(value == (int) value);
    assert(iv->getCurrentValue() >= 0);
    sumInvariant->setValue(value);

    if (value != iv->getCurrentValue()) {
        std::cout << value << " vs " << iv->getCurrentValue() << " ";

    }

    iv->setDefinedBy(sumInvariant, cons);
    cons->setInvariant(sumInvariant);
    //            sumInvariant->variableID = iv->getID();
    sumInvariant->setVariableID(iv->getID());
    sumInvariant->setBounds(iv->getLowerBound(), iv->getUpperBound());
    if (!sumInvariant->test()) {

    }
    cons->isOneway(true);

}

void LSSpace::optimizeSolution(int time) {

    double timelimit = (double) time;
    double usedTime = 0;
    std::clock_t start = std::clock();
    //        currentState->saveSolution();
    //    std::cout << "ini obj val " << currentState->getEvaluation().at(0) << std::endl;
    //    std::cout << "ini sol val " << st->getSolutionValue() << std::endl;
    //    unsigned fourPercent = model->getNonFixedVariables().size() / 25;
    unsigned twoPercent = model->getNonFixedVariables().size() / 25;
    //    std::cout << "two % " << twoPercent << " " << model->getNonFixedVariables().size() << "/"<< 25 << std::endl;
    unsigned randomMoves = std::min(twoPercent, (unsigned) 10);
    //    randomMoves = 6;
    for (unsigned i = 0; i < currentState->getSolution().size(); i++) {
        assert(currentState->getSolution().at(i) == bestState->getSolution().at(i));
    }
    //    setSolution(bestState);
    //    std::cout << "solution set" << std::endl;

    std::cout << "randomMoves " << randomMoves << std::endl;
    FlipNeighborhood* NE = new FlipNeighborhood(model, currentState);
    RandomWalk RW(model, NE);
    BestImprovement BI(model, NE);
    unsigned loopCounter = 0;
    while (usedTime < timelimit) {
        loopCounter++;
        while (BI.Start()) {
            //        std::cout << __LINE__ << std::endl;
            //        

            iterations++;
            //        if (!st->recalculateAll()) {
            //            std::cout << "Line " << __LINE__ << std::endl;
            //            sleep(5);
            //        }
            //        break;
            //    std::cout << "Iter " << iterations << std::endl;
            for (int val : currentState->getEvaluation()) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
        if (currentState->getEvaluation().at(0) < bestState->getEvaluation().at(0) && currentState->isFeasible()) {
            bestState->copy(currentState);
            //            setSolution(bestState);
            std::cout << "improved solution value to: " << bestState->getEvaluation().at(0) << " after " << iterations << " iterations" << std::endl;
            if (testInvariant()) {
                std::cout << "Something is wrong with invariants" << std::endl;
                exit(1);
            }
        }
        RW.Start(randomMoves);
        //        for (int i = 0; i < randomMoves; i++) {
        //            NE->randomWalk(currentState);
        iterations += randomMoves;
        //            if (!st->recalculateAll()) {
        //                std::cout << "Line " << __LINE__ << std::endl;
        //                sleep(5);
        //            }
        //        }
        usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    }
    std::cout << "number of loops " << loopCounter << std::endl;
    std::cout << "Time used " << usedTime << std::endl;
    std::cout << "obj val " << currentState->getEvaluation().at(0) << " violations ";
    for (unsigned i = 1; i < currentState->getEvaluation().size(); i++) {
        std::cout << currentState->getEvaluation().at(i) << " ";
    }
    std::cout << std::endl;
    std::cout << "obj val " << bestState->getEvaluation().at(0) << std::endl;
    std::cout << "Set soluton " << std::endl;
    setSolution(bestState);
    if (testInvariant()) {
        std::cout << "failed test of invariant after setSolution" << std::endl;
        exit(1);
    }
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
    std::cout << "Solution value: " << bestState->getEvaluation().at(0) << std::endl;
    std::cout << "Number of moves " << iterations << std::endl;
    //    delete mv;
    //    delete bestMove;
    delete NE;
    std::cout << "O " << bestState->getEvaluation().at(0) << " ";
    //
    //    std::cout << "Solution value: " << st->getObjectiveValue() << std::endl;
    //    std::cout << "Number of moves " << iterations << std::endl;
}

bool LSSpace::testInvariant() {
    bool failed = false;
    for (invariant invar : model->getInvariants()) {
        if (invar->isUsedByConstraint()) {
            continue;
        }
        if (!invar->test()) {
            failed = true;
        }
    }
    return failed;
}

void LSSpace::setSolution(std::shared_ptr<State> st) {

    int change;
    //    for (unsigned i = 0; i < solution->size(); i++) {
    for (IntegerVariable* iv : model->getLSVariables()) {

        //        IntegerVariable* current = model->->getAllIntegerVariable(solution->at(i));
        //            for (unsigned j = 0; j < model->getUpdate(iv); j++) {
        assert(!iv->isDef() && !iv->isFixed());
        change = st->getSolution().at(iv->getID()) - iv->getCurrentValue();
        if (change != 0) {
            iv->setCurrentValue(st->getSolution().at(iv->getID()));

            for (updateType invar : model->getUpdate(iv)) {
                //                updateType invariant = iv->getUpdateVector().at(j);
                invar->addChange(iv->getID(), change);
            }
        }


    }
    //    std::cout << "value of 20340 " << solution.at(20340) << std::endl;
    //    for (unsigned i = 0; i < model->getInvariants()->size(); i++) {
    for (std::shared_ptr<Invariant> invar : model->getInvariants()) {
        debug;
        change = invar->calculateDeltaValue();
        if (change != 0) {
            for (invariant inv : model->getUpdate(invar)) {
                assert(invar != inv);
                inv->addChange(invar->getVariableID(), change);
            }
        }
        invar->updateValue();
        if (invar->getVariableID() != -1) {
            IntegerVariable* iv = model->getVariable(invar->getVariableID());
            assert(iv->getOneway() == invar);
            if (invar->getCurrentValue() < iv->getLowerBound()) {
                std::cout << "FML!!!!!!!!!!!" << std::endl;

                exit(1);

            }
            assert(invar->getCurrentValue() >= iv->getLowerBound());
            iv->setCurrentValue(invar->getCurrentValue());
            //            assert(solution.at(iv->getID())== iv->getCurrentValue());
            //            if(solution.at(iv->getID())!= iv->getCurrentValue()){
            //                std::cout << solution.at(iv->getID()) << " vs " << iv->getCurrentValue() << " type " << invar->getType() << std::endl;
            //            }
            st->getSolution().at(iv->getID()) = iv->getCurrentValue();
        }
        //        model->getInvariants()->at(i)->calculateDeltaValue();
        //        model->getInvariants()->at(i)->updateValue();
    }

    std::cout << std::endl;
    // Setting constraints
    int violations = 0;
    for (unsigned i = 1; i < model->getConstraints().size(); i++) {
        std::shared_ptr<std::vector<std::shared_ptr < Constraint>>>prio = model->getConstraintsWithPriority(i);
        //        for (unsigned j = 0; j < prio->size(); j++) {

        for (std::shared_ptr<Constraint> cons : *prio) {
            if (!cons->isOneway()) {
                cons->updateViolation();
                violations += cons->getViolation();
                //                int violation = cons->updateViolation();
                //                if(violation != 0){
                //                    std::cout << "Violation " << violation << std::endl;
                //                }
                //                cons->testCons();
                //                if (violation == 1) {
                //                    std::cout << "invar id " << cons->getInvariant()->getID() << " value " << cons->getInvariant()->getCurrentValue() << " rhs " << cons->getArgument(1) << std::endl;
                //                    violations += violation;
                //                }
            }
        }
    }
    st->getEvaluation().at(1) = violations;
    if (violations != 0) {
        std::cout << "Final solution not feasible? violations: " << violations << std::endl;
        sleep(2);
    }
    // setting objective value
    int value = 0;
    for (invariant invar : model->getObjectiveInvariant()) {
        value += invar->getCurrentValue();

    }
    std::cout << std::endl;
    //    for (unsigned i = 0; i < getConstraintsWithPriority(0)->size(); i++) {
    //    for (std::shared_ptr<Constraint> obj : *model->getConstraints().at(0)) {
    //        value += obj->updateViolationDegree();
    //        //        violations += getConstraintsWithPriority(0)->at(i)->updateViolationDegree();
    //    }
    std::cout << "Final solution " << value << " (" << violations << ")" << std::endl;
    st->getEvaluation().at(0) = value;

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
