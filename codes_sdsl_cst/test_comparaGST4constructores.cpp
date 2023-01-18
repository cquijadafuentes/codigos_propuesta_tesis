#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <map>
#include <algorithm>
#include "TopoRel_GST_4.hpp"

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
	TopoRelGST_4 tgst_old(rutas, n_stops);
    unsigned t1 = clock();
	TopoRelGST_4 tgst_tdP(rutas, n_stops, true);
	unsigned t2 = clock();
	TopoRelGST_4 tgst_bottomUp(rutas, n_stops, 0);
	unsigned t3 = clock();

	double tConsOld = (((double)(t1 - t0)) / CLOCKS_PER_SEC) * 1000000;
	double tConsTDParallel = (((double)(t2 - t1)) / CLOCKS_PER_SEC) * 1000000;
	double tConsBottomUp = (((double)(t3 - t2)) / CLOCKS_PER_SEC) * 1000000;

	cout << "Tiempo Construcción TopDown: " << tConsOld << " - TDParallel: "  << tConsTDParallel << " - BottomUp: "  << tConsBottomUp << endl;

	cout << "Comparando con TDParallel ...";
	bool checkParallel = tgst_old.iguales(tgst_tdP);
	if(checkParallel){
		cout << "Iguales con TDParallel" << endl;
	}else{
		cout << "DISTINTOS con TDParallel" << endl;
	}

	cout << "Comparando con BottomUp ...";
	if (tgst_old.iguales(tgst_bottomUp)){
		cout << "Iguales con BottomUp" << endl;
	}else{
		cout << "DISTINTOS con BottomUp" << endl;
	}

	if(!checkParallel){
		cout << "Navegando gstMapa de las estructuras: " << endl;

		for(int i=0; i<tgst_tdP.gstMapa.size(); i++){
			int id0, id1;
			id0 = tgst_old.cst.id(tgst_old.gstMapa[i]);
			id1 = tgst_tdP.cst.id(tgst_tdP.gstMapa[i]);
			cout << i << " - " << id0 << " - " << id1 << endl;
		}		
	}
	
}