#ifndef RANDOMWALK_HPP
#define	RANDOMWALK_HPP
#include "SearchEngine.hpp"
#include "Move.hpp"
#include "Neighborhood.hpp"
#include "Model.hpp"


class RandomWalk : public SearchEngine {
public:
    RandomWalk(std::shared_ptr<Model> model, Neighborhood* NE);
    bool Start(unsigned  iterations);
//    RandomWalk(const RandomWalk& orig);
    virtual ~RandomWalk();
private:
    std::shared_ptr<Model> model;

};

#endif	/* RANDOMWALK_HPP */
