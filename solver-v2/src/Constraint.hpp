#ifndef CONSTRAINT_HPP
#define	CONSTRAINT_HPP
#include "Invariant.hpp"
#include <memory>
#include <functional>

class Constraint {
protected:
    int Violation = 0;
    int ViolationDegree = 0;
    int DeltaViolation = 0;
    int DeltaViolationDegree = 0;
    int priority;
    int type;
    bool oneway = false;
//    bool containsOneway = false;
    int numberOfIntegerVariables = 0;
    unsigned domainSize;
    std::vector<int> arguments;
    //    std::vector<std::shared_ptr<Invariant>> invariants;
    std::shared_ptr<Invariant> invariant;
//    std::shared_ptr<Invariant> orgInvariant;
    //    Invariant* invariant;
    //    Invariant* invariant;


public:

    Constraint() {

    }

    //    Constraint(const Constraint &c) {
    //        this->DeltaViolation = c.DeltaViolation;
    //        this->DeltaViolationDegree = c.DeltaViolationDegree;
    //        this->Violation = c.Violation;
    //        this->ViolationDegree = c.ViolationDegree;
    //        this->arguments = c.arguments;
    //    }
    //
    //    Constraint& operator=(const Constraint &c) {
    //        this->DeltaViolation = c.DeltaViolation;
    //        this->DeltaViolationDegree = c.DeltaViolationDegree;
    //        this->Violation = c.Violation;
    //        this->ViolationDegree = c.ViolationDegree;
    //        this->arguments = c.arguments;
    //        return *this;
    //    }

    ~Constraint() {

    }

    void setNumberOfIntegerVariables(int number) {
        numberOfIntegerVariables = number;
    }

    int& getNumberOfIntegerVariables() {
        return numberOfIntegerVariables;
    }

    int getType() {
        return type;
    }

    int getDeltaViolation() {
        return DeltaViolation;
    }

    int getDeltaViolationDegree() {
        return DeltaViolationDegree;
    }

    int getViolation() {
        return Violation;
    }

    bool isOneway() {
        return oneway;
    }

    void isOneway(bool set) {
//        containsOneway = set;
        oneway = set;
    }

//    bool gotOneway() {
//        return containsOneway;
//    }

//    void gotOneway(bool set) {
//        containsOneway = set;
//    }

    int getViolationDegree() {
        return ViolationDegree;
    }

    int getArgument(int i) {
        return arguments[i];
    }

//    std::shared_ptr<Invariant>& getOrgInvariant() {
//        return orgInvariant;
//    }

    std::shared_ptr<Invariant>& getInvariant() {
        
        return invariant;
    }
    unsigned getDomainSize(){
        return domainSize;
    }
    //    std::shared_ptr<Invariant> getInvariant(int i){
    //        return invariants[i];
    //    }

    struct SortGreater {

        bool operator()(const std::shared_ptr<Constraint>& cons1,const std::shared_ptr<Constraint>& cons2) const {
//            std::cout << "sorter" << std::endl;
            
            return (cons1->getDomainSize() > cons2->getDomainSize());
        }
    };
//    struct SortGreater {
//
//        bool operator()(const std::shared_ptr<Constraint>& cons1,const std::shared_ptr<Constraint>& cons2) const {
////            std::cout << "sorter" << std::endl;
//
//            return (cons1->getOrgInvariant()->getVariables().size() > cons2->getOrgInvariant()->getVariables().size());
//        }
//    };

    bool operator<(Constraint& cons) const {
        std::cout << "used to sort <" << std::endl;
        return (invariant->getVariables().size() < cons.getInvariant()->getVariables().size());
    }

    bool operator>(Constraint& cons) const {
        std::cout << "used to sort >" << std::endl;
        return (invariant->getVariables().size() > cons.getInvariant()->getVariables().size());
    }

    virtual int setDeltaViolation() {
        std::cout << "setDeltaViolation called in Constraint.hpp" << std::endl;
        sleep(1);
        return 0;
    }

    virtual int setDeltaViolationDegree() {
        std::cout << "setDeltaViolationDegree() called in Constraint.hpp" << std::endl;
        sleep(1);
        return 0;
    }
    //   void updateViolation(int violation){
    //       Violation = violation;
    //   }
    //   void updateViolationDegree(int degree){
    //       ViolationDegree = degree;
    //   }
    // Return change 

    virtual int updateViolation() {
        std::cout << "Update Violation called in Constraint.hpp" << std::endl;
        sleep(1);
        return 0;
    }

    virtual int updateViolationDegree() {
        std::cout << "Update Violation Degree called in Constraint.hpp" << std::endl;
        sleep(1);
        return 0;
    }

    virtual bool testCons() {
        std::cout << "TestCons called in Constraint.hpp" << std::endl;
        sleep(1);
        return false;
    }

    virtual bool testObj() {
        std::cout << "TestObj called in Constraint.hpp" << std::endl;
        sleep(1);
        return false;
    }




};

class ConstraintSorter {
public:
    ConstraintSorter(){
        
    }
    bool operator()(std::shared_ptr<Constraint>& cons1, std::shared_ptr<Constraint>& cons2){
        return (cons1->getInvariant()->getVariables().size() > cons2->getInvariant()->getVariables().size());
    }
};

#endif	/* CONSTRAINT_HPP */

