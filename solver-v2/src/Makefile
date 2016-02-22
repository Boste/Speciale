include ../../../make/make.local




## set path to gecode
#UNAME_S := $(shell uname -s)
#ifeq ($(UNAME_S),Linux)
#	GECODE_HOME=${HOME}/gecode/release-4.3.3
#else
#	ifeq ($(UNAME_S),Darwin)
#		GECODE_HOME=/opt/gecode/release
#  	endif
#endif


# Set path to Coin
#COIN = ${HOME}/Coin/coin-CoinUtils


PROG=xyz
MAKE=make
LIBDIR=../lib
GECODE_LINKS= -lgecodeint -lgecodesearch -lgecodekernel -lgecodesupport 

LDFLAGS = -L../lib -L/usr/lib -L/usr/lib/i386-linux-gnu/ 


LD=g++
CXXFLAGS = -std=c++14 -O3 -Wextra -Wall  -Wfatal-errors # -DNDEBUG
CPPFLAGS = -I../include -I$(COIN)/include -I${GECODE_HOME} 
LDFLAGS +=   -pthread -Wl,--rpath,$(GECODE_HOME) -Wl,--rpath,$(COIN)/lib -L${GECODE_HOME} -L$(COIN)/lib  #-L/usr/lib/gcc/i686-linux-gnu/4.9#-Wl, --rpath,$(LIBDIR)
LIBS += ${GECODE_LINKS} -lCoinUtils -lz -llapack -lblas -lm -lbz2  #-lgomp

OBJECT_FILES = main.o BP_Data.o LocalSearchEngine.o GPSolver.o  Sum.o Storage.o State.o Neighborhood.o FlipNeighborhood.o \
Random.o Clock.o Multistop.o GecodeEngine.o Max.o DependencyDigraph.o BestImprovement.o FirstImprovement.o RandomWalk.o  \
LEQviolation.o EQviolation.o TabuSearch.o RandomConflictConNE.o EvalFlipNE.o RestrictedFlipNE.o ConflictOnlyNE.o  # Test.o Move.o #FlipMove.o ValueChangeMove.o #linear.o #Invariant.o Flip2Neighborhood.o

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
