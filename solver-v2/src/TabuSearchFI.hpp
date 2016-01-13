#include "SearchEngine.hpp"
#include "Move.hpp"
#include "Neighborhood.hpp"
#include "Model.hpp"

#ifndef TABUSEARCHFI_HPP
#define	TABUSEARCHFI_HPP

class TabuSearchFI : SearchEngine {
public:
    TabuSearchFI(std::shared_ptr<Model> model, Neighborhood* neighborhood);
//    TabuSearchFI(const TabuSearchFI& orig);
    virtual ~TabuSearchFI();

    bool Start();
    bool Start(unsigned iterations, std::shared_ptr<State>& bestState, std::shared_ptr<State>&  currentState, std::vector<int>& tabulist);


private:
    std::shared_ptr<Model> model;
    unsigned sidewalks = 0;
    bool betterThanBest(std::vector<int> current, std::vector<int> delta, std::vector<int> best);
};

#endif	/* TABUSEARCHFI_HPP */

