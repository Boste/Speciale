#ifndef LINEAR_HPP
#define	LINEAR_HPP
#include "Sum.hpp"
#include "Invariant.hpp"
#include "Constraint.hpp"
#include "Constants.hpp"

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
        if (relation == LQ) {
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
        DeltaViolationDegree = lhs->getDeltaValue();
        return DeltaViolationDegree;
    }

    int updateViolation() {
        int change = 0;
        if (relation == LQ) {
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
        ViolationDegree += DeltaViolationDegree;

        return DeltaViolationDegree;
    }
    //    int updateViolationDegree() {
    //        int change = 0;
    //        if (relation == LQ) {
    //            if (lhs->getCurrentValue() <= rhs) {
    //                change = -ViolationDegree;
    //                ViolationDegree = 0;
    //            } else {
    //                change = lhs->getCurrentValue() - rhs - ViolationDegree;
    //                ViolationDegree += change;
    //            }
    //        } else {
    //            if (lhs->getCurrentValue() == rhs) {
    //                change = -ViolationDegree;
    //                ViolationDegree = 0;
    //            } else {
    //                change = lhs->getCurrentValue() - ViolationDegree;
    //                ViolationDegree += change;
    //            }
    //        }
    //        return change;
    //    }

    bool testCons() {
        if (lhs->getCurrentValue() <= rhs) {
            if (Violation != 0) {
                std::cout << "failed test Linear" << std::endl;
                std::cout << "Violation " << Violation << " lhs " << lhs << " rhs " << rhs << std::endl;
                return false;
            }
        } else {
            if (Violation != 1) {
                std::cout << "failed test Linear" << std::endl;
                std::cout << "Violation " << Violation << " lhs " << lhs << " rhs " << rhs << std::endl;
                return false;
            }
        }
        return true;
    }

    bool testObj() {
        if (ViolationDegree != lhs->getCurrentValue() - rhs) {
            std::cout << "failed test Linear" << std::endl;
            std::cout << "ViolationDegree " << ViolationDegree << " lhs " << lhs << " rhs " << rhs << std::endl;
            return false;

        }
        return true;
    }

};

#endif	/* LINEAR_HPP */

