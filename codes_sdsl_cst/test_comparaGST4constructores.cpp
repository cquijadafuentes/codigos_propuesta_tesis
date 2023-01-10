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

	TopoRelGST_4 tgst_old(rutas, n_stops);
	TopoRelGST_4 tgst_new(rutas, n_stops, true);

}