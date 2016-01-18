#include "TabuSearch.hpp"

TabuSearch::TabuSearch( Neighborhood* neighborhood) {
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
    Move* mv; // = new Move();


    //    unsigned tabuTenure = Random::Integer(0, 10) + std::min(NE->getSize()/5 , tabulist.size() / 100);
    unsigned tabuTenure = Random::Integer(0, 10) + std::min(NE->getSize()*2, (tabulist.size() / 200));
    //    unsigned tabuTenure = 0;
    //            std::cout << tabuTenure << std::endl;
    //        debug;
    //    unsigned tabuTenure = 3;
    bool isTabu = false;
    while (!allowed || isTabu) {
        bestMove = NE->next();
        if (bestMove != NULL) {
            //        if (NE->hasNext()) {

            bestMove = NE->next();
            allowed = NE->calculateDelta(bestMove);
            //            std::cout << iteration << " - " << tabulist.at(bestMove->var->getID()) << " > " << tabuTenure << std::endl;
            if (!allowed) {
                delete bestMove;
                bestMove = NE->next();

                continue;
            }
            isTabu = (iteration - tabulist.at(bestMove->var->getID())) <= tabuTenure;
            if (isTabu) {
                if (betterThanBest(currentState->getEvaluation(), bestMove->deltaVector, bestState->getEvaluation())) {
                    if (allowed) {
                        break;
                    }
                } else {
                    delete bestMove;
                    bestMove = NE->next();

                }
            }
        } else {

            return false;
        }
    }
    //    std::cout << "best " << bestMove->deltaVector.at(0) << " " << bestMove->deltaVector.at(1) << std::endl;

    bool improvement = true;
    //    int suggestedMove = 0;
    mv = NE->next();
    while (mv != NULL) {
        //    while (NE->hasNext()) {
        //        mv = NE->next();
        allowed = NE->calculateDelta(mv);
        if (!allowed) {
            //            illegal++;
            delete mv;
            mv = NE->next();

            continue;
        }
        isTabu = (iteration - tabulist.at(mv->var->getID())) <= tabuTenure;
        if (isTabu) {
            if (!betterThanBest(currentState->getEvaluation(), mv->deltaVector, bestState->getEvaluation())) {
                delete mv;
                mv = NE->next();

                continue;
            } else {
                NE->commitMove(bestMove);
                tabulist.at(bestMove->getVar()->getID()) = iteration;
                delete bestMove;
                return true;
            }
        }
        int compare = NE->compareMoves(mv, bestMove);
        switch (compare) {
            case 1:

                bestMove->copy(mv);
                NE->numberOfEqualMoves = 0;
                break;
            case 2:
                break;
            default:
                int choose = Random::Integer(NE->numberOfEqualMoves);
                if (choose == 0) {
                    bestMove->copy(mv);
                    //                    bestMove->copy(mv);
                }

        }
        delete mv;

        mv = NE->next();

    }
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