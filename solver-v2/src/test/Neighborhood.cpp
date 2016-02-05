#include "Neighborhood.hpp"

Neighborhood::Neighborhood() {
}
//
//Neighborhood::Neighborhood(const Neighborhood& orig) {
//}
//

Neighborhood::~Neighborhood() {
}

/// return 1 if first move is best, 2 if second move is best, returns 0 if they are equal. (Use numberOfEqualMoves to determine which move to keep as "best" and remember to reset it when better move is found)

int Neighborhood::compareMoves(Move* mv1, Move* mv2) {
    //    std::vector<int>& mv1Delta = mv1->getDeltaVector();
    //    std::vector<int>& mv2Delta = mv2->getDeltaVector();
    std::vector<int>& mv1Delta = mv1->getDeltaVector();
    std::vector<int>& mv2Delta = mv2->getDeltaVector();
//    std::cout << "mv1delta " << mv1Delta.at(0) << " " << mv1Delta.at(1) << std::endl;
//    std::cout << "mv2delta " << mv2Delta.at(0) << " " << mv2Delta.at(1) << std::endl;
//    debug;
    //    for (unsigned j = mv1Delta.size() - 1; j >= 0; j--) { // No ties in favor of newest move
    for (int j = mv1Delta.size() - 1; j >= 0; j--) { // No ties in favor of newest move

        if (mv1Delta[j] < mv2Delta[j]) {
//            std::cout << "return 1 " << std::endl;
//            debug;
            //            std::cout << mv1Delta[j] << " < " << mv2Delta[j] << std::endl;
            return 1;

        } else if (mv1Delta[j] > mv2Delta[j]) {
            //            std::cout << mv1Delta[j] << " > " << mv2Delta[j] << std::endl;
//            std::cout << "return 2 " << std::endl;
//            debug;
            return 2;
            //                std::cout << "more violations" << std::endl;

        }
    }
    //    if (mv1Delta.front() != mv2Delta.front()) { 
    //        if (mv1Delta[0] < mv2Delta[0]) {
    //            return 1;
    //        } else if(mv1Delta[0] > mv2Delta[0]){
    //            return 2;
    //        }
    //    } else {
//    std::cout << "return 0 " << std::endl;
//    debug;
    numberOfEqualMoves++;
    return 0;
    //    }
    //    assert(false);
    //    return -1;
    //    }
    //
}

//bool Neighborhood::hasNext() {
//    debug;
//    return false;
//}
//
////Move* next();
//
//Move* Neighborhood::next() {
//    Move* mv = new Move();
//    debug;
//    return mv;
//}
//
////void Neighborhood::calculateDelta(Move* mv)  =0;
////    void Neighborhood::calculateDelta(Move* mv)  {
////    debug;
////}
//
//void Neighborhood::commitMove(Move* mv) {
//    debug;
//
//}