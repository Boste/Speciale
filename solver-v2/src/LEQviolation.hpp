
#ifndef LEQVIOLATION_HPP
#define	LEQVIOLATION_HPP
#include <memory>
#include "Constants.hpp"
#include <unordered_map>
#include "Invariant.hpp"
class LEQviolation : public Invariant {
   
        
protected:
    
    std::vector<int> VariableChange;
    coefType RHS;
    invariant LHS;
    
    
public:
    
    LEQviolation(invariant invar, coefType RHS);
    
    

    ~LEQviolation();


    bool calculateDelta();
    void proposeChange(int variableNumber, int changeInValue);
    void updateValue();

    bool test();
};
#endif	/* LEQVIOLATION_HPP */

