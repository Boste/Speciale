/* 
 * File:   DependencyDigraph.cpp
 * Author: bo
 * 
 * Created on October 8, 2015, 1:47 PM
 */

#include "DependencyDigraph.hpp"

DependencyDigraph::DependencyDigraph() {
}

DependencyDigraph::DependencyDigraph(const DependencyDigraph& orig) {
}

void DependencyDigraph::addVariables(variableContainer& vars) {
    for (IntegerVariable* iv : vars) {
        assert(!iv->isIntegerVariable());
        //        variableNode var(iv->getID(), iv);
        std::shared_ptr<variableNode> var = std::make_shared<variableNode>();
        ;
        var->id = iv->getID();
        var->iv = iv;

        std::pair<unsigned, std::shared_ptr < variableNode >> mapping(var->id, var);
        variable_nodes.insert(mapping);
        if (var->id == 42342) {
            std::cout << &var << " " << &mapping.second << " " << &variable_nodes.at(var->id) << std::endl;
        }
        //        std::pair<unsigned, variableNode&> mapping(var.getID(), var);
        //        variable_nodes.insert(mapping);
        //        if (var.getID() == 42342) {
        //            std::cout << &var << " " << &mapping.second << " " << &variable_nodes.at(var.getID()) << std::endl;
        //        }
    }

}

void DependencyDigraph::addInvariant(invariant newInvariant, InvariantContainer& invars) {

    std::shared_ptr<invariantNode> invar = std::make_shared<invariantNode>();
    invar->invar = newInvariant;
    invar->id = newInvariant->getID();
    std::pair<unsigned, std::shared_ptr < invariantNode >> mapping(invar->id, invar);
    invariant_nodes.insert(mapping);

    for (invariant inv : invars) {
        //        invariant_nodes.at(inv->getID()).update.insert(newInvariant);
        //        std::cout << inv->getID() << " inv" << std::endl;
        invariant_nodes.at(inv->getID())->update.push_back(newInvariant);
    }
    //    std::cout << "-1" << std::endl;
}

void DependencyDigraph::addInvariant(invariant newInvariant, variableContainer& vars) {

    std::shared_ptr<invariantNode> invar = std::make_shared<invariantNode>();
    invar->invar = newInvariant;
    invar->id = newInvariant->getID();
    std::pair<unsigned, std::shared_ptr < invariantNode >> mapping(invar->id, invar);
    invariant_nodes.insert(mapping);


    for (IntegerVariable* iv : vars) {
        if (iv->isFixed()) {
            continue;
        }
        //        variable_nodes.at(iv->getID()).update.insert(newInvariant);
        //        std::cout << iv->getID() << std::endl;
        //        if(iv->getID() == 27600){
        //            std::cout <<iv->isIntegerVariable() << " " << iv->usedInConstraints().size() << std::endl;
        //        }
        assert(!iv->isIntegerVariable());
        variable_nodes.at(iv->getID())->update.push_back(newInvariant);
    }
    //    debug;
    //    std::cout << "size " << invars.size() << std::endl;

    //    for (invariant inv : invars) {
    //        //        invariant_nodes.at(inv->getID()).update.insert(newInvariant);
    //        std::cout << inv->getID() << " inv" << std::endl;
    //        invariant_nodes.at(inv->getID()).update.push_back(newInvariant);
    //    }
    //    std::cout << "-1" << std::endl;
}

