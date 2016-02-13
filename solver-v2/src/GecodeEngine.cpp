

#include <gecode/int.hh>

#include "GecodeEngine.hpp"

GecodeEngine::GecodeEngine(std::shared_ptr<Storage> model) {
    this->model = model;
    tmpVars = Gecode::BoolVarArgs();

}

GecodeEngine::~GecodeEngine() {

}

void GecodeEngine::linear(std::vector<int>& coefficients, const std::vector<Variable*>& variables, int relation, int upperbound) {

    Gecode::IntArgs c(coefficients.size());
    Gecode::BoolVarArgs x(coefficients.size());
    assert(coefficients.size() == variables.size());
    for (unsigned j = 0; j < coefficients.size(); j++) {

        c[j] = coefficients.at(j);
        x[j] = tmpVars[variables.at(j)->getID()];
    }

    IntConLevel icl;
    icl = Gecode::ICL_DOM;
    switch (relation) {
        case EQ:
            Gecode::linear(*this, c, x, Gecode::IRT_EQ, upperbound, icl); //Gecode::ICL_BND);
            break;
        case LQ:
            Gecode::linear(*this, c, x, Gecode::IRT_LQ, upperbound, icl); //Gecode::ICL_BND);
            break;
        case LE:
            Gecode::linear(*this, c, x, Gecode::IRT_LE, upperbound, icl); //Gecode::ICL_BND);
            break;
        case GQ:
            Gecode::linear(*this, c, x, Gecode::IRT_GQ, upperbound, icl); //Gecode::ICL_BND);
            break;
        case GR:
            Gecode::linear(*this, c, x, Gecode::IRT_GR, upperbound, icl); //Gecode::ICL_BND);
            break;
    }
}

void GecodeEngine::createGecodeVariable(int lb, int ub) {
    std::shared_ptr<BoolVar> GecodeVar = std::make_shared<BoolVar>(*this, lb, ub);
    tmpVars << *GecodeVar;

}

GecodeEngine::GecodeEngine(bool share, GecodeEngine & s) :
Gecode::Space(share, s) {
    AllVars.update(*this, share, s.AllVars);

}

void GecodeEngine::print(std::ostream & os) const {
    for (BoolVar iv : AllVars) {
        os << iv << ", ";
    }
    os << std::endl;
}

void GecodeEngine::createArray() {
    AllVars = BoolVarArray(*this, tmpVars);


    std::vector<Variable*>& m = model->getAllVariables();
    for (int i = 0; i < AllVars.size(); i++) {
        m.at(i)->setVariablePointer(AllVars[i]);
    }

}

void GecodeEngine::fixVariables() {
    std::vector<Variable*> preprocessed; // = new std::vector<IntegerVariable*>();

    int numberOfFixedVariables = 0;
    for (Variable* iv : model->getAllVariables()) {
        if (!iv->getVariablePointer()->assigned()) {
            preprocessed.push_back(iv);
        } else {
            numberOfFixedVariables++;

            iv->setAsFixed();
        }
    }
    std::cout << "Number of variables fixed in preprocessing " << numberOfFixedVariables << std::endl;
    model->setNonFixedVariables(preprocessed);
}

bool GecodeEngine::findSolution(int TimeForGecode, bool fix) {
    Gecode::branch(*this, AllVars, Gecode::INT_VAR_DEGREE_MAX(), INT_VAL_MIN());
    std::cout << "Gecode time " << TimeForGecode << std::endl;
    Multistop* ms = new Multistop(0, 1, TimeForGecode * 1000);
    Gecode::Search::Options* so = new Gecode::Search::Options();
    so->stop = ms;
    std::cout << "Stuck in Status? " << std::endl;
    printSpaceStatus();
    if (fix) {
        fixVariables();
    }
    so->a_d = AllVars.size(); // Default 2 
    so->c_d = AllVars.size(); // Default 8
    //    this->print(std::cout);
    bool solutionFound = false;
    GecodeEngine* s;
    try {
        std::clock_t GecodeClock = std::clock();
        Gecode::DFS<GecodeEngine> e(this, *so);
        std::cout << "Searching for solution...." << std::endl;
        s = e.next();

        std::cout << "stop called " << ms->called << " times" << std::endl;
        if (e.stopped()) {
            std::cout << "WARNING: solver stopped, solution is not optimal!\n";

            if (so->stop->stop(e.statistics(), *so)) {
                solutionFound = false;
                Gecode::Search::Statistics stat = e.statistics();
                print_stats(stat);
                double time = (std::clock() - GecodeClock) / (double) CLOCKS_PER_SEC;
                std::cout << "\tTime spend searching for solution: " << time << " seconds" << std::endl;

//                std::cout << "## fail " << e.statistics().fail << std::endl;

            }
        } else {

            solutionFound = true;

            std::cout << "Gecode found solution after " << (std::clock() - GecodeClock) / (double) CLOCKS_PER_SEC << std::endl;
            Gecode::Search::Statistics stat = e.statistics();
            print_stats(stat);
            SetValues(s->AllVars);

        }

    } catch (Gecode::Exception e) {
        std::cerr << "Gecode exception: " << e.what() << std::endl;
    }
    if (solutionFound) {
        delete s;
        if (fix) {

        }
    }
    delete ms;
    delete so;
    return solutionFound;
}

void GecodeEngine::SetValues(Gecode::BoolVarArray vars) {
    for (Variable* iv : model->getAllVariables()) {
        if (vars[iv->getID()].assigned()) {
            iv->setCurrentValue(vars[iv->getID() ].val());
        } else {
            iv->setCurrentValue(Random::Integer(0, 1));

        }
    }
}

void GecodeEngine::randomInitialize() {
    for (Variable* var : model->getAllVariables()) {
        if (AllVars[var->getID()].assigned()) {
            var->setCurrentValue(AllVars[var->getID() ].val());
        } else {
            var->setCurrentValue(Random::Integer(0, 1));

        }
    }

}

void GecodeEngine::printSpaceStatus() {
    Gecode::SpaceStatus status = this->status();
    if (status == Gecode::SS_FAILED) {
        std::cout << "Status: " << this->status() << " the space is failed at root."
                << std::endl;
        std::cout << "No feasible solution" << std::endl;
        exit(1);
    } else if (status == Gecode::SS_SOLVED) {
        std::cout << "Status: " << this->status()
                << " the space is not failed but the space has no brancher left."
                << std::endl;
    } else if (status == Gecode::SS_BRANCH) {
        std::cout << "Status: " << this->status()
                << " the space is not failed and we need to start branching."
                << std::endl;
    }

}

void GecodeEngine::print_stats(Gecode::Search::Statistics & stat) {
    std::cout << "\tfail: " << stat.fail << std::endl;
    std::cout << "\tnodes: " << stat.node << std::endl;
    std::cout << "\tpropagators: " << stat.propagate << std::endl; // see page 145 MPG
    std::cout << "\tdepth: " << stat.depth << std::endl;
    std::cout << "\trestarts: " << stat.restart << std::endl;
    std::cout << "\tnogoods: " << stat.nogood << std::endl;
}
// Copy during cloning

Gecode::Space * GecodeEngine::copy(bool share) {
    return new GecodeEngine(share, *this);
}