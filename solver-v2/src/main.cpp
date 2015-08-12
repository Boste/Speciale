
#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
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
#include "Test.hpp"
#include "Multistop.hpp"
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
    string str = argv[1];
    for (unsigned i = 0; i < str.length(); i++) {
        if (str[i] == '/')
            str[i] = ' ';
    }
    std::vector<string> array;
    stringstream ss(str);
    string temp;
    while (ss >> temp)
        array.push_back(temp);
    std::cout << array.back() << std::endl;
    int time;
    if (argc >= 3) {
        time = std::atoi(argv[2]);
    } else {
        std::cout << "time not set. Set optimization time to 10 seconds" << std::endl;
        time = 20;
    }
    int TimeForGecode;
    if(argc>=4){
        TimeForGecode = std::atoi(argv[3]);
    } else {
        TimeForGecode = 300;
    }
    Random::Seed(RANDOMSEED);
    //    gecode(argc, argv);
    Clock::globalClock = std::clock();
//    InstanceOptions opt("BPSolver");
//    opt.instance("data/toy.txt");
    //    opt.time(180 * 1000); // in milliseconds
    //    std::cout << opt.time() << std::endl;

//    opt.parse(argc, argv);
    
    
    BP_Input* p = new BP_Input(argv[1]);
    std::cout << "Instance read after " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << " seconds" << std::endl;
    size_t peakSize4 = getPeakRSS();
    std::cout << "Peak memory usage for reading instance " << (double) peakSize4 / 1024 / 1024 << " mb" << std::endl;
    Search::Options* so = new Gecode::Search::Options();


    // Let's start the Timer
    Support::Timer t;
    t.start();

    //    Script::run<MagicSquare, DFS, InstanceOptions>(opt);
    //    size_t peakSize3 = getPeakRSS();
    //    std::cout << "Peak memory usage for pure gecode " << (double) peakSize3 / 1024 / 1024 << " mb" << std::endl;
    //    Test* test = new Test();
    //    delete test;

    BPSolver* m = new BPSolver(p);
    std::cout << "Initialize solution" << std::endl;
    so->stop = new Multistop(0, 1, TimeForGecode*1000);
//    so->stop = new Multistop(1, 1, TimeForGecode*1000);
    GeneralSolver* GS = m->InitialSolution(so); 
    //    assert(!GS.stopped());
    double iniTime = (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC;
    size_t peakSize2 = getPeakRSS();
    std::cout << "Peak memory usage for gecode " << (double) peakSize2 / 1024 / 1024 << " mb" << std::endl;
    //    std::cout << "Initializing LSS" << std::endl;
    m->initializeLS(GS);
    
    m->printCurrent();
    
    std::cout << "LS solver initialized after " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << " seconds" << std::endl;
    //    m->printCurrent();
    m->optimizeSolution(time);
    std::cout << m->getInitialValue() << " ";
    std::cout << iniTime << " ";


    //    string str = argv[1];
    //    for (int i = 0; i < str.length(); i++) {
    //        if (str[i] == '/')
    //            str[i] = ' ';
    //    }
    //    std::vector<string> array;
    //    stringstream ss(str);
    //    while (ss >> temp)
    //        array.push_back(temp);
    std::cout << getPeakRSS() / 1024 / 1024;
    //    std::cout << " " << array.back();
    std::cout << std::endl;

    // Output: improved sol; initial sol; time gecode used to find sol; peak memory use in mb; instance name



    //    size_t peakSize = getPeakRSS();
    //    std::cout << "Peak memory usage " << (double) peakSize / 1024 / 1024 << " mb" << std::endl;
    //    std::cout << "Total run time " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << " seconds" << std::endl;
    delete m;

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
    delete so;
    //    delete p;
    return 0;
}


