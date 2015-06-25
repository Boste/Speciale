// File BP_Basics.hh
#ifndef BP_BASICS_HH
#define BP_BASICS_HH

#include "BP_Data.hpp"
#include <array>

class BP_State
{  
    friend ostream& operator<<(ostream&, const BP_State&);
    friend bool operator==(const BP_State& st1, const BP_State& st2);
public:
    BP_State(const BP_Input &in);
    BP_State& operator=(const BP_State& s);
    bool setAssignment(unsigned x_j,bool b);
    void setAssignment(unsigned x_j);
    bool getAssignment(unsigned x_j) const {return varAssignment.at(x_j);}
    bool setViolated(unsigned i,bool b);
    bool getViolated(unsigned i) const {return violatedCons.at(i);}
    int setConstVioleted(int i);
    vector<int> getConstVioleted() const {return constViolated;}
    void resetConstVioleted();
    int constViolatedSize() const {return constViolated.size();}
    double setSum(int i,double cost);
    double getSum(int i) const {return constraintSum.at(i);}
    int getMask(int i) {return mask[i];}
    //    double getDistToBound(int i)  const {
    //        return distToBound[i];
    //    }
    //    void setDistToBound(int i, double dist);
    //    double getCoef(int i, int j) const {return coef[i*in.getNvars()+j];}
    //    std::vector<int> inConstraintList(int x_j) const {return inConstraint[x_j];}
    int nrMoves = 0;
    
    bool operator[](int var) const { return varAssignment[var]; }
    //    vector<bool>::const_reference operator[](unsigned int i)const{return varAssignment[i];}
//    vector<bool>::reference operator[](unsigned int i){return varAssignment[i];}
    
    
    std::vector<double> constraintSum;
    
    
protected:
    const BP_Input & in;
    std::vector<int> mask;
  std::vector<std::vector<unsigned int>> neighbor;    
  std::vector<bool> varAssignment;
  std::vector<bool> violatedCons;
  std::vector<int> constViolated;
//  vector<double> distToBound;

//  std::vector<double> coef;
//  std::vector<vector<int>> inConstraint;
private:

    

//  int size = in.getNvars();
};

class BP_Move
{
  friend bool operator==(const BP_Move& m1, const BP_Move& m2);
  friend bool operator!=(const BP_Move& m1, const BP_Move& m2);
  friend bool operator<(const BP_Move& m1, const BP_Move& m2);
  friend ostream& operator<<(ostream& os, const BP_Move& c);
  friend istream& operator>>(istream& is, BP_Move& c);
 public:
  BP_Move();
  // Ved ikke om de skal initialiseres og til hvad
  
    int var;
  bool old;
  bool newV;
//  int var1;
//  bool old1;
//  bool newV1;
//  int var2;
//  bool old2;
//  bool newV2;
//  void flip();
  int counter;
};
#endif

