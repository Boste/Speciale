#include "GPSolver.hpp"

GPSolver::GPSolver() {
}

GPSolver::~GPSolver() {

}

void GPSolver::addObjective(std::vector<int>& coefficients, std::vector<Variable*>& variables) {
    std::shared_ptr<Linear> LinearConstraint = std::make_shared<Linear>(coefficients, variables, 0, EQ, 0);
    for (Variable* iv : variables) {
        iv->inObjective = true;

    }
    if (storage->getConstraints().size() <= 0) {
        std::shared_ptr<std::vector<std::shared_ptr < Constraint>>>prioVector = std::make_shared<std::vector<std::shared_ptr < Constraint >>> ();
        storage->getConstraints().push_back(prioVector);

    }

    constraintContainer prio = storage->getConstraintsWithPriority(0);
    prio->push_back(LinearConstraint);
}

void GPSolver::linear(std::vector<int>& coefficients, std::vector<Variable*>& variables, int relation, int ub, unsigned priority) {


    if (priority != OBJ) {
        GS->linear(coefficients, variables, relation, ub);
    }
    if (relation > 2) {
        if (relation == 4) {
            ub = ub + 1;
            relation = 3;
        }
        if (relation == 3) {
            for (unsigned i = 0; i < coefficients.size(); i++) {
                coefficients[i] = -coefficients[i];

            }
            ub = -ub;
            relation = 1;
        } else {
            std::cout << "Not a valid relation given to Linear constraint" << std::endl;
            debug;
            exit(1);
        }


    } else {
        if (relation == 2) {
            ub = ub - 1;
            relation = 1;
        }

    }

    std::shared_ptr<Linear> LinearConstraint = std::make_shared<Linear>(coefficients, variables, ub, relation, priority);
    for (Variable* iv : variables) {
        if (priority != 0) {
            iv->addToUsedInConstraints();
        } else {
            iv->inObjective = true;
        }
    }
    if (storage->getConstraints().size() <= priority) {
        for (unsigned i = storage->getConstraints().size(); i <= priority; i++) {
            std::shared_ptr<std::vector<std::shared_ptr < Constraint>>>prioVector = std::make_shared<std::vector<std::shared_ptr < Constraint >>> ();
            storage->getConstraints().push_back(prioVector);
        }
    }
    constraintContainer prio = storage->getConstraintsWithPriority(priority);
    prio->push_back(LinearConstraint);

}

///Create a single variable with given lower and upper bound

Variable* GPSolver::createVariable(int lb, int ub) {
    Variable* iv = storage->addBinaryVariable(lb, ub);
    GS->createGecodeVariable(lb, ub);
    return iv;

}

std::vector<Variable*> GPSolver::createVariables(unsigned numberOfVariables, int lb, int ub) {
    std::vector<Variable*> variableVector;
    for (unsigned i = 0; i < numberOfVariables; i++) {
        Variable* iv = createVariable(lb, ub);
        variableVector.push_back(iv);

    }
    return variableVector;

}

std::vector<Variable*>& GPSolver::getAllVariables() {
    return storage->getAllVariables();
}
/// Takes a vector of IntegerVariable* that will be made search on. Should be 
/// called multiple times in order of variable importance. (First call most important
/// variables)

void GPSolver::Search(std::vector<Variable*> variables) {
    for (Variable* iv : variables) {
        iv->searchPrio = storage->priorityVectorsOfVariables.size();
    }
    storage->priorityVectorsOfVariables.push_back(variables);

}


void GPSolver::initialSolution(int TimeForGecode) {
    auto tid = std::clock();
    GS->createArray();
    if (GS->findSolution(TimeForGecode, true)) {

        double gecode = (std::clock() - tid) / (double) CLOCKS_PER_SEC;
        LS->createDDG(true);
        LS->initializeLS();

    } else {
        relax(TimeForGecode);

        LS->createDDG(false);
        LS->initializeLS();

    }

    int initialValue = 0;
    constraint obj = storage->getConstraintsWithPriority(0)->at(0);
    std::unordered_map<int, coefType>& coef = obj->getCoefficients();
    for (Variable* iv : obj->getVariables()) {
        double coeff = coef.at(iv->getID());
        if (coeff != 0) {
            //                std::cout << "id " << iv->getID() << " value " << iv->getCurrentValue() << " coeff " << coeff << std::endl;
            initialValue += coeff * iv->getCurrentValue();
        }
    }
    std::cout << "This should be initial value " << initialValue << std::endl;
}

