#ifndef STATE_HPP
#define	STATE_HPP
#include "Model.hpp"
#include "Clock.hpp"

class State {
private:


//    int solutionValue;
    //    std::vector<int>* mask;
    std::shared_ptr<Model> model;
    std::vector<int> solution;
//    unsigned numberOfViolations = 0;
    std::vector<int> evaluation;
    bool feasible;
    unsigned violation;

    //    propagation_queue deltaQueue;


public:

    State(std::shared_ptr<Model> model);
    State(const State& orig);
    bool compare(std::shared_ptr<State>& st);
    virtual ~State();
    void copy(std::shared_ptr<State> st); 
    void setViolation();
    unsigned getViolations();
//    void updateEvaluation(std::vector<int>& changes);

    /// Maybe all the initialize should be moved to model (again).
    //    void initializeInvariants();

    //    void initializeConstraints();

    //    void initializeObjective();

    std::vector<int>& getEvaluation();
//    int getObjectiveValue();
//    void saveSolution();
    std::vector<int>& getSolution();
    int getSolutionValue();
//    void setSolution();
    bool recalculateAll();
    bool isFeasible();
    //    int maskAt(int i);
    //    void shuffleMask();
private:

};

#endif	/* STATE_HPP */

