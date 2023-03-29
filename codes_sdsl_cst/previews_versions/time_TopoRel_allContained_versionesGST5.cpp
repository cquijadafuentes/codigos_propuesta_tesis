/*
	Toma de tiempo promedio para consultas allContained sobre implementaciones
	gst5 (versión original recorriendo SA, versión 2 reccoriendo desde raíz y
	versión 3 utilizando wl)
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <set>
#include <cstdlib>
#include "TopoRel_Naive.hpp"
#include "TopoRel_Naive_PreComp.hpp"
#include "TopoRel_GST_5.hpp"

using namespace std;

vector<int> generarQueries(int n_rutas, int num_queries);

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "Programa para medir tiempo promedio en la operación allContained" << endl;
		cout << "Error! faltan argumentos:" << endl;
		cout << argv[0] << " <input_file_GST5> num_queries repeticiones" << endl;
		return 0;
	}
	int num_queries = (int) atoi(argv[2]);
	int repeticiones = (int) atoi(argv[3]);
	if(num_queries < 1 || repeticiones < 1){
		cout << "Error! en la cantidad de num_queries/repeticiones: " << num_queries << endl;
		return 0;
	}
	cout << "Cargando estructuras..." << endl;
	TopoRelGST_5 gst(argv[1]);
	
	int aux;
	
	if(num_queries > gst.n_rutas){
		num_queries = gst.n_rutas;
	}

	cout << "Rutas: " << gst.n_rutas << endl;
	cout << "Ruta más corta: " << gst.len_min << endl;
	cout << "Ruta más larga: " << gst.len_max << endl;
	cout << "Consultas a realizar: " << num_queries << endl;
	cout << "Repeticiones a realizar: " << repeticiones << endl;

	vector<int> queries = generarQueries(gst.n_rutas, num_queries);
	unsigned t0, t1;


	cout << "Ejecutando consultas GST5 versión 1..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	gst.tr_allContained(queries[i]);
	    }
	}
	t1 = clock();
	double tV1 = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;


	cout << "Ejecutando consultas GST5 versión 2..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	gst.tr_allContained2(queries[i]);
	    }
	}
	t1 = clock();
	double tV2 = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;


	cout << "Ejecutando consultas GST5 versión 3..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	gst.tr_allContained3(queries[i]);
	    }
	}
	t1 = clock();
	double tV3 = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;

	cout << "rutas\tqueries\tv1\tv2\tv3" << endl;
	cout << gst.n_rutas << "\t" << num_queries << "\t" << tV1 << "\t" << tV2 << "\t" << tV3 << "\t[us]" << endl;
	return 0;
}

vector<int> generarQueries(int n_rutas, int num_queries){
	cout << "Generando queries aleatorias ..." << endl;
	srand((unsigned) time(NULL));
	set<int> ms;
	while(ms.size() < num_queries){
		ms.insert(rand() % n_rutas);
	}

	vector<int> mv(ms.begin(), ms.end());
	return mv;
}
