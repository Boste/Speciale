#include "Move.hpp"
#include "Storage.hpp"
#include "Constants.hpp"
#include <memory>
#include "Neighborhood.hpp"
#include "Random.hpp"
#include "State.hpp"
#ifndef FLIPNEIGHBORHOOD_HPP
#define	FLIPNEIGHBORHOOD_HPP

class FlipNeighborhood : public Neighborhood {
public:
    std::shared_ptr<Storage> model;
    std::shared_ptr<State> state;
    FlipNeighborhood(std::shared_ptr<Storage> model, std::shared_ptr<State> st);
    FlipNeighborhood(const FlipNeighborhood& orig);
    ~FlipNeighborhood();
    Move* next();
    bool commitMove(Move* mv);
    bool calculateDelta(Move* mv);
    Move* nextRandom();
    unsigned getSize();


private:
    unsigned testCounter =0;
    unsigned moveCounter = 0;
    unsigned randomCounter = 0;
    unsigned randomMovesWanted = 0;
    unsigned moveCounterStart = 0;
    bool flag = false;
    unsigned  size;

};

#endif	/* FLIPNEIGHBORHOOD_HPP */

