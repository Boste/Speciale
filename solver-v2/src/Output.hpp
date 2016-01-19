#ifndef OUTPUT_HPP
#define	OUTPUT_HPP
using namespace std;

class Output {
public:
    Output() {
        
    }
    ~Output(){
        
    }
    void setName(std::string name) {
        this->name = name;
        gecodePrint += name + " ";
        searchPrint += name + " ";
        LSPrint += name + " ";
        table1 += name + " ";
        table2 += name + " ";
    }

    void addToGecodePrint(string str) {
        gecodePrint += str + " ";
    }

    void addToSearchPrint(string str) {
        searchPrint += str += " ";
    }

    void addToLSPrint(string str) {
        LSPrint += str + " ";
    }

    void addToTable1(string str) {
        table1 += str + " ";
    }

    void addToTable2(string str) {
        table2 += str + " ";
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
private:
    string name;
    string gecodePrint;
    string searchPrint;
    string LSPrint;
    string table1;
    string table2;

};


#endif	/* OUTPUT_HPP */

