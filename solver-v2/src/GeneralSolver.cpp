#include "GeneralSolver.hpp"

GeneralSolver::GeneralSolver() {
    //    for (int i = 0; i < 10; i++) {
    //        int max = std::numeric_limits<int>::max() / std::pow(10, i);
    //        //            std::cout << max << std::endl;
    //        maxCoef.push_back(max);
    //    }
}

GeneralSolver::~GeneralSolver() {


    //        std::cout << "Destructing GS" << std::endl;
    //        delete GS;
    //        delete st;
}

//GeneralSolver& GeneralSolver::operator=(const GeneralSolver &a) {
//    this->model = a.model;
//
//    return *this; 
//
//}


// Skal Gecode::IntConLevel icl være et argument?
// ingen shared i det her kald

//unsigned GeneralSolver::getNumberOfDigits(double number) {
//    //    std::cout << "Number " << number << std::endl;
//    //    double dig = number - static_cast<int> (number);
//    //    std::cout << dig << std::endl;
//    //    std::cout << number - (int) number << std::endl;
//    //    std::string asString = std::to_string(dig);
//    //    
//    //    sstream << dig ;
//    //    sstream << " ";
//    //    sstream << number - (int) number;
//    //    std::string varAsString = sstream.str();
//    //    std::cout << " hest " <<  varAsString << std::endl;
//    //    
//    std::cout.precision(6);
//
//    double intpart;
//    std::cout << "Number " << number << std::endl;
//    std::ostringstream sstream;
//    sstream.precision(6);
//    double fractpart = modf(number, &intpart);
//    sstream << fractpart;
//    std::cout << fractpart << std::endl;
//    std::string varAsString = sstream.str();
//    std::cout << varAsString << std::endl;
//    sleep(1);
//    int digits = varAsString.length() - 2;
//    std::cout << fractpart * 10000000 << std::endl;
//    std::cout << "Length - 2 = " << varAsString.length() << " - 2 " << digits << std::endl;
//
//    if (fractpart < 0) {
//        digits--;
//        std::cout << fractpart << std::endl;
//    }
//    assert(digits < 10);
//    //    std::cout << "Digits " << digits << std::endl;
//    //    std::cout << sstream.width() << std::endl;
//    //    int digits = sstream.width();
//    //    std::cout << sstream << std::endl;
//    //    std::cout << asString << std::endl;
//    //    unsigned digits = asString.length() - 2;
//    //    std::cout << "Digits " << digits << std::endl;
//    return digits;
//}

void GeneralSolver::linear(std::vector<int>& coefficients, std::vector<Variable*>& variables, int relation, int ub, unsigned priority) {
    //    bool hasDouble = false;
    //    int digits = 0;
    //    int ub = std::round(upperbound);
    //    //    getNumberOfDigits(1.1);
    //    if (upperbound != ub) {
    //        std::cout << upperbound << " vs " << ub << " diff " << ub-upperbound<< std::endl;
    //
    //        //        std::cout << "Do something ub" << std::endl;
    //        digits = getNumberOfDigits(upperbound);
    //        //        std::cout << upperbound << std::endl;
    //
    //
    //        hasDouble = true;
    //    }
    //    if (!hasDouble) {
    //        for (double coef : coeff) {
    //            if (coef != std::round(coef)) {
    //                //                std::cout << "Do something" << std::endl;
    //                int newDigit = getNumberOfDigits(coef);
    //                if (newDigit > digits) {
    //                    digits = newDigit;
    //                }
    //                //                std::cout << coef << std::endl;
    //                hasDouble = true;
    //                //                break;
    //            }
    //        }
    //    }

    //    std::vector<int> coefficients;
    //    if (hasDouble) {
    //        std::cout << "Digits " << digits << " Max coeff " << maxCoef[digits] << std::endl;
    //
    //
    //        for (double coef : coeff) {
    //            //            std::cout << coef << " vs " << coef*pow(10,digits) << "     "; 
    //            coefficients.push_back(std::round(coef * std::pow(10, digits)));
    //            //            std::cout << coef << " " << std::endl;
    //            if (std::abs(coef) > maxCoef[digits]) {
    //                std::cout << "Cannot convert to integer " << std::endl;
    //                std::cout << "exit" << std::endl;
    //                exit(1);
    //            }
    //        }
    //        if (std::abs(upperbound) > maxCoef[digits]) {
    //            std::cout << "Cannot convert to integer " << std::endl;
    //            std::cout << "exit" << std::endl;
    //
    //            exit(1);
    //        }
    //        ub = std::round(upperbound * std::pow(10, digits));
    //        //        sleep(1);
    //
    //    } else {
    //        for (double coef : coeff) {
    //            coefficients.push_back(std::round(coef));
    //
    //        }
    //
    //        //        ub = std::round(upperbound * std::pow(10, digits));
    //    }
    //    debug;
    std::shared_ptr<Linear> LinearConstraint = std::make_shared<Linear>(coefficients, variables, ub, relation, priority);

    //        bool haveInt = false;
    int numberOfIntegerVariables = 0;
    for (Variable* iv : variables) {
        if (priority != 0) {
            iv->addToUsedInConstraints(LinearConstraint);
        }
        if (iv->isInteger) {
            LinearConstraint->addIntegerVariable(iv);
            numberOfIntegerVariables++;
        }
    }
    if (priority == 0) {
        std::cout << "objective has " << numberOfIntegerVariables << " integer variables " << std::endl;
    }
    LinearConstraint->setNumberOfIntegerVariables(numberOfIntegerVariables);

    //    if (priority == OBJ) {
    //        if (model->getConstraints().size() == 0) {
    //            constraintContainer prioVector = std::make_shared<std::vector < constraint >> ();
    //            model->getConstraints().push_back(prioVector);
    //}
    //            for (IntegerVariable* iv : variables) {
    //                if (iv->isInteger && LinearConstraint->getCoefficients().at(iv->getID()) != 0) {
    //                    std::cout << iv->getID() << std::endl;
    //                }
    //            }

    //        model->getConstraintsWithPriority(0)->push_back(LinearConstraint);
    //    } else {
    //            GS->linear(coefficients, &variables, relation, ub);
    //            std::cout << "Skal Gecode have constraints? " << std::endl;
    if (priority != OBJ) {
        GS->linear(coefficients, variables, relation, ub);
    }
    //                        GS->print(std::cout );
    //                        sleep(1);
    //             What should be given to LSSpace
    if (model->getConstraints().size() <= priority) {
        for (unsigned i = model->getConstraints().size(); i <= priority; i++) {
            std::shared_ptr<std::vector<std::shared_ptr < Constraint>>>prioVector = std::make_shared<std::vector<std::shared_ptr < Constraint >>> ();
            model->getConstraints().push_back(prioVector);
        }
    }
    constraintContainer prio = model->getConstraintsWithPriority(priority);
    //        if (relation == EQ) {
    //            model->functionalConstraints.push_back(LinearConstraint);
    //        }
    prio->push_back(LinearConstraint);

    //    }
}

