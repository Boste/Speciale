#ifndef SUM_HH
#define SUM_HH
#include <memory>
#include "Constants.hpp"
#include <unordered_map>
//#include "IntegerVariable.hpp"
#include "Invariant.hpp"
//#include "DependencyDigraph.hpp" 
//#include "DependencyDigraph.hpp"
//#include <utility>
//struct elem {
//	int index;
//	double coeff;
//	~elem() {};
//};
class Sum : public Invariant {
   
        
protected:
    
//    std::vector<IntegerVariable*> VariablePointers;
    //    double oldValue = 0;
//    std::vector<std::pair<int, int>> VariableChange;
    std::vector<int> VariableChange;
//    std::shared_ptr<DependencyDigraph> DDG;
    
    
public:
    
//    Sum(std::vector<IntegerVariable*>& vars, std::vector<int>& c, unsigned id);
    /// Purely used for testing
    Sum(std::vector<Variable*> vars, std::unordered_map<int,coefType> map);
//    Sum(std::unordered_map<int,coefType> map, unsigned id);
//    Sum(std::unordered_map<int,coefType> map,  std::shared_ptr<DependencyDigraph> DDG);
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

