#include "Move.hpp"
#include "State.hpp"
#include "Constants.hpp"
#include <memory>
//#include "Random.hpp"
#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_int_distribution.hpp"
#include "Random.hpp"
#ifndef NEIGHBORHOODEXPLORER_HPP
#define	NEIGHBORHOODEXPLORER_HPP

class NeighborhoodExplorer {
public:
    NeighborhoodExplorer();
    NeighborhoodExplorer(const NeighborhoodExplorer& orig);
    virtual ~NeighborhoodExplorer();

    //    template<typename returnType>
    void randomWalk(Move* mv, std::shared_ptr<State> st);
    //    template<typename returnType>
    bool bestImprovement(Move* mv, std::shared_ptr<State> st);
    //    template<typename returnType>
//    bool firstImprovement(Move* mv, State* st);
    
    void makeMove(Move* mv ,std::shared_ptr<State> st);

private:
    //    template<typename returnType>
    std::pair<int, int> calculateDeltaChange(Move* mv, std::shared_ptr<State> st);
    void commitMove(Move* mv, std::shared_ptr<State> st);

};

#endif	/* NEIGHBORHOODEXPLORER_HPP */

