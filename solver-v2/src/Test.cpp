#include "Test.hpp"
#include <boost/timer.hpp>
#include <ctime>
#include <math.h>
#include <c++/4.9/bits/stl_bvector.h>

Test::Test() {
    std::cout << std::endl;
    std::cout << "#############################################################################" << std::endl;
    std::cout << "Start tests " << std::endl;
    std::cout << "#############################################################################" << std::endl;
    std::cout << std::endl;
    start = std::clock();


    GS = new GeneralSolver();
//    GS.
    varInt = GS->createIntVars(10, 0, 1);
    GS->branch(*GS, varInt, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());
    createLinearEQConst();
        std::cout << __LINE__ << std::endl;

    GS->InitialSolution();

    std::cout << __LINE__ << std::endl;

    Invariants();







    double seconds = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    int minuts = floor(seconds / 60);
    std::cout << std::endl;
    std::cout << "#############################################################################" << std::endl;
    std::cout << "All test finish after " << minuts << " minuts and " << seconds << " seconds" << std::endl;
    std::cout << "#############################################################################" << std::endl;
    std::cout << std::endl;
}

Test::Test(const Test& orig) {
}

Test::~Test() {
}

void Test::Constraints() {

}

void Test::Invariants() {
    Sum();

}

void Test::ObjectiveFunction() {

}

void Test::Linear() {

}

void Test::Sum() {
    GS->Invariants.at(0);
}

void Test::createLinearEQConst() {
    startTest();
    std::cout << "Test " << __FUNCTION__ << std::endl;
    unsigned invarSize = GS->Invariants.size();
    unsigned constSize = GS->Constraints.size();

    std::vector<int>* coef = new vector<int>();
    for (int i = 0; i < 10; i++) {
        coef->push_back(i);
    }
    GS->linear(*GS, coef, varInt, Gecode::IRT_EQ, 50, Gecode::ICL_DOM, 2);
    if (invarSize + 1 != GS->Invariants.size() || constSize + 1 != GS->Constraints.size()) {
        testFailed(string(__FUNCTION__));
    }
    testDone();


}

void Test::startTest() {
    testStart = std::clock();
}

void Test::testDone() {
    double seconds = (std::clock() - testStart) / (double) CLOCKS_PER_SEC;
    int minuts = floor(seconds / 60);
    std::cout << "Test finish after " << minuts << " minuts and " << seconds << " seconds" << std::endl;
}

void Test::testFailed(string func) {
    std::cout << "#############################################################################" << std::endl;
    std::cout << "TEST " << func << " FAILED" << std::endl;
    std::cout << "#############################################################################" << std::endl;
    sleep(5);
}