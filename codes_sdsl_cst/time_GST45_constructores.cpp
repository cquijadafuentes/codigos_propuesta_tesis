#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <map>
#include <algorithm>
#include "TopoRel_GST_4.hpp"
#include "TopoRel_GST_5.hpp"

using namespace std;

void imprime_vector(vector<int> x);

int main(int argc, char const *argv[]){

	int n_rutas, n_t, n_stops, aux;
	vector<vector<int>> rutas;
    cin >> n_rutas >> n_stops;
	for(int j = 0; j < n_rutas; j++){
		cin >> n_t;
        vector<int> vt(n_t, 0);
        for(int i = 0; i < n_t; i++){
            cin >> vt[i];
        }
        rutas.push_back(vt);
	}

	cout << "cantidad de rutas: " << n_rutas << endl;

	unsigned t0 = clock();
	TopoRelGST_4 tgst4(rutas, n_stops);
    unsigned t1 = clock();
	TopoRelGST_5 tgst5(rutas, n_stops);
	unsigned t2 = clock();
	TopoRelGST_4 tgst_bottomUp(rutas, n_stops, 0);
	unsigned t3 = clock();

	double tConsOld = (((double)(t1 - t0)) / CLOCKS_PER_SEC) * 1000000;
	double tConsTDParallel = (((double)(t2 - t1)) / CLOCKS_PER_SEC) * 1000000;
	double tConsBottomUp = (((double)(t3 - t2)) / CLOCKS_PER_SEC) * 1000000;

	cout << "Tiempo Construcción TopDown: " << tConsOld << " - TDParallel: "  << tConsTDParallel << " - BottomUp: "  << tConsBottomUp << endl;

	return 0;
	
}