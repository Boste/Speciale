#include <memory>
#include <gecode/driver.hh>
#include <gecode/int.hh>
#include "Variable.hpp"
#include "Clock.hpp"
#include "Constants.hpp"
#include "Storage.hpp"
#include "Multistop.hpp"
#include <stdlib.h>

#ifndef GECODESOLVER_HPP
#define	GECODESOLVER_HPP



using namespace Gecode;

class GecodeEngine : public Space {
protected:
    std::shared_ptr<Storage> model;
//    IntVarArray IntVars;
    Gecode::BoolVarArray AllVars;
    /// allocated in stack space and later moved to heap (AllVars)
    Gecode::BoolVarArgs tmpVars;
    /// All binary variables
    Gecode::BoolVarArgs binVars;
public:

    GecodeEngine(std::shared_ptr<Storage> model);

    virtual ~GecodeEngine();
    
    bool findSolution(int TimeForGecode, bool fix);
    void linear(std::vector<int>& coefficients, const std::vector<Variable*>& variables, int relation, int upperbound);
    void createGecodeVariable(int lb,int ub);
    void SetValues(Gecode::BoolVarArray vars);
    void randomInitialize();
    void fixVariables();
    void printSpaceStatus();
    void createArray();
    void print_stats(Gecode::Search::Statistics & stat);
    void print(std::ostream& os) const;
    GecodeEngine(bool share, GecodeEngine& s); // : Space(share, s);
    void postCovSol();
    virtual Space* copy(bool share);
};

#endif	/* GECODESOLVER_HPP */

