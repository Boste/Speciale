#ifndef BESTIMPROVEMENT_HPP
#define	BESTIMPROVEMENT_HPP
#include "SearchEngine.hpp"
#include "Move.hpp"
#include "Neighborhood.hpp"
#include "Model.hpp"

class BestImprovement : public SearchEngine {
public:
    BestImprovement(std::shared_ptr<Model> model, Neighborhood* NE) ;
    bool Start();
//    BestImprovement(const BestImprovement& orig);
    ~BestImprovement();
private:
    std::shared_ptr<Model> model;
//    unsigned sidewalk = 0;
};

#endif	/* BESTIMPROVEMENT_HPP */

