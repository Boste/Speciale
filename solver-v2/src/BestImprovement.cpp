#include "BestImprovement.hpp"

BestImprovement::BestImprovement(std::shared_ptr<Model> model, Neighborhood* neighborhood) {
    this->model = model;
    NE = neighborhood;

}

//BestImprovement::BestImprovement(const BestImprovement& orig) {
//}

BestImprovement::~BestImprovement() {
}

bool BestImprovement::Start() {
//    debug;
    bool legal = false;
    Move* bestMove;
    Move* mv; // = new Move();

    while (!legal) {
//        debug;
        if (NE->hasNext()) {
//            debug;
            bestMove = NE->next();
            legal = NE->calculateDelta(bestMove);
        } else {
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


    //    }
    //    delete bestMove;

    //    bool improvement = false;
    //    //    std::cout <<model->getMask().size() << std::endl;
    //    for (IntegerVariable* iv : model->getMask()) {
    //        //    for (unsigned i = 0; i < model->getNonFixedBinaryVariables().size(); i++) {
    //        //        for(IntegerVariable* iv : *model->getIntegerVariables()){
    //        //        IntegerVariable* iv = model->getMaskAt(i);
    //        assert(!iv->isDef() && !iv->isFixed());
    //        mv->var = iv;
    //        //        mv->deltaValueFirst = 1 - iv->getCurrentValue() - iv->getCurrentValue();
    //        mv->flip();
    //        calculateDeltaChange(mv);
    //
    //        bool fewerViolations = false;
    //        bool moreViolations = false;
    //        //        for (int i : mv->getDeltaChanges()) {
    //        //            std::cout << i << " ";
    //        //        }
    //        //        std::cout << std::endl;
    //
    //
    //        for (unsigned j = 1; j < mv->getDeltaVector().size(); j++) { // No ties in favor of newest move
    //
    //            if (mv->getDeltaVector()[j] < bestMove->getDeltaVector()[j]) {
    //                //                std::cout << "prob never here" << std::endl;
    //                //                
    //                //                std::cout << "fewer violations" << std::endl;
    //                fewerViolations = true;
    //                break;
    //            } else if (mv->getDeltaVector()[j] > bestMove->getDeltaVector()[j]) {
    //                moreViolations = true;
    //                //                std::cout << "more violations" << std::endl;
    //                break;
    //            }
    //        }
    //        if (!moreViolations) {
    //            if (fewerViolations || mv->getDeltaVector()[0] <= bestMove->getDeltaVector()[0]) {
    //                //                for (int i : delta) {
    //                //                    std::cout << i << " ";
    //                //                }
    //                //                std::cout << std::endl;
    //                //                bestDelta = delta;
    //                //                std::cout << "got new best move " << std::endl;
    //                //            violationChange = delta.first;
    //                //            objectiveChange = delta.second;
    //                improvement = true;
    //                bestMove->copy(mv);
    //            }
    //        }
    //    }
    //    if (improvement) {
    //        //    std::cout << "commit delta" << std::endl;
    //        //        for (int i : bestMove->getDeltaChanges()) {
    //        //            std::cout << i << " ";
    //        //        }
    //        //        std::cout << std::endl;
    //        //        std::cout << "this was one best iter " << std::endl;
    //
    //        unsigned changeAt = 0;
    //        std::vector<int> bestDelta = bestMove->getDeltaVector();
    //        for (unsigned i = 1; i < bestDelta.size(); i++) {
    //            if (bestDelta[i] != 0) {
    //                changeAt = i;
    //                break;
    //            }
    //        }
    //        if (changeAt > 0) {
    //            if (bestDelta[changeAt] > 0) {
    //                //                delete bestMove;
    //                return false;
    //            } else {
    //                //            std::cout << "commit move" << std::endl;
    //                commitMove(bestMove, st);
    //                //                std::cout << "obj val " << st->getObjectiveValue() << std::endl;
    //                return true;
    //            }
    //        } else if (bestDelta[0] < 0) {
    //            commitMove(bestMove, st);
    //            //            std::cout << "obj val " << st->getObjectiveValue() << std::endl;
    //            return true;
    //
    //            //        std::cout << "commit move" << std::endl;
    //        }
    //    }

    //    return true;
    //    if (violationChange <= 0 && objectiveChange < 0) {
    //       commitMove(bestMove, st);
    //    } else {
    //    delete bestMove;
    //        std::cout << "no improving move" << std::endl;
    //        return false;
    //        std::exit(1);


    delete bestMove;
    return false;
}