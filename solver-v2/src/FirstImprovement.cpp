#include "FirstImprovement.hpp"

FirstImprovement::FirstImprovement(std::shared_ptr<Model> model, Neighborhood* neighborhood) {
    this->model = model;
    NE = neighborhood;

}

//FirstImprovement::FirstImprovement(const FirstImprovement& orig) {
//}

FirstImprovement::~FirstImprovement() {
}

bool FirstImprovement::Start() {
    bool legal = false;
    //    Move* firstMove;
    //    //    Move* mv; // = new Move();
    //
    //    while (!legal) {
    //        if (NE->hasNext()) {
    //            firstMove = NE->next();
    //            std::cout << "fisk" << std::endl;
    //            legal = NE->calculateDelta(firstMove);
    //            if(!legal){
    //                delete firstMove;
    //            }
    //        } else {
    //            //            delete bestMove;
    //            //            delete mv;
    //            return false;
    //        }
    //    }

    //    bool improvement = false;
    //    for (unsigned i = 1; i < firstMove->deltaVector.size(); i++) {
    //        if (firstMove->getDeltaVector().at(i) < 0) {
    //            improvement = true;
    //            break;
    //        } else if (firstMove->getDeltaVector().at(i) > 0) {
    //            improvement = false;
    //            delete firstMove;
    //
    //            break;
    //        }
    //        if (i == firstMove->deltaVector.size() - 1) {
    //            if (firstMove->getDeltaVector().at(0) < 0) {
    //                improvement = true;
    ////                delete firstMove;
    //
    //                break;
    //            } else {
    //                delete firstMove;
    //            }
    //        }
    //    }
    while (NE->hasNext()) {
        Move* firstMove = NE->next();
        legal = NE->calculateDelta(firstMove);
        if (!legal) {
            delete firstMove;
            //            illegal++;
            continue;
        }
        //        legalmoves++;
        //        int compare = NE->compareMoves(mv, firstMove);
        for (int i = firstMove->deltaVector.size() - 1; i >= 0; i--) {
            //    for (unsigned i = 1; i < bestMove.deltaVector.size(); i++) {
            if (firstMove->getDeltaVector().at(i) < 0) {
                //        if (bestMove.getDeltaVector().at(i) < 0) {
                //                improvement = true;
//                std::cout << "FirstMove: var " << firstMove->getVar()->getID() << " value " << firstMove->getVar()->getCurrentValue()<< " " <<  firstMove->getDeltaVector().at(0) << " " << firstMove->getDeltaVector().at(1) << std::endl;

                NE->commitMove(firstMove);
                delete firstMove;
                return true;
                //                break;
            } else if (firstMove->getDeltaVector().at(i) > 0) {
                //        } else if (bestMove.getDeltaVector().at(i) > 0) {
                //                improvement = false;
                //                delete firstMove;

                break;
            }

        }
        delete firstMove;



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