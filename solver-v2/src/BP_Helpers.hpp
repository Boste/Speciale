// File BP_Helpers.hh
#ifndef BP_HELPERS_HH
#define BP_HELPERS_HH

#include "BP_Basics.hpp"
#include <easylocal/easylocal.hh>
#include <random>
#include "Constants.hpp"

//#define CONST 0.0000000000001
using namespace EasyLocal::Core;

/***************************************************************************
 * State Manager 
 ***************************************************************************/

class BP_CostComponent1 : public CostComponent<BP_Input,BP_State,double> 
{
public:
  BP_CostComponent1(const BP_Input & in, int w, bool hard) :    CostComponent<BP_Input,BP_State,double>(in,w,hard,"BP_CostComponent1") 
  {}
  double ComputeCost(const BP_State& st) const;
  void PrintViolations(const BP_State& st, ostream& os = cout) const;
};

class  BP_CostComponent2: public CostComponent<BP_Input,BP_State,double> 
{
public:
  BP_CostComponent2(const BP_Input & in, int w, bool hard) : CostComponent<BP_Input,BP_State,double>(in,w,hard,"BP_CostComponent2") 
  {}
//  double globalCost;
//  double nonTerm = CONST;

  double ComputeCost(const BP_State& st) const;
  void PrintViolations(const BP_State& st, ostream& os = cout) const;
};

class BP_StateManager : public StateManager<BP_Input,BP_State,double> 
{
public:
    
  BP_StateManager(const BP_Input &);
  void RandomState(BP_State& st);   
  void GreedyState(BP_State& st);   
  bool CheckConsistency(const BP_State& st) const;
protected:
}; 

/***************************************************************************
 * BP_Move Neighborhood Explorer:
 ***************************************************************************/

class BP_MoveDeltaCostComponent1
  : public DeltaCostComponent<BP_Input,BP_State,BP_Move,double>
{
public:
  BP_MoveDeltaCostComponent1(const BP_Input & in, BP_CostComponent1& cc) 
    : DeltaCostComponent<BP_Input,BP_State,BP_Move,double>(in,cc,"BP_MoveDeltaCostComponent1") 
  {}
  double ComputeDeltaCost(const BP_State& st, const BP_Move& mv) const;
};

class BP_MoveDeltaCostComponent2
  : public DeltaCostComponent<BP_Input,BP_State,BP_Move,double>
{
public:
  BP_MoveDeltaCostComponent2(const BP_Input & in, BP_CostComponent2& cc) 
    : DeltaCostComponent<BP_Input,BP_State,BP_Move,double>(in,cc,"BP_MoveDeltaCostComponent2") 
  {}
  double ComputeDeltaCost(const BP_State& st, const BP_Move& mv) const;
  double constraintCost(const BP_State& st, const int i ) const;
};


class BP_MoveNeighborhoodExplorer
: public NeighborhoodExplorer<BP_Input,BP_State,BP_Move,double> 
{
public:
    
    BP_MoveNeighborhoodExplorer(const BP_Input & pin, StateManager<BP_Input,BP_State,double>& psm)  
    : NeighborhoodExplorer<BP_Input,BP_State,BP_Move,double>(pin, psm, "BP_MoveNeighborhoodExplorer") {} 
    void RandomMove(const BP_State&, BP_Move&) const throw(EmptyNeighborhood);          
    bool FeasibleMove(const BP_State&, const BP_Move&) const;  
    void MakeMove(BP_State&,const BP_Move&) const;             
    void FirstMove(const BP_State&,BP_Move&) const throw(EmptyNeighborhood);  
    bool NextMove(const BP_State&,BP_Move&) const;
    
protected:
};

/***************************************************************************
 * Output Manager:
 ***************************************************************************/
class BP_OutputManager
  : public OutputManager<BP_Input,BP_Output,BP_State,double> 
{
public:
  BP_OutputManager(const BP_Input & pin)
    : OutputManager<BP_Input,BP_Output,BP_State,double>(pin,"XYZOutputManager") {}
  void InputState(BP_State&, const BP_Output&) const;  
  void OutputState(const BP_State&, BP_Output&) const; 
}; 


class BP_TabuListManager
  : public TabuListManager<BP_State,BP_Move,double>
{
public:
  BP_TabuListManager()
    : TabuListManager<BP_State,BP_Move,double>() {}
  bool Inverse(const BP_Move& m1, const BP_Move& m2) const;
};
#endif
