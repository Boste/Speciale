#include "TabuSearch.hpp"

TabuSearch::TabuSearch(Neighborhood* neighborhood) {
    //    this->model = model;
    NE = neighborhood;
    //    calculated.resize(neighborhood->getSize(),-1);
}

//TabuSearch::TabuSearch(const TabuSearch& orig) {
//}

TabuSearch::~TabuSearch() {

}

bool TabuSearch::Start(unsigned iteration, std::shared_ptr<State>& bestState, std::shared_ptr<State>& currentState, std::vector<int>& tabulist) {
    bool allowed = false;
    Move* bestMove;
    //    Move* mv; // = new Move();
    //    if (iteration == 23) {
    //        debug;
    //    }
    //    unsigned tabuTenure = Random::Integer(0, 10) + std::min(NE->getSize()/5 , tabulist.size() / 100);
    unsigned tabuTenure = Random::Integer(0, 10) + std::min(NE->getSize()*2, (unsigned) (tabulist.size() / 200));
    //    unsigned tabuTenure = 0;
    //            std::cout << tabuTenure << std::endl;
    //        debug;
    //    unsigned tabuTenure = 3;
    bool isTabu = false;
    bestMove = NE->next();

    while (!allowed || isTabu) {
        //        if (iteration == 23) {
        //            debug;
        //        }
        if (bestMove != NULL) {
            //            if (iteration == 23) {
            //                debug;
            //            }
            allowed = NE->calculateDelta(bestMove);
            //            std::cout << iteration << " - " << tabulist.at(bestMove->var->getID()) << " > " << tabuTenure << std::endl;
            if (!allowed) {

                //                debug;
                delete bestMove;
                bestMove = NE->next();
                continue;
            }
            isTabu = (iteration - tabulist.at(bestMove->var->getID())) <= tabuTenure;
            if (isTabu) {

                //                if (iteration == 23) {
                //                    debug;
                //                }
                if (betterThanBest(currentState->getEvaluation(), bestMove->deltaVector, bestState->getEvaluation())) {
                    //                    if (iteration == 23) {
                    //                        debug;
                    //                    }
                    NE->commitMove(bestMove);
                    tabulist.at(bestMove->getVar()->getID()) = iteration;
                    delete bestMove;
                    return true;

                } else {
                    //                    if (iteration == 23) {
                    //                        debug;
                    //                    }
                    delete bestMove;

                    bestMove = NE->next();

                }
            }
        } else {
            //            debug;
            return false;
        }
        //        if (iteration == 23) {
        //            debug;
        //        }
    }
    //    if (iteration == 23) {
    //        debug;
    //        delete bestMove;
    //        return false;
    //    }

    //    delete bestMove;
    //    return false;
    //    std::cout << "best " << bestMove->deltaVector.at(0) << " " << bestMove->deltaVector.at(1) << std::endl;

    bool improvement = true;
    //    int suggestedMove = 0;
    Move* mv = NE->next();
    while (mv != NULL) {
        //        if (iteration == 23) {
        //            debug;
        //        }
        //    while (NE->hasNext()) {
        //        mv = NE->next();
        allowed = NE->calculateDelta(mv);
        if (!allowed) {
//            if (iteration == 23) {
//                std::cout << "!allowed" << std::endl;
//                debug;
//            }
            //            illegal++;
            delete mv;
            mv = NE->next();

            continue;
        }
        isTabu = (iteration - tabulist.at(mv->var->getID())) <= tabuTenure;
        if (isTabu) {
            //            if (iteration == 23) {
            //                debug;
            //            }
            if (!betterThanBest(currentState->getEvaluation(), mv->deltaVector, bestState->getEvaluation())) {
                delete mv;
                mv = NE->next();
                //                if (iteration == 23) {
                //                    debug;
                //                }
                continue;
            } else {
                //                if (iteration == 23) {
                //                    debug;
                //                }
                NE->commitMove(bestMove);
                tabulist.at(bestMove->getVar()->getID()) = iteration;
                delete bestMove;
                delete mv;
                return true;
            }
        }
        int compare = NE->compareMoves(mv, bestMove);
        switch (compare) {
            case 1:
                //                if (iteration == 23) {
                //                    debug;
                //                }
                bestMove->copy(mv);
                NE->numberOfEqualMoves = 0;
                break;

            case 2:
                //                if (iteration == 23) {
                //                    debug;
                //                }
                break;
            default:
                //                if (iteration == 23) {
                //                    debug;
                //                }
                int choose = Random::Integer(NE->numberOfEqualMoves);
                if (choose == 0) {
                    bestMove->copy(mv);
                    break;
                }

        }
        //        if (iteration == 23) {
        //            debug;
        //        }
        delete mv;

        mv = NE->next();

    }
    //    if (iteration == 23) {
    //        debug;
    //    }
    delete mv;


    for (int i = bestMove->deltaVector.size() - 1; i >= 0; i--) {
        //    for (unsigned i = 1; i < bestMove.deltaVector.size(); i++) {
        if (bestMove->getDeltaVector().at(i) < 0) {
            //        if (bestMove.getDeltaVector().at(i) < 0) {
            //            sidewalk = 0;
            improvement = true;
            break;
        } else if (bestMove->getDeltaVector().at(i) > 0) {
            improvement = false;
            break;
        }
    }
    //    if (iteration == 23) {
    //        debug;
    //        return false;
    //    }
    allowed = NE->commitMove(bestMove);
    if (allowed) {
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

bool TabuSearch::betterThanBest(std::vector<int>& current, std::vector<int>& delta, std::vector<int>& best) {
    if (best.size() == 1) {
        std::cout << "best " << std::endl;
    }
    if (current.size() == 1) {
        std::cout << "current " << std::endl;
    }
    if (delta.size() == 1) {
        std::cout << "delta " << std::endl;
    }
    for (int i = current.size() - 1; i >= 0; i--) {
        if ((current.at(i) + delta.at(i)) < best.at(i)) {
            return true;
        } else if ((current.at(i) + delta.at(i)) > best.at(i)) {
            return false;
        }
    }
    return false;
}