#ifndef DEPENDENCYDIGRAPH_HPP
#define	DEPENDENCYDIGRAPH_HPP
#include "Constants.hpp"
#include "Invariant.hpp"
#include "IntegerVariable.hpp"
#include <list>
//#include <boost/heap/binomial_heap.hpp>

struct compare_invariant : public std::binary_function<invariant, invariant, bool> {

    bool operator()(const invariant invar1, const invariant invar2) const {
        //                std::cout << "is used"  << invar1 << " " << invar2 << " id1 "<< invar1->getID() <<" id2 "<< invar2->getID() << " compare " << (invar1 < invar2) << std::endl;
        //                sleep(1);
        return (invar1->getID() < invar2->getID());
    }

    bool operator()(invariant invar1, invariant invar2) {

        //                std::cout << "is used123 " <<" id1 "<< invar1->getID() <<" id2 "<< invar2->getID() << " compare " << (invar1 < invar2) << std::endl;
        //                sleep(1);
        return (invar1->getID() < invar2->getID());
    }
    //
    //    bool operator<(const invariant invar) const {
    //                std::cout << "used to sort1 <" << std::endl;
    //        return (this->operator<(invar));
    //    }
    //
    //    bool operator>(const invariant invar) const {
    //                std::cout << "used to sort2 >" << std::endl;
    //        return (this->operator>(invar));
    //    }
    //    bool operator>=(const invariant invar) const {
    //                std::cout << "used to sort3 >" << std::endl;
    //        return (this->operator>=(invar));
    //    }
    //    bool operator<=(const invariant invar) const {
    //                std::cout << "used to sort4 >" << std::endl;
    //        return (this->operator<=(invar));
    //    }
    //    bool operator==(const invariant invar) const {
    //                std::cout << "used to sort5 >" << std::endl;
    //        return (this->operator==(invar));
    //    }
    //    bool operator<( invariant invar)  {
    //                std::cout << "used to sort11 <" << std::endl;
    //        return (this->operator<(invar));
    //    }
    //
    //    bool operator>( invariant invar)  {
    //                std::cout << "used to sort12 >" << std::endl;
    //        return (this->operator>(invar));
    //    }
    //    bool operator>=( invariant invar)  {
    //                std::cout << "used to sort13 >" << std::endl;
    //        return (this->operator>=(invar));
    //    }
    //    bool operator<=( invariant invar)  {
    //                std::cout << "used to sort14 >" << std::endl;
    //        return (this->operator<=(invar));
    //    }
    //    bool operator==( invariant invar)  {
    //                std::cout << "used to sort15 >" << std::endl;
    //        return (this->operator==(invar));
    //    }
};
typedef std::set<invariant, compare_invariant> propagation_queue;
//typedef boost::heap::binomial_heap<invariant,boost::heap::compare<compare_invariant>> propagation_queue;
//typedef std::set<invariant, compare_invariant> updateVector;
typedef std::list<invariant> updateVector;
//typedef std::vector<invariant> updateVector;

struct variableNode {

    updateVector update;
    propagation_queue propagationQueue;
    unsigned id;
    //    invariant invar;
    IntegerVariable* iv;
};

struct invariantNode {

    updateVector update;
    unsigned id;
    //    invariant invar;
    invariant invar;
};

class DependencyDigraph {
public:

    DependencyDigraph();
    DependencyDigraph(const DependencyDigraph& orig);
    ~DependencyDigraph();
    void addVariables(variableContainer& vars);
    void addInvariant(invariant invar, variableContainer& vars, InvariantContainer& invars);
    void addInvariant(invariant invar, InvariantContainer& invars);
    void addInvariant(invariant invar, variableContainer& vars);
    updateVector& getUpdate(invariant invar);
    updateVector& getUpdate(IntegerVariable* var);
    propagation_queue& getPropagationQueue(IntegerVariable* iv);
    bool propagationQueueHasBeenMade();
    void createPropagationQueue();
    void addToQueue(propagation_queue& orgQueue, updateVector& queue);

private:
    std::unordered_map<unsigned, std::shared_ptr<variableNode>> variable_nodes;
    std::unordered_map<unsigned, std::shared_ptr<invariantNode>> invariant_nodes;
    bool madePropagationQueues = false;
    //    int counter = 0;
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


