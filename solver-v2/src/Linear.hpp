#ifndef LINEAR_HPP
#define	LINEAR_HPP
#include "Sum.hpp"
#include "Invariant.hpp"
#include "Constraint.hpp"
#include "Constants.hpp"

class Linear : public Constraint {
protected:
    //    Sum* lhs;
    int rhs;
    int relation; // 0 is equal, 1 is LQ

public:


    /// Used to create the original (given by user) constraints

    Linear(std::vector<int>& coefficients, std::vector<IntegerVariable*>& variables, int ub, int relation, unsigned priority) { //: Constraint() {
        //    Linear(Sum* lhs, int ub, int relation) : Constraint() {
        this->relation = relation;
        //        domainSize = lhs->getVariables().size();
        //        this->lhs = lhs;
        //        invariant = std::move(lhs);
        //        invariant = lhs;
        this->variables = variables;
        for (unsigned i = 0; i < variables.size(); i++) {
//            int id = variables.at(i)->getID();
//            this->coefficients[id] = coefficients.at(i);
            std::pair<int, coefType> coef(variables.at(i)->getID(), coefficients.at(i));
            this->coefficients.insert(coef);
        }
        rhs = ub;
        type = LINEAR;
        arguments.push_back(relation);
        arguments.push_back(ub);
        this->priority = priority;
    }
    /// Used to create the Linear constraint used by local search
    //    Linear(std::vector<std::shared_ptr<Sum>>&lhs, int ub, int relation) : Constraint() {
    //        this->relation = relation;
    //        //        this->lhs = lhs;
    //        //        invariant = std::move(lhs);
    //        for (std::shared_ptr<Sum> sum : lhs) {
    //            invariants.push_back(sum);
    //        }
    //        std::cout << &invariants << " vs " << &lhs << std::endl;
    //        std::cout << "In Linear constructor" << std::endl;
    //        sleep(1);
    //        rhs = ub;
    //        type = LINEAR;
    //        arguments.push_back(relation);
    //        arguments.push_back(ub);
    //    }
    //    Linear(Linear &c) : Constraint() {
    ////        this->lhs = c.lhs;
    //        invariant = c.invariant;
    //        this->relation = c.relation;
    //        this->rhs = c.rhs;
    //        this->type = c.type;
    //    }

    //    Linear& operator=(const Linear &c) {
    ////        this->lhs = c.lhs;
    //        invariant = c.invariant;
    //        this->relation = c.relation;
    //        this->rhs = c.rhs;
    //        this->type = c.type;
    //        return *this;
    //    }

    ~Linear() {
        //        delete lhs;

    }


    // either LQ or EQ constraint
    // Four cases either violated before or not, and either violated after or not. 

    int setDeltaViolation() {
        int deltaValue = 0;
        int currentValue = 0;
        std::shared_ptr<Invariant> invar = getInvariant();
        //        std::shared_ptr<Invariant> invar = invariant;
        //        for (std::shared_ptr<Invariant> invar : getInvariant()) {
        deltaValue += invar->getDeltaValue();
        currentValue += invar->getCurrentValue();
        //        }
        // if constraint is LQ
        if (relation == LQ) {
            // if not violated before change
            if (Violation == 0) {
                // if not violated after change

                if (deltaValue + currentValue <= rhs) {
                    DeltaViolation = 0;
                    return 0;
                } else {
                    DeltaViolation = 1;
                    return 1;
                }
                // violated before change
            } else {
                // if not violated after change
                if (deltaValue + currentValue <= rhs) {
                    DeltaViolation = -1;
                    return -1;
                } else {
                    DeltaViolation = 0;
                    return 0;
                }
            }
            // if constraint is EQ
        } else {
            // if not violated before change
            if (Violation == 0) {
                // if not violated after change
                if (deltaValue + currentValue == rhs) {
                    DeltaViolation = 0;
                    return 0;
                } else {
                    DeltaViolation = 1;
                    return 1;
                }
            } else {
                // if not violated after change
                if (deltaValue + currentValue == rhs) {
                    DeltaViolation = -1;
                    return -1;
                } else {
                    DeltaViolation = 0;
                    return 0;
                }
            }
        }
        return 0;
    }

    //    int setDeltaViolationDegree() {
    //        int deltaValue = 0;
    //        std::shared_ptr<Invariant> invar = getInvariant();
    //        //        for (std::shared_ptr<Invariant> invar : getInvariant()) {
    //        deltaValue += invar->getDeltaValue();
    //        //        }
    //
    //        //        std::cout << "obj func: " << invar->getID() << " violation degree " << deltaValue << std::endl;
    //
    //
    //        DeltaViolationDegree = deltaValue;
    ////        std::cout << "DeltaViolation Degree " << DeltaViolationDegree << std::endl;
    //        return DeltaViolationDegree;
    //    }

    // return the change in violation. Should be using delta i think (and should be void)

    int updateViolation() {
        int currentValue = 0;
        std::shared_ptr<Invariant> invar = getInvariant();
        //        for (std::shared_ptr<Invariant> invar : getInvariant()) {
        currentValue += invar->getCurrentValue();
        //        }
        assert(invar->getType() == SUM);
        int change = 0;
        if (relation == LQ) {
            if (currentValue <= rhs) {
                change = -Violation;
                Violation = 0;
            } else {
                change = 1 - Violation;
                Violation = 1;
            }
        } else {
            if (currentValue == rhs) {
                change = -Violation;
                Violation = 0;
            } else {
                change = 1 - Violation;
                Violation = 1;
            }
        }

        return change;
    }

    //    int updateViolationDegree() {
    //        assert(getInvariant()->isUsedByConstraint());
    ////        std::cout << getInvariant()->getID() << std::endl;
    ////        std::cout << ViolationDegree << " + " << DeltaViolationDegree << " = " << getInvariant()->getCurrentValue() << std::endl;
    //
    //        ViolationDegree = getInvariant()->getCurrentValue();
    //        //        ViolationDegree += DeltaViolationDegree;
    ////        std::cout <<  "update Violation Degree " << std::endl;
    //        return ViolationDegree;
    //    }
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
        int currentValue = 0;
        std::shared_ptr<Invariant> invar = getInvariant();

        //        for (std::shared_ptr<Invariant> invar : getInvariant()) {
        currentValue += invar->getCurrentValue();
        //        }
        if (currentValue <= rhs) {
            if (Violation != 0) {
                std::cout << "failed test Linear" << std::endl;
                std::cout << "Violation " << Violation << " lhs " << invar->getCurrentValue() << " rhs " << rhs << std::endl;
                return false;
            }
        } else {
            if (Violation != 1) {
                std::cout << "failed test Linear" << std::endl;
                std::cout << "Violation " << Violation << " lhs " << invar->getCurrentValue() << " rhs " << rhs << std::endl;
                return false;
            }
        }
        return true;
    }

    //    bool testObj() {
    //
    //        int currentValue = 0;
    //        std::shared_ptr<Invariant> invar = getInvariant();
    //
    //        //        for (std::shared_ptr<Invariant> invar : getInvariant()) {
    //        currentValue += invar->getCurrentValue();
    //        //        }
    //        if (ViolationDegree != currentValue - rhs) {
    //            std::cout << "failed test Linear obj" << std::endl;
    //            std::cout << "ViolationDegree " << ViolationDegree << " lhs " << invar->getCurrentValue() << " rhs " << rhs << std::endl;
    //            return false;
    //
    //        }
    //        return true;
    //    }

};

#endif	/* LINEAR_HPP */

