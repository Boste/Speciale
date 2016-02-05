#ifndef BESTIMPROVEMENT_HPP
#define	BESTIMPROVEMENT_HPP
#include "SearchEngine.hpp"
#include "Move.hpp"
#include "Neighborhood.hpp"
//#include "Model.hpp"

class BestImprovement {
public:
    BestImprovement(Neighborhood* NE);
    bool Start(bool alwaysCommit);
    //    BestImprovement(const BestImprovement& orig);
    ~BestImprovement();
private:
    Neighborhood* NE;

    //    std::shared_ptr<Model> model;
    //    unsigned sidewalk = 0;
};

#endif	/* BESTIMPROVEMENT_HPP */

