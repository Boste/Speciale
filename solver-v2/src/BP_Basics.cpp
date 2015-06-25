// File BP_Basics.cc
#include <memory>
#include <algorithm>

#include "BP_Basics.hpp"


BP_State::BP_State(const BP_Input &my_in) 
: in(my_in)  //, violatedCons(in.getNcons(),false), 
        

{
    cout << "State" << endl;
    constraintSum.resize(in.getNcons(),0);
    varAssignment.resize(in.getNvars(),false);
    violatedCons.resize(in.getNcons(),false);
        mask.resize(in.getNvars());
    for(unsigned i = 0; i < in.getNvars();i++){
        mask[i]=i;
    }
    std::random_shuffle(mask.begin(),mask.end());
//    neighbor.resize(in.getNvars());
//    for(int j = in.getNvars()-1; j >= 0; j-- ){
//        vector<elem> cons = in.getMatcoeff2(j);
//        vector<unsigned int>* neigh = &neighbor.at(j);
//        for(unsigned i = 0; i < cons.size();i++){
//            vector<elem> c = in.getMatcoeff(cons.at(i).index);
//            for(int k = 0; k < c.size(); k++){
//                neigh->push_back(c.at(k).index);
//            }
//        }
//    }
//    for(unsigned j = 0; j < in.getNvars(); j++){
//        vector<unsigned int>* neigh = &neighbor.at(j);
//        std::sort(neigh->begin(),neigh->end());
//        unsigned lastVar = in.getNvars();
//        for(int i = neigh->size()-1; i >= 0; i--){
//            unsigned var = neigh->at(i);
//            if(var == lastVar){
//                neigh->erase(neigh->begin()+i);
//            } else {
//                lastVar = var;
//            }
//        }
//    }
//    for(unsigned j = 0; j < in.getNvars(); j++){
//        vector<unsigned int>* neigh = &neighbor.at(j);
////        std::cout << "var " << j << std::endl;
//        for(unsigned i = 0; i < neigh->size();i++){
////            std::cout << neigh->at(i) << std::endl;
//        }
//
//    }


    
//    distToBound.resize(in.getNcons());
//    for(unsigned i = 0; i< in.getNcons();i++){
//        bounds b = in.getBterms(i);
//        distToBound.at(i) = b.lb;
////        violatedCons[i] = false;
//        
//    }
//    std::cout << "make state" << std::endl;
  // Insert the code that initialized the data structure of the
  // state object based on the input object
//    for(int i =0; i< in.getNcons();i++){
//        std::vector<elem> ai = in.getMatcoeff(i);
//        for(unsigned j = 0; j< ai.size();j++){
//            elem aij = ai.at(j);
//            coef[i*in.getNvars() +aij.index] = aij.coeff;
//            vector<int>* tmp = &inConstraint[aij.index];
//            tmp->push_back(i);
//        }   
//    }

//    for(int j = 0;j< in.getNvars();j++){
//        vector<int> tmp = inConstraint[j];
//        for(unsigned i = 0; i < tmp.size(); i++){
//            std::cout << tmp[i] << std::endl;
//        }
//    }
    

   
}
//
//std::vector<int> BP_State::inConstraintList(int x_j) {
//    assert(x_j>=0 && x_j < in.getNvars());
//    return inConstraint[x_j];
//}


bool BP_State::setAssignment(unsigned j, bool b){
    
    assert(j>=0 && j < in.getNvars());
    varAssignment[j] = b;
    return b;
    
}
void BP_State::setAssignment(unsigned j){
    
    assert(j>=0 && j < in.getNvars());
    varAssignment[j] = !varAssignment[j];
//    return b;
    
}
bool BP_State::setViolated(unsigned i,bool b){
    assert(i>=0 && i < in.getNcons());
    violatedCons[i] = b;
    return b;
}

double BP_State::setSum(int i, double cost){
    constraintSum[i] = cost;
    return constraintSum[i];
}

int BP_State::setConstVioleted(int i){
    constViolated.push_back(i);
    return i;
}
void BP_State::resetConstVioleted(){
    constViolated.clear();
}
//void BP_State::setDistToBound(int i, double dist){
//    distToBound[i]= dist;
//}



BP_State& BP_State::operator=(const BP_State& s)
{
  // Insert the code that copies all data structures of the
  // state object from the ones of the parameter out
  // (excluding the reference to the input object, that is constant)
    for(unsigned i= 0; i < in.getNvars(); i++){
        varAssignment[i] = s.varAssignment[i];
    }
  return *this;
}
  
bool operator==(const BP_State& st1, const BP_State& st2)
{
  // Insert the code that compares two states (this is used by the tester)
    for(unsigned i= 0; i < st1.in.getNvars(); i++){
        if(st1.varAssignment[i] != st2.varAssignment[i]){        
            return false;
        }
    }
  return true;
}


ostream& operator<<(ostream& os, const BP_State& as)
{
  // Insert the code that writes the state object (for debugging)
    
    
  return os;
}

BP_Move::BP_Move()
{
  // Insert the code that initialize the move
    
    var = -1;
    old = false;
    newV = true;
    counter = 1;
    
//    var1 = -1;
//    old1 = false;
//    newV1 = true;
//    var2 = -1;
//    old2 = false;
//    newV2 = true;
//    counter = -1;
    
    
    
}
//void BP_Move::flip(){
//    newV = !old;
    
    
//    newV1 = !old1;
//    newV2 = !old2;
    
    
    
//    old = old ? false : true;
//    newV = newV ? false : true;
//    return  newV; 
//}

bool operator==(const BP_Move& mv1, const BP_Move& mv2)
{
  // Insert the code that checks if two moves are identical
    
    //dobbelt flip
//    if( mv1.var1 == mv2.var1){
//        if(mv1.var2 == mv2.var2){
//            return true;
//        } 
//    } else if (mv1.var1 == mv2.var2){
//        if(mv1.var2 == mv2.var1){
//            return true;
//        }
//}
//    return false;
    
    
    return mv1.var == mv2.var;
}

bool operator!=(const BP_Move& mv1, const BP_Move& mv2)
{
  // Insert the code that checks if two moves are different
    
    // Dobbelt flip
//    if( mv1.var1 == mv2.var1){
//        if(mv1.var2 == mv2.var2){
//            return false;
//        } 
//    } else if (mv1.var1 == mv2.var2){
//        if(mv1.var2 == mv2.var1){
//            return false;
//        }
//}
//  return true;
    
    
    return mv1.var != mv2.var;
}

bool operator<(const BP_Move& mv1, const BP_Move& mv2)
{
  // Insert the code that checks if one move precedes another one
  // (in any selected order)
    return mv1.counter < mv2.counter;
    
//  return true;
}

istream& operator>>(istream& is, BP_Move& c)
{
  // Insert the code that read a move
//    char ch;
    is >> c.var;

    
//    dobbelt flip
//    is >> c.var1 >> ch >> c.old1 >> ch >> ch >> c.newV1 
//            >> ch >> c.var2 >> ch >> c.old2 >> ch >> ch >> c.newV2;
//    
  return is;
}

ostream& operator<<(ostream& os, const BP_Move& c)
{
  // Insert the code that writes a move
        os << c.var << ": " << c.old << "->" << c.newV ; 

    //dobblet flip
//    os << c.var1 << ':' << c.old1 << "->" << c.newV1 << "&"
//            << c.var2 << ':' << c.old2 << "->" << c.newV2;
//           
  return os;
}
