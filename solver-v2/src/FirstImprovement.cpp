#include "FirstImprovement.hpp"

FirstImprovement::FirstImprovement( Neighborhood* neighborhood) {
//    this->model = model;
    NE = neighborhood;

}

//FirstImprovement::FirstImprovement(const FirstImprovement& orig) {
//}

FirstImprovement::~FirstImprovement() {
}

bool FirstImprovement::Start() {
    bool legal = false;
  
    Move* firstMove = NE->next();
    while (firstMove != NULL) {
//        Move* firstMove = NE->next();
        legal = NE->calculateDelta(firstMove);
        if (!legal) {
            delete firstMove;
            //            illegal++;
            firstMove = NE->next();
            continue;
        }
        //        legalmoves++;
        //        int compare = NE->compareMoves(mv, firstMove);
        for (int i = firstMove->deltaVector.size() - 1; i >= 0; i--) {
            //    for (unsigned i = 1; i < bestMove.deltaVector.size(); i++) {
            if (firstMove->getDeltaVector().at(i) < 0) {
                NE->commitMove(firstMove);
                delete firstMove;
                return true;
                //                break;
            } else if (firstMove->getDeltaVector().at(i) > 0) {
                break;
            }

        }
        delete firstMove;
        firstMove = NE->next();



    }

    //    std::cout << "legal moves " << legalmoves << " illegal moves " << illegal << " total " << illegal + legalmoves << " improvement " << improvement << std::endl;
    //    debug;
    //    if (improvement) {
    //        /// Returns if it is a legal move or not
    //        //        Move* move = &bestMove;
    //        //        std::cout << "Commiting improvement " << std::endl;
    ////        for (int d : firstMove->getDeltaVector()) {
    ////                        std::cout << d << " ";
    ////        }
    ////                std::cout << std::endl;
    //        bool legal = NE->commitMove(firstMove);
    //        //        bool legal = NE->commitMove(&bestMove);
    //        if (legal) {
    //            //            if (NE->commitMove(bestMove)) {
    //            delete firstMove;
    //
    //            return true;
    //        } else {
    //            delete firstMove;
    //            std::cout << "Illegal move suggested as best move" << std::endl;
    //            debug;
    //            exit(1);
    //        }
    //        //            return true;
    //    }

    //    delete firstMove;
    return false;
}