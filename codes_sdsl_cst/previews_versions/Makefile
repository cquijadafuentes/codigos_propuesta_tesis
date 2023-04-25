CPP=g++

PREFLAGS=-std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib

POSFLAGS=-lsdsl -ldivsufsort -ldivsufsort64

BINS=size_TopoRel\
	test_comparaGST4constructores\
	test_cst_data\
	test_cst_rep\
	test_sdvector_rank_select\
	test_TopoRel_all_contain\
	test_TopoRel_all_contained\
	test_TopoRel_all_contained_howManyLCP\
	test_TopoRel_all_equal\
	time_allTopoRels_byQueriesFile\
	time_GST45_constructores\
	time_TopoRel_all_contain_GST5\
	time_TopoRel_allContain_GST5_xLote\
	time_TopoRel_allContained_naive_gst_byQueriesFile\
	time_TopoRel_allContained_versionesGST5\
	time_TopoRel_allContained_vsGST5_byQueriesFile\
	time_TopoRel_allContained_vsGST6_byQueriesFile\
	time_TopoRel_boolean\
	time_TopoRel_groupRelations\
	TopoRel_GST_0_test\
	TopoRel_GST_1_test\
	TopoRel_GST_2_test\
	TopoRel_GST_4_test\
	TopoRel_GST_5_load\
	TopoRel_GST_5_save\
	TopoRel_GST_5_save-load-test\
	TopoRel_GST_5_size\
	TopoRel_GST_5_test\
	TopoRel_GST_5_test_allContained_byQueries\
	TopoRel_GST_6_allContained\
	TopoRel_GST_6_build\
	TopoRel_GST_6_check_allContained\
	TopoRel_GST_6_size\
	TopoRel_GST_6_test\
	TopoRel_GST_6_test_allContained_byQueries\
	TopoRel_GST_6_test_allIntersect_byQueries\
	TopoRel_GST_6_time_allContained\
	TopoRel_Naive_allContained_byQueries\
	TopoRel_Naive_allIntersect_byQueries\
	TopoRel_Naive_PreComp_build\
	TopoRel_Naive_PreComp_load\
	TopoRel_Naive_PreComp_save\
	TopoRel_Naive_PreComp_save-load-test\
	TopoRel_Naive_PreComp_test\
	TopoRel_Naive_test\
	TopoRel_Naive_test_allContained_byQueries\
	TopoRel_Naive_time_allContained\
	TopoRel_testall

OBJECTS=TopoRel_GST_0.o TopoRel_GST_1.o\
		TopoRel_GST_2.o TopoRel_GST_4.o\
		TopoRel_GST_5.o TopoRel_GST_6.o\
		TopoRel_Naive.o TopoRel_Naive_PreComp.o

%.o: %.cpp
	$(CPP) $(PREFLAGS) -c $< -o $@ $(POSFLAGS)

#all: clean bin
all: bin

bin: $(OBJECTS) $(BINS)

size_TopoRel:
	$(CPP) $(PREFLAGS) -o size_TopoRel size_TopoRel.cpp $(OBJECTS) $(POSFLAGS)

test_comparaGST4constructores:
	$(CPP) $(PREFLAGS) -o test_comparaGST4constructores test_comparaGST4constructores.cpp $(OBJECTS) $(POSFLAGS)

test_cst_data:
	$(CPP) $(PREFLAGS) -o test_cst_data test_cst_data.cpp $(POSFLAGS)

test_cst_rep:
	$(CPP) $(PREFLAGS) -o test_cst_rep test_cst_rep.cpp $(POSFLAGS)

test_sdvector_rank_select:
	$(CPP) $(PREFLAGS) -o test_sdvector_rank_select test_sdvector_rank_select.cpp $(POSFLAGS)

test_TopoRel_all_contain:
	$(CPP) $(PREFLAGS) -o test_TopoRel_all_contain test_TopoRel_all_contain.cpp $(OBJECTS) $(POSFLAGS)

test_TopoRel_all_contained:
	$(CPP) $(PREFLAGS) -o test_TopoRel_all_contained test_TopoRel_all_contained.cpp $(OBJECTS) $(POSFLAGS)

test_TopoRel_all_contained_howManyLCP:
	$(CPP) $(PREFLAGS) -o test_TopoRel_all_contained_howManyLCP test_TopoRel_all_contained_howManyLCP.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_allContained_vsGST6_byQueriesFile:
	$(CPP) $(PREFLAGS) -o time_TopoRel_allContained_vsGST6_byQueriesFile time_TopoRel_allContained_vsGST6_byQueriesFile.cpp $(OBJECTS) $(POSFLAGS)

