#include <memory>
#include <gecode/driver.hh>
#include <gecode/int.hh>
#include "IntegerVariable.hpp"
#include "Clock.hpp"
#include "Constants.hpp"
#include "State.hpp"
#ifndef GECODESOLVER_HPP
#define	GECODESOLVER_HPP



using namespace Gecode;

class GecodeSolver : public Space {
protected:
    std::shared_ptr<State> st;
public:
    IntVarArray IntVars;
    
    GecodeSolver( std::shared_ptr<State> st);
    
    virtual ~GecodeSolver();
    void branch(bool fix);
    bool initialize();
    bool FindSolution();
    void linear(std::vector<int>& coefficients, std::vector<IntegerVariable*>* variables, int relation, int upperbound);
    void createGecodeVariables(std::shared_ptr<State> st);
    void SetValues(Gecode::IntVarArray& vars);
    void printSpaceStatus();
    void print_stats(Gecode::Search::Statistics & stat);
    void print(std::ostream& os) const;
    GecodeSolver(bool share, GecodeSolver& s); // : Space(share, s);
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

