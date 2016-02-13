    #include "BestImprovement.hpp"

BestImprovement::BestImprovement(Neighborhood* neighborhood) {
    NE = neighborhood;

}

BestImprovement::~BestImprovement() {
}

bool BestImprovement::Start(bool alwaysCommit) {
    bool allowed = false;
    Move* bestMove;
    Move* mv; // = new Move();

    while (!allowed) {
        bestMove = NE->next();
        if (bestMove != NULL) {
            allowed = NE->calculateDelta(bestMove);
            if (!allowed) {
                delete bestMove;
            }
        } else {

            return false;
        }
    }
    bool improvement = false;
   
    mv = NE->next();
    while (mv != NULL) {
        allowed = NE->calculateDelta(mv);
        if (!allowed) {
            delete mv;
            mv = NE->next();
            continue;
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
                }

        }
        delete mv;
        mv = NE->next();

    }
    for (int i = bestMove->deltaVector.size() - 1; i >= 0; i--) {
        if (bestMove->getDeltaVector().at(i) < 0) {
            improvement = true;
            break;
        } else if (bestMove->getDeltaVector().at(i) > 0) {
            improvement = false;
            break;
        }

    }
    if (improvement || alwaysCommit) {

        allowed = NE->commitMove(bestMove);
        if (allowed) {
            delete bestMove;

            return improvement;
        } else {
            delete bestMove;
            std::cout << "Illegal move suggested as best move" << std::endl;
            debug;
            exit(1);
        }
    }



    delete bestMove;
    return false;
}