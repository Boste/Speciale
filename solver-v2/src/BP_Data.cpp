// File BP_Data.cc
#include "BP_Data.hpp"



// Turn off compiler warning about long names.
//#pragma warning(disable:4786)

// #include "readwrite.h"

BP_Input::BP_Input(string file_name) {
    // Insert the code that reads the input from the file
    // and stored it into the data structures of the object
    //read_mps(file_name.c_str());
    
    

    CoinMpsIO t;
    
//    std::cout << "Can coin read?" << std::endl;
    t.readMps(file_name.c_str());
//    std::cout << "Coin have read" << std::endl;
    ncons = t.getNumRows();
    nvars = t.getNumCols();
    vars.resize(nvars);
    matcoeff.resize(ncons);
    bterms.resize(ncons);
    matcoeff2.resize(nvars);
    assert(ncons > 0);
    assert(nvars > 0);
//    cons.resize(ncons);

    
//    int index[nvars];
//    double coeff[nvars];
    
    //    nbinvars = t.getNumElements();
    //   
    //    nintvars = solver.getNumIntegers();
    //    std::cout << nbinvars << " " << nintvars << " " << nvars <<  std::endl;
    //    assert(nbinvars == nvars);
    
    
    
    //    direction = t.getObjSense();
    
    //    if (direction == 1 || direction ==0)
    //        cout << "WARNING: objective is maximization!" << endl;
//    std::cout << "start read" << std::endl;
    
    for (int j = 0; j < nvars; j++) {
        var tmp;
        tmp.objcoeff  = t.getObjCoefficients()[j];
        
        // Hvad var det var type blev brugt til? Kun til udskrift?
        //        char* t = solver;
        //        if(t=='G'){
        //            tmp.type = 2;
        //        } else if (t == 'E'){
        //            tmp.type =5;
        //        } else {
        //            tmp.type =3;
        //        }
        
        tmp.lb = t.getColLower()[j];
        tmp.ub = t.getColUpper()[j];
        vars[j] = tmp;
    }
    
    
//    std::cout << "before matrix" << std::endl;
    const CoinPackedMatrix* matrix = t.getMatrixByRow();
//    std::cout << "after matrix" << std::endl;
    int G = 0;
    int E = 0;
    int L = 0;
    for (int i = 0; i < ncons; i++) {
        const CoinShallowPackedVector vec = matrix->getVector(i);
//        Constraint co(ele,ind);
//        cons[i] = co;
        
        const double* val = vec.getElements();
        int non_zeros = vec.getNumElements();
        const int* indices =  vec.getIndices();
        //        int non_zeros = glp_get_mat_row(lp, i, index, coeff);
        //        int non_zeros = solver.get
        
        
        matcoeff[i].resize(non_zeros);
        bounds tmp;
        
        char c = t.getRowSense()[i];
        
        int type;
        if(c=='G'){
            type = 2;
            G++;
        } else if (c == 'E'){
            type =5;
            E++;
        } else {
            type =3;
            L++;
        }
        
        //        int type = glp_get_row_type(lp, i);
        if(type == 2){
            tmp.type = 3;
            tmp.lb = -t.getRowUpper()[i];
            //            tmp.lb = -glp_get_row_ub(lp, i);
            tmp.ub = -t.getRowLower()[i];
            //            tmp.ub = -glp_get_row_lb(lp, i);
            
            bterms[i] = tmp;
//            double rhs = tmp.lb;
//            LinearGreat con(i, type, rhs, ind, ele);
//            cons.push_back(&con);
            for (int j = 0; j < non_zeros; j++) {
                elem tmp;
                tmp.index = indices[j];
                tmp.coeff = -val[j];
                matcoeff[i][j] = tmp;
                
            }
            
        } else {
            tmp.type = type;
            tmp.lb = t.getRowLower()[i];
            //            tmp.lb = glp_get_row_lb(lp, i);
            tmp.ub = t.getRowUpper()[i];
            //            tmp.ub = glp_get_row_ub(lp, i);
            bterms[i] = tmp;
//            double rhs = tmp.lb;
            if(type == 5){
                
//                Equal con = Equal(matcoeff[i],tmp,1);
//                cons.push_back(&con);
                
            } else {
//                LinearGreat con(i, type, rhs,ind,ele);
//                cons.push_back(&con);
              
            }
            for (int j = 0; j < non_zeros; j++) {
                elem tmp;
                tmp.index = indices[j];
                tmp.coeff = val[j];
                matcoeff[i][j] = tmp;
                
            }

        }
        
        // Lav constraints her
        
        
        
        
    }
    const CoinPackedMatrix* matrix2 = t.getMatrixByCol();
    
    for (int i = 0; i < nvars; i++) {
        
        const CoinShallowPackedVector vec = matrix2->getVector(i);
        int non_zeros = vec.getNumElements();
        const double* val = vec.getElements();
        

        const int* indices = vec.getIndices();
        matcoeff2[i].resize(non_zeros);
 
        for (int j = 0; j < non_zeros; j++) {
            
            char c = t.getRowSense()[indices[j]];

            int type;
            if(c=='E'){
                type = 5;
            }
            else if (c=='L'){
                type = 3;
                        
            } else {
                type =2;
            }
            
            elem tmp;
            tmp.index = indices[j];
            
            if(type==3){
                
                tmp.coeff = -val[j];
            } else {
                tmp.coeff = val[j];
            }
            
            matcoeff2[i][j] = tmp;
            
            
        }
    }
    std::cout << "G: " << G << "   E: " << E << "    L: " << L << std::endl;
//    for(int i =0; i< cons.size();i++){
//        Constraint* c = cons[i];
//        
//    }
}
    

