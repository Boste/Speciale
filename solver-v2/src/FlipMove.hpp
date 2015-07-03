#include "IntegerVariable.hpp"
#ifndef FLIPMOVE_HPP
#define	FLIPMOVE_HPP

class FlipMove : public Move {
public:
    FlipMove(IntegerVariable* iv);
    FlipMove(const FlipMove& orig);
    virtual ~FlipMove();
        FlipMove& operator=(const FlipMove &a);

private:

};

#endif	/* FLIPMOVE_HPP */

