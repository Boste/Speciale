#ifndef RANDOM_HPP
#define	RANDOM_HPP

#include <random>

class Random {
public:
    static std::mt19937 mt;
    static int seed;
    static int Integer(int lb, int ub);
    static double Double(double lb = 0, double ub = 1);
    static int Seed(int seed);
};

#endif	/* RANDOM_HPP */

