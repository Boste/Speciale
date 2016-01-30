#ifndef OUTPUT_HPP
#define	OUTPUT_HPP
using namespace std;

class Output {
private:
    string gecodePrint = "";
    string searchPrint = "";
    string LSPrint = "";
    string table1 = "";
    string table2 = "";

public:
    string name = "";

    Output() {

    }

    ~Output() {

    }

    void setName(std::string name) {
        this->name.append(name).append(" ");
        gecodePrint.append(name).append(" ");
        searchPrint.append(name).append(" ");
        LSPrint.append(name).append(" ");
        table1.append(name).append(" ");
        table2.append(name).append(" ");
    }

    void addToGecodePrint(string str) {
        gecodePrint.append(str).append(" ");
    }

    void addToSearchPrint(string str) {
        searchPrint.append(str).append(" ");
    }

    void addToLSPrint(string str) {
        LSPrint.append(str).append(" ");
    }

    void addToTable1(string str) {
        table1.append(str).append(" ");
    }

    void addToTable2(string str) {
        table2.append(str).append(" ");
    }

    string getToGecodePrint() {
        return gecodePrint;
    }

    string getToSearchPrint() {
        return searchPrint;
    }

    string getToLSPrint() {
        return LSPrint;
    }

    string getToTable1() {
        return table1;
    }

    string getToTable2() {
        return table2;
    }
    bool feasible = false;
    vector<int> evalfunc;
    vector<int> violdegree;
    vector<double> time;
    vector<unsigned> iteration;
    unsigned relax;
    unsigned relaxp;
    double feasibleTime;
    coefType feasibleVal;
    double solTime;
    unsigned oneway =0;
};

#endif	/* OUTPUT_HPP */

