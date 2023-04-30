#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include "TopoRel_Naive.hpp"

// Tiempo de realizar consultas sobre naive

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
	int nQueries;
	consultas >> nQueries;

	int cantRutasRetorna = 0;
	auto t0 = clock();
	for(int i=0; i<nQueries; i++){
		consultas >> x;
		vector<int> res = tr_allContained(rutas, x);
		cantRutasRetorna += res.size();
	}
	auto t1 = clock();
	consultas.close();
	
	double time = ((double)(t1 - t0) / CLOCKS_PER_SEC)* 1000000;
	double timeProm = time / nQueries;
	double cantPromResp = (cantRutasRetorna + 0.0) / nQueries;
	
	cout << "************************** RESULTADOS NAIVE **************************" << endl;
	cout << "Archivo entrada: " << argv[1] << endl;
	cout << "Archivo consultas: " << argv[2] << endl;
	cout << "Rutas del conjunto: " << n << endl;
	cout << "Stops del conjunto: " << max << endl;
	cout << "Cantidad de consultas: " << nQueries << endl;
	cout << "+ Tiempo total de consultas: " << time << "[us]" << endl;
	cout << "+ Tiempo promedio de consulta: " << timeProm << "[us]" << endl;
	cout << "Total de rutas retornadas: " << cantRutasRetorna << endl;
	cout << "Promedio de rutas retornadas: " << cantPromResp << endl;

	return 0;
}
