#include "BestImprovement.hpp"

BestImprovement::BestImprovement(std::shared_ptr<Model> model, Neighborhood* neighborhood) : SearchEngine(neighborhood) {
    this->model = model;
}

//BestImprovement::BestImprovement(const BestImprovement& orig) {
//}

BestImprovement::~BestImprovement() {
}

bool BestImprovement::Start() {
    Move* bestMove = new Move();
    //    std::vector<int>& delta = bestMove->getDeltaVector();
    //    for (int& d : delta) {
    //        d = 0;
    //    }
    Move* mv; // = new Move();
    if (NE->hasNext()) {
        mv = NE->next();
    } else {
        return false;
    }
    NE->calculateDelta(mv);

    bestMove->copy(mv);
    delete mv;
    bool improvement = true;
    while (improvement) {
        while (NE->hasNext()) {
            mv = NE->next();
            NE->calculateDelta(mv);
            int compare = NE->compareMoves(mv, bestMove);
            switch (compare) {
                case 1:
                    bestMove->copy(mv);
                    NE->numberOfEqualMoves = 0;
                case 2:
                    NE->numberOfEqualMoves = 0;
                default:
                    int choose = Random::Integer(NE->numberOfEqualMoves);
                    if (choose == 0) {
                        bestMove->copy(mv);
                    }

            }
            delete mv;


        }
        for (unsigned i = 1; i < bestMove->deltaVector.size(); i++) {
            if (bestMove->getDeltaVector().at(i) < 0) {
                //                improvement = true;
                break;
            } else if (bestMove->getDeltaVector().at(i) > 0) {
                improvement = false;
                break;
            }
            if (i == bestMove->deltaVector.size() - 1) {
                if (bestMove->getDeltaVector().at(0) >= 0) {
                    improvement = false;
                }
            }
        }
        if (improvement) {
            NE->commitMove(bestMove);

        }


    }
    delete bestMove;

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


    //    delete bestMove;
    return false;
}