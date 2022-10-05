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
    int contAgregadas = 0;
    int contBasicas = 0;

    string rNaive, rNaivePC, rGST;
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            rNaive = toporel(routes[i], routes[j]);
            rNaivePC = trpc.obtenerRelacion(i, j);
            rGST = tt.obtenerRelacion(i, j);
            // 8 Relaciones básicas 
            if(rNaive != rNaivePC || rNaivePC != rGST){
            	cout << "i: " << i << " - j: " << j << endl;
            	cout << "\trNaive: " << rNaive << endl;
            	cout << "\trNaivePC: " << rNaivePC << endl;
            	cout << "\trGST: " << rGST << endl;
	            cout << "\t(" << routes[i].size() << "): ";
	            for(int x=0; x<routes[i].size(); x++){
	            	cout << routes[i][x] << " ";
	            }
	            cout << endl << "\t(" << routes[j].size() << "): ";
	            for(int x=0; x<routes[j].size(); x++){
	            	cout << routes[j][x] << " ";
	            }
	            cout << endl;
	            cout << " ---- " << endl;
	            bool bb = tt.tr_touches(i, j);
	            cout << "Resultado tt.tr_touches: " << bb << endl;
	            cout << " ---- " << endl;
	            contBasicas++;
            }


            // 3 Relaciones agregadas
			bool trWith = tr_within(routes[i], routes[j]);
			bool trpcWith = trpc.within(i, j);
			bool trCont = tr_contains(routes[i], routes[j]);
			bool trpcCont = trpc.contains(i, j);
			bool trInte = tr_intersects(routes[i], routes[j]);
			bool trpcInte = trpc.intersects(i, j);

            if((trWith != trpcWith) || (trCont != trpcCont) || (trInte != trpcInte)){
            	cout << "\tNaive\tNaivePC" << endl;
            	cout << "With\t" << trWith << "\t" << trpcWith << endl;
            	cout << "Cont\t" << trCont << "\t" << trpcCont << endl;
            	cout << "Inte\t" << trInte << "\t" << trpcInte << endl;
            	cout << "Rel\t" << rNaive << "\t" << rNaivePC << endl;
            	cout << "\t(" << routes[i].size() << "): ";
	            for(int x=0; x<routes[i].size(); x++){
	            	cout << routes[i][x] << " ";
	            }
	            cout << endl << "\t(" << routes[j].size() << "): ";
	            for(int x=0; x<routes[j].size(); x++){
	            	cout << routes[j][x] << " ";
	            }
	            cout << endl;
	            cout << endl << " ---- " << endl;
	            contAgregadas++;
            }
        }
    }
	cout << "Problemas en relaciones agregadas: " << contAgregadas << "/" << endl;
	cout << "Problemas en relaciones básicas: " << contBasicas << "/" << endl;
	cout << "Total rutas: " << n_routes << " >>> " << (n_routes*n_routes) << " relaciones posibles." << endl;
    return 0;
}