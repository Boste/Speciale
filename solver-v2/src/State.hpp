#ifndef STATE_HPP
#define	STATE_HPP
#include "Storage.hpp"
#include "Clock.hpp"

class State {
private:


    std::shared_ptr<Storage> model;
    std::vector<int> solution;
    std::vector<int> evaluation;
    bool feasible;
    unsigned violation;



public:

    State(std::shared_ptr<Storage> model);
    State(const State& orig);
    bool compare(std::shared_ptr<State>& st);
    virtual ~State();
    void copy(std::shared_ptr<State> st); 
    void setViolation();
    unsigned getViolations();
    std::vector<int>& getEvaluation();
    std::vector<int>& getSolution();
    int getSolutionValue();
    bool recalculateAll();
    bool isFeasible();
private:

};

#endif	/* STATE_HPP */

