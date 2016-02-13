#ifndef SUM_HH
#define SUM_HH
#include <memory>
#include "Constants.hpp"
#include <unordered_map>
#include "Invariant.hpp"
class Sum : public Invariant {
   
        
protected:
    
    std::vector<int> VariableChange;
    
    
public:
    Sum(std::vector<Variable*> vars, std::unordered_map<int,coefType> map);
    Sum(std::unordered_map<int,coefType> map);
    Sum();
    Sum(const Sum &a);
    ~Sum();

    bool calculateDelta();

    void proposeChange(int variableNumber, int changeInValue);
    void updateValue();


    bool test();


};



#endif