test_TopoRel_all_equal:
	$(CPP) $(PREFLAGS) -o test_TopoRel_all_equal test_TopoRel_all_equal.cpp $(OBJECTS) $(POSFLAGS)

time_GST45_constructores:
	$(CPP) $(PREFLAGS) -o time_GST45_constructores time_GST45_constructores.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_all_contain:
	$(CPP) $(PREFLAGS) -o time_TopoRel_all_contain time_TopoRel_all_contain.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_allContained_naive_gst_byQueriesFile:
	$(CPP) $(PREFLAGS) -o time_TopoRel_allContained_naive_gst_byQueriesFile time_TopoRel_allContained_naive_gst_byQueriesFile.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_all_contain_GST5:
	$(CPP) $(PREFLAGS) -o time_TopoRel_all_contain_GST5 time_TopoRel_all_contain_GST5.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_allContain_GST5_xLote:
	$(CPP) $(PREFLAGS) -o time_TopoRel_allContain_GST5_xLote time_TopoRel_allContain_GST5_xLote.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_allContained_versionesGST5:
	$(CPP) $(PREFLAGS) -o time_TopoRel_allContained_versionesGST5 time_TopoRel_allContained_versionesGST5.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_allContained_vsGST5_byQueriesFile:
	$(CPP) $(PREFLAGS) -o time_TopoRel_allContained_vsGST5_byQueriesFile time_TopoRel_allContained_vsGST5_byQueriesFile.cpp $(OBJECTS) $(POSFLAGS)

time_allTopoRels_byQueriesFile:
	$(CPP) $(PREFLAGS) -o time_allTopoRels_byQueriesFile time_allTopoRels_byQueriesFile.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_boolean:
	$(CPP) $(PREFLAGS) -o time_TopoRel_boolean time_TopoRel_boolean.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_groupRelations:
	$(CPP) $(PREFLAGS) -o time_TopoRel_groupRelations time_TopoRel_groupRelations.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_0_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_0_test TopoRel_GST_0_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_1_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_1_test TopoRel_GST_1_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_2_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_2_test TopoRel_GST_2_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_4_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_4_test TopoRel_GST_4_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_5_load:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_5_load TopoRel_GST_5_load.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_5_save-load-test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_5_save-load-test TopoRel_GST_5_save-load-test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_5_save:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_5_save TopoRel_GST_5_save.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_5_size:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_5_size TopoRel_GST_5_size.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_5_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_5_test TopoRel_GST_5_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_5_test_allContained_byQueries:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_5_test_allContained_byQueries TopoRel_GST_5_test_allContained_byQueries.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_6_allContained:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_6_allContained TopoRel_GST_6_allContained.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_6_build:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_6_build TopoRel_GST_6_build.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_6_check_allContained:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_6_check_allContained TopoRel_GST_6_check_allContained.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_6_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_6_test TopoRel_GST_6_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_6_size:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_6_size TopoRel_GST_6_size.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_6_test_allContained_byQueries:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_6_test_allContained_byQueries TopoRel_GST_6_test_allContained_byQueries.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_6_test_allIntersect_byQueries:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_6_test_allIntersect_byQueries TopoRel_GST_6_test_allIntersect_byQueries.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_6_time_allContained:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_6_time_allContained TopoRel_GST_6_time_allContained.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_allContained_byQueries:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_allContained_byQueries TopoRel_Naive_allContained_byQueries.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_allIntersect_byQueries:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_allIntersect_byQueries TopoRel_Naive_allIntersect_byQueries.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_build:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_PreComp_build TopoRel_Naive_PreComp_build.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_load:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_PreComp_load TopoRel_Naive_PreComp_load.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_save-load-test:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_PreComp_save-load-test TopoRel_Naive_PreComp_save-load-test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_save:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_PreComp_save TopoRel_Naive_PreComp_save.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_test:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_PreComp_test TopoRel_Naive_PreComp_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_test:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_test TopoRel_Naive_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_test_allContained_byQueries:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_test_allContained_byQueries TopoRel_Naive_test_allContained_byQueries.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_time_allContained:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_time_allContained TopoRel_Naive_time_allContained.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_testall:
	$(CPP) $(PREFLAGS) -o TopoRel_testall TopoRel_testall.cpp $(OBJECTS) $(POSFLAGS)

clean:
	rm -f $(BINS) $(OBJECTS)