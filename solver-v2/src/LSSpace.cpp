//#include <c++/4.8/bits/stl_vector.h>

#include "LSSpace.hpp"


//using namespace Gecode;

LSSpace::LSSpace(std::shared_ptr<Model> model) {
    this->model = model;

    //        std::cout << "constructed" << std::endl;

}

//std::vector<IntegerVariable*>* LSSpace::addIntVariablesToModel(Gecode::IntVarArray* vars) {
//    return 
//}


//void LSSpace::addInvariantToIntVariable(int variableNumber, int invariantNumber) {
//    st->getIntegerVariables()->at(variableNumber)->addToUpdate(invariantNumber);
//
//}

//void LSSpace::linear(std::vector<int>* coefficients, vector<IntegerVariable*>* variables, int relation, int upperbound, int type) {
//    //        std::cout << variables << std::endl;
//    Sum* sumInvariant = new Sum(variables, coefficients);
//
//    //        std::cout << sumInvariant << std::endl;
//    for (unsigned i = 0; i < coefficients->size(); i++) {
//        addInvariantToIntVariable(variables->at(i)->getID(), st->getInvariants()->size());
//    }
//    st->getInvariants()->push_back(sumInvariant);
//    Linear* LinearConstraint = new Linear(sumInvariant, upperbound, relation);
//    if (type == HARD) {
//        sumInvariant->usedByConstraint(st->getHardConstraints()->size());
//        st->getHardConstraints()->push_back(LinearConstraint);
//
//    } 
//    
//}

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

    model->startUp();
    model->addVariablesToDDG(model->getNonFixedBinaryVariables());


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

    //        for (IntegerVariable* iv : model->getIntegerVariables()) {
    //            if (!iv->isInteger) {
    //                std::cout << "integer not integer" << std::endl;
    //            }
    //            if (!iv->isDefined) {
    //                std::cout << "integer but not defined" << std::endl;
    //            }
    //            assert(iv->isInteger == iv->isDefined);
    //        }


    //        unsigned highestLayer;
    // Create the new model that is used in Local search



    for (constraint cons : *model->getObjectives()) {
        assert(!cons->isOneway());
    }
    for (unsigned i = 0; i < model->getConstraints().size(); i++) {
        for (constraint cons : *model->getConstraintsWithPriority(i)) {
            if (!cons->isOneway()) {
                int value = 0;
                std::unordered_map<int, coefType>& coef = cons->getCoefficients();
                std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(coef, model->getDDG());
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
                model->addInvariantToDDG(sumInvariant, variables, invars);
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

    //    for (constraint cons : *model->getObjectives()) {
    //        assert(!cons->isOneway());
    //        int value = 0;
    //        std::unordered_map<int, coefType>& coef = cons->getCoefficients();
    //        std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(coef, model->getDDG());
    //        variableContainer variables;
    //        InvariantContainer invars;
    //        for (IntegerVariable* iv : cons->getVariables()) {
    //            unsigned id = iv->getID();
    //            if (!iv->isDef()) {
    //                value += coef.at(id) * iv->getCurrentValue();
    //                variables.push_back(iv);
    //            } else {
    //                value += coef.at(id) * iv->getOneway()->getCurrentValue();
    //                invars.push_back(iv->getOneway());
    //
    //            }
    //        }
    //        sumInvariant->setValue(value);
    //        model->addInvariant(sumInvariant);
    //        model->addInvariantToDDG(sumInvariant, variables, invars);
    //        cons->setInvariant(sumInvariant);
    //        if (cons->getPriority() == OBJ) {
    //            model->addToObjectiveInvariant(sumInvariant);
    //            sumInvariant->setObjective();
    //        }
    //    }
    /// Create propagate queue either from invariant or variables
    std::cout << "Total time used so far " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << std::endl;
    std::cout << "Create propagator queues" << std::endl;
    model->createPropagationQueue();
    std::cout << "Total time used so far " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << std::endl;
    //        for(IntegerVariable* iv : model->getNonFixedBinaryVariables()){
    //            std::cout << model->getPropagationQueue(iv).size() << std::endl;

    //        }
    //        for (IntegerVariable* iv : model->getNonFixedBinaryVariables()) {
    //            //            propagation_queue& queue = iv->getPropagationQueue();
    //            //            updateVector allUpdates;
    //            propagation_queue& allUpdates = iv->getPropagationQueue();
    //            addToQueue(allUpdates, iv->getUpdateVector());
    //            //            addToQueue(allUpdates, iv->getUpdateVector());
    //            //            bool first = true;
    //            //            invariant lastInvar;
    ////            for (invariant invar : allUpdates) {
    ////                //                  if(first){
    ////                //                    lastInvar = invar;
    ////                //                    first=false;
    ////                //                } else {
    ////                //                    if(invar == lastInvar){
    ////                //                        std::cout << invar->getID() << " " << lastInvar->getID() << std::endl;
    ////                //                    }
    ////                //                }
    ////                iv->addToPropagationQueue(invar);
    ////                //                
    ////            }
    ////            std::cout << iv->getPropagationQueue().size() << std::endl;
    //
    //        }
    unsigned last = model->getAllVariables().size() - 1;
    for (invariant invar : model->getInvariants()) {
        unsigned current = invar->getID();
        if (current != last + 1) {
            std::cout << "last " << last << " current " << current << std::endl;
            sleep(10);
            std::cout << model->getInvariants().size() << std::endl;
            
        }
        last = current;
    }

    //
    for (IntegerVariable* iv : model->getIntegerVariables()) {
        iv->setCurrentValue(iv->getOneway()->getCurrentValue());
        if (iv->getCurrentValue() < iv->getLowerBound()) {
            std::cout << "invariant initialized wrong " << std::endl;
        }
    }
    //        std::cout << "obj delta val " << model->getObjectives()->at(0)->getInvariant()->getDeltaValue() << std::endl;

    model->initialize();
    currentState = std::make_shared<State>(model);
    bestState = std::make_shared<State>(model);
}

// Not taking into account if the coefficient in objective function is negative

bool LSSpace::canBeMadeOneway(IntegerVariable* iv, constraint cons) {
    if (cons->isOneway()) {
        //            std::cout << "Is one-way" << std::endl;
        return false;
    }
    if (cons->getPriority() == OBJ) {
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
                if (!iv->isDef()) {
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

void LSSpace::makeOneway(IntegerVariable* iv, constraint cons, int coeff) {
    // prob not from invariant 
    variableContainer& oldVars = cons->getVariables();
    
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
    std::shared_ptr<Sum> sumInvariant = std::make_shared<Sum>(newCoefficients, model->getDDG());

    InvariantContainer invars;
    variableContainer vars;
    int value = -cons->getArgument(1);

    for (IntegerVariable* oldiv : oldVars) {


        if (oldiv != iv) {
            unsigned id = oldiv->getID();
            if (oldiv->isDef()) {
                //                    assert(oldiv->isDefined);
                value += newCoefficients.at(id) * oldiv->getOneway()->getCurrentValue();
                invars.push_back(oldiv->getOneway());
                //                    sumInvariant->addInvariant(oldiv->oneway);
                //                    oldiv->getOneway()->addToUpdate(sumInvariant.get());
                //                    oldiv->getOneway()->addToUpdate(sumInvariant);



            } else {
                value += newCoefficients.at(id) * oldiv->getCurrentValue();

                vars.push_back(oldiv);
              sleep(1);
            }
        }
    }
    //        sumInvariant->invariantID = model->getInvariants().size();
    model->addInvariant(sumInvariant);

    model->addInvariantToDDG(sumInvariant, vars, invars);
    sumInvariant->setValue(value);

    if (cons->getArgument(0) == LQ) {

        std::shared_ptr<Max> maxInvariant = std::make_shared<Max>(sumInvariant, iv->getLowerBound(), iv->getID(), model->getDDG());
        InvariantContainer invars;
        invars.push_back(sumInvariant);
        //            maxInvariant->invariantID = model->getInvariants().size();
        model->addInvariant(maxInvariant);
        model->addInvariantToDDG(maxInvariant, invars);
        //            sumInvariant->addToUpdate(maxInvariant.get());
        //            sumInvariant->addToUpdate(maxInvariant);
        iv->setDefinedBy(maxInvariant, cons);
        cons->setInvariant(maxInvariant);
        //            iv->getLowerBound(), 
        maxInvariant->setValue(std::max(value, iv->getLowerBound()));

    } else {

        iv->setDefinedBy(sumInvariant, cons);
        cons->setInvariant(sumInvariant);
        //            sumInvariant->variableID = iv->getID();
        sumInvariant->setVariableID(iv->getID());

    }

    //        sumInvariant->setBounds(iv->lowerBound, iv->upperBound);
    cons->isOneway(true);

}

void LSSpace::optimizeSolution(int time) {
    NeighborhoodExplorer* NE = new NeighborhoodExplorer(model);
    //    std::cout << "NE created" << std::endl;
    //    std::cout << "Segmentation fault right after this " << std::endl;
    IntegerVariable* var = model->getNonFixedBinaryVariable(0);
    //    std::cout << "segmentation fault before this" << std::endl;
    Move* mv = new Move(var, 1 - var->getCurrentValue() - var->getCurrentValue(), FLIP);
    mv->setDeltaChangeSize(model->getConstraints().size());
    //        std::cout << __LINE__ << std::endl;
    //    NE.bestImprovement<int>()
    //    NE->randomWalk(mv,st);

    //    double timelimit = 10;
    double timelimit = (double) time;
    double usedTime = 0;
    std::clock_t start = std::clock();
    //    currentState->saveSolution();
    std::cout << "ini obj val " << currentState->getEvaluation().at(0) << std::endl;
    //    std::cout << "ini sol val " << st->getSolutionValue() << std::endl;
    int twoPercent = model->getNonFixedBinaryVariables().size() / 50;
    int randomMoves = std::min(twoPercent, 10);
    //    std::cout << "Number of random moves " << randomMoves << std::endl;
    //    std::cout << "Timelimit " << timelimit << std::endl;
    //        std::cout << "optimize" << std::endl;
    //    debug;
    //    mv->first = model->getMaskAt(0);
    //    mv->first = model->getNonFixedBinaryVariable(model->getMaskAt(0));
    //        std::cout << "optimize" << std::endl;
    //    mv->deltaValueFirst = 1 - mv->first->getCurrentValue() - mv->first->getCurrentValue();
    while (NE->bestImprovement(mv, currentState)) {
        std::cout << "can de improvement " << std::endl;
        while (usedTime < timelimit) {
            //        std::cout << __LINE__ << std::endl;
            //        debug;

            iterations++;
            //        if (!st->recalculateAll()) {
            //            std::cout << "Line " << __LINE__ << std::endl;
            //            sleep(5);
            //        }
            //        break;
            usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
            //    std::cout << "Iter " << iterations << std::endl;
        }
    }
    std::cout << "Iter " << iterations << std::endl;

    //    std::cout << "in optimize" << std::endl;
    //    std::cout << st->getObjectiveValue() << " " << st->getSolutionValue() << " " << st->numberOfViolations << std::endl;
    if (currentState->getEvaluation().at(0) < bestState->getEvaluation().at(0) && currentState->isFeasible()) {
        //    if (st->getEvaluation().at(0) < st->getSolutionValue() && st->getEvaluation().at(1) == 0) {
        //        currentState->saveSolution();
        bestState->copy(currentState);
        std::cout << "improved solution value to: " << bestState->getEvaluation().at(0) << " after " << iterations << " iterations" << std::endl;
    }
    //    if (!st->recalculateAll()) {
    //        std::cout << "Line " << __LINE__ << std::endl;
    //        sleep(5);
    //    }
    while (usedTime < timelimit) {
        for (int i = 0; i < randomMoves; i++) {
            NE->randomWalk(mv, currentState);
            iterations++;
            //            if (!st->recalculateAll()) {
            //                std::cout << "Line " << __LINE__ << std::endl;
            //                sleep(5);
            //            }
        }
        //        std::cout << "objective value after random moves " << st->getObjectiveValue() << std::endl;
        //        mv->first = model->getNonFixedBinaryVariable(0);
        //        mv->deltaValueFirst = 1 - mv->first->getCurrentValue() - mv->first->getCurrentValue();
        while (NE->bestImprovement(mv, currentState) && usedTime < timelimit) {
            iterations++;
            //            if (!st->recalculateAll()) {
            //                std::cout << "Line " << __LINE__ << std::endl;
            //                sleep(5);
            //            }
            usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
            //            std::cout << iterations << " ";


        }
        //        if (iterations != 0) {
        //            std::cout << iterations << std::endl;
        //        }
        //        std::cout << iterations << std::endl;
        //        std::cout << currentState->getEvaluation().at(0) << " " << bestState->getEvaluation().at(0) << " " << currentState->isFeasible() << std::endl;
        //        sleep(1);
        if (currentState->getEvaluation().at(0) < bestState->getEvaluation().at(0) && currentState->isFeasible()) {
            bestState->copy(currentState);
            std::cout << "improved solution value to: " << bestState->getEvaluation().at(0) << " after " << iterations << " iterations" << std::endl;
            //            st->recalculateAll();
        }
        //        if (!st->recalculateAll()) {
        //            std::cout << "Line " << __LINE__ << std::endl;
        //            sleep(5);
        //        }
        //        usedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    }
    std::cout << "Time used " << usedTime << std::endl;
    std::cout << "obj val " << currentState->getEvaluation().at(0) << " violations ";
    for (unsigned i = 1; i < currentState->getEvaluation().size(); i++) {
        std::cout << currentState->getEvaluation().at(i) << " ";
    }
    std::cout << std::endl;
    std::cout << "obj val " << bestState->getEvaluation().at(0) << std::endl;
    bestState->setSolution();

    std::cout << "Solution value: " << bestState->getEvaluation().at(0) << std::endl;
    std::cout << "Number of moves " << iterations << std::endl;
    delete mv;
    delete NE;
    std::cout << "O " << bestState->getEvaluation().at(0) << " ";
    //
    //    std::cout << "Solution value: " << st->getObjectiveValue() << std::endl;
    //    std::cout << "Number of moves " << iterations << std::endl;
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
////        std::exit(1);
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