//BP_Constraints::BP_Constraints(const double& my_value, const double& my_indicies) :
//               value(my_value),indicies(my_indicies) {
//    
//}
//const char * getStringKind(int kind) {
//    switch (kind) {
//	case GLP_CV:
//            return "Continuous";
//	case GLP_IV:
//            return "Integer";
//	case GLP_BV:
//            return "Binary";
//	default:
//            return "";
//    }
//}

//ostream& operator<<(ostream& os, const BP_Input& pa) {
//    // Insert the code that write the input object (needed for debugging purposes)
//    
//     printf("obj: %s", pa.getDirection() == GLP_MAX ? "maximize " : "minimize ");
//    for (unsigned j = 1; j <= pa.getNvars(); j++) {
//        var tmp = pa.getVars(j - 1);
//        
//        if (tmp.objcoeff >= 0)
//            printf("+");
//        printf("%gx[%d]", tmp.objcoeff, j); //glp_get_obj_coef(lp, j));
//    }
//    puts("");
//    for (unsigned i = 1; i <= pa.getNcons(); i++) {
//        bounds tmp = pa.getBterms(i - 1);
//        const vector<elem> v = pa.getMatcoeff(i - 1);
//        
//        
//        int non_zeros = v.size();
//        //		printf("cons%05d: ", i);
//        for (int j = 1; j <= non_zeros; j++) {
//            //printf("%c%gx[%d]", coeff[j] < 0 ? '+' : '-', coeff[j], index[j]);
//            if (v[j - 1].coeff >= 0)
//                printf("+");
//            printf("%gx[%d]", v[j - 1].coeff, v[j - 1].index);
//        }
//        //puts("");
//        switch (tmp.type) {
//            case GLP_FR:
//                break;
//            case GLP_LO:
//                printf(">=%g\n", tmp.lb);
//                break;
//            case GLP_UP:
//                printf("<=%g\n", tmp.ub);
//                break;
//            case GLP_DB:
//                printf("/ %g <= c <= %g\n", tmp.lb, tmp.ub);
//                break;
//            case GLP_FX:
//                printf("=%g\n", tmp.ub);
//                break;
//        }
//    }
//    for (unsigned j = 1; j <= pa.getNvars(); j++) {
//        var tmp = pa.getVars(j - 1);
//        switch (tmp.type) {
//            case GLP_FR:
//                break;
//            case GLP_LO:
//                printf("%g<= x[%d] (<=%g) kind: %s\n", tmp.lb, j, tmp.ub, getStringKind(tmp.kind));
//                break;
//            case GLP_UP:
//                printf("(%g<=) x[%d] <=%g kind: %s\n", tmp.lb, j, tmp.ub, getStringKind(tmp.kind));
//                break;
//            case GLP_DB:
//                printf("%g<= x[%d] <=%g kind: %s\n", tmp.lb, j, tmp.ub, getStringKind(tmp.kind));
//                break;
//            case GLP_FX:
//                printf("x[%d]=%g %g  kind: %s\n", j, tmp.ub, tmp.lb, getStringKind(tmp.kind));
//                break;
//        }
//    }
//    return os;
//}

BP_Output::BP_Output(const BP_Input& my_in) :
		in(my_in), varAssignment(in.getNvars()) {
    // Insert the code that initialize the data structures of the
    // output object based in the input object
    
    
    
}
void BP_Output::assign(int var, bool b){
    varAssignment[var]=b;
//    std::cout << "var " << var << "   " << b << std::endl;
}

BP_Output& BP_Output::operator=(const BP_Output& out) {
    // Insert the code that copies all data structures of the
    // output object from the ones of the parameter out
    // (excluding the reference to the input object, that is constant)
    varAssignment = out.varAssignment;
    
    return *this;
}


ostream& operator<<(ostream& os, const BP_Output& out) {
    // Insert the code that writes the output object
    
//    os << "v ";
    for (unsigned j = 0; j < out.in.getNvars(); j++){
        os << j+1 << " " << out.varAssignment[j] << std::endl;
//        if(out.varAssignment[j]){
            
//            os << j+1 << " ";
//        }
    }
    
//    os << "0";
    return os;
}

istream& operator>>(istream& is, BP_Output& out) {
    // Insert the code that reads the output object
    
    int r;
    char ch;
    is >> ch;
    for (unsigned j = 0; j < out.in.getNvars(); j++)
    {
        is >> r >> ch;
        cerr << j << " " << r << " " << ch << endl;
        out.assign(j,r);
    }
    return is;
}
