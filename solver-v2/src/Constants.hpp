#ifndef CONSTANTS_HH
#define CONSTANTS_HH 
#
#include <memory>
#include <vector>
#include <queue>
#include <set>
#include <iostream>

//#include "Invariant.hpp"
//#include <boost/heap/fibonacci_heap.hpp>
//#include <boost/heap/binomial_heap.hpp>
class Constraint;
class Invariant;
class IntegerVariable;
//struct compare_invariant;
//#include "Invariant.hpp"
// Old stuff not needed
#define nonTerm 0.000000000001
#define weight 100
#define tabulistLB 5
#define tabulistUB 20
#define maxIter 200000000
#define iniTrails 20
#define maxMoves 600

// General/Random things
#define RANDOMSEED 1337
#define PRINT TRUE
#define debug std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << std::endl; sleep(1);

// Relations
#define EQ 0
#define LQ 1
#define GQ 3
#define GR 4
#define LE 2

// Constraint priority
#define OBJ 0
#define HARD 1
#define SOFT 2


// Move types
#define FLIP 1
#define SWAP 2
#define VALUECHANGE 3

// Constraints
#define LINEAR 1

// Invariants
#define SUM 1
#define MAX 2


//  Variables 
//#define propagationQueue boost::heap::fibonacci_heap<>







// Return types
typedef double coefType;
typedef std::shared_ptr<Constraint> constraint;
typedef std::vector<IntegerVariable*> variableContainer;
typedef std::vector<constraint> VariableInConstraints;
typedef std::shared_ptr<Invariant> invariant;
//typedef std::shared_ptr<int> invariant;
//typedef std::shared_ptr<Invariant> invariant;
typedef std::vector<invariant> InvariantContainer;
//typedef InvariantContainer updateVector;

//struct compare_invariant : public std::binary_function<invariant, invariant, bool> {
//
//    bool operator()(const invariant invar1, const invariant invar2) const {
//struct compare_invariant : public std::binary_function<Invariant*, Invariant*, bool> {
//
//   
//
//    bool operator<(const Invariant& invar) const {
//        std::cout << "used to sort1 <" << std::endl;
////        debug;
//        return (this->operator <(invar));
//    }
////
//    bool operator>(const Invariant& invar) const {
//        std::cout << "used to sort2 >" << std::endl;
////        debug;
//        return (this->operator >(invar));
//    }
//    bool operator<(const invariant invar) const {
//        std::cout << "used to sort1 <" << std::endl;
////        debug;
//        return (this->operator <(invar));
//    }
////
//    bool operator>(const invariant invar) const {
//        std::cout << "used to sort2 >" << std::endl;
////        debug;
//        return (this->operator >(invar));
//    }
//     bool operator()(const Invariant* invar1, const Invariant* invar2) const {
////        std::cout << "compare " << (*invar1 < *invar2) << std::endl;
//        
//        return (*invar1 < *invar2);
//    }
//};
//
//
//typedef std::set<Invariant*, compare_invariant> propagation_queue;
//typedef std::set<Invariant*, compare_invariant> updateVector;
//typedef std::set<invariant, compare_invariant> propagation_queue;
//typedef std::set<invariant, compare_invariant> updateVector;

//typedef boost::heap::binomial_heap<invariant, boost::heap::compare<compare_invariant>> propagation_queue;
//typedef boost::heap::binomial_heap<invariant, boost::heap::compare<compare_invariant>> propagation_queue;

//typedef boost::heap::binomial_heap<invariant, boost::heap::compare<compare_invariant>> updateVector;
//typedef std::priority_queue<invariant,std::vector<invariant>,compare_invariant> updateVector;

//typedef std::vector<InvariantContainer> updateVector;
//typedef Invariant* updateType;
typedef invariant updateType;
typedef std::shared_ptr<std::vector<constraint>> constraintContainer;
typedef std::vector<constraintContainer> allConstraints;


#endif