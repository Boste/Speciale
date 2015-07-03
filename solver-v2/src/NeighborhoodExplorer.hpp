#include "Move.hpp"
#ifndef NEIGHBORHOODEXPLORER_HPP
#define	NEIGHBORHOODEXPLORER_HPP

class NeighborhoodExplorer {
public:
    NeighborhoodExplorer();
    NeighborhoodExplorer(const NeighborhoodExplorer& orig);
    virtual ~NeighborhoodExplorer();
    
    template<typename returnType>
    void randomWalk(Move mv);
    template<typename returnType>
    void bestImprovement(Move mv);
    template<typename returnType>
    void firstImprovement(Move mv);
private:

};

#endif	/* NEIGHBORHOODEXPLORER_HPP */

