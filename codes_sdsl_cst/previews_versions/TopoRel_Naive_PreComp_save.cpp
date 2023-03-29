#include <iostream>
#include "TopoRel_Naive_PreComp.hpp"

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){

	if(argc < 2){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <output_filename>" << endl;
		return 0;
	}
	cout << "Input:" << endl;
	cout << "cant_rutas max_stop" << endl;
	cout << "cant_stops_r1 r1_st1 r1_st2 ···" << endl;
	cout << "cant_stops_r2 r2_st1 r2_st2 ···" << endl;
	cout << "···" << endl << endl;

	int n, x, aux, max;
	cin >> n >> max;
	vector<vector<int>> lx(n);
	for(int i = 0; i < n; i++){
		cin >> x;
		for(int j=0; j<x; j++){
			cin >> aux;
			lx[i].push_back(aux);
		}
	}
	cout << lx.size() << endl;
	for(int i=0; i<lx.size(); i++){
		cout << lx[i].size() << ": ";
		for(int j=0; j<lx[i].size(); j++){
			cout << lx[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	
	TopoRelNaivePreComp tpnPC_1(lx, max);

	string filename(argv[1]);
	if(tpnPC_1.save(filename)){
		cout << "Se ha guardado exitosamente." << endl;
	}else{
		cout << "Falla en el guardado." << endl;
	}
	return 0;
}