//#include <c++/4.8/bits/stl_vector.h>

#include "LSSpace.hpp"
//using namespace Gecode;

LSSpace::LSSpace() {
    //    std::cout << "constructed" << std::endl;
    //    initialState = State();
    
}

std::vector<IntegerVariable*>* LSSpace::addIntVariablesToState(Gecode::IntVarArray* vars) {
    return st.addIntegerVariable(vars);
}

void LSSpace::SetValues(Gecode::IntVarArray vars){
            for (int i = 0; i < vars.size(); i++) {
            assert(vars[i].assigned());
            st.getIntegerVariables()->at(i)->setCurrentValue(vars[i].val());
        }
}
void LSSpace::addInvariantToIntVariable(int variableNumber, int invariantNumber) {
    st.getIntegerVariables()->at(variableNumber)->addToUpdate(invariantNumber);
    
}

void LSSpace::linear(std::vector<int>* coefficients, vector<IntegerVariable*>* variables, int relation, int upperbound, int type) {
    //        std::cout << variables << std::endl;
    Sum* sumInvariant = new Sum(variables, coefficients);

    //        std::cout << sumInvariant << std::endl;
    for (unsigned i = 0; i < coefficients->size(); i++) {
        addInvariantToIntVariable(variables->at(i)->getID(), st.getInvariants()->size());
    }
    st.getInvariants()->push_back(sumInvariant);
    Linear* LinearConstraint = new Linear(sumInvariant, upperbound, relation);
    if (type == INVARIANT) {
        sumInvariant->usedByConstraint(st.getConstraints()->size());
        st.getConstraints()->push_back(LinearConstraint);

    } else {
        sumInvariant->usedByObjective(st.getObjectives()->size());
        st.getObjectives()->push_back(LinearConstraint);
    }
}


//    FlipMove mv = FlipMove(st.getIntegerVariables()->at(0));
//    Move bestMove = mv;
//    std::pair<int, int> delta = calculateDeltaValueOfMove(mv);
//    int violationChange = delta.first;
//    int objectiveChange = delta.second;
int LSSpace::bestMove() {
    int violationChange = 0;
    int objectiveChange = 0;
    int bestVariable = -1;
    for (unsigned i = 0; i < st.getIntegerVariables()->size(); i++) {
        IntegerVariable* variable = st.getIntegerVariables()->at(i);
        std::pair<int, int> delta = calculateDeltaValueOfVariableChange(i, 1 - variable->getCurrentValue());
        if (delta.first <= violationChange) {
            if (delta.second <= objectiveChange) {
                violationChange = delta.first;
                objectiveChange = delta.second;
                bestVariable = variable->getID();
            }
        }
    }
    if (bestVariable != -1) {
        commitDeltaOfVariable(bestVariable);
    } else {
        std::cout << "no improving move" << std::endl;
        sleep(1);
        std::exit(1);

    }
    std::cout << "changed variable " << bestVariable << std::endl;
    std::cout << "violation change " << violationChange << " objective change " << objectiveChange << std::endl;

    return bestVariable;
}

void LSSpace::commitDeltaOfVariable(int changedVariable) {
    IntegerVariable* var = st.getIntegerVariables()->at(changedVariable);
    vector<int>* update = var->getUpdateVector();
    for (unsigned i = 0; i < update->size(); i++) {
        Invariant* invar = st.getInvariants()->at(update->at(i));
        invar->updateValue();
        if (invar->getUsedInConstraint() != -1) {
            st.getConstraints()->at(invar->getUsedInConstraint())->updateViolation();
        }

        if (invar->getUsedInObjective() != -1) {
            st.getObjectives()->at(invar->getUsedInObjective())->updateViolationDegree();
        }
    }
    var->setCurrentValue(1 - var->getCurrentValue());

}

std::pair<int, int> LSSpace::calculateDeltaValueOfVariableChange(int variableNumber, int newValue) {

    IntegerVariable* variable = st.getIntegerVariables()->at(variableNumber);
    int oldValue = variable->getCurrentValue();
    vector<int>* updateVector = variable->getUpdateVector();
    int violationChange = 0;
    int objectiveChange = 0;
    //    std::cout << "Variable " << variableNumber << std::endl;
    for (unsigned i = 0; i < updateVector->size(); i++) {
        Invariant* invar = st.getInvariants()->at(updateVector->at(i));
        invar->addChange(variableNumber, newValue - oldValue);
        invar->calculateDeltaValue();
        if (invar->getUsedInConstraint() != -1) {
            //            std::cout << "Used in Constraint " << std::endl;
            violationChange += st.getConstraints()->at(invar->getUsedInConstraint())->setDeltaViolation();
        }
        if (invar->getUsedInObjective() != -1) {
            //            std::cout << "Used in objective " << std::endl;
            objectiveChange += st.getObjectives()->at(invar->getUsedInObjective())->setDeltaViolationDegree();
        }
    }
    //    std::cout << std::endl;
    std::pair<int, int> change(violationChange, objectiveChange);
    return change;
    //    return std::pair<int,int> change = new pair<int,int>(violationChange,objectiveChange);


    //        variable->setCurrentValue(newValue);
}
void LSSpace::initializeInvariants() {
    st.initializeInvariants();
}

void LSSpace::initializeConstraints() {
    st.initializeConstraints();
}

void LSSpace::initializeObjective() {
    st.initializeObjective();
}
void LSSpace::printCurrent() {

    if (st.getIntegerVariables()->size() > 0) {
        std::cout << "Integer Variables:" << std::endl;
    }
    
    for (unsigned i = 0; i < st.getIntegerVariables()->size(); i++) {
        std::cout << st.getIntegerVariables()->at(i)->getCurrentValue() << " ";
        //        std::cout << IntVarVector[i].VariablePointer << " ";

    }
    std::cout << std::endl;
    if (BoolVarVector.size() > 0) {
        std::cout << "Boolean Variables:" << std::endl;
    }
    for (unsigned i = 0; i < BoolVarVector.size(); i++) {
        std::cout << BoolVarVector[i].getCurrentVal() << " ";
    }
    std::cout << std::endl;


}

//LSSpace::~LSSpace() {
//}

