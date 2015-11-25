
#include <gecode/driver.hh>
#include <gecode/int.hh>
//#include <gecode/minimodel.hh>
//#include <gecode/float.hh>
#include "BP_Data.hpp"
#include <cmath>
#include <algorithm>
//#include <iomanip>
#include <limits>
#include "BPSolver.hpp"
#include "Clock.hpp"
#include "getRSS.hpp"
#include "GeneralSolver.hpp"
#include "GecodeSolver.hpp"
#include "Test.hpp"
#include "Multistop.hpp"
//#include <boost/algorithm/string.hpp>
//#include "GecodeLinear.hpp"

//#include "BP_Helpers.hpp"

using namespace Gecode;
//using namespace EasyLocal::Debug;


//void print_stats(Search::Statistics &stat) {
//    cout << "\tfail: " << stat.fail << endl;
//    cout << "\tnodes: " << stat.node << endl;
//    cout << "\tpropagators: " << stat.propagate << endl; // see page 145 MPG
//    cout << "\tdepth: " << stat.depth << endl;
//    cout << "\trestarts: " << stat.restart << endl;
//    cout << "\tnogoods: " << stat.nogood << endl;
//}



//void gecode(int argc, char* argv[]);
//void easylocal(int argc, const char* argv[]);

int main(int argc, char* argv[]) {


    

    std::vector<std::string> strs;

    //    string str = argv[1];
//    boost::split(strs, argv[1], boost::is_any_of("/"));
//    string name = strs.back();
//    std::cout << name << std::endl;
    int time;
    if (argc >= 3) {
        time = std::atoi(argv[2]);
    } else {
        std::cout << "time not set. Set optimization time to 20 seconds" << std::endl;
        time = 20;
    }
    int TimeForGecode;
    if (argc >= 4) {
        TimeForGecode = std::atoi(argv[3]);
    } else {
        TimeForGecode = 10;
        std::cout << "Time for Gecode not given (arg 3) time set to 10 sec" << std::endl;
    }
    Random::Seed(RANDOMSEED);
    std::cout <<  "Random seed " << Random::seed << std::endl;
    //    gecode(argc, argv);
    Clock::globalClock = std::clock();
    //    InstanceOptions opt("BPSolver");
    //    opt.instance("data/toy.txt");
    //    opt.time(180 * 1000); // in milliseconds
    //    std::cout << opt.time() << std::endl;

    //    opt.parse(argc, argv);


    BP_Input* input = new BP_Input(argv[1]);
    std::cout << "Instance read after " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << " seconds" << std::endl;
    size_t peakSize4 = getPeakRSS();
    std::cout << "Peak memory usage for reading instance " << (double) peakSize4 / 1024 / 1024 << " mb" << std::endl;
    //    Search::Options* so = new Gecode::Search::Options();

    // Let's start the Timer
    Support::Timer t;
    t.start();
    //    Script::run<MagicSquare, DFS, InstanceOptions>(opt);
    //    size_t peakSize3 = getPeakRSS();
    //    std::cout << "Peak memory usage for pure gecode " << (double) peakSize3 / 1024 / 1024 << " mb" << std::endl;
    //    Test* test = new Test();
    //    delete test;  
    
//    BPSolver* userModel = new BPSolver(input);
    BPSolver userModel(input);
    std::cout << "Initialize solution" << std::endl;
    //Need my own option class

    //    so->stop = ms;


    //    so->stop = new Multistop(1, 1, TimeForGecode*1000);
    //    GeneralSolver* GS = m->InitialSolution(so);
    userModel.InitialSolution(TimeForGecode);

    //    assert(!GS.stopped());
    double iniTime = (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC;
    size_t peakSize2 = getPeakRSS();
    std::cout << "Peak memory usage for Gecode " << (double) peakSize2 / 1024 / 1024 << " mb" << std::endl;
    //    std::cout << "Initializing LSS" << std::endl;
    //    m->initializeLS(GS);

    //    m->printCurrent();

    std::cout << "LS solver initialized after " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << " seconds" << std::endl;
    //    m->printCurrent();

    std::cout << "\n ######################################################################################################### " << std::endl;
    std::cout << "BEGINING LOCALSEARCH \n "
            "######################################################################################################### \n" << std::endl;


    //    m->printCurrent();

    //    userModel->initializeLS();
    userModel.optimizeSolution(time);
//    std::cout << userModel->getInitial() << " "; // value of solution gecode found
    std::cout << iniTime << " "; // time for initializing problem 
    std::cout << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << " "; // total time usage


    //    string str = argv[1];
    //    for (int i = 0; i < str.length(); i++) {
    //        if (str[i] == '/')
    //            str[i] = ' ';
    //    }
    //    std::vector<string> array;
    //    stringstream ss(str);
    //    while (ss >> temp)
    //        array.push_back(temp);
    std::cout << getPeakRSS() / 1024 / 1024; // peak memory in mb
//    std::cout << " " << name; // Instance name
    std::cout << std::endl;
    // Output: improved sol; initial sol; time gecode used to find sol; peak memory use in mb; instance name
    std::cout << "solution, time Gecode used, total time, peak memory in mb, " << std::endl;


    //    size_t peakSize = getPeakRSS();
    //    std::cout << "Peak memory usage " << (double) peakSize / 1024 / 1024 << " mb" << std::endl;
    //    std::cout << "Total run time " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << " seconds" << std::endl;
//    delete userModel;
    delete input;

    //    m->printCurrent();



    //    // Test big 
    //    int vars = 1 * 100;
    //    int cons = 200000;
    //    GeneralSolver* GS = new GeneralSolver();
    //    std::vector<IntegerVariable*>* varInt = GS->createIntVars(vars, 0, 1);
    //    for (int i = 0; i < cons; i++) {
    //        vector<int>* c = new vector<int>(vars);
    //        vector<IntegerVariable*>* x = new vector<IntegerVariable*>();
    //        for (int j = 0; j < vars; j++) {
    //            c->at(j) = 1;
    //            x->push_back(varInt->at(j));
    //        }
    //        int upperbound = vars/2;
    //        GS->linear(*GS, c, x, Gecode::IRT_LQ, upperbound, Gecode::ICL_DOM, 2);
    ////        GS->linear(*GS, c, x, Gecode::IRT_GQ, 50, Gecode::ICL_DOM, 2);
    //        // deleter den også pointer inden i vector?
    //        delete x;
    //        delete c;
    //    }
    //    // Add objective function
    //    vector<int>* c = new vector<int>(varInt->size());
    //    for (unsigned i = 0; i < varInt->size(); i++) {
    //        c->at(i) = -1;
    //    }
    //    GS->linear(*GS, c, varInt, Gecode::IRT_LQ, 0, Gecode::ICL_DOM, 1);
    //    delete c;
    //    // Branch
    //    GS->branch(*GS, varInt, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());
    //    
    //    GeneralSolver* General = GS->InitialSolution(so);
    //    size_t peakSize2 = getPeakRSS();
    //    std::cout << "Peak memory usage for gecode " << (double) peakSize2 / 1024 / 1024 << " mb" << std::endl;
    //    std::cout << "Initializing LSS" << std::endl;
    //    GS->initializeLS(General);
    //    std::cout << "LS solver initialized after " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << " seconds" << std::endl;
    //    GS->optimizeSolution();
    //    size_t peakSize = getPeakRSS();
    //    std::cout << "Peak memory usage " << (double) peakSize / 1024 / 1024 << " mb" << std::endl;
    //    std::cout << "Total run time " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << " seconds" << std::endl;
    //    delete GS;


    //    const char* argv2[argc];
    //    for(int i = 0; i< argc; i++){
    //        argv2[i] = argv[i];
    //    }
    //    easylocal(argc, argv2);
    //    delete ms;
    //    delete so;
    //    delete p;
    return 0;
}

/*
GeneralSolver* userModel = new GeneralSolver();
    for (var intVar : input->getVars()) {
        //            counter++;
        //            std::cout << "adding variable number " << counter << std::endl;
        userModel->createIntVar(intVar.lb, intVar.ub);
    }
    vector<IntegerVariable*>* varInt = userModel->getBinaryVariables();
    //        for (unsigned i = 0; i < varInt->size(); i++) {
    //            assert(varInt->at(i)->getID() == i);
    //        }
    //        std::vector<IntegerVariable*>* varInt = GeneralSolver::createIntVars(in->getNvars(), 0, 1);
    for (unsigned i = 0; i < input->getNcons(); i++) {
        const std::vector<elem> leftside = input->getMatcoeff(i);
        bounds b = input->getBterms(i);
        vector<int> c(leftside.size());

        vector<IntegerVariable*> x(leftside.size());
        //        vector<IntegerVariable*>* x = new vector<IntegerVariable*>();

        for (unsigned j = 0; j < leftside.size(); j++) {
            elem e = leftside[j];
            c.at(j) = static_cast<int> (e.coeff);
            //            x->push_back(varInt->at(e.index));
            x.at(j) = varInt->at(e.index);
        }
        int upperbound = static_cast<int> (b.ub);
        if (b.type == 5) {
            //                GeneralSolver::linear(*this, c, x, EQ, upperbound, Gecode::ICL_DOM, HARD);
            userModel->linear(c, x, EQ, upperbound, HARD);
        } else {
            //                GeneralSolver::linear(*this, c, x, LQ, upperbound, Gecode::ICL_DOM, HARD);
            userModel->linear(c, x, LQ, upperbound, HARD);
        }
        //            delete x;
        // deleter den også pointer inden i vector?
        //            delete x;
        //            delete c;
        //            this->print(std::cout);
        //            std::cout << std::endl;
        //            sleep(1);
    }

    // Add objective function
    vector<int> c(varInt->size());
    for (unsigned i = 0; i < varInt->size(); i++) {
        c.at(i) = static_cast<int> (input->getVar(i).objcoeff);
    }

    userModel->linear(c, *varInt, LQ, 0, OBJ);
 */