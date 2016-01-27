#include "GPSolver.hpp"

GPSolver::GPSolver() {
    //    for (int i = 0; i < 10; i++) {
    //        int max = std::numeric_limits<int>::max() / std::pow(10, i);
    //        //            std::cout << max << std::endl;
    //        maxCoef.push_back(max);
    //    }
}

GPSolver::~GPSolver() {


    //        std::cout << "Destructing GS" << std::endl;
    //        delete GS;
    //        delete st;
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
//            iv->addToUsedInConstraints(LinearConstraint);
            iv->addToUsedInConstraints();
        } else {
            iv->inObjective = true;
        }
    }
    //                        GS->print(std::cout );
    //                        sleep(1);
    //             What should be given to LSSpace  
    if (storage->getConstraints().size() <= priority) {
        for (unsigned i = storage->getConstraints().size(); i <= priority; i++) {
            std::shared_ptr<std::vector<std::shared_ptr < Constraint>>>prioVector = std::make_shared<std::vector<std::shared_ptr < Constraint >>> ();
            storage->getConstraints().push_back(prioVector);
        }
    }
    constraintContainer prio = storage->getConstraintsWithPriority(priority);
    //        if (relation == EQ) {
    //            model->functionalConstraints.push_back(LinearConstraint);
    //        }
    prio->push_back(LinearConstraint);

    //    }
}

///Create a single variable with given lower and upper bound

Variable* GPSolver::createVariable(int lb, int ub) {
    //    if (ub == 1 && lb == 0) {
    Variable* iv = storage->addBinaryVariable(lb, ub);
    GS->createGecodeVariable(lb, ub);
    return iv;

    //    } else {
    //        Variable* iv = model->addIntegerVariable(lb, ub);
    //        GS->createGecodeVariable(lb, ub);
    //        return iv;
    //    }
    //        GS->createGecodeVariable(lb, ub);
    //        std::cout <<  "integer var pointer " << newVar << std::endl;
    //        std::cout << lb << " " << ub << std::endl;

}
/// Returns a vector of the variables created (copying the vector)

