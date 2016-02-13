#ifndef BESTIMPROVEMENT_HPP
#define	BESTIMPROVEMENT_HPP
#include "Move.hpp"
#include "Neighborhood.hpp"

class BestImprovement {
public:
    BestImprovement(Neighborhood* NE);
    bool Start(bool alwaysCommit);
    ~BestImprovement();
private:
    Neighborhood* NE;

};

#endif	/* BESTIMPROVEMENT_HPP */

