#include "FlipMove.hpp"
#include "Move.hpp"

FlipMove::FlipMove(IntegerVariable* iv) {
    variables.push_back(iv);
    deltaValue.push_back(1-iv->getCurrentValue() - iv->getCurrentValue());
}

FlipMove::FlipMove(const FlipMove& orig) {
}

FlipMove::~FlipMove() {
}
FlipMove::operator =(const FlipMove& a){
    
}
