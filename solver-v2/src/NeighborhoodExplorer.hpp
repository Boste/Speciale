#include "Move.hpp"
#include "Model.hpp"
#include "Constants.hpp"
#include <memory>
//#include "Random.hpp"
#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_int_distribution.hpp"
#include "Random.hpp"
#include "State.hpp"
#ifndef NEIGHBORHOODEXPLORER_HPP
#define	NEIGHBORHOODEXPLORER_HPP

class NeighborhoodExplorer {
public:
    std::shared_ptr<Model> model;
    NeighborhoodExplorer(std::shared_ptr<Model> model);
    NeighborhoodExplorer(const NeighborhoodExplorer& orig);
    virtual ~NeighborhoodExplorer();

    //    template<typename returnType>
    void randomWalk(Move* mv,std::shared_ptr<State> st);
    //    template<typename returnType>
    bool bestImprovement(Move* mv,std::shared_ptr<State> st);
    //    template<typename returnType>
//    bool firstImprovement(Move* mv, Model* st);
    
    void makeMove(Move* mv, std::shared_ptr<State> st);

private:
    //    template<typename returnType>
    void calculateDeltaChange(Move* mv);
//    std::pair<int, int> calculateDeltaChange(Move* mv, std::shared_ptr<State> st);
    void commitMove(Move* mv, std::shared_ptr<State> st);

};

#endif	/* NEIGHBORHOODEXPLORER_HPP */

