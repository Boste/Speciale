//#include "GeneralSolver.hpp"

//GeneralSolver::GeneralSolver() {
//}
//void GeneralSolver::InitialSolution() {
//        //	// This is to stop the search at the time limit imposed
//        Gecode::Search::Options so;
//
//        startTimer(so);
//        printSpaceStatus();
//
//        try {
//            Gecode::DFS<GeneralSolver> e(this, so);
//            GeneralSolver* s = e.next();
//            assert(!e.stopped());
//            this->SetValues(s->IntVars);
//            //            for(int i = 0; i < IntVars.size(); i++ ){
//            //                if(IntVarVector.at(i).getCurrentValue() != s->IntVars[i].val()){
//            //                    std::cout << IntVarVector.at(i).getCurrentValue() << " vs " << s->IntVars[i] << std::endl;
//            //                }
//            //            }
//            initializeInvariants();
//            initializeConstraints();
//            //            for (int i = 0; i < Invariants.size(); i++) {
//            //                std::cout << Invariants.at(i).getValue() << " " << LSSpace::testInvariant(i) << "  ";
//            //                
//            //            }
//
//            if (e.stopped()) {
//                cout << "WARNING: solver stopped, solution is not optimal!\n";
//                if (so.stop->stop(e.statistics(), so)) {
//                    cout << "\t Solver stopped because of TIME LIMIT!\n";
//                }
//            }
//            delete s;
//
//        } catch (Gecode::Exception e) {
//            std::cerr << "Gecode exception: " << e.what() << std::endl;
//            //        return 1;
//        }
//        //
//        //        this->print(cout);
//
//    }
//    
//
//
//
//GeneralSolver::~GeneralSolver() {
//    delete IntVars;
//    
//}

