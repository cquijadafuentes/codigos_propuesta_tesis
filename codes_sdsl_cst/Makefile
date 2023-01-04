CPP=g++

PREFLAGS=-std=c++11 -g -O0 -DNDEBUG -I ~/include -L ~/lib

POSFLAGS=-lsdsl -ldivsufsort -ldivsufsort64

BINS=test_cst_data test_cst_rep TopoRel_Naive_test  TopoRel_Naive_PreComp_test\
		time_TopoRel_boolean size_TopoRel TopoRel_testall\
		TopoRel_GST_0_test TopoRel_GST_1_test TopoRel_GST_2_test\
		TopoRel_GST_4_test

OBJECTS=TopoRel_GST_0.o TopoRel_GST_1.o\
		TopoRel_GST_2.o TopoRel_GST_4.o\
		TopoRel_Naive.o TopoRel_Naive_PreComp.o

%.o: %.cpp
	$(CPP) $(PREFLAGS) -c $< -o $@ $(POSFLAGS)

#all: clean bin
all: bin

bin: $(OBJECTS) $(BINS)

test_cst_data:
	$(CPP) $(PREFLAGS) -o test_cst_data test_cst_data.cpp $(POSFLAGS)

test_cst_rep:
	$(CPP) $(PREFLAGS) -o test_cst_rep test_cst_rep.cpp $(POSFLAGS)

TopoRel_Naive_test:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_test TopoRel_Naive_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_0_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_0_test TopoRel_GST_0_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_1_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_1_test TopoRel_GST_1_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_2_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_2_test TopoRel_GST_2_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_4_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_4_test TopoRel_GST_4_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_test:	
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_PreComp_test TopoRel_Naive_PreComp_test.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_boolean:
	$(CPP) $(PREFLAGS) -o time_TopoRel_boolean time_TopoRel_boolean.cpp $(OBJECTS) $(POSFLAGS)

size_TopoRel:
	$(CPP) $(PREFLAGS) -o size_TopoRel size_TopoRel.cpp $(OBJECTS) $(POSFLAGS)
	
TopoRel_testall:	
	$(CPP) $(PREFLAGS) -o TopoRel_testall TopoRel_testall.cpp $(OBJECTS) $(POSFLAGS)

clean:
	rm -f $(BINS) $(OBJECTS)