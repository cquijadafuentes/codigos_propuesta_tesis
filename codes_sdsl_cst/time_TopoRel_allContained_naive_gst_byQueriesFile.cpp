/*
	Toma de tiempo promedio para consultas allContained sobre implementaciones
	GST6 (versión original recorriendo SA, versión 2 reccoriendo desde raíz y
	versión 3 utilizando wl)
	Las consultas provienen de un archivo de consulta generado con 
	./extras/generador_consultas
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <set>
#include <cstdlib>
#include "TopoRel_Naive.hpp"
#include "TopoRel_GST.hpp"

using namespace std;

vector<int> generarQueries(int n_rutas, int num_queries);

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "Programa para medir tiempo promedio en la operación allContained en implementaciones naive, naivepc y gst6." << endl;
		cout << "Error! faltan argumentos:" << endl;
		cout << argv[0] << " <input_file.txt> <intput_file.gst6> <queries_file> repeticiones" << endl;
		return 0;
	}
	
	cout << "Cargando estructuras..." << endl;
	// Cargando estructura Naive
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

	// Cargando estructura GST
	TopoRelGST gst(argv[2]);

	cout << "Cargando consultas..." << endl;
	ifstream qFile(argv[3], ifstream::in);
	int num_queries;
	qFile >> num_queries;
	vector<int> queries(num_queries);
	for(int i=0; i<num_queries; i++){
		qFile >> queries[i];
	}

	// Cargando cantidad de repeticiones
	int repeticiones = (int) atoi(argv[4]);
	if(num_queries < 1 || repeticiones < 1){
		cout << "Error! en la cantidad de num_queries/repeticiones: " << num_queries << endl;
		return 0;
	}

	if(num_queries > gst.n_rutas){
		num_queries = gst.n_rutas;
	}

	// Mostrando información de los datos:
	cout << "Rutas: " << gst.n_rutas << endl;
	cout << "Ruta más corta: " << gst.len_min << endl;
	cout << "Ruta más larga: " << gst.len_max << endl;
	cout << "Consultas a realizar: " << num_queries << endl;
	cout << "Repeticiones a realizar: " << repeticiones << endl;

	unsigned t0, t1;

	gst.statsReset();
	cout << "Ejecutando consultas en implementación Naive..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	tr_allContained(rutas, i);
	    }
	}
	t1 = clock();
	double tNaive = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;
	double cantLCPNaive = (0.0 + gst.howManyLCP) / queries.size() / repeticiones;
	double cantNodosNaive = (0.0 + gst.howManyNodes) / queries.size() / repeticiones;


	gst.statsReset();
	cout << "Ejecutando consultas en implementación GST6 versión 6..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	gst.tr_allContained(queries[i]);
	    }
	}
	t1 = clock();
	double tGST6v6 = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;
	double cantLCPGST6v6 = (0.0 + gst.howManyLCP) / queries.size() / repeticiones;
	double cantNodosGST6v6 = (0.0 + gst.howManyNodes) / queries.size() / repeticiones;

	cout << "rutas\tqueries\tt_{n}\tt_{gst}" << endl;
	cout << gst.n_rutas << "\t" << num_queries << "\t" << tNaive << "\t" << tGST6v6 << "\t[us]" << endl;
	cout << endl;
	return 0;
}