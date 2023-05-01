#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include "TopoRel_Naive.hpp"

// Tiempo de realizar consultas sobre naive

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <input_file>" << endl;
		return 0;
	}

	// Cargando datos
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

	// Calculando espacio
	int bytes_rutas = 0;
	for(int i = 0; i < n; i++){
		bytes_rutas += size_in_bytes(rutas[i]);
	}

	// Mostrando tamaños
	cout << "rutas\tstops\tnaive" << endl;
	cout << n << "\t" << max << "\t" << bytes_rutas << endl;
	return 0; 
}