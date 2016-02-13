
#include "ConflictOnlyNE.hpp"
//#include "State.hpp"

ConflictOnlyNE::ConflictOnlyNE(std::shared_ptr<Storage> model, std::shared_ptr<State> st) {
    this->model = model;
    this->state = st;
    calculated.resize(model->getAllVariables().size(), 0);
    lastSuggested = model->getViolatedConstraints().size()*2;
}

ConflictOnlyNE::~ConflictOnlyNE() {
}

Move* ConflictOnlyNE::next() {
    if (firstMove) {
        iter++;
        firstMove = false;
        lastSuggested = suggested;
        moveCounter = 0;
        suggested = 0;
        moveIterator = model->getViolatedConstraints().begin();


    }

    if (model->getViolatedConstraints().empty()) {
        return NULL;
    }
    while (moveIterator != model->getViolatedConstraints().end()) {
        while (moveCounter < (*moveIterator).second->getVariablePointers().size()) {
            int id = (*moveIterator).second->getVariablePointers().at(moveCounter)->getID();
            if (calculated.at(id) != iter) {
                calculated.at(id) = iter;
                Variable* var = model->getVariable(id);
                if (var->isFixed()) {
                    continue;
                }
                suggested++;

                moveCounter++;
                Move* mv = new Move(var, (1 - var->getCurrentValue()) - var->getCurrentValue());
                mv->deltaVector.resize(state->getEvaluation().size(), 0);
                return mv;


            } else {
                moveCounter++;
            }
        }
        moveIterator++;
        moveCounter = 0;
    }
    firstMove = true;
    return NULL;

}

unsigned ConflictOnlyNE::getSize() {
    return lastSuggested;
}

Move* ConflictOnlyNE::nextRandom() {
    moveIterator = model->getViolatedConstraints().begin();
    int rand = Random::Integer(0, model->getViolatedConstraints().size() - 1);
    for (int i = 0; i < rand; i++) {
        moveIterator++;
    }
    int rand2 = Random::Integer(0, (*moveIterator).second->getVariablePointers().size() - 1);
    Variable* var = (*moveIterator).second->getVariablePointers().at(rand2);
    Move* mv = new Move(var, (1 - var->getCurrentValue()) - var->getCurrentValue());
    mv->deltaVector.resize(state->getEvaluation().size(), 0);
    return mv;
}

bool ConflictOnlyNE::calculateDelta(Move* mv) {
    std::vector<int>& change = mv->getDeltaVector();
    for (unsigned i = 0; i < change.size(); i++) {
        model->getEvaluationInvariantNr(i)->calculateDelta();
    }

    Variable* variable = mv->var;
    calculated.at(variable->getID()) = iter;
    propagation_queue& queue = model->getPropagationQueue(variable);
    updateVector& update = model->getUpdate(variable);

    for (updateType& invar : update) {
        invar->proposeChange(variable->getID(), mv->getVariableChange());
    }
    bool legal = true;
    for (updateType invar : queue) {

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
    if (!legal) {
        for (updateType invar : queue) {
            invar->calculateDelta();
        }
    } else {
        for (unsigned i = 0; i < change.size(); i++) {
            change[i] = model->getEvaluationInvariantNr(i)->getDeltaValue();
        }
    }

    return legal;
}

bool ConflictOnlyNE::commitMove(Move* mv) {
    firstMove = true;
    lastSuggested = suggested;
    Variable* var = mv->getVar();

    std::vector<int>& evaluation = state->getEvaluation();

    // Skal genberegne!!!!!
    bool legal = calculateDelta(mv);
    if (!legal) {
        return false;
    }
    var->setCurrentValue(1 - var->getCurrentValue());

    propagation_queue queue = model->getPropagationQueue(var);
    for (updateType invar : queue) {

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
    for (unsigned i = 0; i < model->getEvaluationInvariants().size(); i++) {
        evaluation[i] = model->getEvaluationInvariantNr(i)->getCurrentValue();
    }
    return true;
}