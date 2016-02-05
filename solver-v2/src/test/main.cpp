#include "GPSolver.hpp"
#include <cstdlib>
#include "BP_Data.hpp"
#include <cmath>
#include <algorithm>
//#include <iomanip>
#include <limits>
#include "Clock.hpp"

#include <iostream>
#include <fstream>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    Clock::globalClock = std::clock();

        string str2 = argv[1];
    for (unsigned i = 0; i < str2.length(); i++) {
        if (str2[i] == '/') {
            str2[i] = ' ';
        } else if (str2[i] == '.') {
            str2[i] = ' ';
        }
    }
    //    std::cout << argv[1] << std::endl;
    string temp2;
    std::vector<string> array2;
    stringstream ss2(str2);
    while (ss2 >> temp2)
        array2.push_back(temp2);
    string name2 = array2[1];
    Random::Seed(60);
    
    
    GPSolver GS;
    if (argc < 2) {
        std::cout << "No instances given" << std::endl;
        exit(1);
    }
    BP_Input* in = new BP_Input(argv[1]);

    //    BPSolver(BP_Input * in) {
    //        std::vector<IntegerVariable*>* varInt;
    //        int counter = 0;
    std::vector<Variable*> integerVariables;
    std::vector<Variable*> binaryVariables;
    for (var variable : in->getVars()) {
        Variable* iv = GS.createVariable(variable.lb, variable.ub);
        if (variable.ub == 1) {
            binaryVariables.push_back(iv);
        } else {
            integerVariables.push_back(iv);
        }

    }
    vector<Variable*>& varInt = GS.getAllVariables();
    for (unsigned i = 0; i < in->getNcons(); i++) {
        const std::vector<elem> leftside = in->getMatcoeff(i);
        bounds b = in->getBterms(i);
        vector<int> c(leftside.size());
        vector<Variable*> x(leftside.size());
        for (unsigned j = 0; j < leftside.size(); j++) {
            elem e = leftside[j];
            c.at(j) = (e.coeff);
            assert(e.coeff == (double) c.at(j));
            x.at(j) = varInt.at(e.index);
        }
        double upperbound = (b.ub);
        if (x.size() != 0) {
            if (b.type == 5) {
                //                GeneralSolver::linear(*this, c, x, EQ, upperbound, Gecode::ICL_DOM, HARD);
                GS.linear(c, x, EQ, upperbound, HARD);
            } else {
                //                GeneralSolver::linear(*this, c, x, LQ, upperbound, Gecode::ICL_DOM, HARD);
                GS.linear(c, x, LQ, upperbound, HARD);
            }
        } else {
            //                std::cout <<  "Posting constraint with no variables?" << std::endl;
        }
    }
    std::vector<int> c;
    std::vector<Variable*> x;
    std::vector<unsigned> obj = in->getObjVars();
    for (unsigned i : obj) {
        c.push_back((int) in->getVar(i).objcoeff);
        //            x->at(i) = varInt.at(i);
        assert(in->getVar(i).objcoeff == (coefType) c.back());

        x.push_back(varInt.at(i));
    }
    GS.addObjective(c, x);
    GS.initialSolution(10);
    int time;
    if (argc >= 3) {
        time = std::stoi(argv[2]);
    } else {
        time = 119;
    }
    int test;
    if (argc >= 4) {
        test = std::stoi(argv[3]);
        if (test < 1 || test > 3) {
            std::cout << "Algorithm not implemented for that number (Third argument). " << argv[3] << std::endl;
            exit(1);
        }
    } else {
        test = 3;
    }
    std::pair<int,int> sol = GS.optimizeSolution(time, test);
 

    //    GS.printVariableValues();
    double timeUsed = (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC;
    
//    std::ofstream myfile;
//    myfile.open("output.txt", std::ios::app);
    std::cout  << "#output " << name2 << " " << sol.first << " " << sol.second << " " << timeUsed << std::endl;
//    myfile.close();
    
    delete in;
}


