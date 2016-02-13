

#include "DependencyDigraph.hpp"

DependencyDigraph::DependencyDigraph() {
    timestampCounter = 0;
    cycleCounter = 0;
}

DependencyDigraph::DependencyDigraph(const DependencyDigraph& orig) {

    std::cout << "should not be copying DDG " << &orig << std::endl;
}

void DependencyDigraph::addVariables(variableContainer& vars) {
    variable_nodes.resize(vars.size(),NULL);
    for (Variable* iv : vars) {
        if (iv->isFixed()) {
            continue;
        }
        std::shared_ptr<variableNode> var = std::make_shared<variableNode>();

        var->id = iv->getID();
        var->iv = iv;

        variable_nodes.at(var->id) = var;
    }

}
///  Used to create invariant for collecting violation

void DependencyDigraph::addInvariant(invariant newInvariant, InvariantContainer& invars) {


    std::shared_ptr<invariantNode> invar = std::make_shared<invariantNode>();

    invar->invar = newInvariant;
    invar->var = invar->invar->getVariable();
    invar->id = newInvariant->getID();

    if (invar->id != invariant_nodes.size()) {

        std::cout << "id " << invar->id << " size " << invariant_nodes.size() << std::endl;
    }
    assert(invar->id == invariant_nodes.size());
    invariant_nodes.push_back(invar);
    for (invariant inv : invars) {
        std::shared_ptr<invariantNode> invNode = invariant_nodes.at(inv->getID());
        invNode->update.push_back(newInvariant);

    }
}
/// Used even though some of the vars are defined by invariants. Clean up made afterwards Dont think it is used anymore

void DependencyDigraph::addInvariant(invariant newInvariant, variableContainer& vars) {
    debug;
    std::shared_ptr<invariantNode> invar = std::make_shared<invariantNode>();
    //    invariants.push_back(invar);

    invar->invar = newInvariant;
    invar->var = invar->invar->getVariable();

    invar->id = newInvariant->getID();
    invariant_nodes.push_back(invar);
    //    brokenInvariants.push_back(false);
    int varID = newInvariant->getVariableID();
    if (varID != -1) {
        auto var = variable_nodes.at(varID);
        for (invariant inv : var->update) {
            invar->update.push_back(inv);
        }
    }

    for (auto it = vars.begin(); it != vars.end(); ++it) {
        //        
        if ((*it)->isFixed()) {
            continue;
        }
        variable_nodes.at((*it)->getID())->update.push_back(newInvariant);
    }

}


/// Used for posting oneway constraints. Start by making variable nodes point to it, variable node and invariant nodes will be merged later.

void DependencyDigraph::addInvariant(invariant newInvariant) {
    //
    std::shared_ptr<invariantNode> invar = std::make_shared<invariantNode>();
    invar->invar = newInvariant;
    invar->var = invar->invar->getVariable();
    invar->id = newInvariant->getID();
    invariant_nodes.push_back(invar);
    std::vector<Variable*>& vars = newInvariant->getVariablePointers();
    //    std::vector<invariant>& invars = newInvariant->getInvariantPointers();
    for (auto it = vars.begin(); it != vars.end(); ++it) {
        //        
        if ((*it)->isFixed()) {
            continue;
        }
        std::shared_ptr<variableNode> vn = variable_nodes.at((*it)->getID());

        vn->update.push_back(newInvariant);
    }
}

/// Used to add invariants created by constraints, auxiliary variables and violations. At this point cannot make cycles hence the pointers never change from here. 

void DependencyDigraph::addInvariant(invariant newInvariant, variableContainer& vars, InvariantContainer& invars) {

    //void DependencyDigraph::addInvariant(invariant newInvariant) {
    //
    std::shared_ptr<invariantNode> invar = std::make_shared<invariantNode>();
    invar->invar = newInvariant;
    invar->id = newInvariant->getID();

    invariant_nodes.push_back(invar);
    for (auto it = vars.begin(); it != vars.end(); ++it) {
        //        
        if ((*it)->isFixed()) {
            continue;
        }

        std::shared_ptr<variableNode> vn = variable_nodes.at((*it)->getID());

        //        
        vn->update.push_back(newInvariant);
    }
    for (invariant inv : invars) {
        invariant_nodes.at(inv->getID())->update.push_back(newInvariant);
    }
}



