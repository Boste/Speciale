#include <memory>
#include <gecode/driver.hh>
#include <gecode/int.hh>
#include "Variable.hpp"
#include "Clock.hpp"
#include "Constants.hpp"
#include "Model.hpp"
#include "Multistop.hpp"
#ifndef GECODESOLVER_HPP
#define	GECODESOLVER_HPP



using namespace Gecode;

class GecodeSolver : public Space {
protected:
    std::shared_ptr<Model> model;
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

    GecodeSolver(std::shared_ptr<Model> model);

    virtual ~GecodeSolver();
    
    /// Always branches the same way. 
//    void branch();
    bool initialize(int TimeForGecode,bool fix);
    bool FindSolution(int TimeForGecode, bool fix);
    void linear(std::vector<int>& coefficients, const std::vector<Variable*>& variables, int relation, int upperbound);
//    void createGecodeVariables(std::shared_ptr<Model> model);
    void createGecodeVariable(int lb,int ub);
    void SetValues(Gecode::BoolVarArray vars);
//    void SetValues(Gecode::IntVarArray vars);
    void fixVariables();
    void printSpaceStatus();
    void createArray();
    void print_stats(Gecode::Search::Statistics & stat);
    void print(std::ostream& os) const;
    GecodeSolver(bool share, GecodeSolver& s); // : Space(share, s);
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

