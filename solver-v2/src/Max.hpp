
#ifndef MAX_HPP
#define	MAX_HPP
#include "Constants.hpp"
#include "Invariant.hpp"
#include "DependencyDigraph.hpp" 

class Max : public Invariant {
public:

    Max(Variable* first, int sec, unsigned variableID);
    Max(invariant first, int sec, unsigned variableID);

    virtual ~Max();
    void proposeChange(int id, int change);
    bool calculateDelta();
    bool test();
    void updateValue();



private:
    std::vector<int> VariableChange;
    int second;
    int variableValue;
    invariant invar;
    int firstChange;
};
#endif	/* MAX_HPP */

