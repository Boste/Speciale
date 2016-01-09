#include "TabuSearch.hpp"

TabuSearch::TabuSearch(std::shared_ptr<Model> model, Neighborhood* neighborhood) {
    this->model = model;
    NE = neighborhood;
}

TabuSearch::TabuSearch(const TabuSearch& orig) {
}

TabuSearch::~TabuSearch() {
}

bool TabuSearch::Start() {
    bool legal = false;
    Move* bestMove;
    Move* mv; // = new Move();

    while (!legal) {
        if (NE->hasNext()) {
            bestMove = NE->next();
            legal = NE->calculateDelta(bestMove);
        } else {
            //            delete bestMove;
            //            delete mv;
            return false;
        }
    }


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
    //    while (improvement) {
    //    unsigned illegal = 0;
    //    unsigned legalmoves = 0;
    while (NE->hasNext()) {
        mv = NE->next();
        legal = NE->calculateDelta(mv);
        if (!legal) {
            //            illegal++;
            continue;
        }
        //        legalmoves++;
        int compare = NE->compareMoves(mv, bestMove);
        //        std::cout << "compare chose " << compare << std::endl;
        switch (compare) {
            case 1:
                //                std::cout << "case 1 " << std::endl;
                bestMove->copy(mv);
                //                bestMove.copy(mv);
                improvement = true;
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
        //        std::cout << "best move " << bestMove->deltaVector.at(0) << " " << bestMove->deltaVector.at(1) << std::endl;
        //        debug;
        //        std::cout << std::endl;
        delete mv;


    }
    //    std::cout << "improvement " << improvement << std::endl;
    //    for (unsigned i = 0; i < bestMove->deltaVector.size(); i++) {
    //        std::cout << bestMove->deltaVector.at(i) << " ";
    //    }
    //    std::cout << std::endl;
    for (unsigned i = 1; i < bestMove->deltaVector.size(); i++) {
        //    for (unsigned i = 1; i < bestMove.deltaVector.size(); i++) {
        if (bestMove->getDeltaVector().at(i) < 0) {
            //        if (bestMove.getDeltaVector().at(i) < 0) {
            improvement = true;
            break;
        } else if (bestMove->getDeltaVector().at(i) > 0) {
            //        } else if (bestMove.getDeltaVector().at(i) > 0) {
            improvement = false;
            break;
        }
        if (i == bestMove->deltaVector.size() - 1) {
            if (bestMove->getDeltaVector().at(0) >= 0) {
                improvement = false;
                break;
                //            }  else {
                //                improvement = true;
                //                break;
            }

        }
        //        if(i == 1 ){
        //            std::cout << "how did i get here? " << improvement<< std::endl;
        //        }
    }
    //    std::cout << "legal moves " << legalmoves << " illegal moves " << illegal << " total " << illegal + legalmoves << " improvement " << improvement << std::endl;
    //    debug;
    if (improvement) {
        /// Returns if it is a legal move or not
        //        Move* move = &bestMove;
        //        for (int d : bestMove->getDeltaVector()) {
        //            std::cout << d << " ";
        //        }
        //        std::cout << std::endl;


        bool legal = NE->commitMove(bestMove);
        //        bool legal = NE->commitMove(&bestMove);
        if (legal) {
            //            if (NE->commitMove(bestMove)) {
            delete bestMove;

            return true;
        } else {
            delete bestMove;
            std::cout << "Illegal move suggested as best move" << std::endl;
            debug;
            exit(1);
        }
        //            return true;
    }

    delete bestMove;
    return false;
}

bool TabuSearch::Start(unsigned iteration, std::shared_ptr<State>& bestState, std::shared_ptr<State>& currentState, std::vector<int>& tabulist) {
    bool legal = false;
    Move* bestMove;
    Move* mv; // = new Move();
            unsigned tabuTenure = Random::Integer(tabulist.size() / 200, tabulist.size() / 100);
//    unsigned tabuTenure = 0;
    //        std::cout << tabuTenure << std::endl;
    //        debug;
    //    unsigned tabuTenure = 3;
    bool isTabu = false;
    while (!legal || isTabu) {

        if (NE->hasNext()) {

            bestMove = NE->next();
            legal = NE->calculateDelta(bestMove);
            //            std::cout << iteration << " - " << tabulist.at(bestMove->var->getID()) << " > " << tabuTenure << std::endl;
            if (!legal) {
                delete bestMove;
                continue;
            }
            isTabu = (iteration - tabulist.at(bestMove->var->getID())) <= tabuTenure;
            if (isTabu) {
                if (betterThanBest(currentState->getEvaluation(), bestMove->deltaVector, bestState->getEvaluation())) {
                    if (legal) {
                        break;
                    }
                } else {
                    delete bestMove;
                }
            }
        } else {

            return false;
        }
    }
    //    std::cout << "best " << bestMove->deltaVector.at(0) << " " << bestMove->deltaVector.at(1) << std::endl;

    bool improvement = true;
//        for (int i = bestMove->deltaVector.size() - 1; i >= 0; i--) {
//            if (bestMove->deltaVector.at(i) < 0) {
//                //            improvement = true; 
//                break;
//            } else if (bestMove->deltaVector.at(i) > 0) {
//                improvement = false;
//                break;
//            }
//        }
    //    delete mv;
    int suggestedMove = 0;
    while (NE->hasNext()) {
        mv = NE->next();
        legal = NE->calculateDelta(mv);
        if (!legal) {
            //            illegal++;
            delete mv;
            continue;
        }
        isTabu = (iteration - tabulist.at(mv->var->getID())) <= tabuTenure;
        if (isTabu) {
            if (!betterThanBest(currentState->getEvaluation(), mv->deltaVector, bestState->getEvaluation())) {
                delete mv;
                continue;
            }
        }
        suggestedMove++;
        //        legalmoves++;
        //        if (mv->deltaVector.at(1) < 0) {
        //            std::cout << "mv \t" << mv->deltaVector.at(0) << " " << mv->deltaVector.at(1) << std::endl;
        //        }
        int compare = NE->compareMoves(mv, bestMove);
        //                std::cout << "compare chose " << compare << std::endl;
        switch (compare) {
            case 1:
                //                std::cout << "bestMpve " << bestMove->deltaVector.at(0) << " " << bestMove->deltaVector.at(1) << std::endl;

                //                std::cout << "mv \t" << mv->deltaVector.at(0) << " " << mv->deltaVector.at(1) << std::endl;

                bestMove->copy(mv);
                //                bestMove.copy(mv);
                //                improvement = true;
                NE->numberOfEqualMoves = 0;
                break;
            case 2:
                //                std::cout << "case 2 " << std::endl;
                break;
                //                NE->numberOfEqualMoves = 0;
            default:
                //                iteration - tabulist.at(mv->var->getID()) >
                //                std::cout << "case default " << std::endl;
                //                std::cout << "number of equal moves " << NE->numberOfEqualMoves << std::endl;
                int choose = Random::Integer(NE->numberOfEqualMoves);
                if (choose == 0) {
                    bestMove->copy(mv);
                    //                    bestMove->copy(mv);
                }

        }
        //        std::cout << "best move " << bestMove->deltaVector.at(0) << " " << bestMove->deltaVector.at(1) << std::endl;
        //        debug;
        //        std::cout << std::endl;
        delete mv;


    }
    //    std::cout << "improvement " << improvement << std::endl;
    //    for (unsigned i = 0; i < bestMove->deltaVector.size(); i++) {
    //        std::cout << bestMove->deltaVector.at(i) << " ";
    //    }
    //    std::cout << std::endl;
    //    bool oldImp = improvement;
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
        //        if(i == 1 ){
        //            std::cout << "how did i get here? " << improvement<< std::endl;
        //        }
    }
    //    if (oldImp != improvement) {
    //        for (int q : bestMove->deltaVector) {
    //            std::cout << q << " ";
    //
    //        }
    //        std::cout << std::endl;
    //        std::cout << oldImp << "   " << improvement << std::endl;
    ////    }
    //    assert(oldImp == improvement);
    //    std::cout << "legal moves " << legalmoves << " illegal moves " << illegal << " total " << illegal + legalmoves << " improvement " << improvement << std::endl;
    //    debug;
    //    if (improvement) {
    /// Returns if it is a legal move or not
    //        Move* move = &bestMove;
    //        for (int d : bestMove->getDeltaVector()) {
    //            std::cout << d << " ";
    //        }
    //        std::cout << std::endl;


    legal = NE->commitMove(bestMove);
    //    std::cout << "Number of suggested moves " << suggestedMove << std::endl;
    //    std::cout << "Commited move " << bestMove->deltaVector.at(0) << " " << bestMove->deltaVector.at(1) << std::endl;
    //    std::cout << improvement << std::endl;

    //    debug;
    //        bool legal = NE->commitMove(&bestMove);
    if (legal) {
        //            if (NE->commitMove(bestMove)) {
        tabulist.at(bestMove->getVar()->getID()) = iteration;
        //        std::cout <<bestMove->getVar()->getID() << " " << iteration << std::endl;
        delete bestMove;

        return improvement;
    } else {
        delete bestMove;
        std::cout << "Illegal move suggested as best move" << std::endl;
        debug;
        exit(1);
    }
    //            return true;
    //    }

    delete bestMove;
    return improvement;
}

bool TabuSearch::betterThanBest(std::vector<int> current, std::vector<int> delta, std::vector<int> best) {
    for (int i = current.size() - 1; i >= 0; i--) {
        if ((current.at(i) + delta.at(i)) < best.at(i)) {
            return true;
        } else if ((current.at(i) + delta.at(i)) > best.at(i)) {
            return false;
        }
    }
    //    debug;
    return false;
}