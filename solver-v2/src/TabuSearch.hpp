#include "SearchEngine.hpp"
#include "Move.hpp"
#include "Neighborhood.hpp"
#include "Model.hpp"

#ifndef TABUSEARCH_HPP
#define	TABUSEARCH_HPP

class TabuSearch {
public:
    TabuSearch(std::shared_ptr<Model> model, Neighborhood* neighborhood);
//    TabuSearch(const TabuSearch& orig);
    virtual ~TabuSearch();

    //    bool Start();
    bool Start(unsigned iterations, std::shared_ptr<State>& bestState, std::shared_ptr<State>& currentState, std::vector<int>& tabulist);


private:
    std::shared_ptr<Model> model;
    bool betterThanBest(std::vector<int>& current, std::vector<int>& delta, std::vector<int>& best);
    Neighborhood* NE;
//    std::vector<unsigned short> calculated;
};

#endif	/* TABUSEARCH_HPP */

