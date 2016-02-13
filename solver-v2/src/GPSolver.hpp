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
#include "Linear.hpp"
#include <limits>
#include "Variable.hpp"
#include "Multistop.hpp"
#include <memory>
#include "State.hpp"
#include <functional>
#include "Clock.hpp"
#include <math.h>
#include "Random.hpp"
#include <stdlib.h>


class GPSolver {
private:
    const std::shared_ptr<Storage> storage = std::make_shared<Storage> ();
    const std::unique_ptr<LocalSearchEngine> LS = std::unique_ptr<LocalSearchEngine> (new LocalSearchEngine(storage));
    std::unique_ptr<GecodeEngine> GS = std::unique_ptr<GecodeEngine> (new GecodeEngine(storage));

    
     bool relax(int TimeForGecode);

    void simpleRelax(int timesRelaxed);

    bool relaxAllNonFunctionel(int TimeForGecode);
    void furtherRelax(std::vector<constraint> cons);

public:

    GPSolver();

    ~GPSolver();
    GPSolver& operator=(const GPSolver &a);

    void linear(std::vector<int>& coefficients, std::vector<Variable*>& variables, int relation, int ub, unsigned priority);

    std::vector<Variable*> createVariables(unsigned numberOfVariables, int lb, int ub);

    Variable* createVariable(int lb, int ub);
    std::vector<Variable*>& getAllVariables();

    void print(std::vector<Variable>& IntegerVariables);

    void Search(std::vector<Variable*> variables);

    void initialSolution(int TimeForGecode);
    void addObjective(std::vector<int>& coefficients, std::vector<Variable*>& variables);
   
    std::vector<int>& getInitialValue();

    void optimizeSolution(int time, int test);

    void printVariableValues();
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

