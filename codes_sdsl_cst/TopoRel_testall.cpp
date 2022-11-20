#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <map>
#include "TopoRel_Naive.hpp"
#include "TopoRel_Naive_PreComp.hpp"
#include "TopoRel_Cons.hpp"
#include "TopoRel_GST_0.hpp"
#include "TopoRel_GST_1.hpp"
#include "TopoRel_GST_2.hpp"

using namespace std;

bool noIguales(bool a, bool b, bool c, bool d, bool e){
	if(a != b || b != c || c != d || d != e){
		return true;
	}
	return false;
}

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

    TopoRelGST_0 tt_0(routes, n_stops);
    TopoRelGST_1 tt_1(routes, n_stops);
    TopoRelGST_2 tt_2(routes, n_stops);
    TopoRelNaivePreComp trpc(routes, n_stops);
    int contAgregadas = 0;
    int contBasicas = 0;

    string rNaive, rNaivePC, rGST_0, rGST_1, rGST_2;
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            rNaive = toporel(routes[i], routes[j]);
            rNaivePC = trpc.obtenerRelacion(i, j);
            rGST_0 = tt_0.obtenerRelacion(i, j);
            rGST_1 = tt_1.obtenerRelacion(i, j);
            rGST_2 = tt_2.obtenerRelacion(i, j);
            // 8 Relaciones básicas 
            if(rNaive != rNaivePC || rNaivePC != rGST_0 || rGST_0 != rGST_1 || rGST_1 != rGST_2){
            	cout << "i: " << i << " - j: " << j << endl;
            	cout << "\trNaive: " << rNaive << endl;
            	cout << "\trNaivePC: " << rNaivePC << endl;
            	cout << "\trGST_0: " << rGST_0 << endl;
            	cout << "\trGST_1: " << rGST_1 << endl;
            	cout << "\trGST_2: " << rGST_2 << endl;
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
	            contBasicas++;
            }


            // 3 Relaciones agregadas
			bool trWith = tr_within(routes[i], routes[j]);
			bool trpcWith = trpc.within(i, j);
			bool tt0With = tt_0.tr_within(i,j);
			bool tt1With = tt_1.tr_within(i,j);
			bool tt2With = tt_2.tr_within(i,j);
			bool trCont = tr_contains(routes[i], routes[j]);
			bool trpcCont = trpc.contains(i, j);
			bool tt0Cont = tt_0.tr_contains(i, j);
			bool tt1Cont = tt_1.tr_contains(i, j);
			bool tt2Cont = tt_2.tr_contains(i, j);
			bool trInte = tr_intersects(routes[i], routes[j]);
			bool trpcInte = trpc.intersects(i, j);
			bool tt0Inte = tt_0.tr_intersects(i, j);
			bool tt1Inte = tt_1.tr_intersects(i, j);
			bool tt2Inte = tt_2.tr_intersects(i, j);

            if(noIguales(trWith, trpcWith, tt0With, tt1With, tt2With) ||
            		noIguales(trCont, trpcCont, tt0Cont, tt1Cont, tt2Cont) ||
            		noIguales(trInte, trpcInte, tt0Inte, tt1Inte, tt2Inte)){
            	cout << "\tNaive\tNaivePC\tGST_0\tGST_1\tGST_2" << endl;
            	cout << "With\t" << trWith << "\t" << trpcWith << "\t" << tt0With << "\t" << tt1With << "\t" << tt2With << endl;
            	cout << "Cont\t" << trCont << "\t" << trpcCont << "\t" << tt0Cont << "\t" << tt1Cont << "\t" << tt2Cont << endl;
            	cout << "Inte\t" << trInte << "\t" << trpcInte << "\t" << tt0Inte << "\t" << tt1Inte << "\t" << tt2Inte << endl;
            	cout << "Rel\t" << rNaive << "\t" << rNaivePC << "\t" << rGST_0 << "\t" << rGST_1 << "\t" << rGST_2 << endl;
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
	cout << "Problemas en relaciones básicas: " << contBasicas << endl;
	cout << "Problemas en relaciones agregadas: " << contAgregadas << endl;
	cout << "Total rutas: " << n_routes << " >>> " << (n_routes*n_routes) << " relaciones posibles." << endl;
    return 0;
}