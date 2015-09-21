#ifndef CONSTANTS_HH
#define CONSTANTS_HH 
#include <memory>
#include <vector>
class Constraint;
class Invariant;
class IntegerVariable;
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
#define debug std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << std::endl;

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

// Return types
typedef double coefType;
typedef std::shared_ptr<Constraint> constraint;
typedef std::vector<IntegerVariable*> variableContainer;
typedef std::vector<constraint> VariableInConstraints;
typedef std::shared_ptr<Invariant> invariant;
typedef std::vector<invariant> InvariantContainer;
typedef InvariantContainer updateVector;
//typedef std::vector<InvariantContainer> updateVector;
typedef std::shared_ptr<Invariant> updateType;
typedef std::shared_ptr<std::vector<constraint>> constraintContainer;
typedef std::vector<constraintContainer> allConstraints;


#endif