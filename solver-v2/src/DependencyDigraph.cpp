

#include "DependencyDigraph.hpp"

DependencyDigraph::DependencyDigraph() {
    //    timeCounter = 0;
    timestampCounter = 0;
    cycleCounter = 0;
}

DependencyDigraph::DependencyDigraph(const DependencyDigraph& orig) {

    std::cout << "should not be copying DDG " << &orig << std::endl;
}

void DependencyDigraph::addVariables(variableContainer& vars) {
    variable_nodes.resize(vars.size());
    for (Variable* iv : vars) {
        if (iv->isFixed()) {
            continue;
        }
        //        assert(!iv->isIntegerVariable());
        //        variableNode var(iv->getID(), iv);
        std::shared_ptr<variableNode> var = std::make_shared<variableNode>();

        var->id = iv->getID();
        var->iv = iv;

        //        std::pair<unsigned, std::shared_ptr < variableNode >> mapping(var->id, var);
        variable_nodes.at(var->id) = var;
        //        if (var->id == 42342) {
        //            std::cout << var.get() << " " << mapping.second.get() << " " << variable_nodes.at(var->id).get() << std::endl;
        //        }
        //        std::pair<unsigned, variableNode&> mapping(var.getID(), var);
        //        variable_nodes.insert(mapping);
        //        if (var.getID() == 42342) {
        //            std::cout << &var << " " << &mapping.second << " " << &variable_nodes.at(var.getID()) << std::endl;
        //        }
    }

}
/// Atm only used to create max invariants, these should never be directly dependent on variables

void DependencyDigraph::addInvariant(invariant newInvariant, InvariantContainer& invars) {

    std::shared_ptr<invariantNode> invar = std::make_shared<invariantNode>();
    //    invariants.push_back(invar);

    invar->invar = newInvariant;
    invar->id = newInvariant->getID();

    //    std::pair<unsigned, std::shared_ptr < invariantNode >> mapping(invar->id, invar);
    if (invar->id != invariant_nodes.size()) {

        std::cout << "id " << invar->id << " size " << invariant_nodes.size() << std::endl;
    }

    assert(invar->id == invariant_nodes.size());
    invariant_nodes.push_back(invar);

    //    int varID = newInvariant->getVariableID();
    //    if (varID != -1) {
    //        auto it = variable_nodes.find(varID);
    //        for (invariant inv : it->second->update) {
    //            invar->update.push_back(inv);
    //        }
    //        variable_nodes.erase(it);
    //    }

    for (invariant inv : invars) {
        //        invariant_nodes.at(inv->getID()).update.insert(newInvariant);
        //        std::cout << inv->getID() << " inv" << std::endl;
        std::shared_ptr<invariantNode> invNode = invariant_nodes.at(inv->getID());
        invNode->update.push_back(newInvariant);
        //        invNode->update.push_back(invar);
        //        invar->myInvariants.push_back(invNode);

    }
    assert(newInvariant->getType() == MAX);
    std::shared_ptr<variableNode> vn = variable_nodes.at(newInvariant->getVariableID());
    for(invariant inv : vn->update){
        invar->update.push_back(inv);
    }
    
    //    std::cout << "-1" << std::endl;
}
/// Used even though some of the vars are defined by invariants. Clean up made afterwards Dont think it is used anymore

void DependencyDigraph::addInvariant(invariant newInvariant, variableContainer& vars) {

    std::shared_ptr<invariantNode> invar = std::make_shared<invariantNode>();
    //    invariants.push_back(invar);

    invar->invar = newInvariant;
    invar->id = newInvariant->getID();
    if (invar->id != invariant_nodes.size()) {

        std::cout << "id " << invar->id << " size " << invariant_nodes.size() << std::endl;
    }
    assert(invar->id == invariant_nodes.size());
    invariant_nodes.push_back(invar);
    int varID = newInvariant->getVariableID();
    if (varID != -1) {
        auto var = variable_nodes.at(varID);
        for (invariant inv : var->update) {
            invar->update.push_back(inv);
        }
        //        variable_nodes.erase(it);
    }

    for (Variable* iv : vars) {
        if (iv->isFixed()) {
            continue;
        }
        variable_nodes.at(iv->getID())->update.push_back(newInvariant);
    }

}

