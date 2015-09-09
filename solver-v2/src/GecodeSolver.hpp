#include <memory>
#include <gecode/driver.hh>
#include <gecode/int.hh>
#include "IntegerVariable.hpp"
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
    Gecode::IntVarArray IntVars;
    Gecode::IntVarArgs tmpVars;
    Gecode::IntVarArgs binVars;
public:

    GecodeSolver(std::shared_ptr<Model> model);

    virtual ~GecodeSolver();
    void branch();
    bool initialize(int TimeForGecode,bool fix);
    bool FindSolution(int TimeForGecode, bool fix);
    void linear(std::vector<int>& coefficients, const std::vector<IntegerVariable*>& variables, int relation, int upperbound);
//    void createGecodeVariables(std::shared_ptr<Model> model);
    void createGecodeVariable(int lb,int ub);
    void SetValues(Gecode::IntVarArray vars);
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

