#ifndef SUM_HH
#define SUM_HH
#include <vector>

#include <unordered_map>
#include "IntegerVariable.hpp"
#include "boost/shared_ptr.hpp"
#include "Invariant.hpp" 
//#include <utility>
//struct elem {
//	int index;
//	double coeff;
//	~elem() {};
//};
using namespace std;

class Sum : public Invariant {
protected:
    std::unordered_map<int, int> coefficients;
    
    std::vector<IntegerVariable*> VariablePointers;
    //    double oldValue = 0;
    std::vector<std::pair<int, int>> VariableChange;

    
public:
    
    Sum(vector<IntegerVariable*>* vars, vector<int>* c);
    
    Sum(const Sum &a);

    ~Sum();

    Sum& operator=(const Sum &a);

//    void rollback() {
//        // Skal den overhovedet bruges?
//    }
//
//    void initialize() {
//        // er det hurtigere at løbe igennem invarianter end at løbe gennem updates?
//    }

    int calculateDeltaValue();
    
    void usedByConstraint(int constraint);
    void usedByObjective(int constraint);

    void addChange(int variableNumber, int changeInValue);


//    void addChange(std::vector<int> variableNumbers, std::vector<int> changes);

//    void addChangeSwap(int variableNumber1, int variableNumber2);


    bool test();


};



#endif

