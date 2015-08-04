
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
    Random::Seed(RANDOMSEED);
    //    gecode(argc, argv);
    Clock::globalClock = std::clock();
    InstanceOptions opt("BPSolver");
    opt.instance("data/toy.txt");
    opt.time(180 * 1000); // in milliseconds

    opt.parse(argc, argv);
    BP_Input* p = new BP_Input(opt.instance());
    std::cout << "Instance read after " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << " seconds" << std::endl;
    size_t peakSize4 = getPeakRSS();
    std::cout << "Peak memory usage for reading instance " << (double) peakSize4 / 1024 / 1024 << " mb" << std::endl;
    //     This is to stop the search at the time limit imposed
    Search::Options so;
    Search::TimeStop* ts = new Search::TimeStop(opt.time());
    so.stop = ts;
    // Let's start the Timer
    Support::Timer t;
    t.start();

//    Script::run<MagicSquare, DFS, InstanceOptions>(opt);
//    size_t peakSize3 = getPeakRSS();
//    std::cout << "Peak memory usage for pure gecode " << (double) peakSize3 / 1024 / 1024 << " mb" << std::endl;



    //    Test* test = new Test();
    //    delete test;

    BPSolver* m = new BPSolver(p);
    GeneralSolver* GS = m->InitialSolution(so); // problem at sende opt med da den bliver slettet efter kald. 

    size_t peakSize2 = getPeakRSS();
    std::cout << "Peak memory usage for gecode " << (double) peakSize2 / 1024 / 1024 << " mb" << std::endl;
