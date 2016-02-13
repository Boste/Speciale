#ifndef RANDOMWALK_HPP
#define	RANDOMWALK_HPP
#include "Move.hpp"
#include "Neighborhood.hpp"
#include "Storage.hpp"


class RandomWalk {
public:
    RandomWalk( Neighborhood* NE, unsigned numberOfRandomMoves);
    bool Start();
    virtual ~RandomWalk();
private:
    unsigned numberOfRandomMoves;
    Neighborhood* NE;
};

#endif	/* RANDOMWALK_HPP */

