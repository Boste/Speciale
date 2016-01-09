
#include <gecode/int.hh>

#include "GecodeSolver.hpp"

GecodeSolver::GecodeSolver(std::shared_ptr<Model> model) {
    this->model = model;
    tmpVars = Gecode::IntVarArgs();
    //    IntVars = new std::vector<Gecode::IntVarArgs*>();

}

GecodeSolver::~GecodeSolver() {

}

void GecodeSolver::linear(std::vector<int>& coefficients, const std::vector<Variable*>& variables, int relation, int upperbound) {
    //    IntVars = IntVarArray(*this, tmpVars);

    if (variables.size() == 1 && coefficients[0] == 1) {
        IntConLevel icl = Gecode::ICL_DOM;
        IntVar x = tmpVars[variables[0]->getID()];
        //        IntVarArgs x(1)
        //        x[0] = tmpVars[variables[0]->getID()];
        //        std::cout << "Dom constraint " << std::endl;
        switch (relation) {
            case EQ:
                Gecode::dom(*this, x, upperbound, icl); //Gecode::ICL_BND);
                break;
            case LQ:
                Gecode::dom(*this, x, x.min(), upperbound, icl); //Gecode::ICL_BND);
                break;
            case LE:
                Gecode::dom(*this, x, x.min(), upperbound - 1, icl); //Gecode::ICL_BND);
                break;
            case GQ:
                Gecode::dom(*this, x, upperbound, x.max(), icl); //Gecode::ICL_BND);
                break;
            case GR:
                Gecode::dom(*this, x, upperbound - 1, x.max(), icl); //Gecode::ICL_BND);
                break;
        }

    } else {

        Gecode::IntArgs c(coefficients.size());
        Gecode::IntVarArgs x(coefficients.size());
        assert(coefficients.size() == variables.size());
        //    if(variables.size() == 0){

        //    }
        for (unsigned j = 0; j < coefficients.size(); j++) {

            c[j] = coefficients.at(j);
            //        x[j] = *(variables->at(j)->getVariablePointer());
            x[j] = tmpVars[variables.at(j)->getID()];
        }

        //    std::cout << tmpVars << std::endl;
        //    sleep(1);
        // ICL_VAL is cheapest ICL_DOM is most expensive
        //    std::cout << "Giv noget til gecode" << std::endl;
        bool integer = false;
        for (Variable* iv : variables) {
            if (tmpVars[iv->getID()].max() > 1) {
                integer = true;
                break;
            }
        }

        IntConLevel icl;
        if (integer) {
            icl = Gecode::ICL_VAL;
        } else {
            icl = Gecode::ICL_DOM;
        }
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
        //    std::cout << "Gecode likes it" << std::endl;
    }
}

//void GecodeSolver::createGecodeVariables(std::shared_ptr<Model> model) {
////    std::cout << "create variables " << std::endl;
////    std::cout << model->getNumberOfVariables() << std::endl;
//    IntVars = Gecode::IntVarArray(*this, model->getAllIntegerVariables()->size(), 0, 1);
//    model->updateIntegerVariables(IntVars);
//    
//
//}

void GecodeSolver::createGecodeVariable(int lb, int ub) {
    //    std::cout << "lb " << lb << " ub " << ub << std::endl;
    //    if (ub > 2) {
    //        std::cout << ub << std::endl;
    //    }
    //    if (ub > Gecode::Int::Limits::max) {
    //
    //        std::cout << "Changed upperbound from " << ub << " to " << Gecode::Int::Limits::max << std::endl;
    //
    //        ub = Gecode::Int::Limits::max;
    //
    //    }
    std::shared_ptr<IntVar> GecodeVar = std::make_shared<IntVar>(*this, lb, ub);
    //    std::cout << Gecode::Int::Limits::max << std::endl;
    //    std::cout << std::numeric_limits<int>::max() << std::endl;
    if (ub == 1) {
        binVars << *GecodeVar;
    }
    tmpVars << *GecodeVar;

}