/// Should not be used for changing constraints to be one-way constraints, hence only invariants that are defining auxiliary variables
/// Now it should, skipping cleanUp

void DependencyDigraph::addInvariant(invariant newInvariant, variableContainer& vars, InvariantContainer& invars) {
    //    
    std::shared_ptr<invariantNode> invar = std::make_shared<invariantNode>();
    invar->invar = newInvariant;
    //    newInvariant->setTimestamp(timestampCounter);
    //    timestampCounter++;
    invar->id = newInvariant->getID();
    if (invar->id != invariant_nodes.size()) {

        std::cout << "id " << invar->id << " size " << invariant_nodes.size() << std::endl;
    }
    assert(invar->id == invariant_nodes.size());
    invariant_nodes.push_back(invar);
    int varID = newInvariant->getVariableID();
    if (varID != -1) {

        auto var = variable_nodes.at(varID);
        for (invariant inv : var->update) {
            invar->update.push_back(inv);
        }
        //        variable_nodes.erase(it);
    }
    //    
    for (Variable* iv : vars) {
        //        
        if (iv->isFixed()) {
            //            
            continue;
        }
        //        assert(variable_nodes.find(iv->getID()) != variable_nodes.end());

        std::shared_ptr<variableNode> vn = variable_nodes.at(iv->getID());
        //        
        vn->update.push_back(newInvariant);
        //        
    }
    //    
    //    std::cout << "size " << invars.size() << std::endl;
    //    
    for (invariant inv : invars) {
        //        invariant_nodes.at(inv->getID()).update.insert(newInvariant);
        //        std::cout << inv->getID() << " inv" << std::endl;
        //        assert(invariant_nodes.find(inv->getID()) != invariant_nodes.end());

        invariant_nodes.at(inv->getID())->update.push_back(newInvariant);
        //        invariant_nodes.at(inv->getID())->update.push_back(invar);
    }
    //    std::cout << "-1" << std::endl;
}
/// Return the invariants that this invar defines

updateVector& DependencyDigraph::getInvariantUpdate(unsigned invarID) {
    //    assert(invariant_nodes.find(invarID) != invariant_nodes.end());

    //    assert(invariant_nodes.at(invarID)->update.size() != 0);
    return invariant_nodes.at(invarID)->update;
}

updateVector& DependencyDigraph::getVariableUpdate(unsigned varID) {
    //    assert(variable_nodes.find(varID) != variable_nodes.end());

    assert(variable_nodes.at(varID)->update.size() != 0);

    return variable_nodes.at(varID)->update;
}

propagation_queue& DependencyDigraph::getPropagationQueue(Variable* iv) {
    //    std::cout << variable_nodes.at(iv->getID())->propagationQueue.size() << std::endl;
    assert(!iv->isDef());
    assert(!iv->isFixed());
    assert(variable_nodes.at(iv->getID())->propagationQueue.size() != 0);
    return variable_nodes.at(iv->getID())->propagationQueue;
}

bool DependencyDigraph::propagationQueueHasBeenMade() {
    return madePropagationQueues;
}

void DependencyDigraph::cleanUpGraph(std::vector<Variable*>& vars) {

    //    for (std::shared_ptr<variableNode> vn : variable_nodes) {
    for (Variable* iv : vars) {


        //        
        if (iv->isDef()) {
            std::shared_ptr<variableNode> vn = variable_nodes.at(iv->getID());
            if (iv)
                //            vn->update;

                mergeNodes(vn, invariant_nodes.at(iv->getOneway()->getID()));

            //            vn.~__shared_ptr();

        }
    }
    //    
    //    unsigned timestampCounter = 0;
    //    for (IntegerVariable* iv : vars) {
    //        if (!iv->isFixed() && !iv->isDef()) {
    //            variableNode vn = variable_nodes.at(iv->getID());
    //        }
    //    }

}

