/* 
 * File:   NeighborhoodExplorer.hpp
 * Author: bo
 *
 * Created on July 2, 2015, 11:01 AM
 */

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

