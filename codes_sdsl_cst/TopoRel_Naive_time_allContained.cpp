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
		//util::bit_compress(rutas[i]); // Borrado para experimentos con y sin bit_compress
	}
	entrada.close();

	ifstream consultas(argv[2], ifstream::in);
	int nQueries;
	consultas >> nQueries;
	vector<int> queries(nQueries);
	for(int i=0; i<nQueries; i++){
		consultas >> queries[i];
	}
	consultas.close();

	int cantRutasRetorna = 0;
	auto t0 = clock();
	for(int i=0; i<nQueries; i++){
		cantRutasRetorna += tr_allContained(rutas, queries[i]).size();
	}
	auto t1 = clock();
	
	double time = ((double)(t1 - t0) / CLOCKS_PER_SEC)* 1000000;
	double timeProm = time / nQueries;
	double cantPromResp = (cantRutasRetorna + 0.0) / nQueries;
	
	cout << "************************** RESULTADOS NAIVE **************************" << endl;
	cout << "Archivo entrada: " << argv[1] << endl;
	cout << "Archivo consultas: " << argv[2] << endl;
	cout << "Rutas del conjunto: " << n << endl;
	cout << "Stops del conjunto: " << max << endl;
	cout << "Cantidad de consultas: " << nQueries << endl;
	cout << "Total de rutas retornadas: " << cantRutasRetorna << endl;
	cout << "Promedio de rutas retornadas: " << cantPromResp << endl;
	cout << "+ Tiempo total de consultas (sin bit_compress): " << time << "[us]" << endl;
	cout << "+ Tiempo promedio de consulta (sin bit_compress): " << timeProm << "[us]" << endl;


	for(int i = 0; i < n; i++){
		util::bit_compress(rutas[i]);
	}
	t0 = clock();
	for(int i=0; i<nQueries; i++){
		cantRutasRetorna += tr_allContained(rutas, queries[i]).size();
	}
	t1 = clock();	
	time = ((double)(t1 - t0) / CLOCKS_PER_SEC)* 1000000;
	timeProm = time / nQueries;
	cout << "+ Tiempo total de consultas (con bit_compress): " << time << "[us]" << endl;
	cout << "+ Tiempo promedio de consulta (con bit_compress): " << timeProm << "[us]" << endl;
	return 0;
}