void DependencyDigraph::mergeNodes(std::shared_ptr<variableNode>& vn, std::shared_ptr<invariantNode>& in) {

    for (invariant invar : vn->update) {
        in->update.push_back(invar);
        std::shared_ptr<invariantNode> inv = invariant_nodes.at(invar->getID());
        //        inv->myInvariants.push_back(in);

    }
    in->iv = vn->iv;
    //    
    //    vn->update.clear();

}

void DependencyDigraph::checkForCycles(InvariantContainer& invars) {
    //    std::cout << "number of invariants " << invars.size() << std::endl;
    //    unsigned id = 5565;
    //    invariant test = invars.at(id);
    //    assert(test->getID() == id);
    //    std::cout << "id " << test->getID() << " " << std::endl;
    //    for (invariant inv : invariant_nodes.at(id)->update) {
    //        std::cout << "\t invariant ids " << inv->getID() << " timestamps " << invariant_nodes.at(inv->getID())->timestamp << std::endl;
    //    }
    ////    5565->5811->5812->5964->5810->5564
    //    id = 5811;
    //    test = invars.at(id);
    //    assert(test->getID() == id);
    //    std::cout << "id " << test->getID() << " " << std::endl;
    //    for (invariant inv : invariant_nodes.at(id)->update) {
    //        std::cout << "\t invariant ids " << inv->getID() << " timestamps " << invariant_nodes.at(inv->getID())->timestamp << std::endl;
    //    }
    ////    exit(1);
    //    id = 5812;
    //    invariant test2 = invars.at(id);
    //    assert(test2->getID() == id);
    //    std::cout << "id " << test2->getID() << " " << std::endl;
    //    for (invariant inv : invariant_nodes.at(id)->update) {
    //        std::cout << "\t invariant ids " << inv->getID() << " timestamps " << invariant_nodes.at(inv->getID())->timestamp << std::endl;
    //    }
    //    //    exit(1);
    //    id = 5964;
    //    test2 = invars.at(id);
    //    assert(test2->getID() == id);
    //    std::cout << "id " << test2->getID() << " " << std::endl;
    //    for (invariant inv : invariant_nodes.at(id)->update) {
    //        std::cout << "\t invariant ids " << inv->getID() << " timestamps " << invariant_nodes.at(inv->getID())->timestamp << std::endl;
    //    }
    //    id = 5810;
    //    test2 = invars.at(id);
    //    assert(test2->getID() == id);
    //    std::cout << "id " << test2->getID() << " " << std::endl;
    //    for (invariant inv : invariant_nodes.at(id)->update) {
    //        std::cout << "\t invariant ids " << inv->getID() << " timestamps " << invariant_nodes.at(inv->getID())->timestamp << std::endl;
    //    }
    //    id = 5564;
    //    test2 = invars.at(id);
    //    assert(test2->getID() == id);
    //    std::cout << "id " << test2->getID() << " " << std::endl;
    //    for (invariant inv : invariant_nodes.at(id)->update) {
    //        std::cout << "\t invariant ids " << inv->getID() << " timestamps " << invariant_nodes.at(inv->getID())->timestamp << std::endl;
    //    }
    //    exit(1);



    brokenInvariants.resize(invars.size(), false);
    for (unsigned i = 0; i < invars.size(); i++) {
        std::vector<unsigned > cycles; // = new std::vector<unsigned >();
        invariantsCycles.push_back(cycles);
    }
    //    assert(invariantsCycles.size() != 0);
    //    brokenCycles.resize(invars.size(), false);

    for (invariant inv : invars) {
        std::shared_ptr<invariantNode> in = invariant_nodes.at(inv->getID());
        if (in->timestamp == 0) {
            DFS(in);
        }
        assert(inv->getTimestamp() != 0);
    }

    std::cout << "number of cycles " << cycleCounter << std::endl;
    breakCycles();
    cycleCounter = 0;
    for (invariant inv : invars) {
        if (brokenInvariants.at(inv->getID())) {
            continue;
        }
        std::shared_ptr<invariantNode> in = invariant_nodes.at(inv->getID());
        in->timestamp = 0;
        in->lowestLink = 0;
    }
    //    for (invariant inv : invars) {
    //        if (brokenInvariants.at(inv->getID())) {
    //            continue;
    //        }
    //        std::shared_ptr<invariantNode> in = invariant_nodes.at(inv->getID());
    //        if (in->timestamp == 0) {
    //            DFS(in);
    //        }
    //        assert(inv->getTimestamp() != 0);
    //    }

    //    auto in = invariant_nodes.at(10150);
    //    std::cout << "10150" << std::endl;
    //    for (auto inv : in->update) {
    //        std::cout << "\t" << inv->getID() << std::endl;
    //        for (auto invar : invariant_nodes.at(inv->getID())->update) {
    //            std::cout << "\t \t" << invar->getID() << std::endl;
    //            for (auto invar2 : invariant_nodes.at(invar->getID())->update) {
    //                std::cout << "\t \t \t" << invar2->getID() << std::endl;
    //
    //            }
    //        }
    //    }

    //    exit(1);

    //    
    //    
    //    std::cout << "number of invariants " << invars.size() << std::endl;
    //    unsigned id = 5565;
    //    invariant test = invars.at(id);
    //    assert(test->getID() == id);
    //    std::cout << "id " << test->getID() << " lowestLink " << invariant_nodes.at(test->getID())->lowestLink<< std::endl;
    //    for (invariant inv : invariant_nodes.at(id)->update) {
    //        std::cout << "\t invariant ids " << inv->getID() << " lowestLink " << invariant_nodes.at(inv->getID())->lowestLink << std::endl;
    //    }
    ////    5565->5811->5812->5964->5810->5564
    //    id = 5811;
    //    test = invars.at(id);
    //    assert(test->getID() == id);
    //    std::cout << "id " << test->getID() << " lowestLink " << invariant_nodes.at(test->getID())->lowestLink<< std::endl;
    //    for (invariant inv : invariant_nodes.at(id)->update) {
    //        std::cout << "\t invariant ids " << inv->getID() << " lowestLink " << invariant_nodes.at(inv->getID())->lowestLink << std::endl;
    //    }
    ////    exit(1);
    //    id = 5812;
    //    test = invars.at(id);
    //    assert(test->getID() == id);
    //    std::cout << "id " << test->getID() << " lowestLink " << invariant_nodes.at(test->getID())->lowestLink<< std::endl;
    //    for (invariant inv : invariant_nodes.at(id)->update) {
    //        std::cout << "\t invariant ids " << inv->getID() << " lowestLink " << invariant_nodes.at(inv->getID())->lowestLink << std::endl;
    //    }
    //    //    exit(1);
    //    id = 5964;
    //    test = invars.at(id);
    //    assert(test->getID() == id);
    //    std::cout << "id " << test->getID() << " lowestLink " << invariant_nodes.at(test->getID())->lowestLink<< std::endl;
    //    for (invariant inv : invariant_nodes.at(id)->update) {
    //        std::cout << "\t invariant ids " << inv->getID() << " lowestLink " << invariant_nodes.at(inv->getID())->lowestLink << std::endl;
    //    }
    //    id = 5810;
    //    test = invars.at(id);
    //    assert(test->getID() == id);
    //    std::cout << "id " << test->getID() << " lowestLink " << invariant_nodes.at(test->getID())->lowestLink<< std::endl;
    //    for (invariant inv : invariant_nodes.at(id)->update) {
    //        std::cout << "\t invariant ids " << inv->getID() << " lowestLink " << invariant_nodes.at(inv->getID())->lowestLink << std::endl;
    //    }
    //    id = 5564;
    //    test = invars.at(id);
    //    assert(test->getID() == id);
    //    std::cout << "id " << test->getID() << " lowestLink " << invariant_nodes.at(test->getID())->lowestLink<< std::endl;
    //    for (invariant inv : invariant_nodes.at(id)->update) {
    //        std::cout << "\t invariant ids " << inv->getID() << " lowestLink " << invariant_nodes.at(inv->getID())->lowestLink << std::endl;
    //    }
    //    exit(1);

    //    for (invariant inv : invars) {
    //        std::shared_ptr<invariantNode> in = invariant_nodes.at(inv->getID());
    //        assert(in->timestamp > 0);
    //        unsigned myTime = in->lowestLink;
    //        for (invariant invar : in->update) {
    //            auto invarNode = invariant_nodes.at(invar->getID());
    //            if (myTime > invarNode->lowestLink) {
    //                //                std::cout << "think there is cycles" << std::endl;
    //                cycleCounter++;
    //            }
    //        }
    //    }
    //    std::cout << "expected number of cycles " << cycleCounter << std::endl;
}

