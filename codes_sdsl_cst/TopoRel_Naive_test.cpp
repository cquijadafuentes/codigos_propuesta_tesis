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

	ifstream entrada(argv[1], ifstream::in);

	int n, x, aux, max;
	entrada >> n >> max;
	vector<int_vector<>> rutas(n);
	for(int i = 0; i < n; i++){
		entrada >> x;
		rutas[i] = int_vector<>(x);
		for(int j=0; j<x; j++){
			entrada >> aux;
			rutas[i][j] = aux;
		}
		util::bit_compress(rutas[i]);
	}
	entrada.close();

	cout << rutas.size() << endl;
	for(int i=0; i<rutas.size(); i++){
		cout << rutas[i].size() << ": ";
		for(int j=0; j<rutas[i].size(); j++){
			cout << rutas[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	cout << "Por implementar..." << endl;

	cout << "Ingresar IDs de la secuencia a usar para consulta: ";
	int idS;
	cin >> idS;
	unordered_map<int,int> r = tr_allIntersectPP(rutas, idS);
	for(const auto& pair:r){
        cout << pair.first << ": " << pair.second << endl;
    }
	return 0;
}
