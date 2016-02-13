#ifndef DEPENDENCYDIGRAPH_HPP
#define	DEPENDENCYDIGRAPH_HPP
#include "Constants.hpp"
#include "Invariant.hpp"
#include "Variable.hpp"
#include <list>
#include <memory>
#include "Random.hpp"
typedef std::vector<invariant> updateVector;

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
    void createPropagationQueue(variableContainer& vars);
    /// Merges variable nodes with the oneway constraints defining them
    void cleanUpGraph(std::vector<Variable*>& vars);
    /// merges variable node and invariant node into the invariant node. 
    void mergeNodes(std::shared_ptr<variableNode>& vn, std::shared_ptr<invariantNode>& in);
    /// Used to find cycles and give timestamps 
    void DFS(std::shared_ptr<invariantNode>& in);

    /// Uses two DFS to check for cycles and report them. 
    void checkForCycles(InvariantContainer& invars);
    void breakCycles();
    std::shared_ptr<invariantNode> breakTie(unsigned cycleNumber);
    void undefineVariable(std::shared_ptr<invariantNode> invar);
    void addToQueue(propagation_queue& orgQueue, updateVector& queue, int depth);
    void printSizes();

private:
    variableNodeContainer variable_nodes;
    /// Used for constant lookup based on id. (segfaults if one attempt to look up an ID not existing)
    invariantNodeContainer invariant_nodes;
    /// Vector of all invariants (probably sorted by id but maybe id != index)
    /// Vector of invariants that should be deleted if true. 
    /// If a cycle id has been broken 
    std::vector<bool> brokenCycles;
    bool madePropagationQueues = false;
    unsigned timestampCounter = 0;
    std::vector<std::shared_ptr<invariantNode>> SCC;
    int cycleCounter = 0;
    /// A vector of cycles. Each cycle has the id of the invariants
    std::vector<std::vector<unsigned >> cycles;
    /// The cycle ids an invariant is in
    std::vector<std::vector<unsigned >> invariantsCycles;

};
struct invariantNode {
    updateVector update;
    unsigned id;
    invariant invar;
    
    unsigned timestamp = 0;
    unsigned lowestLink;
    bool inCurrentSSC = false;
    Variable* var; 
};
struct compare_invariant : public std::binary_function<invariant, invariant, bool> {

    bool operator()(const invariant invar1, const invariant invar2) const {
        return (invar1->getTimestamp() > invar2->getTimestamp());
    }

    bool operator()(invariant invar1, invariant invar2) {

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