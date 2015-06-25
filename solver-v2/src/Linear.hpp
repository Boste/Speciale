#ifndef LINEAR_HPP
#define	LINEAR_HPP
#include "Sum.hpp"
#include "Invariant.hpp"
#include "Constraint.hpp"

class Linear : public Constraint {
protected:
    Sum* lhs;
    int rhs;
    int relation; // 0 is equal, 1 is LQ

public:

    Linear(Sum* lhs, int ub, int relation) : Constraint() {
        this->relation = relation;
        this->lhs = lhs;
        rhs = ub;
    }

    Linear(Linear &c) : Constraint() {
        this->lhs = c.lhs;
    }

    Linear& operator=(const Linear &c) {
        this->lhs = c.lhs;
        return *this;
    }

    ~Linear() {
        delete lhs;
    }


    // either LQ or EQ constraint
    // Four cases either violated before or not, and either violated after or not. 
    int setDeltaViolation() {

        // if constraint is LQ
        if (relation == 1) {
            // if not violated before change
            if (Violation == 0) {
                // if not violated after change
                if (lhs->getDeltaValue() + lhs->getCurrentValue() <= rhs) {
                    return 0;
                } else {
                    return 1;
                }
                // violated before change
            } else {
                // if not violated after change
                if (lhs->getDeltaValue() + lhs->getCurrentValue() <= rhs) {
                    return -1;
                } else {
                    return 0;
                }
            }
            // if constraint is EQ
        } else {
            // if not violated before change
            if (Violation == 0) {
                // if not violated after change
                if (lhs->getDeltaValue() + lhs->getCurrentValue() == rhs) {
                    return 0;
                } else {
                    return 1;
                }
            } else {
                // if not violated after change
                if (lhs->getDeltaValue() + lhs->getCurrentValue() == rhs) {
                    return -1;
                } else {
                    return 0;
                }
            }
        }
        return 0;
    }

    int setDeltaViolationDegree() {
        int change = 0;
        if (relation == 1) {
            if (lhs->getCurrentValue() <= rhs) {
                change = -ViolationDegree;
                ViolationDegree = 0;
            } else {
                change = lhs->getCurrentValue() - rhs - ViolationDegree;
                ViolationDegree += change;
            }
        } else {
            if (lhs->getCurrentValue() == rhs) {
                change = -ViolationDegree;
                ViolationDegree = 0;
            } else {
                change = lhs->getCurrentValue() - ViolationDegree;
                ViolationDegree += change;
            }
        }
        return change;
    }

    int updateViolation() {
        int change = 0;
        if (relation == 1) {
            if (lhs->getCurrentValue() <= rhs) {
                change = -Violation;
                Violation = 0;
            } else {
                change = 1 - Violation;
                Violation = 1;
            }
        } else {
            if (lhs->getCurrentValue() == rhs) {
                change = -Violation;
                Violation = 0;
            } else {
                change = 1 - Violation;
                Violation = 1;
            }
        }
        return change;
    }

    int updateViolationDegree() {
        int change = 0;
        if (relation == 1) {
            if (lhs->getCurrentValue() <= rhs) {
                change = -ViolationDegree;
                ViolationDegree = 0;
            } else {
                change = lhs->getCurrentValue() - rhs - ViolationDegree;
                ViolationDegree += change;
            }
        } else {
            if (lhs->getCurrentValue() == rhs) {
                change = -ViolationDegree;
                ViolationDegree = 0;
            } else {
                change = lhs->getCurrentValue() - ViolationDegree;
                ViolationDegree += change;
            }
        }
        return change;
    }

};

#endif	/* LINEAR_HPP */

