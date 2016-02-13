
#include "Storage.hpp"
//#include "Random.hpp"

Storage::Storage() {

}

Storage::~Storage() {
    for (Variable* iv : original) {
        delete iv;
    }
    for (invariant invar : Invariants) {
        delete invar;
    }

}

std::vector<std::vector<Variable*>>&Storage::getPriorityVectors() {
    return priorityVectorsOfVariables;
}

std::vector<Variable*>& Storage::getPriorityVectorNr(unsigned i) {
    assert(i < priorityVectorsOfVariables.size());

    return priorityVectorsOfVariables.at(i);
}

std::vector<Variable*>& Storage::getIntegerVariables() {
    return IntegerVariables;
}

Variable* Storage::addBinaryVariable(int lb, int ub) {
    //    for (int i = 0; i < numberOfVariables; i++) {
    int id = original.size();
    Variable* v = new Variable(lb, ub, id);
    original.push_back(v);
    return v;

}
void Storage::setNonFixedVariables(std::vector<Variable*>& nonFixed) {
    nonFixedVariables = nonFixed;
}

void Storage::addViolatedConstraint(invariant inv) {
    violatedConstraints[inv->getID()] = inv;
    inv->setInViolatedConstraints(true);

}

void Storage::removeViolatedConstraint(invariant inv) {
    violatedConstraints.erase(inv->getID());
    inv->setInViolatedConstraints(false);

}

std::unordered_map<unsigned, invariant>& Storage::getViolatedConstraints() {
    return violatedConstraints;

}

std::shared_ptr<DependencyDigraph>& Storage::getDDG() {
    return DDG;
}

propagation_queue& Storage::getPropagationQueue(Variable* iv) {
    assert(DDG->propagationQueueHasBeenMade());
    assert(!iv->isDef());
    assert(!iv->isFixed());
    return DDG->getPropagationQueue(iv);

}

updateVector& Storage::getUpdate(invariant invar) {
    return DDG->getInvariantUpdate(invar->getID());
}

updateVector& Storage::getUpdate(Variable* iv) {
    return DDG->getVariableUpdate(iv->getID());
}

Variable* Storage::getMaskAt(int i) {
    return mask.at(i);
}

std::vector<Variable*>& Storage::getMask() {
    return mask;
}

void Storage::shuffleMask() {
    std::random_shuffle(mask.begin(), mask.end());

}


/// Should only be used before propagation queue is made

variableContainer& Storage::getNonFixedVariables() {
    return nonFixedVariables;
}

Variable* Storage::getNonFixedVariable(int i) {
    return nonFixedVariables.at(i);
}

Variable* Storage::getVariable(unsigned id) {
    return original.at(id);
}

variableContainer& Storage::getAllVariables() {
    return original;
}

InvariantContainer& Storage::getInvariants() {

    return Invariants;
}

/// adds Invariant and gives it an id.

void Storage::addInvariant(invariant invar) {
    invar->invariantID = id++;
    Invariants.push_back(invar);
}

allConstraints& Storage::getConstraints() {
    return Constraints;
}

constraintContainer Storage::getConstraintsWithPriority(int prio) {
    return Constraints.at(prio);
}

InvariantContainer& Storage::getEvaluationInvariants() {
    return evaluationInvariants;
}

invariant Storage::getEvaluationInvariantNr(unsigned nr) {
    return evaluationInvariants.at(nr);
}


void Storage::addToEvaluationInvariants(invariant invar) {
    evaluationInvariants.push_back(invar);
}

std::vector<int>& Storage::getInitialEvaluation() {
    return initialEvaluation;
}

std::vector<Variable*>& Storage::getEvaluationVariables() {
    return evalVariables;
}

Variable* Storage::getEvaluationVariableNr(unsigned nr) {
    return evalVariables.at(nr);
}

void Storage::initialize() {
    for (invariant inv : Invariants) {
        if (!inv->test()) {
            debug;
        }
    }



    for (unsigned int i = 0; i < getEvaluationInvariants().size(); i++) {
        initialEvaluation.push_back(getEvaluationInvariants().at(i)->CurrentValue);

    }
    for (constraint cons : *getConstraintsWithPriority(0)) {
        for (Variable* var : cons->getVariables()) {

            if (!var->isDef() && !var->isFixed()) {
                evalVariables.push_back(var);
            }
        }
    }
    std::cout << "EvalVariables size " << evalVariables.size() << std::endl;
    for (Variable* iv : getNonFixedVariables()) {
        if (iv->isDef() || iv->isFixed()) { // || iv->isIntegerVariable()) {
            continue;

        } else {
            mask.push_back(iv);
        }
    }
    shuffleMask();


}

std::set<Variable*, Variable::compare_variable>& Storage::getInConstraintWithAt(unsigned id) {

    return inConstraintWith.at(id);
}

std::vector<constraint>& Storage::getFeasibleFunctionalConstraints() {

    return feasibleFunctionalConstriants;
}

void Storage::setFeasibleFunctionalConstraints(std::vector<constraint> funcCons) {

    feasibleFunctionalConstriants = funcCons;
}

void Storage::cleanUp() {
    InvariantContainer tmp;
    for (invariant invar : Invariants) {
        if (!invar->isBroken()) {
            tmp.push_back(invar);
        } else {

            delete invar;

        }
    }

    Invariants.swap(tmp);

    std::sort(Invariants.begin(), Invariants.end(), compare_invariant());
    for (invariant inv : Invariants) {

        for (invariant invar : inv->getInvariantPointers()) {
            assert(!invar->isBroken());
            assert(original[invar->getVariableID()]->isDef());
        }

        for (auto it = inv->getVariablePointers().begin(); it != inv->getVariablePointers().end(); ++it) {
            if ((*it)->isDef()) {
                inv->getInvariantPointers().push_back((*it)->getOneway());
                inv->VariablePointers.erase(it);
                --it;

            }

        }


        if (inv->getInvariantPointers().size() + inv->getVariablePointers().size() != inv->getCoefficients().size()) {
            std::cout << inv->getInvariantPointers().size() << " + " << inv->getVariablePointers().size() << " != " << inv->getCoefficients().size();

        }
    }
}
