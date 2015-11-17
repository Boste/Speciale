#ifndef GENERALSOLVER_HPP
#define	GENERALSOLVER_HPP
#include <cmath>
#include <algorithm>
#include "LSSpace.hpp"
#include <assert.h>
#include "Model.hpp"
#include <gecode/driver.hh>
#include <gecode/int.hh>
#include "Constants.hpp"
#include "GecodeSolver.hpp"
//#include <gecode/minimodel.hh>
#include "Linear.hpp"
#include <limits>
#include "IntegerVariable.hpp"
#include "Multistop.hpp"
//#include "Sum.hpp"
//#include"Max.hpp"
#include <memory>
#include "State.hpp"
#include <functional>
//#include "Invariant.hpp"
//#include "Constraint.hpp"
#include "Clock.hpp"
//#include "getRSS.hpp"
//#include "Random.hpp"
//using namespace Gecode;

//class GeneralSolver : public Gecode::Space, private LSSpace {

class GeneralSolver {
//    friend class Test;
private:

    const std::shared_ptr<Model> model =  std::make_shared<Model> ();
    const std::unique_ptr<LSSpace> LS = std::unique_ptr<LSSpace> (new LSSpace(model));
    std::unique_ptr<GecodeSolver> GS = std::unique_ptr<GecodeSolver> (new GecodeSolver(model));
    
public:

    GeneralSolver() ;

    ~GeneralSolver();
    GeneralSolver& operator=(const GeneralSolver &a);


    // Skal Gecode::IntConLevel icl være et argument?
    // ingen shared i det her kald

    void linear(std::vector<int>& coefficients, std::vector<IntegerVariable*>& variables, int relation, int ub, unsigned priority);
    /// Returns a vector of the variables created (copying the vector)

    std::vector<IntegerVariable*> createIntVars(unsigned numberOfVariables, int lb, int ub);

//    ///Create a single variable with given lower and upper bound
    IntegerVariable* createIntVar(int lb, int ub);

    std::vector<IntegerVariable*>& getAllVariables();

    /// Only for testing, should be removed 
    void print(std::vector<IntegerVariable>& IntegerVariables);

    void Search(std::vector<IntegerVariable*> variables);
    
    
    /// Uses Gecode to find initial solution
    /// returns a new GeneralSolver with the initial solution the old (the one this method is called from) is not updated with new solution. 

    void InitialSolution(int TimeForGecode);

    /// relaxes the space (reduce the number of constraints). Used when Gecode cant find a solution in time.
    /// Only works for binary


    /// Different relaxation can be chosen (not atm) needs to create a new GecodeSolver (Space) and recreate some 
    /// of the calls the user made (those that should not be relaxed).

    void relax(int timesRelaxed);

    void simpleRelax(int timesRelaxed);
    
    std::vector<int>& getInitialValue();

    void optimizeSolution(int time);

    void printCurrent();


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

