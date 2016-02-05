#ifndef GENERALSOLVER_HPP
#define	GENERALSOLVER_HPP
#include <cmath>
#include <algorithm>
#include "LocalSearchEngine.hpp"
#include <assert.h>
#include "Storage.hpp"
#include <gecode/driver.hh>
#include <gecode/int.hh>
#include "Constants.hpp"
#include "GecodeEngine.hpp"
//#include <gecode/minimodel.hh>
#include "Linear.hpp"
#include <limits>
#include "Variable.hpp"
#include "Multistop.hpp"
//#include "Sum.hpp"
//#include"Max.hpp"
#include <memory>
#include "State.hpp"
#include <functional>
//#include "Invariant.hpp"
//#include "Constraint.hpp"
#include "Clock.hpp"
#include <math.h>
#include "Random.hpp"
#include <stdlib.h>

//#include "getRSS.hpp"
//#include "Random.hpp"
//using namespace Gecode;

//class GeneralSolver : public Gecode::Space, private LSSpace {

class GPSolver {
    //    friend class Test;
private:
    //    std::vector<int> maxCoef;
    const std::shared_ptr<Storage> storage = std::make_shared<Storage> ();
    const std::unique_ptr<LocalSearchEngine> LS = std::unique_ptr<LocalSearchEngine> (new LocalSearchEngine(storage));
    std::unique_ptr<GecodeEngine> GS = std::unique_ptr<GecodeEngine> (new GecodeEngine(storage));

    
     bool relax(int TimeForGecode);

    void simpleRelax(int timesRelaxed);

    bool relaxAllNonFunctionel(int TimeForGecode);
//    std::vector<constraint> furtherRelax(std::vector<constraint> cons);
    void furtherRelax(std::vector<constraint> cons);

public:

    GPSolver();

    ~GPSolver();
    GPSolver& operator=(const GPSolver &a);

    // Skal Gecode::IntConLevel icl være et argument?
    // ingen shared i det her kald

    void linear(std::vector<int>& coefficients, std::vector<Variable*>& variables, int relation, int ub, unsigned priority);
    /// Returns a vector of the variables created (copying the vector)

    std::vector<Variable*> createVariables(unsigned numberOfVariables, int lb, int ub);

    //    ///Create a single variable with given lower and upper bound
    Variable* createVariable(int lb, int ub);
    std::vector<Variable*>& getAllVariables();

    /// Only for testing, should be removed 
    void print(std::vector<Variable>& IntegerVariables);

    void Search(std::vector<Variable*> variables);
    /// Uses Gecode to find initial solution
    /// returns a new GeneralSolver with the initial solution the old (the one this method is called from) is not updated with new solution. 

    void initialSolution(int TimeForGecode);
    void addObjective(std::vector<int>& coefficients, std::vector<Variable*>& variables);
    /// relaxes the space (reduce the number of constraints). Used when Gecode cant find a solution in time.
    /// Only works for binary

    /// Different relaxation can be chosen (not atm) needs to create a new GecodeSolver (Space) and recreate some 
    /// of the calls the user made (those that should not be relaxed).

   
    std::vector<int>& getInitialValue();

    void optimizeSolution(int time, int test);

    void printVariableValues();
    //    unsigned getNumberOfDigits(double number);
    void setOutput(Output* out);
private:

    void print_stats(Gecode::Search::Statistics & stat) {
        std::cout << "\tfail: " << stat.fail << std::endl;
        std::cout << "\tnodes: " << stat.node << std::endl;
        std::cout << "\tpropagators: " << stat.propagate << std::endl; // see page 145 MPG
        std::cout << "\tdepth: " << stat.depth << std::endl;
        std::cout << "\trestarts: " << stat.restart << std::endl;
        std::cout << "\tnogoods: " << stat.nogood << std::endl;
    }



};

#endif	/* GENERALSOLVER_HPP */