///Create a single variable with given lower and upper bound

Variable* GeneralSolver::createVariable(int lb, int ub) {
    if (ub == 1 && lb == 0) {
        Variable* iv = model->addBinaryVariable(lb, ub);
        GS->createGecodeVariable(lb, ub);
        return iv;

    } else {
        Variable* iv = model->addIntegerVariable(lb, ub);
        GS->createGecodeVariable(lb, ub);
        return iv;
    }
    //        GS->createGecodeVariable(lb, ub);
    //        std::cout <<  "integer var pointer " << newVar << std::endl;
    //        std::cout << lb << " " << ub << std::endl;

}
/// Returns a vector of the variables created (copying the vector)

std::vector<Variable*> GeneralSolver::createVariables(unsigned numberOfVariables, int lb, int ub) {
    // Given to gecode space
    //        Gecode::IntVarArray vars(*this, numberOfVariables, lb, ub);
    std::vector<Variable*> variableVector;
    for (unsigned i = 0; i < numberOfVariables; i++) {
        Variable* iv = createVariable(lb, ub);
        variableVector.push_back(iv);

    }
    return variableVector;

}

std::vector<Variable*>& GeneralSolver::getAllVariables() {
    return model->getAllVariables();
}
/// Takes a vector of IntegerVariable* that will be made search on. Should be 
/// called multiple times in order of variable importance. (First call most important
/// variables)

void GeneralSolver::Search(std::vector<Variable*> variables) {
    for (Variable* iv : variables) {
        iv->searchPrio = model->priorityVectorsOfVariables.size();
    }
    model->priorityVectorsOfVariables.push_back(variables);

}

/// Only for testing, should be removed 

void GeneralSolver::print(std::vector<Variable>& IntegerVariables) {
    std::cout << "{";
    for (unsigned i = 0; i < IntegerVariables.size() - 1; i++) {
        std::cout << *(IntegerVariables[i].getVariablePointer()) << ", ";
    }
    std::cout << IntegerVariables.back().getVariablePointer() << "}" << std::endl;
}


//    void InitialSolution(Gecode::InstanceOptions opt) {

/// Uses Gecode to find initial solution
/// returns a new GeneralSolver with the initial solution the old (the one this method is called from) is not updated with new solution. 

//    GeneralSolver* InitialSolution(Gecode::Search::Options* so) {

