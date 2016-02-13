#ifndef CONSTRAINT_HPP
#define	CONSTRAINT_HPP
#include "Invariant.hpp"
#include <memory>
#include <functional>
#include "Constants.hpp"
#include "Random.hpp"
#include <unordered_map>
//#include "IntegerVariable.hpp"

class Constraint {
protected:
    int Violation = 0;

    int DeltaViolation = 0;

    int priority;
    int type;
    bool oneway = false;
    unsigned scopeSize;
    std::vector<int> arguments;
    std::vector<Variable*> variables;
    std::unordered_map<int, coefType> coefficients;
    Invariant* onewayInvariant;
    Variable* defining;
    bool functional = false;

public:

    Constraint() {

    }

    ~Constraint() {
        coefficients.clear();
        variables.clear();

        variables.shrink_to_fit();
    }
    int getType() {
        return type;
    }

    bool isOneway() {
        return oneway;
    }

    void isOneway(bool set) {
        oneway = set;
    }

    int getPriority() {
        return priority;
    }

    std::unordered_map<int, coefType>& getCoefficients() {
        return coefficients;
    }

    std::vector<Variable*>& getVariables() {
        return variables;
    }


    int getArgument(int i) {
        return arguments[i];
    }

    bool isFunctional() {
        return functional;
    }
    /// Returns if this constraint can be used to define one of its variables and the method should set which variable (Variable* defining) that should be defined (since it found now, but it is optional)
    virtual bool canBeMadeOneway() = 0;

    /// Create a oneway constraint defining a variable found in canBeMadeOneway() (or find it here)
    virtual invariant makeOneway() = 0;

/// The invariants can be used to speed up local search. The last invariant created is expected to be the one measuring violation.
    virtual InvariantContainer& createInvariants() = 0;

    unsigned getScope() {
        return variables.size();
    }

    struct prioritySort {

        bool operator()(const std::shared_ptr<Constraint>& cons1, const std::shared_ptr<Constraint>& cons2) const {
                        std::cout << "sorter" << std::endl;
            if(cons1->priority == cons2->priority){
                std::cout <<  cons1->getScope()<<  std::endl;
                int rand = Random::Integer(0,1);
                return (rand==1);
            }
            return (cons1->priority > cons2->getPriority());
        }
    };
    struct SortGreater {

        bool operator()(const std::shared_ptr<Constraint>& cons1, const std::shared_ptr<Constraint>& cons2) const {
            //            std::cout << "sorter" << std::endl;

            return (cons1->getScope() > cons2->getScope());
        }
    };

    struct Sortlower {

        bool operator()(const std::shared_ptr<Constraint>& cons1, const std::shared_ptr<Constraint>& cons2) const {
            //            std::cout << "sorter" << std::endl;

            return (cons1->getScope() < cons2->getScope());
        }
    };

    virtual bool testCons() {
        std::cout << "TestCons called in Constraint.hpp" << std::endl;
        sleep(1);
        return false;
    }

};

#endif	/* CONSTRAINT_HPP */

