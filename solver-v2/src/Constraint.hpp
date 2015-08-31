#ifndef CONSTRAINT_HPP
#define	CONSTRAINT_HPP
#include "Invariant.hpp"
#include <memory>
class Constraint {
protected:
    int Violation = 0;
    int ViolationDegree = 0;
    int DeltaViolation = 0;
    int DeltaViolationDegree = 0;
    int priority;
    int type;
    std::vector<int> arguments;
    std::shared_ptr<Invariant> invariant;
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

    int getViolationDegree() {
        return ViolationDegree;
    }
    int getArgument(int i) {
        return arguments[i];
    }
    std::shared_ptr<Invariant> getInvariant(){
//    Invariant* getInvariant(){
        return invariant;
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

#endif	/* CONSTRAINT_HPP */

