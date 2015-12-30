#include "SwapNeighborhood.hpp"

SwapNeighborhood::SwapNeighborhood(std::shared_ptr<Model> model, std::shared_ptr<State> st) {
    this->model = model;
    this->state = st;
}

SwapNeighborhood::~SwapNeighborhood() {
}

Move* SwapNeighborhood::next() {
    Variable* iv;
    Variable* iv2;
    if (moveCounter2 == model->getMask().size() - 1) {
        iv2 = model->getMaskAt(moveCounter2);
        moveCounter2 = 0;
        iv = model->getMaskAt(moveCounter);
        moveCounter++;
    } else {
        iv = model->getMaskAt(moveCounter);
        iv2 = model->getMaskAt(moveCounter2);
        moveCounter2++;
    }
    while (iv->getCurrentValue() == iv2->getCurrentValue() && moveCounter != model->getMask().size() - 1) {
        if (moveCounter2 == model->getMask().size() - 1) {
            iv2 = model->getMaskAt(moveCounter2);
            moveCounter2 = 0;
            iv = model->getMaskAt(moveCounter);
            moveCounter++;
        } else {
            iv = model->getMaskAt(moveCounter);
            iv2 = model->getMaskAt(moveCounter2);
            moveCounter2++;
        }
    }
    std::vector<Variable*> vars;
    vars.push_back(iv);
    vars.push_back(iv2);

    std::vector<int> delta;
    delta.push_back(iv2->getCurrentValue() - iv->getCurrentValue());
    delta.push_back(iv->getCurrentValue() - iv2->getCurrentValue());
    //    IntegerVariable* iv = model->getMaskAt(moveCounter);
    //    moveCounter++;
    Move* mv = new Move(vars, delta);
    //    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
    //    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
    //    mv->deltaVector.resize(model->getPriorityVectors().size());
    mv->deltaVector.resize(model->getPriorityVectors().size(), 0);
    return mv;
}

bool SwapNeighborhood::hasNext() {
    if (moveCounter < model->getMask().size() - 1) {
        return true;
    } else {
        moveCounter = 0;
        moveCounter2 = 1;
        return false;
    }
}

Move* SwapNeighborhood::nextRandom() {
    Variable* iv1 = model->getMaskAt(Random::Integer(0, (int) model->getMask().size() - 1));
    Variable* iv2 = model->getMaskAt(Random::Integer(0, (int) model->getMask().size() - 1));
    while (iv1->getCurrentValue() == iv2->getCurrentValue() && model->getMask().size() > 1) {
        iv1 = model->getMaskAt(Random::Integer(0, (int) model->getMask().size() - 1));
        iv2 = model->getMaskAt(Random::Integer(0, (int) model->getMask().size() - 1));
    }
    randomCounter++;
    //    Move* mv = new Move(iv, (1 - iv->getCurrentValue()) - iv->getCurrentValue());
    std::vector<Variable*> vars;
    vars.push_back(iv1);
    vars.push_back(iv2);
    std::vector<int> delta;
    delta.push_back(iv2->getCurrentValue() - iv1->getCurrentValue());
    delta.push_back(iv1->getCurrentValue() - iv2->getCurrentValue());
    //    IntegerVariable* iv = model->getMaskAt(moveCounter);
    //    moveCounter++;
    Move* mv = new Move(vars, delta); //    mv->deltaVector.resize(model->getPriorityVectors().size());
    mv->deltaVector.resize(model->getPriorityVectors().size(), 0);
    return mv;
}

bool SwapNeighborhood::hasNextRandom() {
    //    std::cout << randomCounter << std::endl;
    if (randomCounter < randomMovesWanted) {
        return true;
    } else {
        randomCounter = 0;
        return false;
    }
}

void SwapNeighborhood::setRandomCounter(unsigned numberOfRandomMoves) {
    randomMovesWanted = numberOfRandomMoves;
}