void GeneralSolver::InitialSolution(int TimeForGecode) {

    GS->createArray();

    //        GS->branch(true);
    if (GS->initialize(TimeForGecode, true)) {
        //    if (false) {

    } else {
        std::cout << "Gecode did not find a solution within limits given (nodes,fail,time). Model will be relaxed according to priorities given to constraints. " << std::endl;
        int timesRelaxed = 0;
        //        int timesRelaxed = 7;
        bool solutionFound = false;
        for (unsigned i = 0; i < model->getConstraints().size(); i++) {
            std::random_shuffle(model->getConstraintsWithPriority(i)->begin(), model->getConstraintsWithPriority(i)->end());
        }
        while (!solutionFound && timesRelaxed != 7) {
            relax(timesRelaxed);
            timesRelaxed++;
            //                GS->branch(false);
            solutionFound = GS->initialize(TimeForGecode, false);
        }
        if (!solutionFound) {
            std::cout << "Relaxation failed, trying with initial assignment of variables to minimum value" << std::endl;
            //            exit(1);
            relax(timesRelaxed);
            GS->initialize(TimeForGecode, false);
        }
        //        assert(s != NULL);
        //        assert(!s->failed());
        //        return s;
        //
        //                this->print(cout);
    }

    //        fixVariables();


    //¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
    // Obj value after gecode
    // ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
    int initialValue = 0;
    constraint obj = model->getConstraintsWithPriority(0)->at(0);
    auto coef = obj->getCoefficients();
    for (Variable* iv : obj->getVariables()) {
        double coeff = coef.at(iv->getID());
        if (coeff != 0) {
            //                std::cout << "id " << iv->getID() << " value " << iv->getCurrentValue() << " coeff " << coeff << std::endl;
            initialValue += coeff * iv->getCurrentValue();
        }
    }
    std::cout << "This should be initial value " << initialValue << std::endl;
    LS->initializeLS();
    //        st = std::make_shared<State>(model);


    //        delete ms;
    //        delete so;
}

/// relaxes the space (reduce the number of constraints). Used when Gecode cant find a solution in time.
/// Only works for binary


/// Different relaxation can be chosen (not atm) needs to create a new GecodeSolver (Space) and recreate some 
/// of the calls the user made (those that should not be relaxed).

void GeneralSolver::relax(int timesRelaxed) {
    //        std::cout << "relax" << std::endl;

    GS = std::unique_ptr<GecodeSolver>(new GecodeSolver(model));

    for (Variable* iv : model->getAllVariables()) {
        int lb = iv->getLowerBound();
        int ub = iv->getUpperBound();


        GS->createGecodeVariable(lb, ub);
    }
    GS->createArray();
    simpleRelax(timesRelaxed);
    //        IntVars = relaxed->IntVars;
    // Brug operator = til at lave det gamle space til det nye? Ellers skal det hele løses med det nye og gamle pointer skal opdateres. 
    // Hvordan fungere det så med at slette? 


}

void GeneralSolver::simpleRelax(int timesRelaxed) {
    std::cout << "SimpleRelax" << std::endl;
    double numberOfConstraints = 0;
    for (unsigned i = 1; i < model->getConstraints().size(); i++) {
        numberOfConstraints += model->getConstraintsWithPriority(i)->size();
    }

    int relax = std::ceil(std::pow(2, timesRelaxed) / 100.0 * numberOfConstraints);
    int keep = std::max(numberOfConstraints - relax, 0.0);
    int posted = 0;

    //        std::cout << "do nothing" << std::endl;
    //        std::cout << model->getObjectives()->at(0)->getInvariant()->VariablePointers->size() << std::endl;
    for (unsigned i = 1; i < model->getConstraints().size()/*-((timesRelaxed+1)*100)*/; i++) {

        //        for ( std::vector<std::shared_ptr < Constraint>>* prio : *st->getConstraints()) {
        constraintContainer prio = model->getConstraintsWithPriority(i);
        //            for (unsigned j = 0; j < prio->size(); j++) {
        for (constraint cons : *prio) {
            if (posted == keep) {
                break;
            }
            //                std::shared_ptr<Invariant> invar = cons->getInvariant();
            //                Invariant* invar = cons->getInvariant();
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
                posted++;
            } else {
                std::cout << "type should be LINEAR and assert should prevent this. Then type is set to " << cons->getType() << std::endl;
            }
        }
    }
    std::cout << posted << std::endl;
    std::cout << keep << std::endl;

    std::cout << "SimpleRelax made " << timesRelaxed + 1 << " times" << std::endl;
}

std::vector<int>& GeneralSolver::getInitialValue() {
    return model->getInitialEvaluation();
}

void GeneralSolver::optimizeSolution(int time) {
    LS->optimizeSolution(time);
}
// Only for testing

//    Gecode::IntVarArray * getIntVars() {
//        return &IntVars;
//    }

void GeneralSolver::printCurrent() {
    if (model->getNonFixedVariables().size() > 0) {
        std::cout << "Integer Variables:" << std::endl;
    }

    for (unsigned i = 0; i < model->getNonFixedVariables().size(); i++) {
        std::cout << model->getNonFixedVariables().at(i)->getCurrentValue() << " ";
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
