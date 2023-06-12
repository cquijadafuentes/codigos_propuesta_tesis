CPP=g++

PREFLAGS=-std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib

POSFLAGS=-lsdsl -ldivsufsort -ldivsufsort64

BINS=time_allContained_byQueriesFile\
		time_allIntersectPPk_byQueriesFile\
		time_allTopoRels_byQueriesFile\
		TopoRel_GST_allContained\
		TopoRel_GST_allIntersectPP\
		TopoRel_GST_build\
		TopoRel_GST_check\
		TopoRel_GST_size\
		TopoRel_GST_test\
		TopoRel_GST_test_allContained_byQueries\
		TopoRel_GST_time\
		TopoRel_GST_time_allContained\
		TopoRel_Naive_allContained_byQueries\
		TopoRel_Naive_allIntersect_byQueries\
		TopoRel_Naive_allIntersectPP_byQueries\
		TopoRel_Naive_allRelation_byQueries\
		TopoRel_Naive_time\
		TopoRel_Naive_size\
		TopoRel_Naive_test\
		TopoRel_Naive_compareTime_allContained

OBJECTS=TopoRel_GST.o\
		TopoRel_Naive.o

%.o: %.cpp
	$(CPP) $(PREFLAGS) -c $< -o $@ $(POSFLAGS)

#all: clean bin
all: bin

bin: $(OBJECTS) $(BINS)

time_allContained_byQueriesFile:
	$(CPP) $(PREFLAGS) -o time_allContained_byQueriesFile time_allContained_byQueriesFile.cpp $(OBJECTS) $(POSFLAGS)

time_allIntersectPPk_byQueriesFile:
	$(CPP) $(PREFLAGS) -o time_allIntersectPPk_byQueriesFile time_allIntersectPPk_byQueriesFile.cpp $(OBJECTS) $(POSFLAGS)

time_allTopoRels_byQueriesFile:
	$(CPP) $(PREFLAGS) -o time_allTopoRels_byQueriesFile time_allTopoRels_byQueriesFile.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_allContained:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_allContained TopoRel_GST_allContained.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_allIntersectPP:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_allIntersectPP TopoRel_GST_allIntersectPP.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_time:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_time TopoRel_GST_time.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_build:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_build TopoRel_GST_build.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_check:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_check TopoRel_GST_check.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_test TopoRel_GST_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_size:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_size TopoRel_GST_size.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_test_allContained_byQueries:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_test_allContained_byQueries TopoRel_GST_test_allContained_byQueries.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_time_allContained:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_time_allContained TopoRel_GST_time_allContained.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_allContained_byQueries:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_allContained_byQueries TopoRel_Naive_allContained_byQueries.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_allIntersect_byQueries:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_allIntersect_byQueries TopoRel_Naive_allIntersect_byQueries.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_allIntersectPP_byQueries:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_allIntersectPP_byQueries TopoRel_Naive_allIntersectPP_byQueries.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_allRelation_byQueries:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_allRelation_byQueries TopoRel_Naive_allRelation_byQueries.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_time:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_time TopoRel_Naive_time.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_size:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_size TopoRel_Naive_size.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_test:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_test TopoRel_Naive_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_compareTime_allContained:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_compareTime_allContained TopoRel_Naive_compareTime_allContained.cpp $(OBJECTS) $(POSFLAGS)

clean:
	rm -f $(BINS) $(OBJECTS)