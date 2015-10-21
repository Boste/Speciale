
#ifndef MAX_HPP
#define	MAX_HPP
#include "Constants.hpp"
#include "Invariant.hpp"
#include "DependencyDigraph.hpp" 

class Max : public Invariant {
public:

    //    Max(T1 first, T2 sec, unsigned id);
    Max(IntegerVariable* first, int sec, unsigned variableID, std::shared_ptr<DependencyDigraph> DDG);
    Max(invariant first, int sec, unsigned id, std::shared_ptr<DependencyDigraph> DDG);
    //    Max(invariant first, int sec, int variableID);
    //    Max(IntegerVariable* var, int integer);
    //    Max(invariant invar, int integer);
    //    Max(const Max& orig);

    virtual ~Max();
    void addChange(int id, int change);
    int calculateDeltaValue();
    bool test();
    void updateValue();



private:
    std::vector<int> VariableChange;
    int second;
    int variableValue;
    int firstChange;
    std::shared_ptr<DependencyDigraph> DDG;




    //    int firstID;
    //    int secondID;
    //    int firstValue;
    //    int secondValue;
};

/// Alternativt skal både cpp og hpp inKluderes der hvor de skal bruges.
//#include "Max.cpp"
#endif	/* MAX_HPP */