std::vector<Variable*> GPSolver::createVariables(unsigned numberOfVariables, int lb, int ub) {
    // Given to gecode space
    //        Gecode::IntVarArray vars(*this, numberOfVariables, lb, ub);
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

/// Only for testing, should be removed 

//void GeneralSolver::print(std::vector<Variable>& IntegerVariables) {
//    std::cout << "{";
//    for (unsigned i = 0; i < IntegerVariables.size() - 1; i++) {
//        std::cout << *(IntegerVariables[i].getVariablePointer()) << ", ";
//    }
//    std::cout << IntegerVariables.back().getVariablePointer() << "}" << std::endl;
//}


//    void InitialSolution(Gecode::InstanceOptions opt) {

/// Uses Gecode to find initial solution
/// returns a new GeneralSolver with the initial solution the old (the one this method is called from) is not updated with new solution. 

//    GeneralSolver* InitialSolution(Gecode::Search::Options* so) {

void GPSolver::initialSolution(int TimeForGecode) {
    auto tid = std::clock();
    GS->createArray();
    //        GS->branch(true);
    if (GS->findSolution(TimeForGecode, true)) {
        //    if (false) {


        //¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
        // Obj value after gecode
        // ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

        double gecode = (std::clock() - tid) / (double) CLOCKS_PER_SEC;
        std::cout << "## gecode " << gecode << std::endl;


        //        std::ofstream myfile;
        //        myfile.open("func2.txt", std::ios::app);
        //        myfile << " 0 " << gecode << "\n";
        //        myfile.close();
        //        std::cout << "#relax 0" << std::endl;
        //        exit(1);
        std::cout << "## relax 0" << std::endl;
        tid = std::clock();
        LS->createDDG(true);
        LS->initializeLS();
        auto inils = (std::clock() - tid) / (double) CLOCKS_PER_SEC;
        std::cout << "## initLSModel " << inils << std::endl;

    } else {
        //        std::cout << "not relaxing" << std::endl;   
        relax(TimeForGecode);
        double gecode = (std::clock() - tid) / (double) CLOCKS_PER_SEC;
        std::cout << "## gecode " << gecode << std::endl;


        //        std::ofstream myfile;
        //        myfile.open("func2.txt", std::ios::app);
        //        myfile << " " << gecode << "\n";
        //        myfile.close();
        //        exit(1);
        tid = std::clock();

        LS->createDDG(false);
        LS->initializeLS();
        double inils = (std::clock() - tid) / (double) CLOCKS_PER_SEC;
        std::cout << "## initLSModel " << inils << std::endl;

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

    unsigned relax = storage->out->relax;
    if (relax == 0) {
        storage->out->addToTable1(std::to_string(100));
        storage->out->feasibleVal = storage->getEvaluationInvariantNr(0)->getCurrentValue();
    } else if (relax == 1) {
        storage->out->addToTable1(std::to_string(50));
    } else if (relax == 2) {
        storage->out->addToTable1(std::to_string(25));
    } else if (relax == 3) {
        storage->out->addToTable1(std::to_string(0));
    } else {

    }
    storage->out->addToTable1(std::to_string(storage->out->solTime));
    if (storage->out->feasible) {
        storage->out->addToTable1(std::to_string(storage->out->feasibleTime));
    }
    std::cout << storage->out->getToTable1() << std::endl;

    //        st = std::make_shared<State>(model);


    //        delete ms;
    //        delete so;
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
            //                    Gecode::IntArgs c(integerVariables->size());
            std::vector<int> c(variables.size());

            //                    Gecode::IntVarArgs x(integerVariables->size());
            for (unsigned j = 0; j < variables.size(); j++) {
                //                    debug;
                c[j] = cons->getCoefficients().at(variables.at(j)->getID()); //                    std::cout << __LINE__ << std::endl;

            }
            //                IntConLevel icl = cons->getICL();
            int relation = cons->getArgument(0);
            int ub = cons->getArgument(1);
            GS->linear(c, variables, relation, ub);
            //            posted++;
        } else {
            std::cout << "type should be LINEAR and assert should prevent this. Then type is set to " << cons->getType() << std::endl;
        }
    }
    storage->out->relax = 1;

    storage->out->addToGecodePrint("1");
    bool solution = GS->findSolution(TimeForGecode, false);
    //    std::vector<constraint> ;
    unsigned numberOfTimesRelaxed = 1;
    if (!solution) {

        //    while (!solution && numberOfTimesRelaxed < 2) {
        //        std::vector<constraint> newKeep;
        //        std::cout << "Number of constraints " << funcCons.size() << std::endl;

        //        for (unsigned i = 0; i < keepOrder.size() / 2; i++) {
        //        for (unsigned i = 0; i < keepOrder.size() / 2; i++) {
        //            newKeep.push_back(keepOrder.at(i));
        //            newKeep.push_back(keepOrder.at(i));
        //        }
        unsigned remove = keepOrder.size() / 2;
        for (unsigned
                i = 0; i < remove; i++) {
            keepOrder.pop_back();
        }
        //        unsigned counter = keepOrder.size() / 2;
        //        for(auto it = keepOrder.end(); counter != 0; --it ){
        //            keepOrder.erase(it);
        //            counter--;
        //        }
        std::cout << "Number of constriants " << keepOrder.size() << std::endl;
        furtherRelax(keepOrder);
        storage->out->relax = 2;

        storage->out->addToGecodePrint("2");

        solution = GS->findSolution(TimeForGecode, false);
        //        keepOrder = newKeep;
        numberOfTimesRelaxed++;

    } else {

    }
    std::vector<constraint> feasibleFunc;
    for (constraint con : keepOrder) {
        if (con->isFunctional()) {
            feasibleFunc.push_back(con);
        }
    }
    if (solution) {
        storage->setFeasibleFunctionalConstraints(feasibleFunc);
    } else {
        storage->out->relax = 3;
        storage->out->addToGecodePrint("3");

        GS->randomInitialize();


    }

    //    }
    //    std::ofstream myfile;
    //    myfile.open("func2.txt", std::ios::app);
    //    myfile << " " << numberOfTimesRelaxed;
    //    myfile.close();
    std::cout << "## relax " << numberOfTimesRelaxed << std::endl;
    //    exit(1);
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

    //            std::vector<constraint> constraintsPosted;
    //    for (unsigned i = 0; i < cons.size() / 2; i++) {

    for (unsigned i = 0; i < cons.size(); i++) {
        //        if (Random::Integer(reapted) == 0) {
        //            continue;
        //        }
        constraint con = cons.at(i);
        std::vector<Variable*> variables = con->getVariables();

        if (con->getType() == LINEAR) { // otherwise it is in objective function atm.
            //                    Gecode::IntArgs c(integerVariables->size());
            std::vector<int> c(variables.size());

            //                    Gecode::IntVarArgs x(integerVariables->size());
            for (unsigned j = 0; j < variables.size(); j++) {
                //                    debug;
                c[j] = con->getCoefficients().at(variables.at(j)->getID()); //                    std::cout << __LINE__ << std::endl;

            }
            //                IntConLevel icl = cons->getICL();
            int relation = con->getArgument(0);
            int ub = con->getArgument(1);
            GS->linear(c, variables, relation, ub);
            //            posted++;
        } else {

            std::cout << "type should be LINEAR and assert should prevent this. Then type is set to " << con->getType() << std::endl;
        }
        //        constraintsPosted.push_back(con);
    }
}
//    return cons;