//    std::cout << "Initializing LSS" << std::endl;
    m->initializeLS(GS);
    std::cout << "LS solver initialized after " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << " seconds" << std::endl;
    //    m->printCurrent();
    m->optimizeSolution();
    std::cout << getPeakRSS()/1024 << std::endl;
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
    delete ts;
    //    delete p;
    return 0;
}
//
//void gecode(int argc, char* argv[]) {
//    InstanceOptions opt("BPSolver");
//    opt.instance("data/toy.txt");
//    opt.time(180 * 1000); // in milliseconds
//
//    opt.parse(argc, argv);
//
//    BP_Input *p = new BP_Input(opt.instance());
//
//    
//    //    //	// This is to stop the search at the time limit imposed
//    //    Search::Options so;
//    //    Search::TimeStop* ts = new Search::TimeStop(opt.time());
//    //    so.stop = ts;
//    //    Search::Statistics stat;
//    //    //
//    //    // Let's start the Timer
//    //    Support::Timer t;
//    //    t.start()
////    Test* t = new Test();
//    
//    BPSolver* m = new BPSolver(p);
//    m->InitialSolution(); // problem at sende opt med da den bliver slettet efter kald. 
////    m->printCurrent();
//    m->optimizeSolution();
////    m->printCurrent();
//    
////    std::cout << &opt << std::endl;
////    m->print(std::cout);
//
//    //    SpaceStatus status = m->status();
//    //    m->print(cout);
//    //    if (status == SS_FAILED){
//    //        cout << "Status: " << m->status() << " the space is failed at root."
//    //                << endl;
//    //    }
//    //    else if (status == SS_SOLVED){
//    //        cout << "Status: " << m->status()
//    //                << " the space is not failed but the space has no brancher left."
//    //                << endl;
//    //    }
//    //    else if (status == SS_BRANCH){
//    //        cout << "Status: " << m->status()
//    //                << " the space is not failed and we need to start branching."
//    //                << endl;
//    //    }
//    ////    m->print(cout);
//    //    //    exit(1);
//    //    //	BP_Output out(*p);
//    //    //
//    //    
//    //    
//    //    try {
//    //        DFS<BPSolver> e(m, so);
//    //        //        BAB<BPSolver> e(m, so);
//    //        //        RBS<BAB, VRPSolver> e(m, so);
//    ////        delete m;
//    //        BPSolver* s=e.next();
//    ////        while (s = e.next()) {
//    ////            
//    //////            			s->print(cout);
//    ////            //			s->setSolution(out); // pass here the solution found to Output for drawing
//    ////            //			out.draw();
//    //////            delete s;
//    ////            //            			stat = e.statistics();
//    ////            //			//cout << "Propagators: " << stat.propagate << endl; // see page 145 MPG
//    ////            //            			print_stats(stat);
//    ////            //			cout << "\ttime: " << t.stop() / 1000 << "s" << endl;
//    ////            //break;
//    ////        }
//    //        s->print(std::cout);
//    //        s->setValues();
//    //        if (e.stopped()) {
//    //            cout << "WARNING: solver stopped, solution is not optimal!\n";
//    //            if (ts->stop(e.statistics(), so)) {
//    //                cout << "\t Solver stopped because of TIME LIMIT!\n";
//    //            }
//    //        }
//    //        
//    //        m->print_stats(stat);
//    //        m->print(std::cout);
//    //        cout << "\ttime: " << t.stop() / 1000 << "s" << endl;
//    //        s->printCurrent();
//    //        //		cout << out << endl;
//    //    } catch (Exception e) {
//    //        std::cerr << "Gecode exception: " << e.what() << std::endl;
//    ////        return 1;
//    //    }
//    //    //
//    ////    m->print(cout);
//    //    
//    //    delete p;
//
//}
//void easylocal(int argc, const char* argv[]){
//    ParameterBox main_parameters("main", "Main Program options");
//
//  // The set of arguments added are the following:
//  
//  Parameter<string> instance("instance", "Input instance", main_parameters); 
//  Parameter<int> seed("seed", "Random seed", main_parameters);
//  Parameter<string> method("method", "Solution method (empty for tester)", main_parameters);   
//  Parameter<string> init_state("init_state", "Initial state (to be read from file)", main_parameters);
////  Parameter<unsigned> observer("observer", "Attach the observers", main_parameters);
//  Parameter<double> timelimit("timelimit","Time limit for solver", main_parameters);
//  Parameter<unsigned> lb("lb", "lb for TL", main_parameters);
//  Parameter<unsigned> ub("ub", "ub for TL", main_parameters);
//
//  // 1, 2 eller 3
//  
//  Parameter<string> output_file("output_file", "Write the output to a file (filename required)", main_parameters);
// 
//  // 3rd parameter: false = do not check unregistered parameters
//  // 4th parameter: true = silent
//  CommandLineParameters::Parse(argc, argv, false, true);  
//  if (!instance.IsSet())
//    {
//      cout << "Error: --main::instance filename option must always be set" << endl;
////      return 1;
//      exit(1);
//    }
//  BP_Input in(instance);
////  BP_Constraints cons(in);
//
//
////  cout<<in;
//
//  if (seed.IsSet())
//    Random::Seed(seed);
//  
//  // cost components: second parameter is the cost, third is the type (true -> hard, false -> soft)
//  BP_CostComponent1 cc1(in, 1, false);
//  BP_CostComponent2 cc2(in, weight, true);
//  
//
// 
//  BP_MoveDeltaCostComponent1 dcc1(in, cc1);
//  BP_MoveDeltaCostComponent2 dcc2(in, cc2);
//  
//  
//
//  // helpers
//  BP_StateManager BP_sm(in);
//  BP_MoveNeighborhoodExplorer BP_nhe(in, BP_sm);
//  BP_TabuListManager BP_tlm;
//  BP_OutputManager BP_om(in);
//  
//  // All cost components must be added to the state manager
//  BP_sm.AddCostComponent(cc1);
//  BP_sm.AddCostComponent(cc2);
//  
//  
//  
//  // All delta cost components must be added to the neighborhood explorer
//  BP_nhe.AddDeltaCostComponent(dcc1);
//  BP_nhe.AddDeltaCostComponent(dcc2);
//  
//  
//  
//  // runners
//  HillClimbing<BP_Input, BP_State, BP_Move, double> BP_hc(in, BP_sm, BP_nhe, "BP_MoveHillClimbing");
//  SteepestDescent<BP_Input, BP_State, BP_Move, double> BP_sd(in, BP_sm, BP_nhe, "BP_MoveSteepestDescent");
//  SimulatedAnnealing<BP_Input, BP_State, BP_Move, double> BP_sa(in, BP_sm, BP_nhe, "BP_MoveSimulatedAnnealing");
//  TabuSearch<BP_Input, BP_State, BP_Move,double> BP_ts(in, BP_sm, BP_nhe, BP_tlm, "BP_TabuSearch");
//  if(!ub.IsSet()){
//      ub = tabulistUB;
//  }
//  if(!lb.IsSet()){
//      lb = tabulistLB;
//  }
//  double limit;
//  if(!timelimit.IsSet()){
//        limit = 120.0;
//        std::cout << "Setting timelimit to 120 sec" << std::endl;
//  } else {
//      limit = timelimit;
//  }
//   
//  BP_tlm.SetLength(lb,ub);
////  method = "TS";
//  BP_ts.SetMaxIdleIteration(maxIter);
//  
//  
//
//  
//  // tester
//  Tester<BP_Input, BP_Output, BP_State, double> tester(in,BP_sm,BP_om);
//  MoveTester<BP_Input, BP_Output, BP_State, BP_Move, double> swap_move_test(in,BP_sm,BP_om,BP_nhe, "BP_Move move", tester); 
//  
////  output_file = "../res/"+instance.ToString()+".res";
//  
//
//  
//  if (!CommandLineParameters::Parse(argc, argv, true, false))
//      exit(1);
////    return 1;
//  
////  if (observer.IsSet())
////  {
////      RunnerObserver<BP_Input, BP_State, BP_Move, double> ob(observer,0);
////      BP_sa.AttachObserver(ob);
////      BP_hc.AttachObserver(ob);
////      BP_sd.AttachObserver(ob);
////      BP_ts.AttachObserver(ob);
////  }
//  
//
//  if (!method.IsSet())
//    { // If no search method is set -> enter in the tester
////      if (init_state.IsSet()){
////	    tester.RunMainMenu(init_state);
////      }else{
//	    tester.RunMainMenu();
////      }
//    }
//  else
//    {
//      SimpleLocalSearch<BP_Input, BP_Output, BP_State, double> BP_solver(in, BP_sm, BP_om, "XYZ solver");
//      BP_solver.SetParameter("timeout",limit);
//      
//
//      
////      BP_solver.SetParameter("random_initial_state",false);
//      
//      if (method == string("SA"))
//        {
//          BP_solver.SetRunner(BP_sa);
//        }
//      else if (method == string("HC"))
//        {
//          BP_solver.SetRunner(BP_hc);
//        }
//      else if (method == string("TS"))
//        {
//          BP_solver.SetRunner(BP_ts);
//        } else {
//          BP_solver.SetRunner(BP_sd);
//        }
//
//
//      unsigned  int trails = iniTrails;
//      BP_solver.SetParameter("init_trials", trails);
////      BP_solver.SetParameter("random_state",false);
//      
//      auto result = BP_solver.Solve();
//	  // result is a tuple: 0: solution, 1: number of violations, 2: total cost, 3: computing time
//      BP_Output out = get<0>(result);      
//      if (output_file.IsSet())
//        { // write the output on the file passed in the command line
//         ofstream os(static_cast<string>(output_file).c_str(), fstream::app);
////         os << get<1>(result)/100 << endl;
//         
////         /*
////         os << out << endl;
////         static_cast<string>(instance).substr(8);
////         os << static_cast<string>(instance).substr(8);
//         if(static_cast<int>(get<1>(result)) == 0){
////            os << "s FEASIBLE" << endl; 
//            
//         }else{
////            os << "s INFEASIBLE" << endl; 
//         //   os << "c Violations: " << get<1>(result) << endl;
////             os << "=infeas=" << std::endl;
//         }
//         os << static_cast<int>(get<1>(result)/weight);
//         os << "\t" << get<2>(result) << std::endl;
////         os << get<2>(result) << endl;
////         os << out;
//           
////         os << "c Violations: " << static_cast<int>(get<1>(result)/100) << endl;
////         os << "c Objective: " << get<2>(result) << endl;
////         os << "c Time: " << get<3>(result) << endl << endl; 
////         */
//         os.close();
//      }
//      else
//      { // write the solution in the standard output
////         cout << out << endl;
//         if(static_cast<int>(get<1>(result)) == 0){
//            cout << "s FEASIBLE" << endl; 
//         }else{
//            cout << "s INFEASIBLE" << endl; 
////            cout << "c Violations: " << get<1>(result) << endl;
//         }
//         cout << "c Violations: " << static_cast<int>(get<1>(result))/weight << endl;
//         cout << "c Objective: " << get<2>(result) << endl;
//          cout << "c Time: " << get<3>(result) << std::endl;
//        }
//   }
//}
