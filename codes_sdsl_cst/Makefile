CPP=g++

PREFLAGS=-std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib

POSFLAGS=-lsdsl -ldivsufsort -ldivsufsort64

BINS=test_csa test_cst my_test_cst pc rt_lcs test_alfabeto_ext

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

test_alfabeto_ext:
	$(CPP) $(PREFLAGS) test_alfabeto_ext.cpp -o test_alfabeto_ext $(POSFLAGS)
	

clean:
	rm -f $(BINS)