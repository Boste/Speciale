// File BP_Data.cc
#include "BP_Data.hpp"



// Turn off compiler warning about long names.
//#pragma warning(disable:4786)

// #include "readwrite.h"

BP_Input::BP_Input(string file_name) {



    CoinMpsIO t;
    std::cout.precision(6);
    t.readMps(file_name.c_str());
    ncons = t.getNumRows();
    nvars = t.getNumCols();
    vars.resize(nvars);
    matcoeff.resize(ncons);
    bterms.resize(ncons);
    assert(ncons > 0);
    assert(nvars > 0);
    for (int j = 0; j < nvars; j++) {
        var tmp;
        tmp.objcoeff = t.getObjCoefficients()[j];
        tmp.lb = t.getColLower()[j];
        tmp.ub = t.getColUpper()[j];

        if (tmp.ub > std::numeric_limits<int>::max() - 1) {
            tmp.ub = std::numeric_limits<int>::max() - 1;
        }
        vars[j] = tmp;
        if(tmp.objcoeff != 0.0){
            objVars.push_back(j);
        }
    }
    const CoinPackedMatrix* matrix = t.getMatrixByRow();
    int G = 0;
    int E = 0;
    int L = 0;
    for (int i = 0; i < ncons; i++) {
        const CoinShallowPackedVector vec = matrix->getVector(i);

        const double* val = vec.getElements();
        int non_zeros = vec.getNumElements();
        const int* indices = vec.getIndices();
        matcoeff[i].resize(non_zeros);
        bounds tmp;
        char c = t.getRowSense()[i];
        int type;
        if (c == 'G') {
            type = 2;
            G++;
        } else if (c == 'E') {
            type = 5;
            E++;
        } else {
            type = 3;
            L++;
        }

        if (type == 2) {
            tmp.type = 3;
            tmp.lb = -t.getRowUpper()[i];
            tmp.ub = -t.getRowLower()[i];

            bterms[i] = tmp;
            for (int j = 0; j < non_zeros; j++) {
                elem tmp;
                tmp.index = indices[j];
                tmp.coeff = -val[j];
                matcoeff[i][j] = tmp;

            }

        } else {
            tmp.type = type;
            tmp.lb = t.getRowLower()[i];
            tmp.ub = t.getRowUpper()[i];
            bterms[i] = tmp;
            for (int j = 0; j < non_zeros; j++) {
                elem tmp;
                tmp.index = indices[j];
                tmp.coeff = val[j];
                matcoeff[i][j] = tmp;

            }

        }

    }
    std::cout << "G: " << G << "   E: " << E << "    L: " << L << std::endl;
}



ostream& operator<<(ostream& os, const BP_Output& out) {
    for (unsigned j = 0; j < out.in.getNvars(); j++) {
        os << j + 1 << " " << out.varAssignment[j] << std::endl;
    }

    return os;
}

