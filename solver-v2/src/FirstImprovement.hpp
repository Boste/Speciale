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
    ~FirstImprovement();
private:
        Neighborhood* NE;

};

#endif	/* FIRSTIMPROVEMENT_HPP */

