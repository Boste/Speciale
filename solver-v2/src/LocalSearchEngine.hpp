//#include <../../../gecode/release-4.3.3/gecode/int.hh>
//#include <gecode/int.hh>
//#include "../include/Constraints.hpp"
#include <memory>
#include "Clock.hpp"
#include "State.hpp"
#include "Variable.hpp"
#include "Constraint.hpp"
#include "Invariant.hpp"
#include "Sum.hpp"
//#include"Max.hpp"
//#include "Linear.hpp"
//#include "Constraint.hpp"
//#include "Variable.hpp"   
#include "Storage.hpp"
#include "DependencyDigraph.hpp"
#include "Neighborhood.hpp"
#include "FlipNeighborhood.hpp"
#include "SwapNeighborhood.hpp"
#include "RandomConflictConNE.hpp"
#include "EvalFlipNE.hpp"
#include "RestrictedFlipNE.hpp"
#include "ConflictOnlyNE.hpp"
#include "Constants.hpp"
//#include "SearchEngine.hpp"
#include "BestImprovement.hpp"
#include "FirstImprovement.hpp"
#include "RandomWalk.hpp"
#include "TabuSearch.hpp"

#include <iostream>
#include <fstream>


#ifndef LSSPACE_HPP
#define	LSSPACE_HPP
//#include 

class LocalSearchEngine {

public:
    LocalSearchEngine(std::shared_ptr<Storage> model);
    void printCurrent();

    ~LocalSearchEngine() {

    }
    void initializeLS();
    void createDDG(bool all);
    void optimizeSolution(int time, int test);
    void setSolution(std::shared_ptr<State> st);
    bool testInvariant();

private:
    int Violations = 0;
    int iterations = 0;
    std::shared_ptr<Storage> model;
    std::shared_ptr<State> currentState;
    std::shared_ptr<State> bestState;
    std::shared_ptr<DependencyDigraph> DDG;
    std::vector<unsigned> defining;
    unsigned oneway = 0;
};

#endif	/* LSSPACE_HPP */

