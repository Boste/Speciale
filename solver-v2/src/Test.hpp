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

class Test {
public:

    Test();
    Test(const Test& orig);
    virtual ~Test();
protected:
    GeneralSolver* GS;
    vector<IntegerVariable*>* varInt;
    std::clock_t start;
    std::clock_t testStart;
    int failedTests = 0;
private:
    void testConstraints();
    void testInvariants();
    void testObjectiveFunction();
    void testLinear();
    void testSum();
    void createLinearEQConst();
    void createLinearLQConst();
    void startTest(string func);
    void testDone(string func);
    void testFailed(string func, string error);
    void testBestMove();
    void testBig(int vars, int cons);
public:
    ;

};

#endif	/* TEST_HPP */

