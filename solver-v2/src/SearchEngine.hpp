#ifndef SEARCHENGINE_HPP
#define	SEARCHENGINE_HPP
#include "Neighborhood.hpp"
#include "Move.hpp"

class SearchEngine {
protected:
    Neighborhood* NE;

public:

    SearchEngine() {
//        NE = neighborhood;
    }



    virtual ~SearchEngine() {
    }
//    virtual bool Start(){
//        debug;
//        return false;
//    }
    bool Start() {
        std::cout <<  "Not implemented for this neighborhood " << std::endl;
        return false;
    }
     bool Start(unsigned iterations){
        std::cout <<  "Not implemented for this neighborhood. Iterations " << iterations << std::endl;
        return false;
    }

};
#endif	/* SEARCHENGINE_HPP */

