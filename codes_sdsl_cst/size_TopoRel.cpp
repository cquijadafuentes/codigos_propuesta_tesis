#include <iostream>
#include <string>
#include <vector>
#include "TopoRel_Naive.hpp"
#include "TopoRel_Naive_PreComp.hpp"
#include "TopoRel_Cons.hpp"
#include "TopoRel_GST_0.hpp"
#include "TopoRel_GST_1.hpp"
#include "TopoRel_GST_2.hpp"
#include "TopoRel_GST_4.hpp"

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
    
    TopoRelNaivePreComp trpc(routes, n_stops);
    trpc.sizeEstructura();

    TopoRelGST_0 tt0(routes, n_stops);
    tt0.sizeEstructura();

    TopoRelGST_1 tt1(routes, n_stops);
    tt1.sizeEstructura();

    TopoRelGST_2 tt2(routes, n_stops);
    tt2.sizeEstructura();

    TopoRelGST_4 tt4(routes, n_stops);
    tt4.sizeEstructura();
	
    return 0;

}