// Tarjan's Algorithm

void DependencyDigraph::DFS(std::shared_ptr<invariantNode>& v) {
    timestampCounter++;
    v->timestamp = timestampCounter;
    v->lowestLink = timestampCounter;
    v->inCurrentSSC = true;
    SCC.push_back(v);
    //    for(std::shared_ptr<invariantNode> u : v->update ){
    for (invariant inv : v->update) {
        std::shared_ptr<invariantNode> u = invariant_nodes.at(inv->getID());
        if (u->timestamp == 0) {
            DFS(u);
            v->lowestLink = std::min(v->lowestLink, u->lowestLink);
        } else if (u->inCurrentSSC) {
            v->lowestLink = std::min(v->lowestLink, u->timestamp);
        }
    }
    if (v->lowestLink == v->timestamp) {
        //        if (SCC.size() > 1) {
        //        std::cout << "make this a pointer" << std::endl;
        std::vector<unsigned> component;
        std::shared_ptr<invariantNode> u = SCC.back();
        //        component.push_back(u);

        do {

            u = SCC.back();
            //            timeCounter++;
            timestampCounter++;
            u->invar->setTimestamp(timestampCounter);
            //            u->time = timeCounter;
            //            u->time = timeCounter;
            SCC.pop_back();
            component.push_back(u->id);

            u->inCurrentSSC = false;


        } while (u != v);
        if (component.size() > 1) {
            cycleCounter++;
            //            breakCycle(component);
            for (unsigned id : component) {

                invariantsCycles.at(id).push_back(cycles.size());
                //                std::cout << id << "<-";
            }
            //            std::cout << std::endl;
            cycles.push_back(component);
            brokenCycles.push_back(false);

            //            std::cout << component[0]->id;
            //            std::cout << std::endl;
        }

    }
}

