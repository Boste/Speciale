#include "RandomWalk.hpp"

RandomWalk::RandomWalk(Neighborhood* neighborhood, unsigned numberOfRandomMoves) {
    NE = neighborhood;
    this->numberOfRandomMoves = numberOfRandomMoves;
}

bool RandomWalk::Start() {
    for (unsigned i = 0; i < numberOfRandomMoves; i++) {
        bool allowed = false;
        while (!allowed) {

            Move* mv = NE->nextRandom();
            allowed = NE->commitMove(mv);

            delete mv;
        }
        i++;
    }
    return true;
}

RandomWalk::~RandomWalk() {
}

