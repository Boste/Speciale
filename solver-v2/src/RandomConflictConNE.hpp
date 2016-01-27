#include "Move.hpp"
#include "Storage.hpp"
#include "Constants.hpp"
#include <memory>
#include "Neighborhood.hpp"
#include "Variable.hpp"
//#include "Random.hpp"
//#include "boost/random/mersenne_twister.hpp"
//#include "boost/random/uniform_int_distribution.hpp"
#include "Random.hpp"
#include "State.hpp"
#ifndef RANDOMCONFLICTCONNE_HPP
#define	RANDOMCONFLICTCONNE_HPP

class RandomConflictConNE : public Neighborhood {
public:
    std::shared_ptr<Storage> model;
    std::shared_ptr<State> state;
    RandomConflictConNE(std::shared_ptr<Storage> model, std::shared_ptr<State> st);
//    RandomConflictConNE(const RandomConflictConNE& orig);
    ~RandomConflictConNE();

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
    unsigned getSize();


private:
    unsigned moveCounter = 0;
    //    std::unordered_map<unsigned, invariant>::iterator moveIterator;
    std::vector<Variable*> varsInNeighborhood;
    bool firstMove = true;
    //    unsigned moveCounter2 = 0;
    unsigned randomCounter = 0;
    unsigned randomMovesWanted = 0;
    //    template<typename returnType>
    //    std::pair<int, int> calculateDeltaChange(Move* mv, std::shared_ptr<State> st);

};

#endif	/* MINCONFLICTFLIPNE_HPP */