void DependencyDigraph::breakCycles() {
    int broken = 0;
    //    std::cout << cycles.size() << std::endl;
    //    debug;
    for (unsigned cycle = 0; cycle < cycles.size(); cycle++) {

        if (brokenCycles[cycle]) {
            //            std::cout << "continue" << std::endl;
            continue;
        }
        broken++;
        std::shared_ptr<invariantNode> best = breakTie(cycle);
        brokenInvariants.at(best->id) = true;
        for (int cyc : invariantsCycles.at(best->id)) {
            brokenCycles.at(cyc) = true;
            //            std::cout << cyc << "  " << cycle << std::endl;
        }
        undefineVariable(best);
    }

    //    std::cout << broken << std::endl;



}

std::shared_ptr<invariantNode> DependencyDigraph::breakTie(unsigned cycleNumber) {
    std::vector<unsigned >& cycle = cycles.at(cycleNumber);
    assert(cycle.size() > 0);
    std::shared_ptr<invariantNode> bestInvar = invariant_nodes.at(cycle.at(0));
    unsigned bestPrio = bestInvar->iv->getSerachPriority();
    unsigned bestArity = bestInvar->invar->getVariablePointers().size();
    unsigned equalCounter = 1;
    for (unsigned i = 1; i < cycle.size(); i++) {
        std::shared_ptr<invariantNode> in = invariant_nodes.at(cycle.at(i));
        if (in->iv->isIntegerVariable()) {
            continue;
        }
        unsigned prio = in->iv->getSerachPriority();
        if (prio < bestPrio) {
            bestInvar = in;
            bestArity = in->invar->getVariablePointers().size();
            bestPrio = prio;
            equalCounter = 1;
            continue;
        } else if (prio > bestPrio) {
            continue;
        }
        unsigned arity = in->invar->getVariablePointers().size();
        if (arity > bestArity) {
            bestInvar = in;
            bestArity = in->invar->getVariablePointers().size();
            bestPrio = prio;
            equalCounter = 1;
            continue;
        } else if (arity < bestArity) {
            continue;
        }
        int choose = Random::Integer(equalCounter);
        equalCounter++;
        if (choose == 0) {
            bestInvar = in;
            bestArity = in->invar->getVariablePointers().size();
            bestPrio = prio;
        }
    }
    return bestInvar;
}

