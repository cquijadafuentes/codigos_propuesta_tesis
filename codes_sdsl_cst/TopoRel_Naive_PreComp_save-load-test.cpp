#include <iostream>
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
	
	unsigned t0 = clock();
	TopoRelNaivePreComp tpnPC_1(lx, max);
	unsigned t1 = clock();

	string filename(argv[1]);
	if(tpnPC_1.save(filename)){
		cout << "Se ha guardado exitosamente." << endl;
	}else{
		cout << "Falla en el guardado." << endl;
		return 0;
	}

	unsigned t2 = clock();
	TopoRelNaivePreComp tpnPC_2(filename);
	unsigned t3 = clock();

	double t_const = (((double)(t1 - t0)) / CLOCKS_PER_SEC) * 1000;
	double t_carga = (((double)(t3 - t2)) / CLOCKS_PER_SEC) * 1000;

	cout << "Tiempo de construcción paralela: " << t_const << "[ms]" << endl;
	cout << "Tiempo de carga desde archivo: " << t_carga << "[ms]" << endl;

	cout << "Archivo cargado exitosamente." << endl;

	cout << "************** size original **************" << endl;
	tpnPC_1.sizeEstructura();

	cout << "************** size archivo **************" << endl;
	tpnPC_2.sizeEstructura();

	cout << "Comparando estructuras ... " << endl;
	if(tpnPC_1.iguales(tpnPC_2)){
		cout << "Son iguales." << endl;
	}else{
		cout << "Falla! NO son iguales." << endl;
	}

	return 0;
}