bool SwapNeighborhood::calculateDelta(Move* mv) {

    std::vector<int>& change = mv->getDeltaVector();
    //    for (unsigned i = 0; i < change.size(); i++) {
    //        change[i] = 0;
    //    }
    std::vector<Variable*>& variables = mv->getVars();
    propagation_queue& queue1 = model->getPropagationQueue(variables.at(0));
    propagation_queue& queue2 = model->getPropagationQueue(variables.at(1));
    testCounter++;


    // #########################################################
    // Create queue by comparison
    // #########################################################
    std::vector<updateType> queue;
    propagation_queue::iterator iter1 = queue1.begin();
    propagation_queue::iterator iter2 = queue2.begin();
    while (iter1 != queue1.end() || iter2 != queue2.end()) {


        if ((*iter1)->getTimestamp()> (*iter2)->getTimestamp()) {
            queue.push_back((*iter1));
            iter1++;
        } else if ((*iter1)->getTimestamp() == (*iter2)->getTimestamp()) {
            queue.push_back((*iter1));
            iter1++;
            iter2++;
        } else {
            queue.push_back(*iter2);
            iter2++;
        }
    }
    while (iter1 != queue1.end()) {
        queue.push_back(*iter1);
        iter1++;
    }
    while (iter2 != queue2.end()) {
        queue.push_back(*iter2);
        iter2++;
    }

    // ###########################################################
    // Test comparison manually vs set
    // ###########################################################
    //    std::cout << "queue1 ";
    //    for (auto it = queue1.begin(); it != queue1.end(); it++) {
    //        std::cout << (*it)->getTimestamp() << " ";
    //    }
    //    std::cout << std::endl;
    //    std::cout << "queue2 ";
    //    for (auto it = queue2.begin(); it != queue2.end(); it++) {
    //        std::cout << (*it)->getTimestamp() << " ";
    //    }
    //    std::cout << std::endl;
    //    std::cout << "New queue ";
    //    for (updateType inv : queue) {
    //        std::cout << inv->getTimestamp() << " ";
    //    }
    //    std::cout << std::endl;
    //    debug;
    //
    //    propagation_queue queue3;
    //    if (queue1.size() > queue2.size()) {
    //        queue3 = queue1;
    //        for (updateType inv : queue2) {
    //            queue3.insert(inv);
    //        }
    //    } else {
    //        queue3 = queue2;
    //        for (updateType inv : queue1) {
    //            queue3.insert(inv);
    //        }
    //    }
    //    int lolhest = 0;
    //    for(auto it = queue3.begin(); it!=queue3.end();it++){
    //        std::cout << (*it)->getTimestamp() << " " <<  queue.at(lolhest)->getTimestamp() << std::endl;
    //        lolhest++;
    //    }
    //    debug;




    // ###############################################
    // Create queue using set
    // ###############################################
    //        propagation_queue queue;
    //        if (queue1.size() > queue2.size()) {
    //            queue = queue1;
    //            for (updateType inv : queue2) {
    //                queue.insert(inv);
    //            }
    //        } else {
    //            queue = queue2;
    //            for (updateType inv : queue1) {
    //                queue.insert(inv);
    //            }
    //        }



    updateVector& update1 = model->getUpdate(variables.at(0));
    updateVector& update2 = model->getUpdate(variables.at(1));
    for (updateType& invar : update1) {
        invar->addChange(variables.at(0)->getID(), mv->getVariableChanges().at(0));
    }
    for (updateType& invar : update2) {
        invar->addChange(variables.at(1)->getID(), mv->getVariableChanges().at(1));
    }
    bool legal = true;

    for (updateType invar : queue) {

        legal = invar->calculateDeltaValue();
        if (!legal) {
            // Should make a reset in invariant to remove all changes added to the rest of the invariants, instead of calculating delta and not using it.
            break;
        }
        if (invar->getDeltaValue() != 0) {
            for (updateType inv : model->getUpdate(invar)) {
                inv->addChange(invar->getVariableID(), invar->getDeltaValue());
            }
        }

        //        if (invar->isUsedByConstraint() && legal) {
        //            std::shared_ptr<Constraint>& cons = invar->getConstraint(); //model->getConstraintsWithPriority(priority)->at(invar->getConstraintNumber());
        //
        //            int priority = invar->getPriority();
        //            if (invar->getPriority() == 0) {
        //                change[0] += invar->getDeltaValue();
        //            } else {
        //                debug;
        ////                change[priority] += cons->setDeltaViolation();
        //            }
        //        }

    }

    //    change = model->get
    if (!legal) {
        for (updateType invar : queue) {
            invar->calculateDeltaValue();
        }
    } else {
        for (unsigned i = 0; i < model->getEvaluationInvariants().size(); i++) {
            change[i] = model->getEvaluationInvariants().at(i)->getDeltaValue();
        }
    }
    return legal;
}

bool SwapNeighborhood::commitMove(Move* mv) {
    moveCounter = 0;
    moveCounter2 = 0;
    Variable* var1 = mv->getVars().at(0);
    Variable* var2 = mv->getVars().at(1);
    std::vector<int>& evaluation = state->getEvaluation();
    // Skal genberegne!!!!!
    bool legal = calculateDelta(mv);
    if (!legal) {
        return false;
    }
    var1->setCurrentValue(1 - var1->getCurrentValue());
    var2->setCurrentValue(1 - var2->getCurrentValue());

    propagation_queue& queue1 = model->getPropagationQueue(var1);
    propagation_queue& queue2 = model->getPropagationQueue(var2);

    // #########################################################
    // Create queue by comparison
    // #########################################################
    std::vector<updateType> queue;
    propagation_queue::iterator iter1 = queue1.begin();
    propagation_queue::iterator iter2 = queue2.begin();
    while (iter1 != queue1.end() || iter2 != queue2.end()) {


        if ((*iter1)->getTimestamp()> (*iter2)->getTimestamp()) {
            queue.push_back((*iter1));
            iter1++;
        } else if ((*iter1)->getTimestamp() == (*iter2)->getTimestamp()) {
            queue.push_back((*iter1));
            iter1++;
            iter2++;
        } else {
            queue.push_back(*iter2);
            iter2++;
        }
    }
    while (iter1 != queue1.end()) {
        queue.push_back(*iter1);
        iter1++;
    }
    while (iter2 != queue2.end()) {
        queue.push_back(*iter2);
        iter2++;
    }
    for (updateType invar : queue) {
        invar->updateValue();

        //        if (invar->isUsedByConstraint()) {
        //
        //            if (invar->getPriority() > 0) {
        //                std::shared_ptr<Constraint> cons = invar->getConstraint(); // model->getConstraintsWithPriority(invar->getPriority())->at(invar->getConstraintNumber());
        //                debug;
        //                //                evaluation.at(cons->getPriority()) += cons->updateViolation();
        //
        //
        //            } else {
        //                evaluation.at(0) += invar->getDeltaValue();
        //            }
        //        }
    }
    for (unsigned i = 0; i < model->getEvaluationInvariants().size(); i++) {
        evaluation[i] = model->getEvaluationInvariants().at(i)->getCurrentValue();
    }
    //    testCounter++;

    return true;
}

//void FlipNeighborhood::makeMove(Move* mv, std::shared_ptr<State> st) {
//    commitMove(mv, st);
//}