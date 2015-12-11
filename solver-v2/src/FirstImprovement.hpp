#ifndef FIRSTIMPROVEMENT_HPP
#define	FIRSTIMPROVEMENT_HPP
#include "SearchEngine.hpp"
#include "Move.hpp"
#include "Neighborhood.hpp"
#include "Model.hpp"

class FirstImprovement : public SearchEngine {
public:
    FirstImprovement(std::shared_ptr<Model> model, Neighborhood* NE) ;
    bool Start();
//    BestImprovement(const BestImprovement& orig);
    ~FirstImprovement();
private:
    std::shared_ptr<Model> model;
};

#endif	/* FIRSTIMPROVEMENT_HPP */

