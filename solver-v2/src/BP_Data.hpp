// File BP_Data.hh
#ifndef BP_DATA_HH
#define BP_DATA_HH

#include <iostream>
#include <vector>
#include <cassert>
//#include <glpk.h>
#include "coin/CoinMpsIO.hpp"
//#include "../include/Constraints.hpp"<>
#include <fstream>
#include <algorithm>

#ifndef NDEBUG
#define DEBUG(X) X
#else
#define DEBUG(X)
#endif

using namespace std;

struct elem {
    int index;
    double coeff;

    ~elem() {
    };
};

struct bounds {
    int type;
    double lb;
    double ub;

    ~bounds() {
    };
};

struct var : bounds {
    double objcoeff;
    int kind;
    bool bin;

    ~var() {
    };
};

class BP_Input {
    friend ostream& operator<<(ostream& os, const BP_Input& bs);
public:
    BP_Input(string file_name);

    bounds getBterms(const int i) const {
        assert(i >= 0 && i < ncons);
        return bterms[i];
    }

    const vector<elem> & getMatcoeff(const int i) const {
        assert(i >= 0 && i < ncons);
        return matcoeff[i];
    }

    const vector<elem> & getMatcoeff2(const int j) const {
        assert(j >= 0 && j < nvars);
        return matcoeff2[j];
    }

    unsigned getNcons() const {
        return ncons;
    }

    unsigned getNvars() const {
        return nvars;
    }

    var getVars(const int j) const {
        assert(j >= 0 && j < nvars);
        return vars[j];
    }

    int getDirection() const {
        return direction;
    }

    unsigned getScale() const {
        return scale;
    }
    void setDigits(double number);
    
    ~BP_Input(){
        
    }
    //        Constraint getConstraint(const int i){
    //            return cons[i];
    //        } 


    // Insert your getters

protected:
    // Insert your data members
    int nvars;
    int ncons;
    int nbinvars;
    int nintvars;
    int direction;
    unsigned scale;
    //  std::vector<std::vector<unsigned int>> neighbor;    


    //vector<double> objcoeff;

    vector<vector<elem> > matcoeff;
    vector<vector<elem> > matcoeff2;
    //  vector<Constraint*> cons;

    vector<bounds> bterms;
    vector<var> vars;
};

class BP_Output {
    friend ostream& operator<<(ostream& os, const BP_Output& out);
    friend istream& operator>>(istream& is, BP_Output& out);
public:
    BP_Output(const BP_Input& i);
    BP_Output& operator=(const BP_Output& out);
    // Insert your getters

    int assignment(int var) const {
        return varAssignment[var];
    }
    void assign(int var, bool b);
protected:
    const BP_Input& in;
    vector<bool> varAssignment;
    // Insert your data members
};


#endif
