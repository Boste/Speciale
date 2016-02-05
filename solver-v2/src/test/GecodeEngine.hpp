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
//    Gecode::IntVarArray AllVars;
    /// allocated in stack space and later moved to heap (AllVars)
    Gecode::BoolVarArgs tmpVars;
//    Gecode::IntVarArgs tmpVars;
    /// All binary variables
    Gecode::BoolVarArgs binVars;
//    Gecode::IntVarArgs binVars;
public:

    GecodeEngine(std::shared_ptr<Storage> model);

    virtual ~GecodeEngine();
    
    /// Always branches the same way. 
//    void branch();
//    bool initialize(int TimeForGecode,bool fix);
    bool findSolution(int TimeForGecode, bool fix);
    void linear(std::vector<int>& coefficients, const std::vector<Variable*>& variables, int relation, int upperbound);
//    void createGecodeVariables(std::shared_ptr<Model> model);
    void createGecodeVariable(int lb,int ub);
    void SetValues(Gecode::BoolVarArray vars);
    void randomInitialize();
//    void SetValues(Gecode::IntVarArray vars);
    void fixVariables();
    void printSpaceStatus();
    void createArray();
    void print_stats(Gecode::Search::Statistics & stat);
    void print(std::ostream& os) const;
    GecodeEngine(bool share, GecodeEngine& s); // : Space(share, s);
    void postCovSol();
    //        IntVars.update(*this, share, s.IntVars);
    // remember to update your main variables!
    //		model = s.model;

    //    }
    // Copy during cloning

    virtual Space* copy(bool share);
    //        return new GecodeSolver(share, *this);
    //    }
    //private:

};

#endif	/* GECODESOLVER_HPP */

