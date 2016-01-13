#include "RandomWalk.hpp"

RandomWalk::RandomWalk(std::shared_ptr<Model> model, Neighborhood* neighborhood, unsigned numberOfRandomMoves) {
    this->model = model;
    NE = neighborhood;
    //    NE->setRandomCounter(numberOfRandomMoves);
    this->numberOfRandomMoves = numberOfRandomMoves;
}

bool RandomWalk::Start() {
    //    if (mv->moveType == FLIP) {
    //    unsigned numberOfIllegalMoves = 0;
    for (unsigned i = 0; i < numberOfRandomMoves; i++) {
        //    unsigned  counter = 0;
        //    while (NE->hasNextRandom()) {
        bool legal = false;
        while (!legal) {

            Move* mv = NE->nextRandom();
            legal = NE->commitMove(mv);

            delete mv;
        }
        i++;
    }
    //    std::cout << counter << " ";
    //    if (numberOfIllegalMoves != 0) {
    //        std::cout << "Number of illegal moves suggested " << numberOfIllegalMoves << std::endl;
    //    }
    return true;
}

//RandomWalk::RandomWalk(const RandomWalk& orig) {
//}

RandomWalk::~RandomWalk() {
}

