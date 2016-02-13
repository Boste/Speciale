#include "FirstImprovement.hpp"

FirstImprovement::FirstImprovement( Neighborhood* neighborhood) {
    NE = neighborhood;

}

FirstImprovement::~FirstImprovement() {
}

bool FirstImprovement::Start() {
    bool legal = false;
  
    Move* firstMove = NE->next();
    while (firstMove != NULL) {
        legal = NE->calculateDelta(firstMove);
        if (!legal) {
            delete firstMove;
            firstMove = NE->next();
            continue;
        }
        for (int i = firstMove->deltaVector.size() - 1; i >= 0; i--) {
            if (firstMove->getDeltaVector().at(i) < 0) {
                NE->commitMove(firstMove);
                delete firstMove;
                return true;
            } else if (firstMove->getDeltaVector().at(i) > 0) {
                break;
            }

        }
        delete firstMove;
        firstMove = NE->next();



    }
    return false;
}