GecodeSolver::GecodeSolver(bool share, GecodeSolver & s) :
Gecode::Space(share, s) {
    //        std::cout << "this is not a line " << IntVars->size() << std::endl;
    //    for (IntVar iv : IntVars) {
    //    std::cout << "Made clone " << std::endl;
    AllVars.update(*this, share, s.AllVars);

    // remember to update your main variables!
    //		model = s.model;


}

void GecodeSolver::print(std::ostream & os) const {
    for (IntVar iv : AllVars) {
        os << iv << ", ";
    }
    os << std::endl;
}

void GecodeSolver::createArray() {
    //    std::cout << "CreateArray" << std::endl;
    AllVars = IntVarArray(*this, tmpVars);
    std::vector<Variable*>& m = model->getAllVariables();
    for (int i = 0; i < AllVars.size(); i++) {
        //        assert(m->size() == IntVars.size());
        m.at(i)->setVariablePointer(AllVars[i]);

        //            std::cout << &IntVars[i] << " ";
        //            std::cout << &tmpVars[i] << std::endl;
    }
    //    for (int j = IntVars.size() - 1; j >= 0; j--) {
    //        std::cout << &tmpVars[j] << std::endl;
    //        delete &(tmpVars[j]);
    //    }

    //    std::cout << IntVars << std::endl;

}

void GecodeSolver::branch() {

    //    for (std::vector<IntegerVariable*>& vector : model->getPriorityVectors()) {
    //        Gecode::IntVarArgs priority;
    //
    //        for (IntegerVariable* iv : vector) {
    //            //    for(int i = 0; i< model->getPriorityVectorNr(0).size();i++){
    //            priority << *iv->getVariablePointer();
    //        }
    //        Gecode::branch(*this, priority, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());
    //    Gecode::IntVarArgs fisk;
    //    for (int i = 0; i < binVars.size()-1 ; i++) {
    //        fisk << binVars[i];
    //    }
    //    Gecode::branch(*this, fisk, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());
    Gecode::branch(*this, binVars, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());
    //            Gecode::branch(*this, AllVars, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());

    //    }
    //    Gecode::branch(*this, IntVars, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());
    //    Gecode::branch(*this, binVars, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());
    //    std::cout << "branched" << std::endl;

}

void GecodeSolver::fixVariables() {
    //    std::cout << "In fix" << std::endl;
    std::vector<Variable*> preprocessed; // = new std::vector<IntegerVariable*>();

    int numberOfFixedVariables = 0;
    for (Variable* iv : model->getAllVariables()) {
        //        std::cout << iv << std::endl;
        //        std::cout << "Gecode Var pointer " << iv->getVariablePointer() << " assigned? " << std::endl;
        //        std::cout << iv->getVariablePointer()->assigned() << std::endl;
        //        std::cout << "ID " << iv->getID() << std::endl;
        //        if (!iv->isIntegerVariable()) {
        if (!iv->getVariablePointer()->assigned()) {
            preprocessed.push_back(iv);
        } else {
            numberOfFixedVariables++;
            iv->setAsFixed();
        }
        //        } else if (iv->getVariablePointer()->assigned()) {
        //            numberOfFixedVariables++;
        //            iv->setAsFixed();
        //            std::cout << "Fixed Integer variable" << std::endl;
        //        }
    }
    std::cout << "Number of variables fixed in preprocessing " << numberOfFixedVariables << std::endl;
    model->setNonFixedVariables(preprocessed);
    //    delete preprocessed;
    //        for (IntegerVariable* iv : )
    //        GS->print(cout);
}

bool GecodeSolver::initialize(int TimeForGecode, bool fix) {
    //    std::cout << IntVars << std::endl;



    //        std::cout << IntVars->size() << std::endl;
    return FindSolution(TimeForGecode, fix);

}

