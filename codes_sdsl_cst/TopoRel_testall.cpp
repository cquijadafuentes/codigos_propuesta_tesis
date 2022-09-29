#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <map>
#include "TopoRel_Naive.hpp"
#include "TopoRel_Naive_PreComp.hpp"
#include "TopoRel_GST.hpp"
#include "TopoRel_Cons.hpp"

using namespace std;

int main(int argc, char const *argv[]){
	int n_routes, n_t, n_stops, aux;
	vector<vector<int>> routes;
    cin >> n_routes >> n_stops;
	for(int j = 0; j < n_routes; j++){
		cin >> n_t;
        vector<int> vt(n_t, 0);
        for(int i = 0; i < n_t; i++){
            cin >> vt[i];
        }
        routes.push_back(vt);
	}

    TopoRelGST tt(routes, n_stops);
    TopoRelNaivePreComp trpc(routes, n_stops);

    string rNaive, rNaivePC, rGST;
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            rNaive = toporel(routes[i], routes[j]);
            rNaivePC = trpc.obtenerRelacion(i, j);
            rGST = tt.obtenerRelacion(i, j);
            if(rNaive != rNaivePC || rNaivePC != rGST){
            	cout << "i: " << i << " - j: " << j << endl;
            	cout << "\trNaive: " << rNaive << endl;
            	cout << "\trNaivePC: " << rNaivePC << endl;
            	cout << "\trGST: " << rGST << endl;
	            cout << "\ti: ";
	            for(int x=0; x<routes[i].size(); x++){
	            	cout << routes[i][x] << " ";
	            }
	            cout << endl << "\tj: ";
	            for(int x=0; x<routes[j].size(); x++){
	            	cout << routes[j][x] << " ";
	            }
	            cout << endl;
	            cout << " ---- " << endl;
	            bool bb = tt.tr_touches(i, j);
	            cout << "Resultado tt.tr_touches: " << bb << endl;
	            cout << " ---- " << endl;
            }
        }
    }
	
    return 0;
}