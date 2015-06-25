// File BP_Helpers.cc
#include "BP_Helpers.hpp"
#include <cstdlib>
#include <utility>
#include <fstream>
//#include "Constants.hh"




double BP_CostComponent1::ComputeCost(const BP_State& st) const
{
  double cost = 0;
  // Insert the code that computes the cost for component 1 
  // of state st
  for(unsigned i = 0; i< in.getNvars(); i++){
      if(st[i]){
          cost += in.getVars(i).objcoeff;
      }
      
  }

  return cost;
}
          
void BP_CostComponent1::PrintViolations(const BP_State& st, ostream& os) const
{
  // Insert the code that prints the violations of component 1 
  // of state st

    
}

double BP_CostComponent2::ComputeCost(const BP_State& st) const
    
{ 
    
    double cost = 0;
    // Insert the code that computes the cost for component 2
    // of state st

    for(unsigned i = 0; i< in.getNcons();i++){
        bounds b = in.getBterms(i);
        double subcost = 0;

        vector<elem> a = in.getMatcoeff(i);
        
        for(unsigned j = 0; j< a.size();j++){
            int var = a.at(j).index;
            if(st.getAssignment(var)){
                elem aij = a.at(j);
                subcost += aij.coeff;
            } 
        }
        if (b.type==5) {
            if(subcost!= b.lb){
                cost++;
            } 
        } else {
            if(subcost< b.lb){
                cost++;
                
            } 
        }
        
    }
//    std::cout << st[0] << " " << st[1] << " " << st[2] << std::endl;
//    std::cout << cost << std::endl;
    return cost+ nonTerm;
}
 
void BP_CostComponent2::PrintViolations(const BP_State& st, ostream& os) const
{

    for(unsigned i = 0; i< in.getNcons();i++){
        double subcost = 0;
        bounds b = in.getBterms(i);
        vector<elem> a = in.getMatcoeff(i);
        
        for(unsigned int j = 0; j< a.size();j++){
            int var = a.at(j).index;
            if(st.getAssignment(var)){
                elem aij = a.at(j);
                subcost += aij.coeff;
          }
        }
        if(b.type==3){
            if(subcost>b.ub){
                os << "Constraint " << i << " not satisfied type 3 (<=)"; 
            } 
        } else if (b.type==5) {
            if(subcost!= b.lb){
                os << "Constraint " << i << " not satisfied type 5 (==)";          
            } 
        } else {
            if(subcost< b.lb){
                os << "Constraint " << i << " not satisfied type 2 (>=)";
            } 
        }
        
    }
}

// constructor
BP_StateManager::BP_StateManager(const BP_Input & pin) 
  : StateManager<BP_Input,BP_State,double>(pin, "XYZStateManager")  {} 

