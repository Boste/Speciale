#ifndef CONSTRAINT_HPP
#define	CONSTRAINT_HPP

class Constraint {
protected:
    int Violation = 0;
    int ViolationDegree = 0;
    int DeltaViolation = 0;
    int DeltaViolationDegree = 0;


public:

    Constraint() {
    }

    Constraint(const Constraint &c) {
        this->DeltaViolation = c.DeltaViolation;
        this->DeltaViolationDegree = c.DeltaViolationDegree;
        this->Violation = c.Violation;
        this->ViolationDegree = c.ViolationDegree;
    }

    Constraint& operator=(const Constraint &c) {
        this->DeltaViolation = c.DeltaViolation;
        this->DeltaViolationDegree = c.DeltaViolationDegree;
        this->Violation = c.Violation;
        this->ViolationDegree = c.ViolationDegree;
        return *this;
    }

    ~Constraint() {

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




};

#endif	/* CONSTRAINT_HPP */

