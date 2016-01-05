#include "Move.hpp"
#include "Model.hpp"
#include "Constants.hpp"
#include <memory>
#include "Neighborhood.hpp"
//#include "Random.hpp"
//#include "boost/random/mersenne_twister.hpp"
//#include "boost/random/uniform_int_distribution.hpp"
#include "Random.hpp"
#include "State.hpp"
#ifndef MINCONFLICTFLIPNE_HPP
#define	MINCONFLICTFLIPNE_HPP

class MinConflictFlipNE : public Neighborhood {
public:
    std::shared_ptr<Model> model;
    std::shared_ptr<State> state;
    MinConflictFlipNE(std::shared_ptr<Model> model, std::shared_ptr<State> st);
    MinConflictFlipNE(const MinConflictFlipNE& orig);
    ~MinConflictFlipNE();

    //    template<typename returnType>
    //    void randomWalk(std::shared_ptr<State> st);
    //    template<typename returnType>
    //    bool bestImprovement(std::shared_ptr<State> st);
    //    template<typename returnType>
    //    bool firstImprovement(Move* mv, Model* st);

    //    void makeMove(Move* mv, std::shared_ptr<State> st);
    bool hasNext();
    Move* next();
    bool commitMove(Move* mv);
    bool calculateDelta(Move* mv);
    Move* nextRandom();
    bool hasNextRandom();
    void setRandomCounter(unsigned numberOfRandomMoves);


private:
    unsigned moveCounter = 0;
    std::set<invariant,compare_invariant>::iterator moveIterator;
    bool firstMove = true;
//    unsigned moveCounter2 = 0;
    unsigned randomCounter = 0;
    unsigned randomMovesWanted = 0;
    //    template<typename returnType>
    //    std::pair<int, int> calculateDeltaChange(Move* mv, std::shared_ptr<State> st);

};

#endif	/* MINCONFLICTFLIPNE_HPP */

