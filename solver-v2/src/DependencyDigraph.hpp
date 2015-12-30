#ifndef DEPENDENCYDIGRAPH_HPP
#define	DEPENDENCYDIGRAPH_HPP
#include "Constants.hpp"
#include "Invariant.hpp"
#include "Variable.hpp"
#include <list>
#include <memory>
#include "Random.hpp"
//#include <boost/heap/binomial_heap.hpp>


//typedef std::list<invariant> propagation_queue;
//typedef boost::heap::binomial_heap<invariant,boost::heap::compare<compare_invariant>> propagation_queue;
//typedef std::set<invariant, compare_invariant> updateVector;
//typedef std::list<invariant> updateVector;
typedef std::vector<invariant> updateVector;
//typedef std::vector<std::shared_ptr<invariantNode>> updateVector;




struct variableNode;
struct invariantNode;
struct compare_invariant;
typedef std::set<invariant, compare_invariant> propagation_queue;

typedef std::vector<std::shared_ptr<variableNode>> variableNodeContainer;
typedef std::vector<std::shared_ptr<invariantNode>> invariantNodeContainer;

class DependencyDigraph {
public:

    DependencyDigraph();
    DependencyDigraph(const DependencyDigraph& orig);
    ~DependencyDigraph();
    void addVariables(variableContainer& vars);
    void addInvariant(invariant invar, variableContainer& vars, InvariantContainer& invars);
    void addInvariant(invariant invar, InvariantContainer& invars);
    void addInvariant(invariant invar, variableContainer& vars);
    void addInvariant(invariant invar);
    updateVector& getInvariantUpdate(unsigned invarID);
    updateVector& getVariableUpdate(unsigned varID);
    propagation_queue& getPropagationQueue(Variable* iv);
    bool propagationQueueHasBeenMade();
    /// Create propagation queue for each variable used in local search
    void createPropagationQueue(variableContainer& vars, InvariantContainer& invars);
    /// Merges variable nodes with the oneway constraints defining them
    void cleanUpGraph(std::vector<Variable*>& vars);
    /// merges variable node and invariant node into the invariant node. 
    void mergeNodes(std::shared_ptr<variableNode>& vn, std::shared_ptr<invariantNode>& in);
    /// Used to find cycles and give timestamps 
    void DFS(std::shared_ptr<invariantNode>& in);

    /// Uses two DFS to check for cycles and report them. 
    void checkForCycles(InvariantContainer& invars);
//    void breakCycles( std::vector<std::shared_ptr<invariantNode>>& cycle);
    void breakCycles();
    std::shared_ptr<invariantNode> breakTie(unsigned cycleNumber);
    void undefineVariable(std::shared_ptr<invariantNode> invar);
    void addToQueue(propagation_queue& orgQueue, updateVector& queue);
    void printSizes();
    std::vector<bool>& getBrokenInvariants();

private:
    //    std::unordered_map<unsigned, std::shared_ptr<variableNode>> variable_nodes;
    //    std::unordered_map<unsigned, std::shared_ptr<invariantNode>> invariant_nodes;
    /// Used for constant lookup based on id. (segfaults if one attempt to look up an ID not existing)
    variableNodeContainer variable_nodes;
//    variableNodeContainer variable_nodes  =variableNodeContainer();
    /// Used for constant lookup based on id. (segfaults if one attempt to look up an ID not existing)
    invariantNodeContainer invariant_nodes;
//    invariantNodeContainer invariant_nodes = invariantNodeContainer();
    /// Vector of all invariants (probably sorted by id but maybe id != index)
    //    invariantNodeContainer invariants;
    std::vector<bool> brokenInvariants;
    std::vector<bool> brokenCycles;
    bool madePropagationQueues = false;
    unsigned timestampCounter = 0;
//    unsigned timeCounter;
    std::vector<std::shared_ptr<invariantNode>> SCC;
//    std::vector<std::shared_ptr<invariantNode>> SCC = invariantNodeContainer();
    int cycleCounter = 0;
    std::vector<std::vector<unsigned >> cycles;
    std::vector<std::vector<unsigned >> invariantsCycles;

};
struct invariantNode {
    //    std::vector<std::shared_ptr<invariantNode>> update;
    updateVector update;
    unsigned id;
//    invariant SumUsedInMax;
    //    invariant invar;
    invariant invar;
    
    unsigned timestamp = 0;
    unsigned lowestLink;
    bool inCurrentSSC = false;
    Variable* var; 
//    std::vector<std::shared_ptr<invariantNode>> myInvariants;
    //    std::vector<std::shared_ptr<invariantNode>> myInvariants;
    //    std::vector<std::shared_ptr<variableNode>> myVariables;
};
struct compare_invariant : public std::binary_function<invariant, invariant, bool> {

    bool operator()(const invariant invar1, const invariant invar2) const {
        //                std::cout << "is used"  << invar1 << " " << invar2 << " id1 "<< invar1->getID() <<" id2 "<< invar2->getID() << " compare " << (invar1 < invar2) << std::endl;
        //                sleep(1);
        return (invar1->getTimestamp() > invar2->getTimestamp());
    }

    bool operator()(invariant invar1, invariant invar2) {

        //                std::cout << "is used123 " <<" id1 "<< invar1->getID() <<" id2 "<< invar2->getID() << " compare " << (invar1 < invar2) << std::endl;
        //                sleep(1);
//        unsigned t1 = invariant_nodes.at(invar1->getID())->timestamp; 
        return (invar1->getTimestamp() >     invar2->getTimestamp());
    }
   
};


struct variableNode {
    updateVector update;
    propagation_queue propagationQueue;
    unsigned id;
    //    invariant invar;
    Variable* iv;
};

#endif	/* DEPENDENCYDIGRAPH_HPP */

//class variableNode {
//public:
//
//    variableNode(unsigned id, IntegerVariable* iv) {
//        this->id = id;
//        this->iv = iv;
//    }
//
//    void addToUpdate(invariant inv) {
//        debug;
//        update.push_back(inv);
//    }
//
//    updateVector& getUpdate() {
//        return update;
//    }
//
//    unsigned getID() {
//        return id;
//    }
//
//    void addToPropagationQueue(updateType invar) {
//        propagationQueue.insert(invar);
//    }
//
//    propagation_queue& getPropagationQueue() {
//        return propagationQueue;
//    }
//
//private:
//    updateVector update;
//    propagation_queue propagationQueue;
//    unsigned id;
//    //    invariant invar;
//    IntegerVariable* iv;
//};
//
//class invariantNode {
//public:
//
//    invariantNode(unsigned id, invariant invar) {
//        this->id = id;
//        this->invar = invar;
//    }
//
//    void addToUpdate(invariant inv) {
//        update.push_back(inv);
//    }
//
//    updateVector& getUpdate() {
//        return update;
//    }
//
//    unsigned getID() {
//        return id;
//    }
//private:
//    updateVector update;
//    unsigned id;
//    invariant invar;
//};
//class invariantNode {
//public:
//
//    invariantNode(unsigned id, invariant invar) {
//        this->id = id;
//        this->invar = invar;
//    }
//
//    void addToUpdate(invariant inv) {
//        update.push_back(inv);
//    }
//
//    updateVector& getUpdate() {
//        return update;
//    }
//
//    unsigned getID() {
//        return id;
//    }
//private:
//    updateVector update;
//    unsigned id;
//    invariant invar;
//};