// initial state builder (random rooms)
void BP_StateManager::RandomState(BP_State& st) 
{
// Insert the code that creates a random state in object st
    
//    Random 50/50
    //    for(unsigned i = 0; i< in.getNvars();i++){
//        st.setAssignment(i,Random::Int(0,1));
//        
//    }
    

    
    // Sum gammelt
//    unsigned prevSize = 0;
//    for(unsigned i = 0; i< in.getNvars();i++){
//        if(in.getMatcoeff2(i).size()<=prevSize){
//            prevSize = in.getMatcoeff2(i).size();
//            st.setAssignment(i,true);
//        }
//        
//    }
//    
//
//    for(unsigned i = 0; i< in.getNcons();i++){
//      double subcost = 0;
//      bounds b = in.getBterms(i);
//      vector<elem> a = in.getMatcoeff(i);
//
//      for(unsigned j = 0; j< a.size();j++){
//          int var = a.at(j).index;
//
//          if(st.getAssignment(var)){
//              elem aij = a.at(j);
//              subcost += aij.coeff;
//          }
//      }
//
//      st.setSum(i,subcost);
//      if(subcost < b.lb ){
//          st.setViolated(i,true);
//          st.setConstVioleted(i);
//      } else if (b.type==5) {
//          if(subcost != b.lb){
//              st.setViolated(i,true);
//              st.setConstVioleted(i);
//              
//          } 
//      } else {
//          st.setViolated(i,false);
//          
//      }
//    }
//    
    
    // Greedy
    int flips = 0;

    
    
    st.constraintSum.clear();
    st.constraintSum.resize(in.getNcons(),0);
    //    for(int i = 0; i < in.getNcons();i++){
    //        if(st.getSum(i) != 0){
    //            std::cout << "what the F" << std::endl;
    //        }
    //    }
    //    vector<bool> checker (in.getNvars(),false);
    unsigned int flipCounter;
    bool flip = true;
    for(unsigned i = 0; i< in.getNvars();i++){
        //        checker[mask[i]] = true;
        flipCounter = 0;
        flip = true;
        vector<elem> a = in.getMatcoeff2(st.getMask(i));
        //        bool objCoefPositive = in.getVars(i).objcoeff < 0 ? false : true;
        
        for(unsigned j = 0; j < a.size(); j++ ){
            elem aij = a.at(j);
            double coef = aij.coeff;
            bounds b = in.getBterms(aij.index);
            //            int leftSize = in.getMatcoeff(aij.index).size();
            //            std::cout << leftSize << " " << aij.index << std::endl;
            //            double dist = st.getDistToBound(aij.index);
            
            double sum = st.getSum(aij.index);
            if(b.type == 5){
                if(sum+coef > b.lb){
                    flip = false;
                    
                } else {
                    flipCounter++;
                }
            } else {
                if(coef < 0){
                    //                    std::cout << coef << std::endl;
//                    flip=false;
                    flipCounter--;
                    //                    flipCounter++; 
                } else {
                    flipCounter++;
                }
            }
        }
        double flipRatio = (double) flipCounter/a.size();
        //        if(flipRatio != 1 && flipRatio !=0){
        //            
        //            std::cout << flipRatio << std::endl;
        //        }
        if(Random::Double() <= flipRatio){
            if(flip){
                st.setAssignment(st.getMask(i),true);
                flips++;
                vector<elem> a = in.getMatcoeff2(st.getMask(i));
                for(unsigned j = 0;j<a.size();j++ ){
                    elem aij = a.at(j);
                    double coef = aij.coeff;
                    //                double dist = st.getDistToBound(aij.index);
                    double sum = st.getSum(aij.index);
                    
                    double newSum = sum + coef;
                    st.setSum(aij.index,newSum);
                }
                
            } else {
                st.setAssignment(st.getMask(i),false);
            }
            //        
        }
    }
    std::cout << "No of flips: " << flips << std::endl;
    for(unsigned i = 0; i< in.getNcons();i++){
        double sum = st.getSum(i);
        vector<elem> a = in.getMatcoeff(i);
//        double subcost = 0;
//        for(unsigned j = 0; j< a.size();j++){
//            int var = a.at(j).index;
//            if(st.getAssignment(var)){
//                elem aij = a.at(j);
//                subcost += aij.coeff;
//            }
//        }
//        std::cout << subcost << " vs " << sum << std::endl;
        bounds b = in.getBterms(i);
        if(sum < b.lb ){
            st.setViolated(i,true);
            st.setConstVioleted(i);
        } else if (b.type==5) {
            if(sum != b.lb){
                st.setViolated(i,true);
                st.setConstVioleted(i);
            } 
        } else {
            st.setViolated(i,false);
            
        }
    }
    
    // Mærkelig form
//    unsigned prevSize = 0;
//    for(unsigned i = 0; i< in.getNvars();i++){
//        if(in.getMatcoeff2(i).size()<=prevSize){
//            prevSize = in.getMatcoeff2(i).size();
//            st.setAssignment(i,true);
//        }
//        
//    }
//    
//    
//    
//    
//// Set violations
//    for(unsigned i = 0; i< in.getNcons();i++){
//      double subcost = 0;
//      bounds b = in.getBterms(i);
//      vector<elem> a = in.getMatcoeff(i);
//
//      for(unsigned j = 0; j< a.size();j++){
//          int var = a.at(j).index;
//
//          if(st.getAssignment(var)){
//              elem aij = a.at(j);
//              subcost += aij.coeff;
//          }
//      }
//      double sum = st.getSum(i);
////      std::cout << sum << " vs " << subcost << std::endl;
//      st.setSum(i,subcost);
//      if(subcost < b.lb ){
//          st.setViolated(i,true);
//          st.setConstVioleted(i);
//      } else if (b.type==5) {
//          if(subcost != b.lb){
//              st.setViolated(i,true);
//
//              st.setConstVioleted(i);
//
//          } 
//      } else {
//          st.setViolated(i,false);
//
//      }
//    }

    // Dist
//    std::mt19937 eng (42);
//    std::uniform_int_distribution<int> dist(0,1 );
//    for(unsigned i = 0; i< in.getNvars();i++){
//        st.setAssignment(i,dist(eng));
//        
//    }
//    for(unsigned i = 0; i< in.getNcons();i++){
//      double subcost = 0;
//      bounds b = in.getBterms(i);
//      vector<elem> a = in.getMatcoeff(i);
//
//      for(unsigned j = 0; j< a.size();j++){
//          int var = a.at(j).index;
//          if(st.getAssignment(var)){
//              elem aij = a.at(j);
//              subcost += aij.coeff;
//          }
//      }
//      double dist = st.getDistToBound(i);
////      std::cout << "dist " << dist << std::endl;
////      st.setSum(i,subcost);
//      dist = b.lb-subcost;
//      st.setDistToBound(i,dist);
//      if(dist > 0 ){
//          st.setViolated(i,true);
////          st.setConstVioleted(i);
//      } else if (b.type==5) {
//          if(dist != 0){
//              st.setViolated(i,true);
////              st.setConstVioleted(i);
//          } 
//      } else {
//          st.setViolated(i,false);
//
//      }
    
    
    
} 
void BP_StateManager::GreedyState(BP_State& st){
    
    
    // Hvordan starter jeg med Greedy? 
//    std::cout << "Greedy" << std::endl;
} 

