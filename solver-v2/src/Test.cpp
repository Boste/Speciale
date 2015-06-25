#include "Test.hpp"

#include <c++/4.9/bits/stl_bvector.h>

Test::Test() {
    std::cout << std::endl;
    std::cout << "#############################################################################" << std::endl;
    std::cout << "Start tests " << std::endl;
    std::cout << "#############################################################################" << std::endl;
    std::cout << std::endl;
    start = std::clock();



    //    GS.
    varInt = GeneralSolver::createIntVars(10, 0, 1);
    createLinearEQConst();

    //        std::cout << __LINE__ << std::endl;


    // add obj
    vector<int>* c = new vector<int>(varInt->size());
    //        std::cout << "objective function " << std::endl;
    for (unsigned i = 0; i < varInt->size(); i++) {
        c->at(i) = 1;
        //            std::cout << c->at(i) << " + ";
    }
    GeneralSolver::linear(*this, c, varInt, Gecode::IRT_LQ, 0, Gecode::ICL_DOM, 1);


    GeneralSolver::branch(*this, varInt, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());

    InitialSolution();

    //    std::cout << __LINE__ << std::endl;

    testInvariants();







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

void Test::testConstraints() {

}

void Test::testInvariants() {
    testSum();

}

void Test::testObjectiveFunction() {

}

void Test::testLinear() {

}

void Test::testSum() {
    //    Invariant inv = Invariants.at(0);
}

void Test::createLinearEQConst() {
    startTest();
    std::cout << "Test " << __FUNCTION__ << std::endl;
    unsigned invarSize = Invariants.size();
    unsigned constSize = Constraints.size();
    std::vector<IntegerVariable*>* x = new std::vector<IntegerVariable*>();

    std::vector<int>* coef = new vector<int>();
    for (int i = 0; i < 10; i++) {
        coef->push_back(i + 1);
        x->push_back(varInt->at(i));
    }
    GeneralSolver::linear(*this, coef, x, Gecode::IRT_EQ, 50, Gecode::ICL_DOM, 2);
    if (invarSize + 1 != Invariants.size() || constSize + 1 != Constraints.size()) {
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