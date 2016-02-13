
#ifndef EQVIOLATION_HPP
#define	EQVIOLATION_HPP
#include <memory>
#include "Constants.hpp"
#include <unordered_map>
#include "Invariant.hpp"
class EQviolation : public Invariant {
   
        
protected:
    
    std::vector<int> VariableChange;
    int RHS;
    invariant LHS;
    
    
public:
    
    EQviolation(invariant invar, int RHS);
    
    

    ~EQviolation();

    bool calculateDelta();
    void proposeChange(int variableNumber, int changeInValue);
    void updateValue();

    bool test();
};
#endif	/* EQVIOLATION_HPP */