void DependencyDigraph::addInvariant(invariant newInvariant, variableContainer& vars, InvariantContainer& invars) {
    //    debug;
    std::shared_ptr<invariantNode> invar = std::make_shared<invariantNode>();
    invar->invar = newInvariant;
    invar->id = newInvariant->getID();
    std::pair<unsigned, std::shared_ptr < invariantNode >> mapping(invar->id, invar);
    invariant_nodes.insert(mapping);

    //    debug;
    for (IntegerVariable* iv : vars) {
        //        debug;
        if (iv->isFixed()) {
            //            debug;
            continue;
        }
        //        variable_nodes.at(iv->getID()).update.insert(newInvariant);
        //        std::cout << iv->getID() << std::endl;
        //        if(iv->getID() == 27600){
        //            std::cout <<iv->isIntegerVariable() << " " << iv->usedInConstraints().size() << std::endl;
        //        }
        assert(!iv->isIntegerVariable());
        //        std::cout << iv->getID() << std::endl;
        //        std::cout << variable_nodes.size() << std::endl;
        std::shared_ptr<variableNode> vn = variable_nodes.at(iv->getID());
        //        debug;
        vn->update.push_back(newInvariant);
        //        debug;
    }
    //    debug;
    //    std::cout << "size " << invars.size() << std::endl;
    //    debug;
    for (invariant inv : invars) {
        //        invariant_nodes.at(inv->getID()).update.insert(newInvariant);
        //        std::cout << inv->getID() << " inv" << std::endl;
        invariant_nodes.at(inv->getID())->update.push_back(newInvariant);
    }
    //    std::cout << "-1" << std::endl;
}
/// Return the invariants that this invar defines

updateVector& DependencyDigraph::getUpdate(invariant invar) {
    assert(invariant_nodes.at(invar->getID())->update.size() != 0);
    return invariant_nodes.at(invar->getID())->update;
}

updateVector& DependencyDigraph::getUpdate(IntegerVariable* var) {
    assert(invariant_nodes.at(var->getID())->update.size() != 0);

    return variable_nodes.at(var->getID())->update;
}

propagation_queue& DependencyDigraph::getPropagationQueue(IntegerVariable* iv) {
    //    std::cout << variable_nodes.at(iv->getID())->propagationQueue.size() << std::endl;
    return variable_nodes.at(iv->getID())->propagationQueue;
}

bool DependencyDigraph::propagationQueueHasBeenMade() {
    return madePropagationQueues;
}

void DependencyDigraph::createPropagationQueue() {
    //    std::cout << "not made yet" << std::endl;
    madePropagationQueues = true;
    //    int counter = 0;
    //    int totalQueueSize = 0;
    //    int totalElements = 0;
    //    propagation_queue global;
    //    std::cout << variable_nodes.size() << std::endl;
    //    std::cout << "fisk" << std::endl;
    for (std::unordered_map<unsigned, std::shared_ptr < variableNode>>::iterator it = variable_nodes.begin(); it != variable_nodes.end(); ++it) {
        //        variableContainer queue;

        std::shared_ptr<variableNode> vn = it->second;
        //        std::cout << vn.id << std::endl;
        //        addToQueue(global,vn.update);
        addToQueue(vn->propagationQueue, vn->update);
        //        addToQueue(vn.getPropagationQueue(), vn.getUpdate());
        //        if (vn->id == 42342) {
        //            std::cout << &vn << " vs " << &it->second << std::endl;
        //            std::cout << vn->propagationQueue.size() << " " << &vn->propagationQueue << std::endl;
        //            std::cout << &vn << std::endl;
        //            std::cout << &variable_nodes.at(vn->id) << sCtd::endl;
        //        }
    }
    //    variableNode vn = variable_nodes.at(42342);
    //    std::cout << vn.propagationQueue.size() << std::endl;
    //    sleep(4000);
    //    for (invariant invar : global) {
    //        std::cout << invar->getID() << std::endl;
    //    }
    //    std::cout << "total number of elements " << totalElements << std::endl;
    //    std::cout << "total queue size " << totalQueueSize << std::endl;
}

void DependencyDigraph::addToQueue(propagation_queue& orgQueue, updateVector& queue) {
    for (updateType invar : queue) {
        //        orgQueue.insert(orgQueue.end(),invar);
        orgQueue.insert(invar);
        //        orgQueue.push(invar);
        //        counter++;

        //                iv->addToPropagationQueue(invar);
        //        unsigned id = invar->getID();
        addToQueue(orgQueue, invariant_nodes.at(invar->getID())->update);
    }


}

DependencyDigraph::~DependencyDigraph() {
}

