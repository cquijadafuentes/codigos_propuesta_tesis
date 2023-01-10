#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <map>
#include <algorithm>
#include "TopoRel_Naive.hpp"
#include "TopoRel_Naive_PreComp.hpp"
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
	int diferencias = 0;

	// Construir estructuras preComp y GST4
	TopoRelNaivePreComp trpc(rutas, n_stops);
	TopoRelGST_4 tgst(rutas, n_stops);


	// Realizar consultas según corresponda
	for(int i=0; i<rutas.size(); i++){
		// Consultas con ruta i
		
		vector<int> resNaive = tr_allContain(rutas, i);
    	vector<int> resPreComp = trpc.allContain(i);
    	vector<int> resGST4 = tgst.tr_allContain(i);

    	if(resNaive.size() != resPreComp.size() || resPreComp.size() != resGST4.size()){
    		diferencias++;
    		cout << "Ruta " << i << endl;
    		cout << "\tNaive (" << resNaive.size() << "): ";
    		imprime_vector(resNaive);
    		cout << "\tPreCo (" << resPreComp.size() << "): ";
    		imprime_vector(resPreComp);
    		cout << "\tGST_4 (" << resGST4.size() << "): ";
    		imprime_vector(resGST4);
    	}else{
    		sort(resNaive.begin(), resNaive.end());
    		sort(resPreComp.begin(), resPreComp.end());
    		sort(resGST4.begin(), resGST4.end());
    		for(int j=0; j<resGST4.size(); j++){
    			if(resNaive[j] != resPreComp[j] || resPreComp[j] != resGST4[j]){
    				cout << "Ruta " << i << endl;
		    		cout << "\tNaive (" << resNaive.size() << "): ";
		    		imprime_vector(resNaive);
		    		cout << "\tPreCo (" << resPreComp.size() << "): ";
		    		imprime_vector(resPreComp);
		    		cout << "\tGST_4 (" << resGST4.size() << "): ";
		    		imprime_vector(resGST4);
    			}
    		}
    	}

    }
    cout << diferencias << " diferencias entre los resultados." << endl; 

}


void imprime_vector(vector<int> x){
	for(int i=0; i<x.size(); i++){
		cout << x[i] << " ";
	}
	cout << endl;
}