bool BP_StateManager::CheckConsistency(const BP_State& st) const
{
//    for(unsigned i = 0; i< in.getNvars();i++){
//        double subcost = 0;
//        vector<elem> a = in.getMatcoeff2(i);
//        
//        for(unsigned int j = 0; j< a.size();j++){
//            elem e = a.at(j);
//
//        }
//
//        
//    }


  // Insert the code that checks if state in object st is consistent
  // (for debugging purposes)
//    for (int i = 0; i < in.getNcons();i++){
//        std::cout << "constraint " << i << " vars: ";
//        
//        vector<elem> tmp = in.getMatcoeff(i);
//        for(int j = 0; j< tmp.size();j++){
//            elem e = tmp[j];
//            std::cout << e.index << " ";
//        }
//        
//       
//        std::cout << std::endl;
//        std::cout << "their coeff: ";
//        for(int j = 0; j< tmp.size();j++){
//            elem e = tmp[j];
//            std::cout << e.coeff << " " ;
//        }
//        std::cout << std::endl;
//    }
//    
//    for (int i = 0; i < in.getNvars();i++){
//        std::cout << "var " << i << " cons: ";
//        
//        vector<elem> tmp = in.getMatcoeff2(i);
//        for(int j = 0; j< tmp.size();j++){
//            elem e = tmp[j];
//            std::cout << e.index << " ";
//        }
//        
//       
//        std::cout << std::endl;
//        std::cout << "their coeff: ";
//        for(int j = 0; j< tmp.size();j++){
//            elem e = tmp[j];
//            std::cout << e.coeff << " " ;
//        }
//        std::cout << std::endl;
//    }
    
    
    
//        vector<elem> a = in.getMatcoeff(0);
//    for (unsigned i = 0; i< a.size();i++){
//        std::cout << a.at(i).coeff << std::endl;
//    }
//    a = in.getMatcoeff(1);
//    for (unsigned i = 0; i< a.size();i++){
//        std::cout << a.at(i).coeff << std::endl;
//    }
//    a = in.getMatcoeff(2);
//    for (unsigned i = 0; i< a.size();i++){
//        std::cout << a.at(i).coeff << std::endl;
//    }
    
//    for(int i = 0; i< in.getNvars();i++){
//        vector<int> tmp = st.inConstraintList(i);
//        std::cout << "Variable " << i << ": ";
//        for(unsigned j = 0; j< tmp.size();j++){
//            std::cout << tmp[j] << " ";
//        }
//        std::cout << endl;
//    }
//        std::ofstream ofs;
//    std::string output= "eilA101b";
//    ofs.open(output,std::ofstream::out);
//    for(int i = 0; i< in.getNcons();i++){
//        bounds b = in.getBterms(i);
//        if(b.type==3){
//            ofs << b.ub << std::endl;
//        } else if (b.type==5) {
//            ofs << b.lb << std::endl;
//
//        } else {
//            ofs << b.lb << std::endl;
//
//        }
//        
//        
//    }
//    ofs.close();
  return true;
}

