#include "GeneralSolver.hpp"
#include "IntegerVariable.hpp"
#include "Invariant.hpp"
#include "Sum.hpp"
#include "Linear.hpp"
#include "Constraint.hpp"
#include <boost/timer.hpp>
#include <ctime>
#include <math.h>

#ifndef TEST_HPP
#define	TEST_HPP

class Test : public GeneralSolver{
public:
    Test();
    Test(const Test& orig);
    virtual ~Test();
private:
    void testConstraints();
    void testInvariants();
    void testObjectiveFunction();
    void testLinear();
    void testSum();
    void createLinearEQConst();
    void startTest();
    void testDone();
    void testFailed(string func);
protected:
//    GeneralSolver* GS;
    vector<IntegerVariable*>* varInt;
    std::clock_t start;
    std::clock_t testStart;
};

#endif	/* TEST_HPP */