void DependencyDigraph::undefineVariable(std::shared_ptr<invariantNode> invar) {
    invar->iv->undefine();

    brokenInvariants.at(invar->id) = true;

    invariant& bestInvariant = invar->invar;
    if(bestInvariant->getType() == SUM){
        std::cout << "Should not be undefining integer variable that is defined by max invariant " << std::endl;
        debug;
        exit(1);
    }
    std::vector<Variable*>& vars = bestInvariant->getVariablePointers();
    for (Variable* iv : vars) {
        assert(iv->getID() != invar->id);
        if (iv->isFixed()) {
            continue;
        }
        if (iv->isDef()) {
            std::shared_ptr<invariantNode>& invarNode = invariant_nodes.at(iv->getOneway()->getID());
            updateVector& update = invarNode->update;
            //            std::iterator<invariant> it = update.begin();
            for (updateVector::iterator it = update.begin(); it != update.end(); ++it) {

//                if (it.base()->get() == bestInvariant.get()) {
                if ((*it) == bestInvariant) {
                    update.erase(it);
                    break;
                }
            }
        } else {
            //            std::cout << iv->getID() << std::endl;
            //            std::cout << "integer var, fixed, def " << iv->isIntegerVariable() << iv->isFixed()<<  iv->isDef() << std::endl;
            std::shared_ptr<variableNode>& varNode = variable_nodes.at(iv->getID());
            //            std::cout << &varNode << " ";
            updateVector& update = varNode->update;
            //            std::cout << varNode->update.size() << std::endl;
            //            std::iterator<invariant> it = update.begin();
            for (updateVector::iterator it = update.begin(); it != update.end(); it++) {
                //                std::cout << it.base()->get() << std::endl;
                //                it.base()->
//                if (it.base()->get() == bestInvariant.get()) {
                if (*it == bestInvariant) {
                    update.erase(it);
                    break;
                }
            }
        }
    }

}

std::vector<bool>& DependencyDigraph::getBrokenInvariants() {
    return brokenInvariants;
}

