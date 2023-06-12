/*
	Toma de tiempo promedio para consultas allIntersectPPk
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
	if(argc < 5){
		cout << "Programa para medir tiempo promedio en la operación allIntersect en implementaciones naive y gst." << endl;
		cout << "Error! faltan argumentos:" << endl;
		cout << argv[0] << " <input_file.txt> <intput_file.gst> <queries_file> repeticiones" << endl;
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
	int resNaive = 0;

	gst.statsReset();
	cout << "Ejecutando consultas en implementación Naive..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	resNaive += tr_allIntersectPP(rutas, queries[i]).size();
	    }
	}
	t1 = clock();
	resNaive /= repeticiones;
	double tNaive = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;
	double cantLCPNaive = (0.0 + gst.howManyLCP) / queries.size() / repeticiones;
	double cantNodosNaive = (0.0 + gst.howManyNodes) / queries.size() / repeticiones;
	cout << "operacion\trutas\tqueries\ttNaive\ttGST\t[us]\trNaive\trGST\tk" << endl;
	cout << "Naive_allIntersectPP\t" << gst.n_rutas << "\t" << num_queries << "\t" << tNaive << "\tN/A\t[us]\t" << resNaive << "\tN/A\t1" << endl;

	gst.statsReset();
	cout << "Ejecutando consultas en implementación GST..." << endl;
	vector<int> ks = {1, 3, 5, 10, 15};
	for(int k=0; k<ks.size(); k++){
		int resGST = 0;
		gst.howManyLCP = 0;
		t0 = clock();
		for(int j=0; j<repeticiones; j++){
		    for(int i=0; i<queries.size(); i++){
		    	resGST += gst.tr_allIntersectPP(queries[i], ks[k]).size();
		    }
		}
		t1 = clock();
		resGST /= repeticiones;
		gst.howManyLCP /= repeticiones;
		double tGST = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;
		double cantLCPGST = (0.0 + gst.howManyLCP) / queries.size() / repeticiones;
		double cantNodosGST = (0.0 + gst.howManyNodes) / queries.size() / repeticiones;
		cout << "operacion\trutas\tqueries\ttNaive\ttGST\t[us]\trNaive\trGST\tk\thowManyLCP" << endl;
		cout << "GST_allIntersectPP\t" << gst.n_rutas << "\t" << num_queries << "\tN/A\t" << tGST << "\t[us]\tN/A\t" << resGST << "\t" << ks[k] << "\t" << gst.howManyLCP << endl;
		cout << endl;
	}
	cout << "---------------------- Fin ----------------------" << endl;
	return 0;
}