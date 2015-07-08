#include "Test.hpp"

//#include <c++/4.9/bits/stl_bvector.h>
//#include <c++/4.9/bits/basic_string.h>

Test::Test() {
    std::cout << std::endl;
    std::cout << "#############################################################################" << std::endl;
    std::cout << "Start tests " << std::endl;
    std::cout << "#############################################################################" << std::endl;
    std::cout << std::endl;
    start = std::clock();

    //    testLinear();
    testObjectiveFunction();



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
//    int minuts = floor(seconds / 60);
//    double seconds = seconds % 60;
    std::cout << std::endl;
    std::cout << "#############################################################################" << std::endl;
    std::cout << "All test finish after " << seconds << " seconds" << std::endl;
    std::cout << "With " << failedTests << " failed tests" << std::endl;
    std::cout << "#############################################################################" << std::endl;
    std::cout << std::endl;
}

Test::Test(const Test& orig) {
}

Test::~Test() {
}

void Test::testConstraints() {
    startTest(string(__FUNCTION__));
    GS = new GeneralSolver();
    varInt = GS->createIntVars(10, 0, 1);
    


    testDone(string(__FUNCTION__));

}

void Test::testInvariants() {
    testSum();

}

void Test::testObjectiveFunction() {
    startTest(string(__FUNCTION__));
    GS = new GeneralSolver();
    varInt = GS->createIntVars(100, 0, 1);
    //    createLinearEQConst();
    createLinearLQConst();
    std::vector<IntegerVariable*>* x = new std::vector<IntegerVariable*>();

    std::vector<int>* coef = new vector<int>();
    for (unsigned i = 0; i < varInt->size(); i++) {
//        int c = -((i + 1) % 5);
        int c = -1;
        coef->push_back(c);
        x->push_back(varInt->at(i));
    }
    GS->GeneralSolver::linear(*GS, coef, x, Gecode::IRT_LQ, 0, Gecode::ICL_DOM, 1);
    string error = "objective not added to LSS";
    if (GS->st.getObjectives()->size() != 1) {
        testFailed(__FUNCTION__, error);
    }
    GS->branch(*GS, varInt, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());
    GS->InitialSolution();
    int objfnc = 0;
    for (unsigned i = 0; i < varInt->size(); i++) {
        objfnc += varInt->at(i)->getCurrentValue() * coef->at(i);
    }

    if (GS->st.getObjectives()->at(0)->getViolationDegree() != objfnc) {
        error = "Wrong objective value. Current Value " + std::to_string(GS->st.getObjectives()->at(0)->getViolationDegree()) + " Value should be " + std::to_string(objfnc);
        testFailed(__FUNCTION__, error);
    }
    GS->optimizeSolution();
//    GS->printCurrent();
    testDone(string(__FUNCTION__));

}

void Test::testLinear() {
    startTest(string(__FUNCTION__));
    GS = new GeneralSolver();
    varInt = GS->createIntVars(100, 0, 1);
    createLinearEQConst();
    createLinearLQConst();
    GS->branch(*GS, varInt, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());
    GS->InitialSolution();
    string error = "Constraints not satisfied";
    for (unsigned i = 0; i < GS->st.getConstraints()->size(); i++) {
        if (GS->st.getConstraints()->at(i)->getViolation() != 0) {
            testFailed(string(__FUNCTION__), error);

        }
    }
    // Testing invariant (sum)

    error = "Lefthand side gives wrong value";
    for (unsigned i = 0; i < GS->st.getInvariants()->size(); i++) {
        int sum = 0;
        for (unsigned j = 0; j < varInt->size(); j++) {
            sum += varInt->at(j)->getCurrentValue()*(j + 1);
        }
        if (GS->st.getInvariants()->at(i)->getCurrentValue() != sum) {
            error += " invariant " + std::to_string(i) + " value " +
                    std::to_string(GS->st.getInvariants()->at(i)->getCurrentValue()) +
                    " should be " + std::to_string(sum);
            testFailed(string(__FUNCTION__), error);

        }
    }

    testDone(string(__FUNCTION__));


}

void Test::testSum() {
    startTest(string(__FUNCTION__));
    GS = new GeneralSolver();
    varInt = GS->createIntVars(100, 0, 1);
    createLinearEQConst();

    //    Invariant inv = Invariants.at(0);
}

void Test::createLinearEQConst() {
    startTest(string(__FUNCTION__));
    unsigned invarSize = GS->st.getInvariants()->size();
    unsigned constSize = GS->st.getConstraints()->size();
    std::vector<IntegerVariable*>* x = new std::vector<IntegerVariable*>();

    std::vector<int>* coef = new vector<int>();
    for (unsigned i = 0; i < varInt->size(); i++) {
        coef->push_back(i + 1);
        x->push_back(varInt->at(i));
    }
    GS->GeneralSolver::linear(*GS, coef, x, Gecode::IRT_EQ, 50, Gecode::ICL_DOM, 2);
    if (invarSize + 1 != GS->st.getInvariants()->size() || constSize + 1 != GS->st.getConstraints()->size()) {
        string error = "";
        testFailed(string(__FUNCTION__), error);
    }



    testDone(string(__FUNCTION__));


}

void Test::createLinearLQConst() {
    startTest(string(__FUNCTION__));
    unsigned invarSize = GS->st.getInvariants()->size();
    unsigned constSize = GS->st.getConstraints()->size();
    std::vector<IntegerVariable*>* x = new std::vector<IntegerVariable*>();

    std::vector<int>* coef = new vector<int>();
    for (unsigned i = 0; i < varInt->size(); i++) {
        coef->push_back(2);
        x->push_back(varInt->at(i));
    }
    GS->GeneralSolver::linear(*GS, coef, x, Gecode::IRT_LQ, 60, Gecode::ICL_DOM, 2);
    if (invarSize + 1 != GS->st.getInvariants()->size() || constSize + 1 != GS->st.getConstraints()->size()) {
        string error = "number of invariants and constraints " + std::to_string(invarSize) + " " + std::to_string(constSize) + ". After linear " + std::to_string(GS->st.getInvariants()->size()) + " " + std::to_string(GS->st.getConstraints()->size());
        testFailed(string(__FUNCTION__), error);
    }
    testDone(string(__FUNCTION__));


}

void Test::startTest(string func) {
    testStart = std::clock();
    std::cout << "Test " << func << std::endl;
}

void Test::testDone(string func) {
    double seconds = (std::clock() - testStart) / (double) CLOCKS_PER_SEC;
    int minuts = floor(seconds / 60);
    std::cout << "Test " << func << " finish after " << minuts << " minuts and " << seconds << " seconds" << std::endl;
}

void Test::testFailed(string func, string error) {
    std::cout << "#############################################################################" << std::endl;
    std::cout << "TEST " << func << " FAILED" << std::endl;
    std::cout << error << std::endl;
    std::cout << "#############################################################################" << std::endl;
    failedTests++;
    sleep(5000);
}