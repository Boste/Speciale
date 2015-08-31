    #include "Sum.hpp"
Sum::Sum(std::vector<IntegerVariable*>* vars, std::vector<int>& c) : Invariant() {
    type=SUM;
//    if(c->size()!=vars->size()){
//        std::cout << c->size() << " " <<vars->size() << std::endl;
//    }
    assert(c.size()==vars->size());
    for (unsigned i = 0; i < vars->size(); i++) {
        //        coefficients.insert(std::make_pair(vars->at(i)->getID(), c->at(i)));
        coefficients[vars->at(i)->getID()] = c.at(i);
    }
    VariablePointers = vars;
    //        std::cout << std::endl;
}

/// Construct that copies a Coefficient map (Only used when relaxing)
Sum::Sum(std::vector<IntegerVariable*>* vars, std::unordered_map<int,int>& map) : Invariant() {//:IntVariables(vars),coefficients(c) {
    type=SUM;
    
    assert(map.size()==vars->size());
    coefficients = map;
    VariablePointers = vars;
    //        std::cout << std::endl;
}

Sum::Sum(const Sum &a) : Invariant(a) {
    //        std::cout << "copy constructor2" << std::endl;
    this->coefficients = a.coefficients;
    this->VariableChange = a.VariableChange;
    //    this->VariablePointers = a.VariablePointers;
    //        sleep(5000);
}

Sum::~Sum() {
    //    delete &coefficients;
    //    delete &VariableChange;
    //    delete &VariablePointers;
}

Sum& Sum::operator=(const Sum &a) {
    if (this != &a) {
        this->coefficients = a.coefficients;

//        std::cout << "operator =" << std::endl;
//        sleep(5);
        //        this->VariablePointers = a.VariablePointers;

        this->VariableChange = a.VariableChange;
    }
    return *this;
}

int Sum::calculateDeltaValue() {
    int valueChange = 0;
    while (!VariableChange.empty()) {
        std::pair<int, int> currentPair = VariableChange.back();
        valueChange += coefficients.at(currentPair.first) * currentPair.second;

        VariableChange.pop_back();
    }
    //        std::cout << valueChange << " ";
    DeltaValue = valueChange;
    return valueChange;
}

void Sum::addChange(int variableNumber, int changeInValue) {
    VariableChange.push_back(std::pair<int, int> (variableNumber, changeInValue));
    //        std::cout << "addChange in sum" << std::endl;
    //        sleep(5000);
}

//void Sum::setUsedByConstraint(int constraint) {
//    usedInConstraintNr = constraint;
//}
//
//void Sum::usedByObjective(int objective) {
//    usedInObjectiveNr = objective;
//
//
//}

//void Sum::addChange(std::vector<int> variableNumbers, std::vector<int> changes) {
//    std::cout << "Ikke implementeret" << std::endl;
//}
//
//void Sum::addChangeSwap(int variableNumber1, int variableNumber2) {
//    std::cout << "Ikke implementeret" << std::endl;
//
//}

bool Sum::test() {
//    double testValue = 0;

    //    for (unsigned i = 0; i < VariablePointers.size(); i++) {
    //        testValue += VariablePointers.at(i)->getCurrentValue() * coefficients.at(VariablePointers.at(i)->getID());
    //        testValue += VariablePointers.at(i)->getCurrentValue() * 1;
    //        std::cout << "Variable " << VariablePointers.at(i)->getID() << " val " << VariablePointers.at(i)->getCurrentValue() << " coef " << coefficientsBUG.at(VariablePointers.at(i)->getID()) << std::endl;
    //        std::cout << &coefficientsBUG.at(VariablePointers.at(i)->getID()) << std::endl;
    //    }
//    for (auto it = coefficients.begin(); it != coefficients.end(); ++it)
        
//        std::cout << " " << it->first << ":" << it->second;
            //    std::cout << "testValue: " << testValue << std::endl;

            //    sleep(5);
//    if (testValue != CurrentValue) {
//        std::cout << "Failed test in sum" << std::endl;
//        std::cout << "testValue: " << testValue << "  CurrentValue: " << CurrentValue << std::endl;
//        return false;
//
//    }
    return true;
}