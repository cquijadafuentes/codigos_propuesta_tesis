#include <iostream>
#include <vector>
#include <fstream>
#include "TopoRel_Naive.hpp"

/*
	Consulta el largo de las secuencias de en un archivo queries
*/

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <input_file> <queries_file>" << endl;
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

	ifstream consultas(argv[2], ifstream::in);
	int n_consultas;
	consultas >> n_consultas;
	cout << n_consultas << endl;
	for(int i=0; i<n_consultas; i++){
		consultas >> x;
		cout << rutas[x].size() << " ";
		cout << endl;
	}
	consultas.close();
	return 0;
}