/*****************************************************************************
 * Output Manager Methods
 *****************************************************************************/

void BP_OutputManager::InputState(BP_State& st, const BP_Output& XYZ) const 
{
  // Insert the code that "translates" an output object to a state object
    for(unsigned j = 0; j< in.getNvars();j++){
        st.setAssignment(j,XYZ.assignment(j));
    }
}

void BP_OutputManager::OutputState(const BP_State& st, BP_Output& XYZ) const 
{

  // Insert the code that "translates" a state object to an output object
    for (unsigned j = 0; j < in.getNvars();j++){
//        std::cout << "var "<< j<<" is " <<st[j] << std::endl; 
                XYZ.assign(j,st[j]);
    }
    
    
}


/*****************************************************************************
 * BP_Move Neighborhood Explorer Methods
 *****************************************************************************/

// initial move builder
void BP_MoveNeighborhoodExplorer::RandomMove(const BP_State& st, BP_Move& mv) const  throw(EmptyNeighborhood)
{

  // insert the code that writes a random move on mv in state st
    
    mv.var = Random::Int(0,in.getNvars()-1);
    mv.old = st[mv.var];
    mv.newV = !mv.old;
     
} 

// check move feasibility
bool BP_MoveNeighborhoodExplorer::FeasibleMove(const BP_State& st, const BP_Move& mv) const
{
  // Insert the code that check is move mv is legal in state st 
  // (return true if legal, false otherwise)
    
    return true; // mv.old != mv.newV;
    
    //dobbelt flip
//  return  mv.old1 != mv.newV1 && mv.old2 != mv.newV2 && mv.var1 != mv.var2;
} 

// update the state according to the move 
void BP_MoveNeighborhoodExplorer::MakeMove(BP_State& st, const BP_Move& mv) const
{
    // Insert the code that modify the state st based on the application of move mv

    
        //dist
//    st.setAssignment(mv.var,mv.newV);
//    
//    st.nrMoves++;
//    
//    vector<elem> constr = in.getMatcoeff2(mv.var); 
//    for(unsigned i = 0; i< constr.size();i++){
//        
//        elem e = constr[i];
//        bounds b  = in.getBterms(e.index);
//        double dist = st.getDistToBound(e.index);
//        if(st[mv.var]){
//            st.setDistToBound(e.index,dist-e.coeff);
//        } else {
//            st.setDistToBound(e.index,dist+e.coeff);
//            
//        }
//        dist = st.getDistToBound(e.index);
//        
//        if(dist == 0){
//            st.setViolated(e.index,false);
//        } else if (dist < 0 && b.type ==2){
//            st.setViolated(e.index,false);
//        } else {
//            st.setViolated(e.index,true);
//        }
//        
//    }
//    if(st.nrMoves % 100000 == 0){
//        std::cout << "move " << st.nrMoves << std::endl << std::flush;
//        
//    }
     st.setAssignment(mv.var);
//    mv.old = !mv.old;
//    mv.newV = !mv.newV;
    st.resetConstVioleted();
    st.nrMoves++;
    
    vector<elem> constr = in.getMatcoeff2(mv.var); 
    for(unsigned i = 0; i< constr.size();i++){
        
        elem e = constr[i];
        bounds b  = in.getBterms(e.index);
        double sum = st.getSum(e.index);
        if(st[mv.var]){
            sum+=e.coeff;
        } else {
            sum-=e.coeff;
        }
        st.setSum(e.index,sum);
        if (b.type==2) {
            if(sum< b.lb){
                st.setViolated(e.index,true);
                st.setConstVioleted(e.index);
            } 
        } else {
            if(sum!= b.lb){
                st.setViolated(e.index,true);
                st.setConstVioleted(e.index);
            } 
        }
        
    }
//
}  

