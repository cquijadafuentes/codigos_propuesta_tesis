CPP=g++

PREFLAGS=-std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib

POSFLAGS=-lsdsl -ldivsufsort -ldivsufsort64

BINS=time_TopoRel_all_contain_GST5 TopoRel_GST_5_save TopoRel_GST_5_test-save-load\
		time_GST45_constructores test_comparaGST4constructores\
		test_sdvector_rank_select test_cst_data test_cst_rep\
		test_TopoRel_all_contain time_TopoRel_all_contain time_TopoRel_boolean\
		size_TopoRel TopoRel_testall\
		TopoRel_Naive_test  TopoRel_Naive_PreComp_test\
		TopoRel_GST_0_test TopoRel_GST_1_test TopoRel_GST_2_test\
		TopoRel_GST_4_test TopoRel_GST_5_test

OBJECTS=TopoRel_GST_0.o TopoRel_GST_1.o\
		TopoRel_GST_2.o TopoRel_GST_4.o\
		TopoRel_GST_5.o\
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

time_GST45_constructores:
	$(CPP) $(PREFLAGS) -o time_GST45_constructores time_GST45_constructores.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_all_contain:
	$(CPP) $(PREFLAGS) -o time_TopoRel_all_contain time_TopoRel_all_contain.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_all_contain_GST5:
	$(CPP) $(PREFLAGS) -o time_TopoRel_all_contain_GST5 time_TopoRel_all_contain_GST5.cpp $(OBJECTS) $(POSFLAGS)

time_TopoRel_boolean:
	$(CPP) $(PREFLAGS) -o time_TopoRel_boolean time_TopoRel_boolean.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_0_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_0_test TopoRel_GST_0_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_1_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_1_test TopoRel_GST_1_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_2_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_2_test TopoRel_GST_2_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_4_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_4_test TopoRel_GST_4_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_5_save:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_5_save TopoRel_GST_5_save.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_5_test-save-load:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_5_test-save-load TopoRel_GST_5_test-save-load.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_GST_5_test:
	$(CPP) $(PREFLAGS) -o TopoRel_GST_5_test TopoRel_GST_5_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_PreComp_test:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_PreComp_test TopoRel_Naive_PreComp_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_Naive_test:
	$(CPP) $(PREFLAGS) -o TopoRel_Naive_test TopoRel_Naive_test.cpp $(OBJECTS) $(POSFLAGS)

TopoRel_testall:
	$(CPP) $(PREFLAGS) -o TopoRel_testall TopoRel_testall.cpp $(OBJECTS) $(POSFLAGS)


clean:
	rm -f $(BINS) $(OBJECTS)