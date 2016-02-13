#include "Neighborhood.hpp"

Neighborhood::Neighborhood() {
}

Neighborhood::~Neighborhood() {
}

/// return 1 if first move is best, 2 if second move is best, returns 0 if they are equal. (Use numberOfEqualMoves to determine which move to keep as "best" and remember to reset it when better move is found)

int Neighborhood::compareMoves(Move* mv1, Move* mv2) {
    std::vector<int>& mv1Delta = mv1->getDeltaVector();
    std::vector<int>& mv2Delta = mv2->getDeltaVector();
    for (int j = mv1Delta.size() - 1; j >= 0; j--) { // No ties in favor of newest move

        if (mv1Delta[j] < mv2Delta[j]) {
            return 1;

        } else if (mv1Delta[j] > mv2Delta[j]) {
            return 2;

        }
    }
    numberOfEqualMoves++;
    return 0;
}