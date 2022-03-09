CPP=g++

PREFLAGS=-std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib

POSFLAGS=-lsdsl -ldivsufsort -ldivsufsort64

BINS=test_csa test_cst my_test_cst pc rt_lcs

all: clean bin

bin: $(BINS)

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

clean:
	rm -f $(BINS)