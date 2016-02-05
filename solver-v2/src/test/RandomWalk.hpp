#ifndef RANDOMWALK_HPP
#define	RANDOMWALK_HPP
#include "SearchEngine.hpp"
#include "Move.hpp"
#include "Neighborhood.hpp"
#include "Storage.hpp"


class RandomWalk : public SearchEngine {
public:
    RandomWalk( Neighborhood* NE, unsigned numberOfRandomMoves);
    bool Start();
//    RandomWalk(const RandomWalk& orig);
    virtual ~RandomWalk();
private:
//    std::shared_ptr<Model> model;
    unsigned numberOfRandomMoves;

};

#endif	/* RANDOMWALK_HPP */

