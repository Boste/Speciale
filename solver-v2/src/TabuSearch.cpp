#include "TabuSearch.hpp"

TabuSearch::TabuSearch(Neighborhood* neighborhood) {
    NE = neighborhood;
}


TabuSearch::~TabuSearch() {

}

bool TabuSearch::Start(unsigned iteration, std::shared_ptr<State>& bestState, std::shared_ptr<State>& currentState, std::vector<int>& tabulist) {
    bool allowed = false;
    Move* bestMove;
    unsigned tabuTenure = Random::Integer(0, 10) + std::min(NE->getSize()*2, (unsigned) (tabulist.size() / 200));
    bool isTabu = false;
    bestMove = NE->next();

    while (!allowed || isTabu) {
        if (bestMove != NULL) {
            allowed = NE->calculateDelta(bestMove);
            if (!allowed) {
                delete bestMove;
                bestMove = NE->next();
                continue;
            }
            isTabu = (iteration - tabulist.at(bestMove->var->getID())) <= tabuTenure;
            if (isTabu) {

                if (betterThanBest(currentState->getEvaluation(), bestMove->deltaVector, bestState->getEvaluation())) {
                    NE->commitMove(bestMove);
                    tabulist.at(bestMove->getVar()->getID()) = iteration;
                    delete bestMove;
                    return true;

                } else {
                    delete bestMove;

                    bestMove = NE->next();

                }
            }
        } else {
            return false;
        }
    }

    bool improvement = true;
    Move* mv = NE->next();
    while (mv != NULL) {
        allowed = NE->calculateDelta(mv);
        if (!allowed) {
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
                delete mv;
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
                    break;
                }

        }
        delete mv;

        mv = NE->next();

    }
    delete mv;


    for (int i = bestMove->deltaVector.size() - 1; i >= 0; i--) {
        if (bestMove->getDeltaVector().at(i) < 0) {
            improvement = true;
            break;
        } else if (bestMove->getDeltaVector().at(i) > 0) {
            improvement = false;
            break;
        }
    }
    allowed = NE->commitMove(bestMove);
    if (allowed) {
        tabulist.at(bestMove->getVar()->getID()) = iteration;
        delete bestMove;

        return improvement;
    } else {
        delete bestMove;
        std::cout << "Illegal move suggested as best move" << std::endl;
        debug;
        exit(1);
    }

    delete bestMove;
    return improvement;
}

bool TabuSearch::betterThanBest(std::vector<int>& current, std::vector<int>& delta, std::vector<int>& best) {
    if (best.size() == 1) {
    }
    if (current.size() == 1) {
    }
    if (delta.size() == 1) {
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