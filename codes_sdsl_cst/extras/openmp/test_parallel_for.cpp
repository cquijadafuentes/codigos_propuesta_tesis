/*
	Compilar:
	g++ -std=c++11 -g -O0 -DNDEBUG -fopenmp -o test_parallel_for test_parallel_for.cpp 
*/

#include <iostream>
#include <string>
#include <omp.h>

using namespace std;

int main(){
	int i;
	int mnt = omp_get_max_threads();
	cout << "mnt: " << mnt << endl;
	string mm;
	cin >> mm;
	#pragma omp parallel for
	for(i=0; i<mnt; i++){
		for(int j=0; j<10000000; j++){
			string s = "";
			for(int k=0; k<i; k++){
				s += "\t";
			}
			s += to_string(j) + "\n";
			cout << s;
		}
	}
	return 0;
}
