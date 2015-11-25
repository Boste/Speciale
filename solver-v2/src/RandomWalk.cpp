#include "RandomWalk.hpp"

RandomWalk::RandomWalk(std::shared_ptr<Model> model, Neighborhood* neighborhood) : SearchEngine(neighborhood) {
    this->model = model;

}

bool RandomWalk::Start(unsigned iterations) {
    //    if (mv->moveType == FLIP) {
//    unsigned numberOfIllegalMoves = 0;
    for (unsigned i = 0; i < iterations; i++) {
        unsigned var = Random::Integer(0, model->getMask().size() - 1);
        //        if (var < 0 || model->getIntegerVariables()->size() <= var) {
        //            std::cout << "var " << var << std::endl;
        //            std::cout << model->getIntegerVariables()->size() << std::endl;
        //        }
//        assert(var < model->getMask().size());
        IntegerVariable* iv = model->getMaskAt(var);
        Move* mv = new Move(iv, 1 - iv->getCurrentValue() - iv->getCurrentValue());
        mv->deltaVector.resize(model->getPriorityVectors().size());
        //        mv->var = model->getNonFixedBinaryVariable(var);

        bool legal = NE->commitMove(mv);
        if (!legal) {
//            i--;
//            numberOfIllegalMoves++;
        }
        delete mv;
        //    } else {
        //        std::cout << "Only making flip moves" << std::endl;
        //    }

    }
//    if (numberOfIllegalMoves != 0) {
//        std::cout << "Number of illegal moves suggested " << numberOfIllegalMoves << std::endl;
//    }
    return true;
}

//RandomWalk::RandomWalk(const RandomWalk& orig) {
//}

RandomWalk::~RandomWalk() {
}

