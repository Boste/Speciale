#include "Move.hpp"
#include "Model.hpp"
#include "Constants.hpp"
#include <memory>
#include "Neighborhood.hpp"
#include "Variable.hpp"
//#include "Random.hpp"
//#include "boost/random/mersenne_twister.hpp"
//#include "boost/random/uniform_int_distribution.hpp"
#include "Random.hpp"
#include "State.hpp"
#ifndef CONFLICTONLYNE_HPP
#define	CONFLICTONLYNE_HPP

class ConflictOnlyNE : public Neighborhood {
public:
    std::shared_ptr<Model> model;
    std::shared_ptr<State> state;
    ConflictOnlyNE(std::shared_ptr<Model> model, std::shared_ptr<State> st);
//    ConflictOnlyNE(const ConflictOnlyNE& orig);
    ~ConflictOnlyNE();

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
//    bool hasNextRandom();
    void setRandomCounter(unsigned numberOfRandomMoves);
    unsigned getSize();


private:
    unsigned moveCounter = 0;
    //    std::unordered_map<unsigned, invariant>::iterator moveIterator;
//    std::vector<Variable*> varsInNeighborhood;
    bool firstMove = true;
    //    unsigned moveCounter2 = 0;
    unsigned randomCounter = 0;
    unsigned randomMovesWanted = 0;
    unsigned short iter = 0;
    std::vector<unsigned short> calculated;
    unsigned var;
    std::unordered_map<unsigned, invariant>::iterator  moveIterator;
    unsigned suggested = 0;
    unsigned  lastSuggested;
    //    template<typename returnType>
    //    std::pair<int, int> calculateDeltaChange(Move* mv, std::shared_ptr<State> st);

};

#endif	/* MINCONFLICTFLIPNE_HPP */

