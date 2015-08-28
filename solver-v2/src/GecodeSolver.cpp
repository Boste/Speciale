#include "GecodeSolver.hpp"
#include "State.hpp"

GecodeSolver::GecodeSolver(std::shared_ptr<State> st) {
    this->st = st;

}

GecodeSolver::~GecodeSolver() {
    
}

void GecodeSolver::linear(std::vector<int>& coefficients, std::vector<IntegerVariable*>* variables, int relation, int upperbound) {
    Gecode::IntArgs c(coefficients.size());
    Gecode::IntVarArgs x(coefficients.size());
    for (unsigned j = 0; j < coefficients.size(); j++) {
        c[j] = coefficients.at(j);
        x[j] = *(variables->at(j)->getVariablePointer());
    }


    switch (relation) {
        case EQ:
            Gecode::linear(*this, c, x, Gecode::IRT_EQ, upperbound, Gecode::ICL_DOM);
            break;
        case LQ:
            Gecode::linear(*this, c, x, Gecode::IRT_LQ, upperbound, Gecode::ICL_DOM);
            break;
        case LE:
            Gecode::linear(*this, c, x, Gecode::IRT_LE, upperbound, Gecode::ICL_DOM);
            break;
        case GQ:
            Gecode::linear(*this, c, x, Gecode::IRT_GQ, upperbound, Gecode::ICL_DOM);
            break;
        case GR:
            Gecode::linear(*this, c, x, Gecode::IRT_GR, upperbound, Gecode::ICL_DOM);
            break;
    }
}

void GecodeSolver::createGecodeVariables(std::shared_ptr<State> st) {
    IntVars = Gecode::IntVarArray(*this, st->getNumberOfVariables(), 0, 1);
    st->updateIntegerVariables(IntVars);

}

GecodeSolver::GecodeSolver(bool share, GecodeSolver & s) :
Gecode::Space(share, s) {
    //        std::cout << "this is not a line " << IntVars.size() << std::endl;
    IntVars.update(*this, share, s.IntVars);
    // remember to update your main variables!
    //		model = s.model;


}

void GecodeSolver::print(std::ostream & os) const {
    os << IntVars << std::endl;
}


void GecodeSolver::branch(bool fix){
    Gecode::branch(*this, IntVars, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());
    if (fix) {
        std::cout << "Should fix those variables that is fixed be preprocessing" << std::endl;
    }
}
bool GecodeSolver::initialize() {
    
    
    
    
    //        std::cout << IntVars.size() << std::endl;
    return FindSolution();

}

bool GecodeSolver::FindSolution() {

    std::shared_ptr<Gecode::Search::Options> so = std::make_shared<Search::Options>();
    printSpaceStatus();

    GecodeSolver* s;
    try {
        std::clock_t GecodeClock = std::clock();
        Gecode::DFS<GecodeSolver> e(this, *so);
        std::cout << "Still searching for solution" << std::endl;
        s = e.next();
        if (e.stopped()) {
            std::cout << "WARNING: solver stopped, solution is not optimal!\n";

            if (so->stop->stop(e.statistics(), *so)) {
                //                    cout << "\t Solver stopped because of TIME LIMIT!\n";
                //                    cout << "\t Solver stopped because of  NODE LIMIT!\n";
                std::cout << "\t Number of nodes expanded: " << e.statistics().node << std::endl;
                std::cout << "\t Number of failed nodes: " << e.statistics().fail << std::endl;
                std::cout << "\t Number of restarts: " << e.statistics().restart << std::endl;
                double time = (std::clock() - GecodeClock) / (double) CLOCKS_PER_SEC;
                std::cout << "\t Time spend searching for solution: " << time << " seconds" << std::endl;
            }
        } else {
            //            s->print(std::cout);
            //            this->print(std::cout);
            SetValues(s->IntVars);
            std::cout << "Gecode found solution after " << (std::clock() - GecodeClock) / (double) CLOCKS_PER_SEC << std::endl;
            std::cout << "Total time used so far " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << std::endl;
            Gecode::Search::Statistics stat = e.statistics();
            print_stats(stat);

        }

    } catch (Gecode::Exception e) {
        std::cerr << "Gecode exception: " << e.what() << std::endl;
        //        return 1;
    }
    return s;
}

void GecodeSolver::SetValues(Gecode::IntVarArray& vars) {
    for (int i = 0; i < st->getNumberOfVariables(); i++) {
        if (vars[i].assigned()) {
            st->getIntegerVariables()->at(i)->setCurrentValue(vars[i].val());
        } else {
            st->getIntegerVariables()->at(i)->setCurrentValue(vars[i].min());

        }
    }
}

void GecodeSolver::printSpaceStatus() {
    std::cout << "in status" << std::endl;
    Gecode::SpaceStatus status = this->status();
    if (status == Gecode::SS_FAILED) {
        std::cout << "Status: " << this->status() << " the space is failed at root."
                << std::endl;
        std::cout << "No feasible solution" << std::endl;
        //            exit(1);
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

void GecodeSolver::print_stats(Gecode::Search::Statistics & stat) {
    std::cout << "\tfail: " << stat.fail << std::endl;
    std::cout << "\tnodes: " << stat.node << std::endl;
    std::cout << "\tpropagators: " << stat.propagate << std::endl; // see page 145 MPG
    std::cout << "\tdepth: " << stat.depth << std::endl;
    std::cout << "\trestarts: " << stat.restart << std::endl;
    std::cout << "\tnogoods: " << stat.nogood << std::endl;
}
// Copy during cloning

Gecode::Space * GecodeSolver::copy(bool share) {
    //        std::cout << "copy" << std::endl;
    return new GecodeSolver(share, *this);
}
