#include <iostream>
#include "TopoRel_GST_0.hpp"
#include "TopoRel_Naive_PreComp.hpp"

using namespace std;
using namespace sdsl;

void print_bool(bool x){
	if(x){
		cout << "true";
	}else{
		cout << "false";
	}
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
	
	TopoRelGST tr_gst(routes, n_stops);
	cout << "************** GST **************" << endl;
	tr_gst.sizeEstructura();

	TopoRelNaivePreComp tr_npc(routes, n_stops);
	cout << "********* Naive PreComp *********" << endl;
	tr_npc.sizeEstructura();

	return 0;
}