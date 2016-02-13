#ifndef LINEAR_HPP
#define	LINEAR_HPP
#include "Sum.hpp"
#include "Invariant.hpp"
#include "Constraint.hpp"
#include "Constants.hpp"
#include "LEQviolation.hpp"
#include "EQviolation.hpp"

class Linear : public Constraint {
protected:
    //    Sum* lhs;
    coefType rhs;
    int relation; // 0 is equal, 1 is LQ
    InvariantContainer invars;

public:


    /// Used to create the original (given by user) constraints

    Linear(std::vector<int> coefficients, std::vector<Variable*>& variables, coefType ub, int relation, unsigned priority) {
        this->relation = relation;

        this->variables = variables;
        for (unsigned i = 0; i < variables.size(); i++) {
            std::pair<int, coefType> coef(variables.at(i)->getID(), coefficients.at(i));
            this->coefficients.insert(coef);
        }
        rhs = ub;
        type = LINEAR;
        arguments.push_back(relation);
        arguments.push_back(ub);
        this->priority = priority;
        if (relation == EQ) {
            bool allUnit = true;
            for (int c : coefficients) {

                if (c != 1) {
                    if (c != -1) {
                        allUnit = false;
                        break;
                    }
                }
            }
            if (allUnit) {
                functional = true;
            }
        }
    }

    ~Linear() {
        //        delete lhs;

        coefficients.clear();
    }

    InvariantContainer& createInvariants() {
            int value = 0;
            std::unordered_map<int, coefType>& coef = this->getCoefficients();
            Sum* sumInvariant = new Sum(coef);
            variableContainer variables;
            InvariantContainer invariants;
            for (Variable* iv : this->getVariables()) {
                unsigned id = iv->getID();
                if (!iv->isDef()) {

                    value += coef.at(id) * iv->getCurrentValue();
                    variables.push_back(iv);
                } else {
                    value += coef.at(id) * iv->getOneway()->getCurrentValue();
                    invariants.push_back(iv->getOneway());
                }

            }
            sumInvariant->setVariablePointers(variables);
            sumInvariant->setInvariantPointers(invariants);
            assert(variables.size() + invariants.size() == coef.size());

            sumInvariant->setValue(value);
            invars.push_back(sumInvariant);
            if (this->getPriority() != OBJ) {

                if (relation == LQ) {
                    LEQviolation* leq = new LEQviolation(sumInvariant, rhs);
                    if (value <= rhs) {
                        leq->setValue(0);
                    } else {
    //                    leq->setValue(1);
                        leq->setValue(value -rhs);
                    }
                    invars.push_back(leq);
                } else if (relation == EQ) {
                    EQviolation* eq = new EQviolation(sumInvariant, rhs);
                    if (value == rhs) {
                        eq->setValue(0);
                    } else {
                        eq->setValue(std::abs(value - rhs));
                        //                    eq->setValue(1);
                    }
                    invars.push_back(eq);
                } else {
                    std::cout << "Only LEQ and EQ linear constraint are implemented" << std::endl;
                }

            }

            assert(invars.size()>0);
            return invars;
        }

    bool canBeMadeOneway() {
        if(!functional){
            return false;
        }
        if (this->isOneway()) {
            return false;
        }
        if (this->getArgument(0) == LQ) {
            return false;
        }
        for (auto it = coefficients.begin(); it != coefficients.end(); it++) {
            double coef = it->second;
            if (coef == 1 || coef == -1) {
            } else {
                return false;
            }
        }
        bool canBeMadeOneway = false;
        Variable* bestVar;
        unsigned prevEqual = 0;
        unsigned equalCounter = 0;
        unsigned defined = std::numeric_limits<unsigned int>::max();
        unsigned constraintsApplyingToiv = std::numeric_limits<unsigned>::max();

        for (Variable* iv : this->getVariables()) {
            if (iv->isFixed()) {
                continue;
            }
            if (iv->isDef()) {
                continue;
            }
            if (defined > iv->getDefining()) {
                bestVar = iv;
                canBeMadeOneway = true;
                constraintsApplyingToiv = iv->getDegree();
                prevEqual = 1;
                defined = iv->getDefining();
                continue;
            } else if (defined == iv->getDefining()) {

                    if (iv->getDegree() < constraintsApplyingToiv) {
                        bestVar = iv;
                        canBeMadeOneway = true;
                        constraintsApplyingToiv = iv->getDegree();
                        prevEqual = 1;
                        defined = iv->getDefining();
                    } else if (iv->getDegree() == constraintsApplyingToiv) {
                        int choose = Random::Integer(prevEqual);
                        prevEqual++;
                        equalCounter++;
                        if (choose == 0) {
                            bestVar = iv;
                            canBeMadeOneway = true;
                            constraintsApplyingToiv = iv->getDegree();
                            defined = iv->getDefining();
                        }
                    }
            }
        }

        if (canBeMadeOneway) {
            defining = bestVar;
            bestVar->increaseDefining();
        }
        return canBeMadeOneway;
    }

    invariant makeOneway() {
        Variable* var = defining;
        assert(priority != OBJ);
        variableContainer& oldVars = this->getVariables();
        std::unordered_map<int, coefType> coefficients = this->getCoefficients();
        std::unordered_map<int, coefType> newCoefficients;
        double coeff = coefficients.at(var->getID());

        coefType value = -this->getArgument(1);
        assert(coeff != 0);
        if (coeff == -1) {
            for (auto it = coefficients.begin(); it != coefficients.end(); ++it) {
                std::pair<int, coefType> coef(it->first, it->second);
                newCoefficients.insert(coef); 
            }
        } else {

            for (auto it = coefficients.begin(); it != coefficients.end(); ++it) {
                std::pair<int, coefType> coef(it->first, it->second / (-coeff));
                newCoefficients.insert(coef);
            }

            value = value / (-coeff);
        }
        if (coeff < -1 || 1 < coeff) {
            std::cout << coeff << std::endl;
            debug;
        }
        newCoefficients.erase(var->getID());
        Sum* sumInvariant = new Sum(newCoefficients);
        variableContainer varsAndInvars;

        sumInvariant->setStartValue(value);

        for (Variable* oldiv : oldVars) {
            if (oldiv != var) {
                unsigned id = oldiv->getID();
                value += newCoefficients.at(id) * (oldiv->getCurrentValue()*1.0);
                oldiv->increaseDefining();
                varsAndInvars.push_back(oldiv);
            }
        }

        sumInvariant->setVariablePointers(varsAndInvars);
        sumInvariant->setVariableID(var->getID());

        assert(value == (int) value);
        assert(var->getCurrentValue() >= var->getLowerBound());
        sumInvariant->setValue(value);
        sumInvariant->setVariable(var);
        assert(var == defining);
            
        var->setDefinedBy(sumInvariant, this);
        onewayInvariant = sumInvariant;
        sumInvariant->setBounds(var->getLowerBound(), var->getUpperBound());
        this->isOneway(true);
        assert(this->isOneway());
        return sumInvariant;


    }


    bool testCons() {
        return true;
    }

};

#endif	/* LINEAR_HPP */