/// Return the invariants that this invar defines

updateVector& DependencyDigraph::getInvariantUpdate(unsigned invarID) {
    return invariant_nodes.at(invarID)->update;
}

updateVector& DependencyDigraph::getVariableUpdate(unsigned varID) {
    return variable_nodes.at(varID)->update;
}

propagation_queue& DependencyDigraph::getPropagationQueue(Variable* iv) {
    assert(!iv->isDef());
    assert(!iv->isFixed());
    assert(variable_nodes.at(iv->getID())->propagationQueue.size() != 0);
    return variable_nodes.at(iv->getID())->propagationQueue;
}

bool DependencyDigraph::propagationQueueHasBeenMade() {
    return madePropagationQueues;
}

void DependencyDigraph::cleanUpGraph(std::vector<Variable*>& vars) {
    for (Variable* iv : vars) {
        if (iv->isDef()) {
            std::shared_ptr<variableNode> vn = variable_nodes.at(iv->getID());
            mergeNodes(vn, invariant_nodes.at(iv->getOneway()->getID()));

        }
    }

}

void DependencyDigraph::mergeNodes(std::shared_ptr<variableNode>& vn, std::shared_ptr<invariantNode>& in) {
    //    std::cout << "Dublicates?" << std::endl;
    for (invariant invar : vn->update) {
        in->update.push_back(invar);

    }
    in->var = vn->iv;
    assert(vn->update.size() == in->update.size());

}

void DependencyDigraph::checkForCycles(InvariantContainer& invars) {

    cycleCounter = 1;
    timestampCounter = 0;
    while (cycleCounter != 0) {
        cycleCounter = 0;
        invariantsCycles.clear();
        for (unsigned i = 0; i < invars.size(); i++) {
            std::vector<unsigned > cycles; // = new std::vector<unsigned >();
            invariantsCycles.push_back(cycles);
        }
        for (invariant inv : invars) {
            std::shared_ptr<invariantNode> in = invariant_nodes.at(inv->getID());
            if (in->timestamp == 0) {
                DFS(in);
            }
            assert(inv->getTimestamp() != 0);
        }

        std::cout << "number of cycles found" << cycleCounter << std::endl;

        if (cycleCounter != 0) {
            breakCycles();
        }
        for (invariant inv : invars) {
            if (inv->isBroken()) {
                continue;
            }
            std::shared_ptr<invariantNode> in = invariant_nodes.at(inv->getID());
            in->timestamp = 0;
            in->lowestLink = 0;
            assert(!invariant_nodes.at(inv->getID())->inCurrentSSC);



        }

    }


}

// Tarjan's Algorithm

void DependencyDigraph::DFS(std::shared_ptr<invariantNode>& v) {
    timestampCounter++;
    v->timestamp = timestampCounter;
    v->lowestLink = timestampCounter;
    v->inCurrentSSC = true;
    SCC.push_back(v);
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
        std::vector<unsigned> component;
        std::shared_ptr<invariantNode> u = SCC.back();

        do {

            u = SCC.back();
            timestampCounter++;
            u->invar->setTimestamp(timestampCounter);
            SCC.pop_back();
            component.push_back(u->id);

            u->inCurrentSSC = false;


        } while (u != v);
        if (component.size() > 1) {
            cycleCounter++;
            for (unsigned id : component) {

                invariantsCycles.at(id).push_back(cycles.size());
            }
            cycles.push_back(component);
            brokenCycles.push_back(false);
        }

    }
}

void DependencyDigraph::breakCycles() {
    for (unsigned cycle = 0; cycle < cycles.size(); cycle++) {

        if (brokenCycles[cycle]) {
            continue;
        }
        std::shared_ptr<invariantNode> best = breakTie(cycle);
        best->invar->setBroken(true);
        for (int cyc : invariantsCycles.at(best->id)) {
            brokenCycles.at(cyc) = true;
        }

        undefineVariable(best);
    }


}

