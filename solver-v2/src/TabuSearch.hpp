#include "Move.hpp"
#include "Neighborhood.hpp"
#include "Storage.hpp"
#include "Random.hpp"

#ifndef TABUSEARCH_HPP
#define	TABUSEARCH_HPP

class TabuSearch {
public:
    TabuSearch( Neighborhood* neighborhood);
    virtual ~TabuSearch();

    bool Start(unsigned iterations, std::shared_ptr<State>& bestState, std::shared_ptr<State>& currentState, std::vector<int>& tabulist);


private:
        Neighborhood* NE;

    bool betterThanBest(std::vector<int>& current, std::vector<int>& delta, std::vector<int>& best);
};

#endif	/* TABUSEARCH_HPP */

    