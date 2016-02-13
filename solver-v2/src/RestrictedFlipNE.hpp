#include "Move.hpp"
#include "Storage.hpp"
#include "Constants.hpp"
#include <memory>
#include "Neighborhood.hpp"
#include "Random.hpp"
#include "State.hpp"
#ifndef RESTRICTEDFLIPNE_HPP
#define RESTRICTEDFLIPNE_HPP


class RestrictedFlipNE : public Neighborhood {
public:
    std::shared_ptr<Storage> model;
    std::shared_ptr<State> state;
    RestrictedFlipNE(std::shared_ptr<Storage> model, std::shared_ptr<State> st);
    ~RestrictedFlipNE();
    bool hasNext();
    Move* next();
    bool commitMove(Move* mv);
    bool calculateDelta(Move* mv);
    Move* nextRandom();
    unsigned getSize();


private:
    unsigned moveCounter = 0;
    bool small;
    double probability;

};

#
#endif	/* RESTRICTEDFLIPNE_HPP */

