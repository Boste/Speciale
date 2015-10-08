#ifndef STATE_HPP
#define	STATE_HPP
#include "Model.hpp"

class State {
private:


    int solutionValue;
//    std::vector<int>* mask;
    std::shared_ptr<Model> model;
    std::vector<int> solution;
    propagation_queue deltaQueue;

public:
    int numberOfViolations;

    State(std::shared_ptr<Model> model);
    State(const State& orig);
    virtual ~State();

    
    /// Maybe all the initialize should be moved to model (again).
    void initializeInvariants();

    void initializeConstraints();

    void initializeObjective();


    int getObjectiveValue();
    void saveSolution();
    std::vector<int>& getSolution();
    int getSolutionValue();
    void setSolution();
    bool recalculateAll();
//    int maskAt(int i);
//    void shuffleMask();
private:

};

#endif	/* STATE_HPP */

