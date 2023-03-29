CPP=g++

PREFLAGS=-std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib

TIMEFLAGS=-std=c++11 -O3 -fopenmp -I ~/include -L ~/lib

POSFLAGS=-lsdsl -ldivsufsort -ldivsufsort64

BINS=time_TopoRel_allContain_GST5_xLote time_TopoRel_all_contain_GST5\
		TopoRel_GST_5_save TopoRel_GST_5_load\
		TopoRel_GST_5_test-save-load TopoRel_GST_5_size\
		time_TopoRel_groupRelations\
		time_GST45_constructores test_comparaGST4constructores\
		test_sdvector_rank_select test_cst_data test_cst_rep\
		test_TopoRel_all_contain test_TopoRel_all_contained\
		test_TopoRel_all_equal\
		time_TopoRel_boolean\
		size_TopoRel TopoRel_testall\
		TopoRel_Naive_test  TopoRel_Naive_PreComp_test\
		TopoRel_Naive_PreComp_test-save-load\
		TopoRel_Naive_PreComp_save TopoRel_Naive_PreComp_load\
		TopoRel_GST_0_test TopoRel_GST_1_test TopoRel_GST_2_test\
		TopoRel_GST_4_test TopoRel_GST_5_test

OBJECTS=TopoRel_GST_0.o TopoRel_GST_1.o\
		TopoRel_GST_2.o TopoRel_GST_4.o\
		TopoRel_GST_5.o\
		TopoRel_Naive.o TopoRel_Naive_PreComp.o

%.o: %.cpp
	$(CPP) $(TIMEFLAGS) -c $< -o $@ $(POSFLAGS)

#all: clean bin
all: bin

bin: $(OBJECTS) $(BINS)

size_TopoRel:
	$(CPP) $(TIMEFLAGS) -o size_TopoRel size_TopoRel.cpp $(OBJECTS) $(POSFLAGS)

test_comparaGST4constructores:
	$(CPP) $(TIMEFLAGS) -o test_comparaGST4constructores test_comparaGST4constructores.cpp $(OBJECTS) $(POSFLAGS)

test_cst_data:
	$(CPP) $(TIMEFLAGS) -o test_cst_data test_cst_data.cpp $(POSFLAGS)

test_cst_rep:
	$(CPP) $(TIMEFLAGS) -o test_cst_rep test_cst_rep.cpp $(POSFLAGS)

test_sdvector_rank_select:
	$(CPP) $(TIMEFLAGS) -o test_sdvector_rank_select test_sdvector_rank_select.cpp $(POSFLAGS)

test_TopoRel_all_contain:
	$(CPP) $(TIMEFLAGS) -o test_TopoRel_all_contain test_TopoRel_all_contain.cpp $(OBJECTS) $(POSFLAGS)

test_TopoRel_all_equal:
	$(CPP) $(TIMEFLAGS) -o test_TopoRel_all_equal test_TopoRel_all_equal.cpp $(OBJECTS) $(POSFLAGS)

time_GST45_constructores:
	$(CPP) $(TIMEFLAGS) -o time_GST45_constructores time_GST45_constructores.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_all_contain:
	$(CPP) $(TIMEFLAGS) -o time_TopoRel_all_contain time_TopoRel_all_contain.cpp $(OBJECTS) $(POSFLAGS)

test_TopoRel_all_contained:
	$(CPP) $(TIMEFLAGS) -o test_TopoRel_all_contained test_TopoRel_all_contained.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_all_contain_GST5:
	$(CPP) $(TIMEFLAGS) -o time_TopoRel_all_contain_GST5 time_TopoRel_all_contain_GST5.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_allContain_GST5_xLote:
	$(CPP) $(TIMEFLAGS) -o time_TopoRel_allContain_GST5_xLote time_TopoRel_allContain_GST5_xLote.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_boolean:
	$(CPP) $(TIMEFLAGS) -o time_TopoRel_boolean time_TopoRel_boolean.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_groupRelations:
	$(CPP) $(TIMEFLAGS) -o time_TopoRel_groupRelations time_TopoRel_groupRelations.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_0_test:
	$(CPP) $(TIMEFLAGS) -o TopoRel_GST_0_test TopoRel_GST_0_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_1_test:
	$(CPP) $(TIMEFLAGS) -o TopoRel_GST_1_test TopoRel_GST_1_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_2_test:
	$(CPP) $(TIMEFLAGS) -o TopoRel_GST_2_test TopoRel_GST_2_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_4_test:
	$(CPP) $(TIMEFLAGS) -o TopoRel_GST_4_test TopoRel_GST_4_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_5_load:
	$(CPP) $(TIMEFLAGS) -o TopoRel_GST_5_load TopoRel_GST_5_load.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_5_save:
	$(CPP) $(TIMEFLAGS) -o TopoRel_GST_5_save TopoRel_GST_5_save.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_5_size:
	$(CPP) $(TIMEFLAGS) -o TopoRel_GST_5_size TopoRel_GST_5_size.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_5_test-save-load:
	$(CPP) $(TIMEFLAGS) -o TopoRel_GST_5_test-save-load TopoRel_GST_5_test-save-load.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_5_test:
	$(CPP) $(TIMEFLAGS) -o TopoRel_GST_5_test TopoRel_GST_5_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_load:
	$(CPP) $(TIMEFLAGS) -o TopoRel_Naive_PreComp_load TopoRel_Naive_PreComp_load.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_save:
	$(CPP) $(TIMEFLAGS) -o TopoRel_Naive_PreComp_save TopoRel_Naive_PreComp_save.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_test:
	$(CPP) $(TIMEFLAGS) -o TopoRel_Naive_PreComp_test TopoRel_Naive_PreComp_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_test-save-load:
	$(CPP) $(TIMEFLAGS) -o TopoRel_Naive_PreComp_test-save-load TopoRel_Naive_PreComp_test-save-load.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_test:
	$(CPP) $(TIMEFLAGS) -o TopoRel_Naive_test TopoRel_Naive_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_testall:
	$(CPP) $(TIMEFLAGS) -o TopoRel_testall TopoRel_testall.cpp $(OBJECTS) $(POSFLAGS)


clean:
	rm -f $(BINS) $(OBJECTS)