void DependencyDigraph::createPropagationQueue(variableContainer & vars, InvariantContainer& invars) {
    //    std::cout << "not made yet" << std::endl;
    madePropagationQueues = true;


    //    std::cout << "Do something here" << std::endl;
//    for (invariant inv : invars) {
//        //        if (brokenInvariants.at(inv->getID())) {
//        //            continue;
//        //        }
//        std::shared_ptr<invariantNode> in = invariant_nodes.at(inv->getID());
//        if (in->timestamp == 0) {
//            DFS(in);
//        }
//        assert(inv->getTimestamp() != 0);
//    }
    assert(cycleCounter == 0);
    //    int counter = 0;
    //    int totalQueueSize = 0;
    //    int totalElements = 0;
    //    propagation_queue global;
    //    std::cout << "fisk" << std::endl;

    //        for (std::unordered_map<unsigned, std::shared_ptr < variableNode>>::iterator it = variable_nodes.begin(); it != variable_nodes.end(); ++it) {
    //        variableContainer queue;
    //    for (std::shared_ptr<invariantNode> in : invariant_nodes) {
    //        in->invar->setTimestamp(in->time);
    //    }
    //            std::shared_ptr<variableNode> vn = it->second;
    for (Variable* iv : vars) {
        //    for (std::shared_ptr<variableNode> vn : variable_nodes) {
        if (!iv->isFixed() && !iv->isDef()) {


            //            std::cout << iv->getID() << std::endl;
            std::shared_ptr<variableNode> vn = variable_nodes.at(iv->getID());


            //            std::cout << vn->id << std::endl;
            //                
            //            //        std::cout << vn.id << std::endl;
            //            //        addToQueue(global,vn.update);
            //            std::cout << vn->update.size() << std::endl;
            //            for (invariant invar : vn->update) {
            //                std::cout << "invar id " << invar->getID() << " size of update ";
            //                std::cout << invariant_nodes.at(invar->getID())->update.size() << " defines " << invar->getVariableID() << std::endl;
            //                for (invariant inv : invariant_nodes.at(invar->getID())->update) {
            //                    std::cout << "\t invar id " << inv->getID() << " size of update ";
            //                    std::cout << invariant_nodes.at(inv->getID())->update.size() << " defines " << inv->getVariableID() << std::endl;
            //                    for (invariant inva : invariant_nodes.at(inv->getID())->update) {
            //                        std::cout << "\t \t invar id " << inva->getID() << " size of update ";
            //                        std::cout << invariant_nodes.at(inva->getID())->update.size() << " defines " << inva->getVariableID() << std::endl;
            //                        for (invariant inva2 : invariant_nodes.at(inva->getID())->update) {
            //                            std::cout << "\t \t \t invar id " << inva2->getID() << " size of update ";
            //                            std::cout << invariant_nodes.at(inva2->getID())->update.size() << " defines " << inva2->getVariableID() << std::endl;
            //                            for (invariant inva3 : invariant_nodes.at(inva2->getID())->update) {
            //                                std::cout << "\t \t \t \t invar id " << inva3->getID() << " size of update ";
            //                                std::cout << invariant_nodes.at(inva3->getID())->update.size() << " defines " << inva3->getVariableID() << std::endl;
            //                            }
            //                        }
            //                    }
            //                }
            //            }
            //            
            assert(vn->update.size() > 0);
//            for (invariant invar : vn->update) {
                //                std::cout << "id " << invar->getID() << " timestamp " << invar->getTimestamp() << std::endl;
//
//            }
            //            std::cout << std::endl;
            addToQueue(vn->propagationQueue, vn->update);

            for (invariant invar : vn->propagationQueue) {
                //                std::cout << "id " << invar->getID() << " timestamp " << invariant_nodes.at(invar->getID())->timestamp << std::endl;

                for (invariant inv : invariant_nodes.at(invar->getID())->update) {
                    if (inv->getTimestamp() > invar->getTimestamp()) {
                        //                        std::cout << "id " << inv->getID() << "  " << inv->getTimestamp() << " " << invar->getTimestamp() << " id " << invar->getID() << std::endl;
                    }
                }
            }

            //            std::cout << std::endl;

            //            std::cout << std::endl;

            //            sleep(5);
            //            if (vn->id == 244) {
            //                
            //            }
            //        addToQueue(vn.getPropagationQueue(), vn.getUpdate());
            //        if (vn->id == 42342) {
            //            std::cout << &vn << " vs " << &it->second << std::endl;
            //            std::cout << vn->propagationQueue.size() << " " << &vn->propagationQueue << std::endl;
            //            std::cout << &vn << std::endl;
            //            std::cout << &variable_nodes.at(vn->id) << sCtd::endl;
            //        } 
        }
    }
    //        for (invariant invar : global) {
    //            std::cout << invar->getID() << std::endl;
    //        }
    //    std::cout << "total number of elements " << totalElements << std::endl;
    //    std::cout << "total queue size " << totalQueueSize << std::endl;
}


/// Not optimal, it adds vertices from outgoing arcs for each ingoing arc. (If a vertex is visited twice it adds the vertices it points to twice) 
/// With a DFS you can set vertices to visited but you need to unvisit them again. 

void DependencyDigraph::addToQueue(propagation_queue& orgQueue, updateVector & queue) {
    for (updateType invar : queue) {
        //        orgQueue.insert(orgQueue.end(),invar);
        //        orgQueue.push(invar);
        //        counter++;

        //                iv->addToPropagationQueue(invar);
        //        unsigned id = invar->getID();

        addToQueue(orgQueue, invariant_nodes.at(invar->getID())->update);
        //        orgQueue.push_front(invar);
        orgQueue.insert(invar);


    }


}

void DependencyDigraph::printSizes() {
    std::cout << "#Variable nodes " << variable_nodes.size() << std::endl;
    std::cout << "#Invariant nodes " << invariant_nodes.size() << std::endl;
}

DependencyDigraph::~DependencyDigraph() {
}