bool GPSolver::relax(int TimeForGecode) {
    unsigned numberOfConstraints = 0;
    GS = std::unique_ptr<GecodeEngine>(new GecodeEngine(storage));

    for (Variable* iv : storage->getAllVariables()) {
        int lb = iv->getLowerBound();
        int ub = iv->getUpperBound();


        GS->createGecodeVariable(lb, ub);
    }
    GS->createArray();
    std::vector<constraint> funcCons;
    std::vector<constraint> nonFuncCons;


    for (int i = storage->getConstraints().size() - 1; i > 0; i--) {
        numberOfConstraints += storage->getConstraintsWithPriority(i)->size();
        constraintContainer prio = storage->getConstraintsWithPriority(i);
        std::vector<constraint> tmp1;
        std::vector<constraint> tmp2;
        for (constraint cons : *prio) {
            assert(cons->getPriority() == i);
            if (cons->isFunctional()) {
                tmp1.push_back(cons);
            } else {
                tmp2.push_back(cons);
            }
        }
        std::random_shuffle(tmp1.begin(), tmp1.end());
        std::random_shuffle(tmp2.begin(), tmp2.end());
        for (constraint cons : tmp1) {
            funcCons.push_back(cons);
        }
        for (constraint cons : tmp2) {
            nonFuncCons.push_back(cons);
        }
    }
    assert(funcCons.size() + nonFuncCons.size() == numberOfConstraints);

    std::vector<constraint> keepOrder;
    unsigned keept = 0;

    unsigned keep = (numberOfConstraints) / 2;

    for (constraint cons : funcCons) {
        if (keept < keep) {
            keepOrder.push_back(cons);
            keept++;
        } else {
            break;
        }

    }
    for (constraint cons : nonFuncCons) {
        if (keept < keep) {
            keepOrder.push_back(cons);
            keept++;
        } else {
            break;
        }
    }
    std::cout << "Posting " << keept << " constriants " << std::endl;
    for (constraint cons : keepOrder) {

        std::vector<Variable*> variables = cons->getVariables();

        if (cons->getType() == LINEAR) { // otherwise it is in objective function atm.
            std::vector<int> c(variables.size());

            for (unsigned j = 0; j < variables.size(); j++) {
                c[j] = cons->getCoefficients().at(variables.at(j)->getID()); //                    std::cout << __LINE__ << std::endl;

            }
            int relation = cons->getArgument(0);
            int ub = cons->getArgument(1);
            GS->linear(c, variables, relation, ub);
        } else {
            std::cout << "type should be LINEAR and assert should prevent this. Then type is set to " << cons->getType() << std::endl;
        }
    }
    bool solution = GS->findSolution(TimeForGecode, false);
    unsigned numberOfTimesRelaxed = 1;
    if (!solution) {
        unsigned remove = keepOrder.size() / 2;
        for (unsigned
                i = 0; i < remove; i++) {
            keepOrder.pop_back();
        }
        std::cout << "Number of constriants " << keepOrder.size() << std::endl;
        furtherRelax(keepOrder);

        solution = GS->findSolution(TimeForGecode, false);
        numberOfTimesRelaxed++;

    }
    std::vector<constraint> feasibleFunc;
    for (constraint con : keepOrder) {
        if (con->isFunctional()) {
            feasibleFunc.push_back(con);
        }
    }
    if (solution) {
    } else {

        GS->randomInitialize();


    }

    return solution;
}
/// Half the constraints posted every time

void GPSolver::furtherRelax(std::vector<constraint> cons) {

    GS = std::unique_ptr<GecodeEngine>(new GecodeEngine(storage));

    for (Variable* iv : storage->getAllVariables()) {
        int lb = iv->getLowerBound();
        int ub = iv->getUpperBound();


        GS->createGecodeVariable(lb, ub);
    }
    GS->createArray();


    for (unsigned i = 0; i < cons.size(); i++) {
        constraint con = cons.at(i);
        std::vector<Variable*> variables = con->getVariables();

        if (con->getType() == LINEAR) { // otherwise it is in objective function atm.
            std::vector<int> c(variables.size());
            for (unsigned j = 0; j < variables.size(); j++) {
                c[j] = con->getCoefficients().at(variables.at(j)->getID()); 

            }
            int relation = con->getArgument(0);
            int ub = con->getArgument(1);
            GS->linear(c, variables, relation, ub);
        } else {

            std::cout << "type should be LINEAR and assert should prevent this. Then type is set to " << con->getType() << std::endl;
        }
    }
}


std::vector<int>& GPSolver::getInitialValue() {

    return storage->getInitialEvaluation();
}

void GPSolver::optimizeSolution(int time, int test) {

    LS->optimizeSolution(time, test);
}
void GPSolver::printVariableValues() {
    if (storage->getAllVariables().size() > 0) {
        std::cout << "Integer Variables:" << std::endl;
    }

    for (unsigned i = 0; i < storage->getAllVariables().size(); i++) {
        std::cout << storage->getAllVariables().at(i)->getCurrentValue() << " ";

    }
    std::cout << std::endl;
}