/// relaxes the space (reduce the number of constraints). Used when Gecode cant find a solution in time.
/// Only works for binary

/// Different relaxation can be chosen (not atm) needs to create a new GecodeSolver (Space) and recreate some 
/// of the calls the user made (those that should not be relaxed).

//void GeneralSolver::relax(int timesRelaxed) {
//    //        std::cout << "relax" << std::endl;
//
//    GS = std::unique_ptr<GecodeSolver>(new GecodeSolver(model));
//
//    for (Variable* iv : model->getAllVariables()) {
//        int lb = iv->getLowerBound();
//        int ub = iv->getUpperBound();
//
//
//        GS->createGecodeVariable(lb, ub);
//    }
//    GS->createArray();
//    simpleRelax(timesRelaxed);
//    //        IntVars = relaxed->IntVars;
//    // Brug operator = til at lave det gamle space til det nye? Ellers skal det hele løses med det nye og gamle pointer skal opdateres. 
//    // Hvordan fungere det så med at slette? 
//
//
//}

//void GeneralSolver::simpleRelax(int timesRelaxed) {
//    std::cout << "SimpleRelax" << std::endl;
//    double numberOfConstraints = 0;
//    for (unsigned i = 1; i < model->getConstraints().size(); i++) {
//        numberOfConstraints += model->getConstraintsWithPriority(i)->size();
//    }
//
//    int relax = std::ceil(std::pow(2, timesRelaxed) / 100.0 * numberOfConstraints);
//    int keep = std::max(numberOfConstraints - relax, 0.0);
//    int posted = 0;
//
//    //        std::cout << "do nothing" << std::endl;
//    //        std::cout << model->getObjectives()->at(0)->getInvariant()->VariablePointers->size() << std::endl;
//    for (unsigned i = 1; i < model->getConstraints().size()/*-((timesRelaxed+1)*100)*/; i++) {
//
//        //        for ( std::vector<std::shared_ptr < Constraint>>* prio : *st->getConstraints()) {
//        constraintContainer prio = model->getConstraintsWithPriority(i);
//        //            for (unsigned j = 0; j < prio->size(); j++) {
//        for (constraint cons : *prio) {
//            if (posted == keep) {
//                break;
//            }
//            //                std::shared_ptr<Invariant> invar = cons->getInvariant();
//            //                Invariant* invar = cons->getInvariant();
//            std::vector<Variable*> variables = cons->getVariables();
//            if (cons->getType() == LINEAR) { // otherwise it is in objective function atm.
//                //                    Gecode::IntArgs c(integerVariables->size());
//                std::vector<int> c(variables.size());
//
//                //                    Gecode::IntVarArgs x(integerVariables->size());
//                for (unsigned j = 0; j < variables.size(); j++) {
//                    //                    debug;
//                    c[j] = cons->getCoefficients().at(variables.at(j)->getID()); //                    std::cout << __LINE__ << std::endl;
//
//                }
//                //                IntConLevel icl = cons->getICL();
//                int relation = cons->getArgument(0);
//                int ub = cons->getArgument(1);
//                GS->linear(c, variables, relation, ub);
//                posted++;
//            } else {
//
//                std::cout << "type should be LINEAR and assert should prevent this. Then type is set to " << cons->getType() << std::endl;
//            }
//        }
//    }
//    std::cout << posted << std::endl;
//    std::cout << keep << std::endl;
//
//    std::cout << "SimpleRelax made " << timesRelaxed << " times" << std::endl;
//}

std::vector<int>& GPSolver::getInitialValue() {

    return storage->getInitialEvaluation();
}

void GPSolver::optimizeSolution(int time, int test) {

    LS->optimizeSolution(time, test);
}
// Only for testing

//    Gecode::IntVarArray * getIntVars() {
//        return &IntVars;
//    }

void GPSolver::printVariableValues() {
    if (storage->getAllVariables().size() > 0) {
        std::cout << "Integer Variables:" << std::endl;
    }

    for (unsigned i = 0; i < storage->getAllVariables().size(); i++) {
        std::cout << storage->getAllVariables().at(i)->getCurrentValue() << " ";
        //        std::cout << IntVarVector[i].VariablePointer << " ";

    }
    std::cout << std::endl;
    //        if (BoolVarVector.size() > 0) {
    //            std::cout << "Boolean Variables:" << std::endl;
    //        }
    //        for (unsigned i = 0; i < BoolVarVector.size(); i++) {
    //            //        std::cout << BoolVarVector[i].getCurrentVal() << " ";
    //        }
    //        std::cout << std::endl;
}

void GPSolver::setOutput(Output* out) {
    storage->out = out;
}