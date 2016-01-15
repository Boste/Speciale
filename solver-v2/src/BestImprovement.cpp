#include "BestImprovement.hpp"

BestImprovement::BestImprovement(std::shared_ptr<Model> model, Neighborhood* neighborhood) {
    this->model = model;
    NE = neighborhood;

}

//BestImprovement::BestImprovement(const BestImprovement& orig) {
//}

BestImprovement::~BestImprovement() {
}

bool BestImprovement::Start(bool alwaysCommit) {
    //    debug;
    bool allowed = false;
    Move* bestMove;
    Move* mv; // = new Move();

    while (!allowed) {
        //        debug;
        bestMove = NE->next();
        if (bestMove != NULL) {
            //            debug;
            //            bestMove = NE->next();
            allowed = NE->calculateDelta(bestMove);
            if (!allowed) {
                delete bestMove;
            }
        } else {
            //            debug;
            //            bestMove = NE->nextRandom();
            //            legal = NE->calculateDelta(bestMove);
            //
            //            if(legal){
            //                break;
            //            }
            //            std::cout << "not legal " << std::endl;
            //            debug;
            //            delete bestMove;
            //            delete mv;
            return false;
        }
    }

    //    debug;


    //    Move* bestMove = NE->next();
    //    NE->calculateDelta(bestMove);

    //    std::vector<int>& delta = bestMove->getDeltaVector();
    //    for (int& d : delta) {
    //        d = 0;
    //    }
    //    if (NE->hasNext()) {
    //        mv = NE->next();
    //    } else {
    //        delete bestMove;
    //        return false;
    //    }
    //    NE->calculateDelta(mv);
    //
    //    bestMove->copy(mv);
    //    delete mv;

    bool improvement = false;
    //    for(int i : bestMove->getDeltaVector()){
    //        if(i < 0){
    //            improvement = true;
    //            break;
    //        } else if(i> 0){
    //            break;
    //        }
    //    }
    //    while (improvement) {
    //        unsigned illegal = 0;
    //        unsigned legalmoves = 0;
    mv = NE->next();
    while (mv != NULL) {
        //        mv = NE->next();
        allowed = NE->calculateDelta(mv);
        if (!allowed) {
            delete mv;
            //                        illegal++;
            mv = NE->next();
            continue;
        }
        //                legalmoves++;
        int compare = NE->compareMoves(mv, bestMove);
        //        std::cout << "compare chose " << compare << std::endl;
        switch (compare) {
            case 1:
                //                std::cout << "case 1 " << std::endl;
                bestMove->copy(mv);
                //                bestMove.copy(mv);
                //                                improvement = true;
                NE->numberOfEqualMoves = 0;
                break;
            case 2:
                //                std::cout << "case 2 " << std::endl;
                break;
                //                NE->numberOfEqualMoves = 0;
            default:
                //                std::cout << "case default " << std::endl;
                //                std::cout << "number of equal moves " << NE->numberOfEqualMoves << std::endl;
                int choose = Random::Integer(NE->numberOfEqualMoves);
                if (choose == 0) {
                    bestMove->copy(mv);
                    //                    bestMove->copy(mv);
                }

        }
        //                std::cout << "best move " << bestMove->deltaVector.at(0) << " " << bestMove->deltaVector.at(1) << std::endl;
        //        debug;
        //        std::cout << std::endl;

        delete mv;
        mv = NE->next();

    }
    //    std::cout << "improvement " << improvement << std::endl;
    //    for (unsigned i = 0; i < bestMove->deltaVector.size(); i++) {
    //        std::cout << bestMove->deltaVector.at(i) << " ";
    //    }
    //    std::cout << std::endl;


    for (int i = bestMove->deltaVector.size() - 1; i >= 0; i--) {
        //    for (unsigned i = 1; i < bestMove.deltaVector.size(); i++) {
        if (bestMove->getDeltaVector().at(i) < 0) {
            //        if (bestMove.getDeltaVector().at(i) < 0) {
            //            sidewalk = 0;
            improvement = true;
            break;
        } else if (bestMove->getDeltaVector().at(i) > 0) {
            //            if (improvement = true) {
            //                    std::cout << "bestMove " << bestMove->getDeltaVector().at(0) << " " << bestMove->getDeltaVector().at(1) << std::endl;
            //
            //            }
            //        } else if (bestMove.getDeltaVector().at(i) > 0) {
            //            sidewalk = 0;
            improvement = false;
            break;
        }



        //        if(sidewalk == 1){
        //            sidewalk = 0;
        //            return false;
        //        } else {
        //            sidewalk++;
        //            return improvement;
        //        }
        //        if (i == bestMove->deltaVector.size() - 1) {
        //            if (bestMove->getDeltaVector().at(0) >= 0) {
        //                improvement = false;
        //                break;
        //                //            }  else {
        //                //                improvement = true;
        //                //                break;
        //            }
        //
        //        }
        //        if(i == 1 ){
        //    for (unsigned i = 1; i < bestMove->deltaVector.size(); i++) {
        //        //    for (unsigned i = 1; i < bestMove.deltaVector.size(); i++) {
        //        if (bestMove->getDeltaVector().at(i) < 0) {
        //            //        if (bestMove.getDeltaVector().at(i) < 0) {
        //            improvement = true;
        //            break;
        //        } else if (bestMove->getDeltaVector().at(i) > 0) {
        //            //        } else if (bestMove.getDeltaVector().at(i) > 0) {
        //            improvement = false;
        //            break;
        //        }
        //        if (i == bestMove->deltaVector.size() - 1) {
        //            if (bestMove->getDeltaVector().at(0) >= 0) {
        //                improvement = false;
        //                break;
        //                //            }  else {
        //                //                improvement = true;
        //                //                break;
        //            }
        //
        //        }
        //        //        if(i == 1 ){
        //            std::cout << "how did i get here? " << improvement<< std::endl;
        //        }
    }
    //        std::cout << "legal moves " << legalmoves << " illegal moves " << illegal << " total " << illegal + legalmoves << " improvement " << improvement << std::endl;
    //    debug;
    if (improvement || alwaysCommit) {
        /// Returns if it is a legal move or not
        //        Move* move = &bestMove;
        //        for (int d : bestMove->getDeltaVector()) {
        //            std::cout << d << " ";
        //        }
        //        std::cout << std::endl;


        allowed = NE->commitMove(bestMove);
        //        bool legal = NE->commitMove(&bestMove);
        if (allowed) {
            //            if (NE->commitMove(bestMove)) {
            delete bestMove;

            return improvement;
        } else {
            delete bestMove;
            //            return false;
            std::cout << "Illegal move suggested as best move" << std::endl;
            debug;
            exit(1);
        }
        //            return true;
    }



    delete bestMove;
    return false;
}