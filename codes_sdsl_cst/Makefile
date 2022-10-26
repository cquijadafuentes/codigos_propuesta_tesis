CPP=g++

PREFLAGS=-std=c++11 -g -O0 -DNDEBUG -I ~/include -L ~/lib

POSFLAGS=-lsdsl -ldivsufsort -ldivsufsort64

BINS=TopoRel_Naive_test TopoRel_GST_test\
		time_TopoRel_boolean TopoRel_Naive_PreComp_test\
		TopoRel_testall

OBJECTS=TopoRel_Naive.o TopoRel_GST_0.o TopoRel_GST_1.o TopoRel_Naive_PreComp.o

%.o: %.cpp
	$(CPP) $(PREFLAGS) -c $< -o $@ $(POSFLAGS)

all: clean bin

bin: $(OBJECTS) $(BINS)

test_csa:
	$(CPP) $(PREFLAGS) test_csa.cpp -o test_csa $(POSFLAGS)

test_cst:
	$(CPP) $(PREFLAGS) test_cst.cpp -o test_cst $(POSFLAGS)

my_test_cst:
	$(CPP) $(PREFLAGS) my_test_cst.cpp -o my_test_cst $(POSFLAGS)

pc:
	$(CPP) $(PREFLAGS) pc.cpp -o pc $(POSFLAGS)

rt_lcs:
	$(CPP) rt_lcs.cpp -o rt_lcs

TopoRel_Naive_test:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_test TopoRel_Naive_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_test TopoRel_GST_test.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_boolean:
	$(CPP) $(PREFLAGS) -o time_TopoRel_boolean time_TopoRel_boolean.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_test:	
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_PreComp_test TopoRel_Naive_PreComp_test.cpp $(OBJECTS) $(POSFLAGS)
	
TopoRel_testall:	
	$(CPP) $(PREFLAGS) -o TopoRel_testall TopoRel_testall.cpp $(OBJECTS) $(POSFLAGS)

clean:
	rm -f $(BINS) $(OBJECTS)