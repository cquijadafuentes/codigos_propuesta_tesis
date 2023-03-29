CPP=g++

PREFLAGS=-std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib

POSFLAGS=-lsdsl -ldivsufsort -ldivsufsort64

BINS=TopoRel_GST_6_size\
		TopoRel_GST_6_allContained TopoRel_GST_6_test_allContained_byQueries\
		TopoRel_GST_6_build TopoRel_GST_6_test\
		TopoRel_Naive_test_allContained_byQueries\
		time_TopoRel_allContained_vsGST6_byQueriesFile\
		TopoRel_Naive_test  TopoRel_Naive_PreComp_test\
		TopoRel_Naive_PreComp_save-load-test\
		TopoRel_Naive_PreComp_save TopoRel_Naive_PreComp_load\

OBJECTS=TopoRel_GST_6.o\
		TopoRel_Naive.o TopoRel_Naive_PreComp.o

%.o: %.cpp
	$(CPP) $(PREFLAGS) -c $< -o $@ $(POSFLAGS)

#all: clean bin
all: bin

bin: $(OBJECTS) $(BINS)

time_TopoRel_allContained_vsGST6_byQueriesFile:
	$(CPP) $(PREFLAGS) -o time_TopoRel_allContained_vsGST6_byQueriesFile time_TopoRel_allContained_vsGST6_byQueriesFile.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_test_allContained_byQueries:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_test_allContained_byQueries TopoRel_Naive_test_allContained_byQueries.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_6_allContained:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_6_allContained TopoRel_GST_6_allContained.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_6_build:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_6_build TopoRel_GST_6_build.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_6_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_6_test TopoRel_GST_6_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_6_size:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_6_size TopoRel_GST_6_size.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_6_test_allContained_byQueries:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_6_test_allContained_byQueries TopoRel_GST_6_test_allContained_byQueries.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_load:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_PreComp_load TopoRel_Naive_PreComp_load.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_save:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_PreComp_save TopoRel_Naive_PreComp_save.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_test:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_PreComp_test TopoRel_Naive_PreComp_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_save-load-test:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_PreComp_save-load-test TopoRel_Naive_PreComp_save-load-test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_test:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_test TopoRel_Naive_test.cpp $(OBJECTS) $(POSFLAGS)

clean:
	rm -f $(BINS) $(OBJECTS)