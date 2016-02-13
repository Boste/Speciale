
#include "BP_Data.hpp"
#include <cmath>
#include <algorithm>
#include <limits>
#include "BPSolver.hpp"
#include "Clock.hpp"
#include "getRSS.hpp"
#include "GPSolver.hpp"

#include "Output.hpp"
#include <iostream>
#include <fstream>
#include <getopt.h>
#include <unistd.h>

using namespace Gecode;

struct globalArgs_t {
    int seed; /* -s option */
    char *instance; /* -i option */
    int alg; /* -a option */
    int time; /* -t option */
    int gecode; /* -g option */
} globalArgs;

static const char *optString = "s:i:a:t:g:h";

static const struct option longOpts[] = {
    { "Seed", required_argument, NULL, 's'},
    { "Instance", required_argument, NULL, 'i'},
    { "Algorithm", required_argument, NULL, 'a'},
    { "Total time", required_argument, NULL, 't'},
    { "Gecode time", no_argument, NULL, 'g'},
    { "help", no_argument, NULL, 'h'},
    { NULL, no_argument, NULL, 0}
};

int main(int argc, char* argv[]) {


    int opt = 0;
    int longIndex = 0;

    globalArgs.seed = RANDOMSEED; 
    globalArgs.instance = NULL;
    globalArgs.alg = 3;
    globalArgs.time = 120;
    globalArgs.gecode = 10;

    opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
    while (opt != -1) {
        switch (opt) {
            case 's':
                globalArgs.seed = atoi(optarg); /* true */
                break;

            case 'i':
                globalArgs.instance = optarg;
                break;

            case 'a':
                globalArgs.alg = atoi(optarg);
                break;

            case 't':
                globalArgs.time = atoi(optarg);

                break;
            case 'g':
                globalArgs.gecode = atoi(optarg);
                break;
            case 'h': 
                std::cout << "Set instance by -i, total time -t, random seed -s, algorithm number -a, time limit for Gecodes search -s" << std::endl;
                exit(1);
                break;

            default:
                break;
        }

        opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
    }

    if (globalArgs.instance == NULL) {
        std::cout << "Must specify an instance" << std::endl;
        exit(1);
    }
    if (globalArgs.alg < 1 || globalArgs.alg > 3) {
        std::cout << "Only algorithm 1,2, and 3 are implemented, using algorithm 3" << std::endl;
        globalArgs.alg = 3;
    }
    Random::Seed(globalArgs.seed);


    Clock::globalClock = std::clock();


    BP_Input* input = new BP_Input(globalArgs.instance);
    auto read = (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC;
    std::cout << "Instance read after " << read << " seconds" << std::endl;
    size_t peakSize4 = getPeakRSS();
    std::cout << "Peak memory usage for reading instance " << (double) peakSize4 / 1024 / 1024 << " mb" << std::endl;
    Support::Timer t;
    t.start();
    auto tid = std::clock();
    BPSolver userModel(input);
    std::cout << "Initialize solution" << std::endl;

    auto posted = (std::clock() - tid) / (double) CLOCKS_PER_SEC;
    userModel.initialSolution(globalArgs.gecode);
    size_t peakSize2 = getPeakRSS();
    std::cout << "Peak memory usage for Gecode " << (double) peakSize2 / 1024 / 1024 << " mb" << std::endl;
    std::cout << "LS solver initialized after " << (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n ######################################################################################################### " << std::endl;
    std::cout << "LOCALSEARCH \n "
            "######################################################################################################### \n" << std::endl;


    userModel.optimizeSolution(globalArgs.time, globalArgs.alg);
    auto totalTime = (std::clock() - Clock::globalClock) / (double) CLOCKS_PER_SEC;

    std::cout << "## CoinRead " << read << std::endl; // time for reading instance with coin
    std::cout << "## postingModel " << posted+read << std::endl; // time for posting model in General Solver
    std::cout << "## TotalTime " << totalTime << std::endl;
    //    std::cout << "## GecodeTime " << TimeForGecode << std::endl;
    std::cout << "## seed " << globalArgs.seed << std::endl;
    std::cout << "## peakMemory " << (double) getPeakRSS() / 1024 / 1024 << std::endl;

    // peak memory in mb
    string name = basename(globalArgs.instance);
    std::cout << "## name " << name << std::endl; // Instance name
    delete input;

    return 0;
}