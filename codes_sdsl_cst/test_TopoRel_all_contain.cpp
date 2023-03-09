#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <map>
#include <algorithm>
#include "TopoRel_Naive.hpp"
#include "TopoRel_Naive_PreComp.hpp"
#include "TopoRel_GST_5.hpp"

using namespace std;

void imprime_vector(vector<int> x);

int main(int argc, char const *argv[]){
	cout << "Programa para comparar resultados de operación AllContain en las 3 estructuras" << endl;
	int queries = 0;
	if(argc > 1){
		queries = (int) atoi(argv[1]);
	}
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
	if(queries == 0){
		queries = n_rutas;
	}
	int diferencias = 0;

	// Construir estructuras preComp y GST5
	TopoRelNaivePreComp trpc(rutas, n_stops);
	TopoRelGST_5 tgst(rutas, n_stops);

	cout << "Rutas: " << tgst.n_rutas << endl;
	cout << "Ruta más corta: " << tgst.len_min << endl;
	cout << "Ruta más larga: " << tgst.len_max << endl;

	// Realizar consultas según corresponda
	for(int i=0; i<queries; i++){
		// Consultas con ruta i		
		vector<int> resNaive = tr_allContain(rutas, i);
    	vector<int> resPreComp = trpc.allContain(i);
    	vector<int> resGST5 = tgst.tr_allContain(i);

    	if(resNaive.size() != resPreComp.size() || resPreComp.size() != resGST5.size()){
    		diferencias++;
    		cout << "Ruta " << i << endl;
    		cout << "\tNaive (" << resNaive.size() << "): ";
    		imprime_vector(resNaive);
    		cout << "\tPreCo (" << resPreComp.size() << "): ";
    		imprime_vector(resPreComp);
    		cout << "\tGST_5 (" << resGST5.size() << "): ";
    		imprime_vector(resGST5);
    	}else{
    		sort(resNaive.begin(), resNaive.end());
    		sort(resPreComp.begin(), resPreComp.end());
    		sort(resGST5.begin(), resGST5.end());
    		for(int j=0; j<resGST5.size(); j++){
    			if(resNaive[j] != resPreComp[j] || resPreComp[j] != resGST5[j]){
    				cout << "Ruta " << i << endl;
		    		cout << "\tNaive (" << resNaive.size() << "): ";
		    		imprime_vector(resNaive);
		    		cout << "\tPreCo (" << resPreComp.size() << "): ";
		    		imprime_vector(resPreComp);
		    		cout << "\tGST_5 (" << resGST5.size() << "): ";
		    		imprime_vector(resGST5);
    			}
    		}
    	}
    }
    cout << diferencias << " diferencias entre los resultados." << endl;
    return 0;
}

void imprime_vector(vector<int> x){
	for(int i=0; i<x.size(); i++){
		cout << x[i] << " ";
	}
	cout << endl;
}