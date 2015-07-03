#include "IntegerVariable.hpp"

#ifndef VALUECHANGEMOVE_HPP
#define	VALUECHANGEMOVE_HPP

class ValueChangeMove : public Move {
public:
    ValueChangeMove(IntegerVariable* iv, int newValue);
    ValueChangeMove(const ValueChangeMove& orig);
    ~ValueChangeMove();
    ValueChangeMove& operator=(const ValueChangeMove &a);
private:

};

#endif	/* VALUECHANGEMOVE_HPP */

