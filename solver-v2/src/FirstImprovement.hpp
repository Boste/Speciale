#ifndef FIRSTIMPROVEMENT_HPP
#define	FIRSTIMPROVEMENT_HPP
//#include "SearchEngine.hpp"
#include "Move.hpp"
#include "Neighborhood.hpp"
//#include "Model.hpp"

class FirstImprovement {
public:
    FirstImprovement( Neighborhood* NE) ;
    bool Start();
//    BestImprovement(const BestImprovement& orig);
    ~FirstImprovement();
private:
//    std::shared_ptr<Model> model;
        Neighborhood* NE;

};

#endif	/* FIRSTIMPROVEMENT_HPP */

