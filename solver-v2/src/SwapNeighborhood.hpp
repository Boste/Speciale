#ifndef SWAPNEIGHBORHOOD_HPP
#define	SWAPNEIGHBORHOOD_HPP

#include "Move.hpp"
#include "Storage.hpp"
#include "Constants.hpp"
#include <memory>
#include "Neighborhood.hpp"
#include "Random.hpp"
#include "State.hpp"

class SwapNeighborhood : public Neighborhood {
public:
    unsigned testCounter = 0;
    std::shared_ptr<Storage> model;
    std::shared_ptr<State> state;
    SwapNeighborhood(std::shared_ptr<Storage> model, std::shared_ptr<State> st);
    SwapNeighborhood(const SwapNeighborhood& orig);
    ~SwapNeighborhood();

    Move* next();
    bool commitMove(Move* mv);
    bool calculateDelta(Move* mv);
    Move* nextRandom();
    unsigned getSize();


private:
    unsigned moveCounter = 0;
    unsigned moveCounter2 = 0;
    unsigned randomCounter = 0;
    unsigned randomMovesWanted = 0;

};



#endif	/* SWAPNEIGHBORHOOD_HPP */

