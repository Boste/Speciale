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
    Sum(std::vector<IntegerVariable*> vars, std::unordered_map<int,coefType> map);
//    Sum(std::unordered_map<int,coefType> map, unsigned id);
//    Sum(std::unordered_map<int,coefType> map,  std::shared_ptr<DependencyDigraph> DDG);
    Sum(std::unordered_map<int,coefType> map);
    
    
    Sum(const Sum &a);

    ~Sum();

//    Sum& operator=(const Sum &a);

//    void rollback() {
//        // Skal den overhovedet bruges?
//    }
//
//    void initialize() {
//        // er det hurtigere at løbe igennem invarianter end at løbe gennem updates?
//    }

    bool calculateDeltaValue();
//    void initialize();
    
//    void usedByConstraint(int constraint, int priority);
//    void usedByObjective(int constraint, int priority);

    void addChange(int variableNumber, int changeInValue);
    void updateValue();


//    void addChange(std::vector<int> variableNumbers, std::vector<int> changes);

//    void addChangeSwap(int variableNumber1, int variableNumber2);


    bool test();


};



#endif

