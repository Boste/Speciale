#include "GeneralSolver.hpp"
#include "IntegerVariable.hpp"

#ifndef TEST_HPP
#define	TEST_HPP

class Test {
public:
    Test();
    Test(const Test& orig);
    virtual ~Test();
private:
    void Constraints();
    void Invariants();
    void ObjectiveFunction();
    void Linear();
    void Sum();
    void createLinearEQConst();
    void startTest();
    void testDone();
    void testFailed(string func);
protected:
    GeneralSolver* GS;
    vector<IntegerVariable*>* varInt;
    std::clock_t start;
    std::clock_t testStart;
};

#endif	/* TEST_HPP */

