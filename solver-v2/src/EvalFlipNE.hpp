#include "Move.hpp"
#include "Storage.hpp"
#include "Constants.hpp"
#include <memory>
#include "Neighborhood.hpp"
#include "Random.hpp"
#include "State.hpp"
#ifndef EVALFLIPNE_HPP
#define	EVALFLIPNE_HPP

class EvalFlipNE : public Neighborhood {
public:
    std::shared_ptr<Storage> model;
    std::shared_ptr<State> state;
    EvalFlipNE(std::shared_ptr<Storage> model, std::shared_ptr<State> st);
    EvalFlipNE(const EvalFlipNE& orig);
    ~EvalFlipNE();

    Move* next();
    bool commitMove(Move* mv);
    bool calculateDelta(Move* mv);
    Move* nextRandom();
unsigned getSize();

private:
    unsigned moveCounter = 0;
    unsigned randomCounter = 0;
    unsigned randomMovesWanted = 0;

};

#endif	/* FLIPOBJNE_HPP */

