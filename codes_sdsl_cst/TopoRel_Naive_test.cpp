#include <iostream>
#include <vector>
#include "TopoRel_Naive.hpp"

using namespace std;

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
		cout << argv[0] << "<input_filename.txt>" << endl;
		cout << "Input_File format:" << endl;
		cout << "cant_rutas max_stop" << endl;
		cout << "cant_stops_r1 r1_st1 r1_st2 ···" << endl;
		cout << "cant_stops_r2 r2_st1 r2_st2 ···" << endl;
		cout << "···" << endl << endl;
		return 0;
	}

	ifstream txtInput(argv[1], ifstream::in);

	int n, x, aux, max;
	txtInput >> n >> max;
	vector<vector<int>> lx(n);
	for(int i = 0; i < n; i++){
		txtInput >> x;
		for(int j=0; j<x; j++){
			txtInput >> aux;
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

	cout << "Por implementar..." << endl;

	return 0;
}
