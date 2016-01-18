#include "TabuSearchFI.hpp"

TabuSearchFI::TabuSearchFI(std::shared_ptr<Model> model, Neighborhood* neighborhood) {
    this->model = model;
    NE = neighborhood;
}

//TabuSearchFI::TabuSearchFI(const TabuSearchFI& orig) {
//}

TabuSearchFI::~TabuSearchFI() {
}

bool TabuSearchFI::Start(unsigned iteration, std::shared_ptr<State>& bestState, std::shared_ptr<State>& currentState, std::vector<int>& tabulist) {
    bool legal = false;
    Move* bestMove;
    Move* mv; // = new Move();
    unsigned tabuTenure = Random::Integer(0, 10) + std::min(NE->getSize() / 5, (tabulist.size() / 100));
    //        std::cout << tabuTenure << std::endl;
    //        debug;
    //    unsigned tabuTenure = 3;
    bool isTabu = false;
    while (!legal || isTabu) {
        bestMove = NE->next();
        if (bestMove != NULL) {

            //            bestMove = NE->next();
            legal = NE->calculateDelta(bestMove);
            //            std::cout << iteration << " - " << tabulist.at(bestMove->var->getID()) << " > " << tabuTenure << std::endl;
            if (!legal) {
                delete bestMove;
                bestMove = NE->next();

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
                    bestMove = NE->next();

                }
            }
        } else {

            return false;
        }
    }
    std::vector<int> compareVector(bestMove->deltaVector.size(), 0);
    //    std::cout << "best " << bestMove->deltaVector.at(0) << " " << bestMove->deltaVector.at(1) << std::endl;

    bool improvement = false;
    for (int i = bestMove->deltaVector.size() - 1; i >= 0; i--) {
        if (bestMove->deltaVector.at(i) < 0) {
            improvement = true;
            break;
        } else if (bestMove->deltaVector.at(i) > 0) {
            //            improvement = false;
            break;
        }
    }
    if (improvement) {
        legal = NE->commitMove(bestMove);
        tabulist.at(bestMove->getVar()->getID()) = iteration;
        //        std::cout << "mv \t" << bestMove->deltaVector.at(0) << " " << bestMove->deltaVector.at(1) << "before while" << std::endl;

        delete bestMove;
        return improvement;
    }
    //    delete mv;
    //    int suggestedMove = 0;
    mv = NE->next();

    while (mv != NULL) {
        //        mv = NE->next();
        legal = NE->calculateDelta(mv);
        if (!legal) {
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
            }
        }
        //        suggestedMove++;
        for (int i = mv->deltaVector.size() - 1; i >= 0; i--) {
            //    for (unsigned i = 1; i < bestMove.deltaVector.size(); i++) {
            if (mv->getDeltaVector().at(i) < 0) {
                //        if (bestMove.getDeltaVector().at(i) < 0) {
                improvement = true;
                break;
            } else if (mv->getDeltaVector().at(i) > 0) {
                //        } else if (bestMove.getDeltaVector().at(i) > 0) {
                improvement = false;
                break;
            }

            //        if(i == 1 ){
            //            std::cout << "how did i get here? " << improvement<< std::endl;
            //        }
        }
        if (improvement) {
            legal = NE->commitMove(mv);
            tabulist.at(mv->getVar()->getID()) = iteration;
            //            std::cout << "mv \t" << mv->deltaVector.at(0) << " " << mv->deltaVector.at(1) << " improvement " << std::endl;
            //            std::cout << mv->var->getID() << std::endl;

            delete bestMove;
            delete mv;
            //            std::cout << "Number of suggested moves1 " << suggestedMove << std::endl;

            return improvement;
        }
        //        legalmoves++;
        //        if (mv->deltaVector.at(1) < 0) {
        //        }
        int compare = NE->compareMoves(mv, bestMove);
        //                std::cout << "compare chose " << compare << std::endl;
        switch (compare) {
            case 1:
                //                std::cout << "mv \t" << mv->deltaVector.at(0) << " " << mv->deltaVector.at(1) << " new best move" << std::endl;

                bestMove->copy(mv);
                NE->numberOfEqualMoves = 0;
                break;
            case 2:
                break;
            default:
                int choose = Random::Integer(NE->numberOfEqualMoves);
                if (choose == 0) {
                    bestMove->copy(mv);
                }

        }

        delete mv;
        mv = NE->next();


    }

    for (int i = bestMove->deltaVector.size() - 1; i >= 0; i--) {
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

        //        if(i == 1 ){
        //            std::cout << "how did i get here? " << improvement<< std::endl;
        //        }
    }


    legal = NE->commitMove(bestMove);
    //    std::cout << "Number of suggested moves2 " << suggestedMove << std::endl;
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
        //        std::cout << "Illegal move suggested as best move" << std::endl;
        debug;
        exit(1);
    }
    //            return true;
    //    }

    delete bestMove;
    return improvement;
}

bool TabuSearchFI::betterThanBest(std::vector<int> current, std::vector<int> delta, std::vector<int> best) {
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