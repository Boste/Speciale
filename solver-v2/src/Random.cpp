#include "Random.hpp"


//std::random_device Random::dev;
int Random::seed = std::random_device()();
std::mt19937 Random::mt(Random::seed);

/// both inclusive
int Random::Integer(int lb, int ub) {
    std::uniform_int_distribution<> d(lb, ub);
    
    return d(mt);
}
/// inclusive
int Random::Integer(int ub) {
    std::uniform_int_distribution<> d(0, ub);

    return d(mt);
}

double Random::Double(double lb, double ub) {
    std::uniform_real_distribution<> d(lb, ub);
    return d(mt);
}

int Random::Seed(int seed) {
    mt.seed(seed);
    return Random::seed = seed;
}