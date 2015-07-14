include ../../../make/make.local
## You may need to edit this path to gecode
#UNAME_S := $(shell uname -s)
#ifeq ($(UNAME_S),Linux)
#	GECODE_HOME=${HOME}/gecode/release-4.3.3
#else
#	ifeq ($(UNAME_S),Darwin)
#		GECODE_HOME=/opt/gecode/release
#  	endif
#endif
#COIN = ${HOME}/Coin/coin-CoinUtils
PROG=xyz
MAKE=make
LIBDIR=../lib
#GECODE_LINKS=-lgecodegist -lgecodeint -lgecodesearch -lgecodekernel -lgecodesupport -lgecodedriver -lgecodeminimodel -lgecodeset -lgecodefloat     Try without gist
GECODE_LINKS= -lgecodeint -lgecodesearch -lgecodekernel -lgecodesupport -lgecodedriver -lgecodeminimodel -lgecodeset -lgecodefloat

#COIN = ${HOME}/Coin/coin-CoinUtils
#BOOST = ${HOME}/boost/boost_1_57_0
#BOOST_LIB = ${HOME}/boost/boost_1_57_0/stage/lib
#CXXFLAGS = -std=c++11 -Wall #-O3 # -g
#EASYLOCALLIB = -lEasyLocal -lboost_program_options

#LDFLAGS = -L$(EASYLOCAL)/lib -L$(BOOST_LIB) -L../lib -L/usr/lib -L/usr/lib/i386-linux-gnu/ 
LDFLAGS = -L$(BOOST_LIB) -L../lib -L/usr/lib -L/usr/lib/i386-linux-gnu/ 
#LIBS = $(EASYLOCALLIB) 




LD=g++
CXX=g++

CXXFLAGS = -std=c++11 -g -Wextra -Wall  -Wfatal-errors # -DNDEBUG
CPPFLAGS = -I$(BOOST) -I../include -I$(COIN)/include -I${GECODE_HOME} 
#CPPFLAGS = -I$(EASYLOCAL)/include -I$(BOOST) -I../include -I$(COIN)/include -I${GECODE_HOME} 
LDFLAGS +=  -pthread -Wl,--rpath,$(GECODE_HOME) -Wl,--rpath,$(COIN)/lib -L${GECODE_HOME} -L$(COIN)/lib#-Wl, --rpath,$(LIBDIR)
LIBS += ${GECODE_LINKS} -lCoinUtils -lz -llapack -lblas -lm -lbz2 

#SOURCE_FILES = main.cpp BP_Data.cpp LSSpace.cpp GeneralSolver.cpp ../include/constraints/linear.cpp Sum.cpp #../include/constraints/invariant.cpp #IntegerVariable.cpp
# BP_Basics.cpp BP_Helpers.cpp 
# ../include/constraints/Equal.cpp ../include/constraints/LinearComb.cpp ../include/constraints/Constraint.cpp ../include/constraints/ConstraintSystem.cpp
OBJECT_FILES = main.o BP_Data.o LSSpace.o GeneralSolver.o  IntegerVariable.o Sum.o Test.o State.o NeighborhoodExplorer.o Random.o Clock.o #Move.o #FlipMove.o ValueChangeMove.o #linear.o #Invariant.o
# BP_Basics.o BP_Helpers.o 
#../include/constraints/Equal.o ../include/constraints/LinearComb.o ../include/constraints/Constraint.o ../include/constraints/ConstraintSystem.o
#HEADER_FILES = BP_Data.hpp Constants.hpp LSSpace.hpp GeneralSolver.hpp ../include/constraints/Sum.hpp ../include/constraints/linear.hpp IntegerVariable.hpp invariant.hpp
#BP_Basics.hpp BP_Helpers.hpp 
#../include/constraints/Equal.hpp ../include/constraints/LinearComb.hpp ../include/constraints/Constraint.hpp ../include/constraints/ConstraintSystem.hpp



xyz: $(OBJECT_FILES)
	$(LD) $(LDFLAGS) $(OBJECT_FILES) $(LIBS) -o ${PROG}
	
${LIBDIR}/libmps.a:
	cd ${LIBDIR}; ${MAKE} libmps.a	
	

%.o: %.cpp 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS)  -c $*.cpp


clean:
	rm -fr ${PROG} *.o

verifier:
	g++ -std=c++11 -Wfatal-errors verifier.cc -o verify

scip:
	g++ -c -Wfatal-errors reader_mps.c 


# build tests
build-tests: .build-tests-post

.build-tests-pre:
# Add your pre 'build-tests' code here...

.build-tests-post: .build-tests-impl
# Add your post 'build-tests' code here...


# run tests
test: .test-post

.test-pre:
# Add your pre 'test' code here...

.test-post: .test-impl
# Add your post 'test' code here...