void BP_MoveNeighborhoodExplorer::FirstMove(const BP_State& st, BP_Move& mv) const  throw(EmptyNeighborhood)
{
    // Insert the code the generate the first move in the neighborhood and store it in mv
    
    mv.var = Random::Int(0,in.getNvars()-1);
//    mv.var=0;
    mv.old = st[mv.var];
    mv.newV = !mv.old;

    
}

bool BP_MoveNeighborhoodExplorer::NextMove(const BP_State& st, BP_Move& mv) const
{
    // Insert the code that generate the move that follows mv in the neighborhood, and writes
    // it back in mv. Return false if mv is already the last move. 
    if(mv.counter< 50 && st.constViolatedSize() >30){
        int index = Random::Int(0,st.getConstVioleted().size()-1);
        int con = st.getConstVioleted().at(index);
        vector<elem> cons =  in.getMatcoeff(con);
        int var = cons.at(Random::Int(0,cons.size()-1)).index;
        mv.var = var;
        mv.old = st[mv.var];
        mv.newV = !mv.old;

        mv.counter++;
        return true;
    }
    
//    Random
    mv.var = Random::Int(0,in.getNvars()-1);
    mv.old = st[mv.var];
    mv.newV = !mv.old;
    
    if(mv.counter > maxMoves ){//in.getNvars()/10000){
        return false;
    }

    mv.counter++;
    return true;
}

double BP_MoveDeltaCostComponent1::ComputeDeltaCost(const BP_State& st, const BP_Move& mv) const
{
  double cost = 0;

  // Insert the code that computes the delta cost of component 1 for move mv in state st
  
    if(!st[mv.var]){
      cost+= in.getVars(mv.var).objcoeff;
  } else {
      cost-= in.getVars(mv.var).objcoeff;
  }

  return cost;
}

double BP_MoveDeltaCostComponent2::ComputeDeltaCost(const BP_State& st, const BP_Move& mv) const
{
    // Insert the code that computes the delta cost of component 2 for move mv in state st
    //    std::cout << mv.var << std::endl;
    double cost = 0;
    // Insert the code that computes the cost for component 2
    // of state st
 
    // Sum
     vector<elem> constraint = in.getMatcoeff2(mv.var);
    double sum, aij;
    bounds b;
    for(unsigned  int i = 0; i< constraint.size();i++){
        
        elem e = constraint[i];
//        std::cout << "e.index " << e.index << std::endl;
        b = in.getBterms(e.index);                    
        sum = st.getSum(e.index); 
//        std::cout << "sum " << sum << std::endl;
        
        aij = e.coeff;
//        std::cout << "aij " << aij << std::endl;
        if(st[mv.var]){
            aij = -aij;
        }
//        std::cout << "b type " << b.type << std::endl;
        double newSum = sum+aij;
        if(b.type == 2){
            if(sum< b.lb){
                if(newSum>=b.lb){
                    cost--;
//                    std::cout << "t1" << std::endl;
                }
            } else if (newSum<b.lb){
                cost++;
//                std::cout << "t2" << std::endl;
            }
        } 
        else {
            if(aij!=0){
                if(sum == b.lb ){
                    cost++;           
                    //                std::cout << "t3" << std::endl;
                } else if(sum+aij == b.lb ){
                    cost--;
                    //                std::cout << "t4" << std::endl;
                }
            }
        }
    }
    
    
    
    return cost;
}


bool BP_TabuListManager::Inverse(const BP_Move& m1, const BP_Move& m2) const
{
    
    return m1==m2;
}