bool GecodeSolver::FindSolution(int TimeForGecode, bool fix) {
    //    postCovSol();
    //    this->print(std::cout);

    branch();
    //    std::cout << "TimeForGecode " << TimeForGecode << std::endl;
    //    std::cout << IntVars->size() << std::endl;
    //    sleep(5);
    //    std::shared_ptr<Gecode::Search::Options> so = std::make_shared<Search::Options>();
    //    Gecode::Search::Options* so = new Gecode::Search::Options();
    std::cout << "Gecode time " << TimeForGecode << std::endl;
    Multistop* ms = new Multistop(0, 0, TimeForGecode * 1000);
    Gecode::Search::Options* so = new Gecode::Search::Options();
    so->stop = ms;

    //    this->print(std::cout);
    printSpaceStatus();
    //    unsigned ub = 0;
    //    unsigned lb = 0;
    //    for (IntVar iv : AllVars) {
    //        if (iv.max() != 1) {
    //            if (iv.max() != 2147483646) {
    //                ub++;
    //                //                std::cout << iv->getUpperBound() << std::endl;
    //            }
    //        }
    //        if (iv.min() != 0) {
    //            lb++;
    //            //            std::cout << iv.min() << std::endl;
    //        }
    //    }
    //    std::cout << "lb " << lb << " ub " << ub << std::endl;


    //    this->print(std::cout);
    //    for(IntVar iv : IntVars){
    //        if(iv.assigned()){
    //            std::cout << "assigned" << std::endl;
    //        }
    //    }
    if (fix) {
        fixVariables();
    }


    so->a_d = AllVars.size() - 1;
    so->c_d = AllVars.size() - 1;
    //    this->print(std::cout);
    bool solutionFound = false;
    GecodeSolver* s;
    try {
        std::clock_t GecodeClock = std::clock();
        //        std::cout << "Before search engine" << std::endl;
        Gecode::DFS<GecodeSolver> e(this, *so);
        std::cout << "Still searching for solution" << std::endl;
        s = e.next();

        std::cout << "stop called " << ms->called << " times" << std::endl;
        if (e.stopped()) {
            std::cout << "WARNING: solver stopped, solution is not optimal!\n";

            if (so->stop->stop(e.statistics(), *so)) {
                //                    cout << "\t Solver stopped because of TIME LIMIT!\n";
                //                //                    cout << "\t Solver stopped because of  NODE LIMIT!\n";
                //                std::cout << "\t Number of nodes expanded: " << e.statistics().node << std::endl;
                //                std::cout << "\t Number of failed nodes: " << e.statistics().fail << std::endl;
                //                std::cout << "\t Number of restarts: " << e.statistics().restart << std::endl;
                //                double time = (std::clock() - GecodeClock) / (double) CLOCKS_PER_SEC;
                //                std::cout << "\t Time spend searching for solution: " << time << " seconds" << std::endl;
                //

                //                SetValues(AllVars);
                solutionFound = false;
                //                std::cout << "Gecode found solution after " << (std::clock() - GecodeClock) / (double) CLOCKS_PER_SEC << std::endl;
                //                std::cout << "Total time used so far " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << std::endl;
                Gecode::Search::Statistics stat = e.statistics();
                print_stats(stat);
                double time = (std::clock() - GecodeClock) / (double) CLOCKS_PER_SEC;
                std::cout << "\t Time spend searching for solution: " << time << " seconds" << std::endl;

            }
        } else {
            //        } 
            //            s->print(std::cout);
            //            for (int i = 0; i < s->IntVars.size(); i++) {
            //                if(s->IntVars[i].val() != 0){
            //                    std::cout << i << " " << s->IntVars[i]<< " ";
            //                }

            //                std::cout << "Gecode::IntVarArgs x" << i << "(1);" << std::endl;
            //                std::cout << "x" << i << "[0] = tmpVars[" << i << "];" << std::endl;
            //                std::cout << "Gecode::linear(*this, iva, x" << i << ", Gecode::IRT_EQ, " << s->IntVars[i].val() << ", icl);" << std::endl;
            //                            Gecode::linear(*this, c, x, Gecode::IRT_EQ, upperbound, icl); //Gecode::ICL_BND);


            //            }
            //            s->print(std::cout); //            this->print(std::cout);
            SetValues(s->AllVars);
            solutionFound = true;
            std::cout << "Gecode found solution after " << (std::clock() - GecodeClock) / (double) CLOCKS_PER_SEC << std::endl;
            std::cout << "Total time used so far " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << std::endl;
            Gecode::Search::Statistics stat = e.statistics();
            print_stats(stat);

        }
        //        delete s; Skal det være her jeg deleter s?

    } catch (Gecode::Exception e) {
        std::cerr << "Gecode exception: " << e.what() << std::endl;
        //        return 1;
    }
    if (solutionFound) {
        delete s;
    }
    //    std::cout << "########################################################################################" << std::endl;
    //    std::cout << "solutionFound  "<< solutionFound << std::endl;
    delete ms;
    delete so;
    return solutionFound;
}

