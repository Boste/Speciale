#ifndef INVARIANT_HPP
#define	INVARIANT_HPP

class Invariant {
public:

    Invariant() {
        //        std::cout << "invariant" << std::endl;
    }

    Invariant(const Invariant &a) {
        std::cout << &a << std::endl;
        
    }
    //    virtual void commit() = 0;
    //    {
    //        std::cout << "commit in invariant" << std::endl;
    //    }

    virtual void addChange(int variableNumber, int change) {
        std::cout << "addChange in invariant" << std::endl;
        std::cout << variableNumber << " " << change << std::endl;
    }

    virtual int calculateDeltaValue() {
        std::cout << "CalculateDeltaValue in  invariant" << std::endl;
        return 0;
    }

    int getDeltaValue() {
        return DeltaValue;
    }

    virtual bool test() {
        std::cout << "test in invariant" << std::endl;
        return false;
    }

    virtual ~Invariant() {
//        std::cout << "destruct invariant" << std::endl;
    }

    int getCurrentValue() {
        return CurrentValue;
    }

    void updateValue() {
        CurrentValue += DeltaValue;
    }

    int getUsedInConstraint() {
        return usedInConstraintNr;
    }

    int getUsedInObjective() {
        return usedInObjectiveNr;
    }

protected:
    double CurrentValue = 0;
    double DeltaValue = 0;
    int usedInConstraintNr = -1;
    int usedInObjectiveNr = -1;
private:

};


#endif	/* INVARIANT_HPP */

