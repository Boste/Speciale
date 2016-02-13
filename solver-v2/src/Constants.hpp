#ifndef CONSTANTS_HH
#define CONSTANTS_HH 
#
#include <memory>
#include <vector>
#include <queue>
#include <set>
#include <iostream>
#include <thread>
#include <unistd.h>
class Constraint;
class Invariant;
class Variable;
// General/Random things
#define RANDOMSEED 1337
#define PRINT TRUE
#define debug std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << std::endl;  sleep(1);

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
#define LEQVIO 3
#define EQVIO 4


typedef int coefType;
typedef std::shared_ptr<Constraint> constraint;
typedef std::vector<Variable*> variableContainer;
typedef std::vector<constraint> VariableInConstraints;
typedef Invariant* invariant;
typedef std::vector<invariant> InvariantContainer;
typedef invariant updateType;
typedef std::shared_ptr<std::vector<constraint>> constraintContainer;
typedef std::vector<constraintContainer> allConstraints;

#endif