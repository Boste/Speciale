#include "Move.hpp"
#include "Storage.hpp"
#include "Constants.hpp"
#include <memory>
#include "Neighborhood.hpp"
#include "Variable.hpp"
#include "Random.hpp"
#include "State.hpp"
#ifndef RANDOMCONFLICTCONNE_HPP
#define	RANDOMCONFLICTCONNE_HPP

class RandomConflictConNE : public Neighborhood {
public:
    std::shared_ptr<Storage> model;
    std::shared_ptr<State> state;
    RandomConflictConNE(std::shared_ptr<Storage> model, std::shared_ptr<State> st);
    ~RandomConflictConNE();
    bool hasNext();
    Move* next();
    bool commitMove(Move* mv);
    bool calculateDelta(Move* mv);
    Move* nextRandom();
    bool hasNextRandom();
    void setRandomCounter(unsigned numberOfRandomMoves);
    unsigned getSize();


private:
    unsigned moveCounter = 0;
    std::vector<Variable*> varsInNeighborhood;
    bool firstMove = true;
    unsigned randomCounter = 0;
    unsigned randomMovesWanted = 0;

};

#endif	/* MINCONFLICTFLIPNE_HPP */