std::shared_ptr<invariantNode> DependencyDigraph::breakTie(unsigned cycleNumber) {
    std::vector<unsigned >& cycle = cycles.at(cycleNumber);
    assert(cycle.size() > 0);
    std::shared_ptr<invariantNode> bestInvar = invariant_nodes.at(cycle.at(0));
    unsigned bestPrio = bestInvar->var->getSerachPriority();
    unsigned bestArity = bestInvar->invar->getVariablePointers().size();
    unsigned equalCounter = 1;
    for (unsigned i = 1; i < cycle.size(); i++) {
        std::shared_ptr<invariantNode> in = invariant_nodes.at(cycle.at(i));
        unsigned prio = in->var->getSerachPriority();
        if (prio < bestPrio) {
            bestInvar = in;
            bestArity = in->invar->getVariablePointers().size();
            bestPrio = prio;
            equalCounter = 1;
        } else if (prio == bestPrio) {
            unsigned arity = in->invar->getVariablePointers().size();
            if (arity > bestArity) {
                bestInvar = in;
                bestArity = in->invar->getVariablePointers().size();
                bestPrio = prio;
                equalCounter = 1;
            } else if (arity == bestArity) {
                int choose = Random::Integer(equalCounter);
                equalCounter++;
                if (choose == 0) {
                    bestInvar = in;
                    bestArity = in->invar->getVariablePointers().size();
                    bestPrio = prio;
                }
            }
        }
    }
    return bestInvar;
}

void DependencyDigraph::undefineVariable(std::shared_ptr<invariantNode> invar) {

    invar->var->undefine();
    assert(invar->var->getOneway()->isBroken());
    invariant& bestInvariant = invar->invar;
    std::vector<Variable*>& vars = bestInvariant->getVariablePointers();

    for (Variable* iv : vars) {
        if (iv->isFixed()) {

            continue;
        }
        if (iv->isDef()) {
            std::shared_ptr<invariantNode>& invarNode = invariant_nodes.at(iv->getOneway()->getID());
            updateVector& update = invarNode->update;
            for (updateVector::iterator it = update.begin(); it != update.end(); ++it) {

                if ((*it) == bestInvariant) {
                    update.erase(it);
                    break;
                }
            }
        }
        std::shared_ptr<variableNode>& varNode = variable_nodes.at(iv->getID());
        updateVector& update = varNode->update;
        for (updateVector::iterator it = update.begin(); it != update.end(); ++it) {
            if (*it == bestInvariant) {

                update.erase(it);
                break;
            }
        }

    }
    std::vector<invariant>& invars = bestInvariant->getInvariantPointers();
    for (invariant inv : invars) {
        std::shared_ptr<invariantNode>& invarNode = invariant_nodes.at(inv->getID());
        updateVector& update = invarNode->update;
        //            std::iterator<invariant> it = update.begin();
        for (updateVector::iterator it = update.begin(); it != update.end(); ++it) {

            if ((*it) == bestInvariant) {
                update.erase(it);
                break;
            }
        }
    }


}

void DependencyDigraph::createPropagationQueue(variableContainer & vars) {
    madePropagationQueues = true;
    assert(cycleCounter == 0);
    for (Variable* iv : vars) {
        if (!iv->isFixed() && !iv->isDef()) {

            std::shared_ptr<variableNode> vn = variable_nodes.at(iv->getID());
            assert(vn->update.size() > 0);
            addToQueue(vn->propagationQueue, vn->update, 1);
        }
    }
}


/// Not optimal, it adds vertices from outgoing arcs for each ingoing arc. (If a vertex is visited twice it adds the vertices it points to twice) 
/// With a DFS you can set vertices to visited but you need to unvisit them again. 

void DependencyDigraph::addToQueue(propagation_queue& orgQueue, updateVector & queue, int depth) {
    for (updateType invar : queue) {
        if (invar->getVariableID() != -1) {

            assert(invariant_nodes.at(invar->getID())->var->isDef());
        }

        addToQueue(orgQueue, invariant_nodes.at(invar->getID())->update, depth + 1);
        orgQueue.insert(invar);


    }

}

void DependencyDigraph::printSizes() {
    std::cout << "#Variable nodes " << variable_nodes.size() << std::endl;
    std::cout << "#Invariant nodes " << invariant_nodes.size() << std::endl;
}

DependencyDigraph::~DependencyDigraph() {
}

