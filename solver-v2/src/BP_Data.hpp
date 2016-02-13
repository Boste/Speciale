// File BP_Data.hh
#ifndef BP_DATA_HH
#define BP_DATA_HH

#include <iostream>
#include <vector>
#include <cassert>
#include "coin/CoinMpsIO.hpp"
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

    unsigned getNcons() const {
        return ncons;
    }

    unsigned getNvars() const {
        return nvars;
    }

    var getVar(const int j) const {
        assert(j >= 0 && j < nvars);
        return vars[j];
    }

    vector<var> getVars() const {
        return vars;
    }
    vector<unsigned> getObjVars() const {  
        return objVars;
    }
    int getDirection() const {
        return direction;
    }

    ~BP_Input(){
    }

protected:
    int nvars;
    int ncons;
    int nbinvars;
    int nintvars;
    int direction;
    unsigned scale;

    vector<vector<elem> > matcoeff;
    vector<bounds> bterms;
    vector<var> vars;
    vector<unsigned> objVars;
};

class BP_Output {
    friend ostream& operator<<(ostream& os, const BP_Output& out);
    friend istream& operator>>(istream& is, BP_Output& out);
public:

protected:
    const BP_Input& in;
    vector<bool> varAssignment;
};


#endif
