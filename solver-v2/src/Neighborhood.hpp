#ifndef NEIGHBORHOOD_HPP
#define	NEIGHBORHOOD_HPP
#include "Move.hpp" 
#include "State.hpp"

class Neighborhood {
public:
    Neighborhood();
    //    Neighborhood(const Neighborhood& orig) = 0;
    virtual ~Neighborhood() = 0;
    virtual bool hasNext() = 0;
    virtual Move* next() = 0;
    virtual void calculateDelta(Move* mv) = 0;
    virtual void commitMove(Move* mv) = 0;
    virtual int compareMoves(Move* mv1, Move* mv2);
    int numberOfEqualMoves = 0;


private:
protected:
};

#endif	/* NEIGHBORHOOD_HPP */

