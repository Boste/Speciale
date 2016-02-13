#include "Move.hpp"
#include "Storage.hpp"
#include "Constants.hpp"
#include <memory>
#include "Neighborhood.hpp"
#include "Variable.hpp"
#include "Random.hpp"
#include "State.hpp"
#ifndef CONFLICTONLYNE_HPP
#define	CONFLICTONLYNE_HPP

class ConflictOnlyNE : public Neighborhood {
public:
    std::shared_ptr<Storage> model;
    std::shared_ptr<State> state;
    ConflictOnlyNE(std::shared_ptr<Storage> model, std::shared_ptr<State> st);
    ~ConflictOnlyNE();
    Move* next();
    bool commitMove(Move* mv);
    bool calculateDelta(Move* mv);
    Move* nextRandom();
    unsigned getSize();


private:
    unsigned moveCounter = 0;
    bool firstMove = true;
    unsigned randomCounter = 0;
    unsigned short iter = 0;
    std::vector<unsigned short> calculated;
    unsigned var;
    std::unordered_map<unsigned, invariant>::iterator  moveIterator;
    unsigned suggested = 0;
    unsigned  lastSuggested = 0;
    unsigned idCounter=0;

};

#endif	/* MINCONFLICTFLIPNE_HPP */

