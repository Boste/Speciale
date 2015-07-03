#include "ValueChangeMove.hpp"
#include "Move.hpp"

ValueChangeMove::ValueChangeMove(IntegerVariable* iv, int nv) {
    variables.push_back(iv);
    newValue.push_back(nv);
    deltaValue.push_back(nv-iv->getCurrentValue());
}

ValueChangeMove::ValueChangeMove(const ValueChangeMove& orig) {
}

ValueChangeMove::~ValueChangeMove() {
}

ValueChangeMove& ValueChangeMove::operator=(const ValueChangeMove &a) {
    if (this != &a) {
        this->variables = a.variables;
        this->newValue = a.newValue;
        this->deltaValue = a.deltaValue;
    }
    return *this;
}

