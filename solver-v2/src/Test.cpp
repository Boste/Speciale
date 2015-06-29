#include "Test.hpp"

#include <c++/4.9/bits/stl_bvector.h>
#include <c++/4.9/bits/basic_string.h>

Test::Test() {
    std::cout << std::endl;
    std::cout << "#############################################################################" << std::endl;
    std::cout << "Start tests " << std::endl;
    std::cout << "#############################################################################" << std::endl;
    std::cout << std::endl;
    start = std::clock();

    testLinear();



    //    varInt = GeneralSolver::createIntVars(10, 0, 1);
    //    createLinearEQConst();
    //
    //    //        std::cout << __LINE__ << std::endl;
    //
    //
    //    // add obj
    //    vector<int>* c = new vector<int>(varInt->size());
    //    //        std::cout << "objective function " << std::endl;
    //    for (unsigned i = 0; i < varInt->size(); i++) {
    //        c->at(i) = 1;
    //        //            std::cout << c->at(i) << " + ";
    //    }
    //    GeneralSolver::linear(*this, c, varInt, Gecode::IRT_LQ, 0, Gecode::ICL_DOM, 1);
    //
    //
    //    GeneralSolver::branch(*this, varInt, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());
    //
    //    InitialSolution();

    //    std::cout << __LINE__ << std::endl;

    //    testInvariants();







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
    GS = new GeneralSolver();
    varInt = GS->createIntVars(10, 0, 1);




}

void Test::testInvariants() {
    testSum();

}

void Test::testObjectiveFunction() {

}

void Test::testLinear() {
    GS = new GeneralSolver();
    varInt = GS->createIntVars(10, 0, 1);
    createLinearEQConst();
    createLinearLQConst();
    GS->InitialSolution();
    string error = "Constraints not satisfied";
    for (int i = 0; i < GS->Constraints.size(); i++) {
        if (GS->Constraints.at(i)->getViolation() != 0) {
            testFailed(string(__FUNCTION__), error);

        }
    }
    // Testing invariant (sum)

    error = "Lefthand side gives wrong value";
    for (int i = 0; i < GS->Invariants.size(); i++) {
        int sum = 0;
        for (int j = 0; j < varInt->size(); j++) {
            sum += varInt->at(j)->getCurrentValue()*(j + 1);
        }
        if (GS->Invariants.at(i)->getCurrentValue() != sum) {
            error += " invariant " + std::to_string(i) + " value " + 
                    std::to_string(GS->Invariants.at(i)->getCurrentValue()) + 
                    " should be " + std::to_string(sum);
            testFailed(string(__FUNCTION__), error);

        }
    }



}

void Test::testSum() {
    GS = new GeneralSolver();
    varInt = GS->createIntVars(10, 0, 1);
    createLinearEQConst();

    //    Invariant inv = Invariants.at(0);
}

void Test::createLinearEQConst() {
    startTest();
    std::cout << "Test " << __FUNCTION__ << std::endl;
    unsigned invarSize = GS->Invariants.size();
    unsigned constSize = GS->Constraints.size();
    std::vector<IntegerVariable*>* x = new std::vector<IntegerVariable*>();

    std::vector<int>* coef = new vector<int>();
    for (int i = 0; i < varInt->size(); i++) {
        coef->push_back(i + 1);
        x->push_back(varInt->at(i));
    }
    GS->GeneralSolver::linear(*GS, coef, x, Gecode::IRT_EQ, 50, Gecode::ICL_DOM, 2);
    if (invarSize + 1 != GS->Invariants.size() || constSize + 1 != GS->Constraints.size()) {
        string error = "";
        testFailed(string(__FUNCTION__), error);
    }



    testDone();


}

void Test::createLinearLQConst() {
    startTest();
    std::cout << "Test " << __FUNCTION__ << std::endl;
    unsigned invarSize = GS->Invariants.size();
    unsigned constSize = GS->Constraints.size();
    std::vector<IntegerVariable*>* x = new std::vector<IntegerVariable*>();

    std::vector<int>* coef = new vector<int>();
    for (int i = 0; i < varInt->size(); i++) {
        coef->push_back(i + 1);
        x->push_back(varInt->at(i));
    }
    GS->GeneralSolver::linear(*GS, coef, x, Gecode::IRT_LQ, 60, Gecode::ICL_DOM, 2);
    if (invarSize + 1 != GS->Invariants.size() || constSize + 1 != GS->Constraints.size()) {
        string error = "number of invariants and constraints " + std::to_string(invarSize) + " " + std::to_string(constSize) + ". After linear " + std::to_string(GS->Invariants.size()) + " " + std::to_string(GS->Constraints.size());
        testFailed(string(__FUNCTION__), error);
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

void Test::testFailed(string func, string error) {
    std::cout << "#############################################################################" << std::endl;
    std::cout << "TEST " << func << " FAILED" << std::endl;
    std::cout << error << std::endl;
    std::cout << "#############################################################################" << std::endl;
    sleep(5);
}