void GecodeSolver::SetValues(Gecode::IntVarArray vars) {
    //    for (int i = 0; i < model->getAllIntegerVariables()->size(); i++) {
    for (Variable* iv : model->getAllVariables()) {
        if (vars[iv->getID()].assigned()) {
            iv->setCurrentValue(vars[iv->getID() ].val());

            //            std::cout << std::endl;
        } else {
            //            std::cout << iv->getID() << " ";
            //            std::cout << "Value not found for variable " << iv->getID() << " , consider adding it to branch" << std::endl;
            //                        std::cout << vars[iv->getID()] << " ";
            iv->setCurrentValue(Random::Integer(vars[iv->getID()].min(), vars[iv->getID()].max()));
            //            iv->setCurrentValue(vars[iv->getID()].min());

        }
    }
}

void GecodeSolver::printSpaceStatus() {
    //    std::cout << "in status" << std::endl;
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

//void GecodeSolver::postCovSol() {
//    Gecode::IntArgs iva(1);
//    iva[0] = 1;
//    IntConLevel icl = Gecode::ICL_VAL;
//
//    Gecode::IntVarArgs x0(1);
//    x0[0] = tmpVars[0];
//    Gecode::linear(*this, iva, x0, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x1(1);
//    x1[0] = tmpVars[1];
//    Gecode::linear(*this, iva, x1, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x2(1);
//    x2[0] = tmpVars[2];
//    Gecode::linear(*this, iva, x2, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x3(1);
//    x3[0] = tmpVars[3];
//    Gecode::linear(*this, iva, x3, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x4(1);
//    x4[0] = tmpVars[4];
//    Gecode::linear(*this, iva, x4, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x5(1);
//    x5[0] = tmpVars[5];
//    Gecode::linear(*this, iva, x5, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x6(1);
//    x6[0] = tmpVars[6];
//    Gecode::linear(*this, iva, x6, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x7(1);
//    x7[0] = tmpVars[7];
//    Gecode::linear(*this, iva, x7, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x8(1);
//    x8[0] = tmpVars[8];
//    Gecode::linear(*this, iva, x8, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x9(1);
//    x9[0] = tmpVars[9];
//    Gecode::linear(*this, iva, x9, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x10(1);
//    x10[0] = tmpVars[10];
//    Gecode::linear(*this, iva, x10, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x11(1);
//    x11[0] = tmpVars[11];
//    Gecode::linear(*this, iva, x11, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x12(1);
//    x12[0] = tmpVars[12];
//    Gecode::linear(*this, iva, x12, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x13(1);
//    x13[0] = tmpVars[13];
//    Gecode::linear(*this, iva, x13, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x14(1);
//    x14[0] = tmpVars[14];
//    Gecode::linear(*this, iva, x14, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x15(1);
//    x15[0] = tmpVars[15];
//    Gecode::linear(*this, iva, x15, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x16(1);
//    x16[0] = tmpVars[16];
//    Gecode::linear(*this, iva, x16, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x17(1);
//    x17[0] = tmpVars[17];
//    Gecode::linear(*this, iva, x17, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x18(1);
//    x18[0] = tmpVars[18];
//    Gecode::linear(*this, iva, x18, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x19(1);
//    x19[0] = tmpVars[19];
//    Gecode::linear(*this, iva, x19, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x20(1);
//    x20[0] = tmpVars[20];
//    Gecode::linear(*this, iva, x20, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x21(1);
//    x21[0] = tmpVars[21];
//    Gecode::linear(*this, iva, x21, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x22(1);
//    x22[0] = tmpVars[22];
//    Gecode::linear(*this, iva, x22, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x23(1);
//    x23[0] = tmpVars[23];
//    Gecode::linear(*this, iva, x23, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x24(1);
//    x24[0] = tmpVars[24];
//    Gecode::linear(*this, iva, x24, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x25(1);
//    x25[0] = tmpVars[25];
//    Gecode::linear(*this, iva, x25, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x26(1);
//    x26[0] = tmpVars[26];
//    Gecode::linear(*this, iva, x26, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x27(1);
//    x27[0] = tmpVars[27];
//    Gecode::linear(*this, iva, x27, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x28(1);
//    x28[0] = tmpVars[28];
//    Gecode::linear(*this, iva, x28, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x29(1);
//    x29[0] = tmpVars[29];
//    Gecode::linear(*this, iva, x29, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x30(1);
//    x30[0] = tmpVars[30];
//    Gecode::linear(*this, iva, x30, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x31(1);
//    x31[0] = tmpVars[31];
//    Gecode::linear(*this, iva, x31, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x32(1);
//    x32[0] = tmpVars[32];
//    Gecode::linear(*this, iva, x32, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x33(1);
//    x33[0] = tmpVars[33];
//    Gecode::linear(*this, iva, x33, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x34(1);
//    x34[0] = tmpVars[34];
//    Gecode::linear(*this, iva, x34, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x35(1);
//    x35[0] = tmpVars[35];
//    Gecode::linear(*this, iva, x35, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x36(1);
//    x36[0] = tmpVars[36];
//    Gecode::linear(*this, iva, x36, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x37(1);
//    x37[0] = tmpVars[37];
//    Gecode::linear(*this, iva, x37, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x38(1);
//    x38[0] = tmpVars[38];
//    Gecode::linear(*this, iva, x38, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x39(1);
//    x39[0] = tmpVars[39];
//    Gecode::linear(*this, iva, x39, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x40(1);
//    x40[0] = tmpVars[40];
//    Gecode::linear(*this, iva, x40, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x41(1);
//    x41[0] = tmpVars[41];
//    Gecode::linear(*this, iva, x41, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x42(1);
//    x42[0] = tmpVars[42];
//    Gecode::linear(*this, iva, x42, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x43(1);
//    x43[0] = tmpVars[43];
//    Gecode::linear(*this, iva, x43, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x44(1);
//    x44[0] = tmpVars[44];
//    Gecode::linear(*this, iva, x44, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x45(1);
//    x45[0] = tmpVars[45];
//    Gecode::linear(*this, iva, x45, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x46(1);
//    x46[0] = tmpVars[46];
//    Gecode::linear(*this, iva, x46, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x47(1);
//    x47[0] = tmpVars[47];
//    Gecode::linear(*this, iva, x47, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x48(1);
//    x48[0] = tmpVars[48];
//    Gecode::linear(*this, iva, x48, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x49(1);
//    x49[0] = tmpVars[49];
//    Gecode::linear(*this, iva, x49, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x50(1);
//    x50[0] = tmpVars[50];
//    Gecode::linear(*this, iva, x50, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x51(1);
//    x51[0] = tmpVars[51];
//    Gecode::linear(*this, iva, x51, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x52(1);
//    x52[0] = tmpVars[52];
//    Gecode::linear(*this, iva, x52, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x53(1);
//    x53[0] = tmpVars[53];
//    Gecode::linear(*this, iva, x53, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x54(1);
//    x54[0] = tmpVars[54];
//    Gecode::linear(*this, iva, x54, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x55(1);
//    x55[0] = tmpVars[55];
//    Gecode::linear(*this, iva, x55, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x56(1);
//    x56[0] = tmpVars[56];
//    Gecode::linear(*this, iva, x56, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x57(1);
//    x57[0] = tmpVars[57];
//    Gecode::linear(*this, iva, x57, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x58(1);
//    x58[0] = tmpVars[58];
//    Gecode::linear(*this, iva, x58, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x59(1);
//    x59[0] = tmpVars[59];
//    Gecode::linear(*this, iva, x59, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x60(1);
//    x60[0] = tmpVars[60];
//    Gecode::linear(*this, iva, x60, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x61(1);
//    x61[0] = tmpVars[61];
//    Gecode::linear(*this, iva, x61, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x62(1);
//    x62[0] = tmpVars[62];
//    Gecode::linear(*this, iva, x62, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x63(1);
//    x63[0] = tmpVars[63];
//    Gecode::linear(*this, iva, x63, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x64(1);
//    x64[0] = tmpVars[64];
//    Gecode::linear(*this, iva, x64, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x65(1);
//    x65[0] = tmpVars[65];
//    Gecode::linear(*this, iva, x65, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x66(1);
//    x66[0] = tmpVars[66];
//    Gecode::linear(*this, iva, x66, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x67(1);
//    x67[0] = tmpVars[67];
//    Gecode::linear(*this, iva, x67, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x68(1);
//    x68[0] = tmpVars[68];
//    Gecode::linear(*this, iva, x68, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x69(1);
//    x69[0] = tmpVars[69];
//    Gecode::linear(*this, iva, x69, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x70(1);
//    x70[0] = tmpVars[70];
//    Gecode::linear(*this, iva, x70, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x71(1);
//    x71[0] = tmpVars[71];
//    Gecode::linear(*this, iva, x71, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x72(1);
//    x72[0] = tmpVars[72];
//    Gecode::linear(*this, iva, x72, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x73(1);
//    x73[0] = tmpVars[73];
//    Gecode::linear(*this, iva, x73, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x74(1);
//    x74[0] = tmpVars[74];
//    Gecode::linear(*this, iva, x74, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x75(1);
//    x75[0] = tmpVars[75];
//    Gecode::linear(*this, iva, x75, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x76(1);
//    x76[0] = tmpVars[76];
//    Gecode::linear(*this, iva, x76, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x77(1);
//    x77[0] = tmpVars[77];
//    Gecode::linear(*this, iva, x77, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x78(1);
//    x78[0] = tmpVars[78];
//    Gecode::linear(*this, iva, x78, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x79(1);
//    x79[0] = tmpVars[79];
//    Gecode::linear(*this, iva, x79, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x80(1);
//    x80[0] = tmpVars[80];
//    Gecode::linear(*this, iva, x80, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x81(1);
//    x81[0] = tmpVars[81];
//    Gecode::linear(*this, iva, x81, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x82(1);
//    x82[0] = tmpVars[82];
//    Gecode::linear(*this, iva, x82, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x83(1);
//    x83[0] = tmpVars[83];
//    Gecode::linear(*this, iva, x83, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x84(1);
//    x84[0] = tmpVars[84];
//    Gecode::linear(*this, iva, x84, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x85(1);
//    x85[0] = tmpVars[85];
//    Gecode::linear(*this, iva, x85, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x86(1);
//    x86[0] = tmpVars[86];
//    Gecode::linear(*this, iva, x86, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x87(1);
//    x87[0] = tmpVars[87];
//    Gecode::linear(*this, iva, x87, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x88(1);
//    x88[0] = tmpVars[88];
//    Gecode::linear(*this, iva, x88, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x89(1);
//    x89[0] = tmpVars[89];
//    Gecode::linear(*this, iva, x89, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x90(1);
//    x90[0] = tmpVars[90];
//    Gecode::linear(*this, iva, x90, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x91(1);
//    x91[0] = tmpVars[91];
//    Gecode::linear(*this, iva, x91, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x92(1);
//    x92[0] = tmpVars[92];
//    Gecode::linear(*this, iva, x92, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x93(1);
//    x93[0] = tmpVars[93];
//    Gecode::linear(*this, iva, x93, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x94(1);
//    x94[0] = tmpVars[94];
//    Gecode::linear(*this, iva, x94, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x95(1);
//    x95[0] = tmpVars[95];
//    Gecode::linear(*this, iva, x95, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x96(1);
//    x96[0] = tmpVars[96];
//    Gecode::linear(*this, iva, x96, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x97(1);
//    x97[0] = tmpVars[97];
//    Gecode::linear(*this, iva, x97, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x98(1);
//    x98[0] = tmpVars[98];
//    Gecode::linear(*this, iva, x98, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x99(1);
//    x99[0] = tmpVars[99];
//    Gecode::linear(*this, iva, x99, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x100(1);
//    x100[0] = tmpVars[100];
//    Gecode::linear(*this, iva, x100, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x101(1);
//    x101[0] = tmpVars[101];
//    Gecode::linear(*this, iva, x101, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x102(1);
//    x102[0] = tmpVars[102];
//    Gecode::linear(*this, iva, x102, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x103(1);
//    x103[0] = tmpVars[103];
//    Gecode::linear(*this, iva, x103, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x104(1);
//    x104[0] = tmpVars[104];
//    Gecode::linear(*this, iva, x104, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x105(1);
//    x105[0] = tmpVars[105];
//    Gecode::linear(*this, iva, x105, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x106(1);
//    x106[0] = tmpVars[106];
//    Gecode::linear(*this, iva, x106, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x107(1);
//    x107[0] = tmpVars[107];
//    Gecode::linear(*this, iva, x107, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x108(1);
//    x108[0] = tmpVars[108];
//    Gecode::linear(*this, iva, x108, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x109(1);
//    x109[0] = tmpVars[109];
//    Gecode::linear(*this, iva, x109, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x110(1);
//    x110[0] = tmpVars[110];
//    Gecode::linear(*this, iva, x110, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x111(1);
//    x111[0] = tmpVars[111];
//    Gecode::linear(*this, iva, x111, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x112(1);
//    x112[0] = tmpVars[112];
//    Gecode::linear(*this, iva, x112, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x113(1);
//    x113[0] = tmpVars[113];
//    Gecode::linear(*this, iva, x113, Gecode::IRT_EQ, 0, icl);
//    Gecode::IntVarArgs x114(1);
//    x114[0] = tmpVars[114];
//    Gecode::linear(*this, iva, x114, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x115(1);
//    x115[0] = tmpVars[115];
//    Gecode::linear(*this, iva, x115, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x116(1);
//    x116[0] = tmpVars[116];
//    Gecode::linear(*this, iva, x116, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x117(1);
//    x117[0] = tmpVars[117];
//    Gecode::linear(*this, iva, x117, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x118(1);
//    x118[0] = tmpVars[118];
//    Gecode::linear(*this, iva, x118, Gecode::IRT_EQ, 1, icl);
//    Gecode::IntVarArgs x119(1);
//    x119[0] = tmpVars[119];
//    Gecode::linear(*this, iva, x119, Gecode::IRT_EQ, 1, icl);
//
//}