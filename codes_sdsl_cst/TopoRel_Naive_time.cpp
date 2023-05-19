#include <iostream>
#include <vector>
#include <fstream>
#include "TopoRel_Naive.hpp"

/*
	Calcula los resultados de la operación <id_operacion> para 
	las rutas en el conjunto del archivo <input_file> usando 
	las consultas de <queries_file>
*/

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <input_file> <queries_file> <id_operacion>" << endl;
		cout << "Donde <id_operacion> es: ";
		cout << "(1) allContain - (2) allContained - (3) allEqual - (4) allIntersect" << endl;
		return 0;
	}
	// Verificando Operación
	int id_operacion = (int) atoi(argv[3]);
	if(id_operacion < 1 || id_operacion > 4){
		cout << "Error! <id_operacion> no corresponde." << endl;
		return 0;
	}
	// Cargando rutas de entrada
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
	// Cargando consultas
	ifstream consultas(argv[2], ifstream::in);
	int n_consultas;
	consultas >> n_consultas;
	vector<int> queries(n_consultas);
	for(int i=0; i<n_consultas; i++){
		consultas >> queries[i];
	}
	consultas.close();
	// Ejecutando consultas
	unsigned long t0 = clock();
	unsigned originalt0 = clock();
	switch(id_operacion){
		case 1:
			for(int i=0; i<n_consultas; i++){
				tr_allContain(rutas, queries[i]);
			}
			cout << "allContain ";
			break;
		case 2:
			for(int i=0; i<n_consultas; i++){
				tr_allContained(rutas, queries[i]);
			}
			cout << "allContained ";
			break;
		case 3:
			for(int i=0; i<n_consultas; i++){
				tr_allEqual(rutas, queries[i]);
			}
			cout << "allEqual ";
			break;
		case 4:
			for(int i=0; i<n_consultas; i++){
				tr_allIntersect(rutas, queries[i]);
			}
			cout << "allIntersect ";
			break;
		default:
			cout << "Error! Operación no definida." << endl;
			return 0;
	}
	unsigned long t1 = clock();
	unsigned originalt1 = clock();
	double tiempo = (double)(t1 - t0);
	tiempo /= CLOCKS_PER_SEC;
	tiempo /= n_consultas;
	tiempo *=  1000000;
	cout << tiempo << " [us]" << endl;
	double tNaive = ((((double)(originalt1 - originalt0)) / CLOCKS_PER_SEC) / n_consultas)* 1000000;
	cout << "Original: " <<tiempo << " [us]" << endl;
	return 0;
}
