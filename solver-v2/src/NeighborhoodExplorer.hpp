#include "Move.hpp"
#include "State.hpp"
#include "Constants.hpp"
//#include "Random.hpp"
#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_int_distribution.hpp"
#include "Random.hpp"
#ifndef NEIGHBORHOODEXPLORER_HPP
#define	NEIGHBORHOODEXPLORER_HPP

class NeighborhoodExplorer {
public:
    boost::random::mt19937 rnd;
    boost::random::uniform_int_distribution<> range;
    NeighborhoodExplorer();
    NeighborhoodExplorer(const NeighborhoodExplorer& orig);
    virtual ~NeighborhoodExplorer();

    //    template<typename returnType>
    void randomWalk(Move* mv, State* st);
    //    template<typename returnType>
    bool bestImprovement(Move* mv, State* st);
    //    template<typename returnType>
    bool firstImprovement(Move* mv, State* st);
    
    void makeMove(Move* mv ,State* st);

private:
    //    template<typename returnType>
    std::pair<int, int> calculateDeltaChange(Move* mv, State* st);
    void commitMove(Move* mv, State* st);

};

#endif	/* NEIGHBORHOODEXPLORER_HPP */

