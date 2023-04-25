/*
	Toma de tiempo promedio para consultas allContain, allEqual, allContained y allIntersect 
	sobre implementaciones Naive, NaivePreComp y GST6 versión 6 de allContained)
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
#include "TopoRel_Naive_PreComp.hpp"
#include "TopoRel_GST_6.hpp"

using namespace std;

vector<int> generarQueries(int n_rutas, int num_queries);

int main(int argc, char const *argv[]){
	if(argc < 6){
		cout << "Programa para medir tiempo promedio operaciones all*" << endl;
		cout << "Error! faltan argumentos:" << endl;
		cout << argv[0] << " <input_file.txt> <input_file.naivepc> <input_file.gst6> <queries_file> repeticiones" << endl;
		return 0;
	}

	int repeticiones = (int) atoi(argv[5]);

	cout << "Cargando consultas desde " << argv[4] << endl;
	ifstream qFile(argv[4], ifstream::in);
	int num_queries;
	qFile >> num_queries;
	vector<int> queries(num_queries);
	for(int i=0; i<num_queries; i++){
		qFile >> queries[i];
	}
	qFile.close();

	if(num_queries < 1 || repeticiones < 1){
		cout << "Error! en la cantidad de num_queries/repeticiones: " << num_queries << endl;
		return 0;
	}
	
	cout << "Leyendo " << argv[1] << endl;
	ifstream naive(argv[1], ifstream::in);
	int n, x, aux, max;
	naive >> n >> max;
	vector<vector<int>> rutas(n);
	for(int i = 0; i < n; i++){
		naive >> x;
		for(int j=0; j<x; j++){
			naive >> aux;
			rutas[i].push_back(aux);
		}
	}
	naive.close();

	cout << "Cargando estructura NaivePC desde " << argv[2] << endl;
	TopoRelNaivePreComp npc(argv[2]);

	cout << "Cargando estructura GST6 desde " << argv[3] << endl;
	TopoRelGST_6 gst(argv[3]);
	if(num_queries > gst.n_rutas){
		num_queries = gst.n_rutas;
	}

	cout << "Rutas: " << gst.n_rutas << endl;
	cout << "Ruta más corta: " << gst.len_min << endl;
	cout << "Ruta más larga: " << gst.len_max << endl;
	cout << "Consultas a realizar: " << num_queries << endl;
	cout << "Repeticiones a realizar: " << repeticiones << endl;

	unsigned t0, t1;
	double tNaive, tNaivePC, tGST6;

	/*****************	allContain 	*****************/

	cout << "Ejecutando allContain Naive..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	tr_allContain(rutas, queries[i]);
	    }
	}
	t1 = clock();
	tNaive = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;
	

	cout << "Ejecutando allContain NaivePC..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	npc.allContain(queries[i]);
	    }
	}
	t1 = clock();
	tNaivePC = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;
	

	cout << "Ejecutando allContain GST6..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	gst.tr_allContain(queries[i]);
	    }
	}
	t1 = clock();
	tGST6 = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;
	
	cout << "operacion\trutas\tqueries\ttnaive\ttnpc\ttgst6" << endl;
	cout << "allContain\t" << gst.n_rutas << "\t" << num_queries << "\t" << tNaive << "\t" << tNaivePC << "\t" << tGST6 << "\t[us]" << endl;

	/*****************	allEqual 	*****************/

	cout << "Ejecutando allEqual Naive..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	tr_allEqual(rutas, queries[i]);
	    }
	}
	t1 = clock();
	tNaive = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;
	

	cout << "Ejecutando allEqual NaivePC..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	npc.allEqual(queries[i]);
	    }
	}
	t1 = clock();
	tNaivePC = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;
	

	cout << "Ejecutando allEqual GST6..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	gst.tr_allEqual(queries[i]);
	    }
	}
	t1 = clock();
	tGST6 = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;


	cout << "operacion\trutas\tqueries\ttnaive\ttnpc\ttgst6" << endl;
	cout << "allEqual\t" << gst.n_rutas << "\t" << num_queries << "\t" << tNaive << "\t" << tNaivePC << "\t" << tGST6 << "\t[us]" << endl;

	/*****************	allIntersect 	*****************/

	cout << "Ejecutando allIntersect Naive..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	tr_allIntersect(rutas, queries[i]);
	    }
	}
	t1 = clock();
	tNaive = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;
	

	cout << "Ejecutando allIntersect NaivePC..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	npc.allIntersect(queries[i]);
	    }
	}
	t1 = clock();
	tNaivePC = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;
	

	cout << "Ejecutando allIntersect GST6..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	gst.tr_allIntersect(queries[i]);
	    }
	}
	t1 = clock();
	tGST6 = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;


	cout << "operacion\trutas\tqueries\ttnaive\ttnpc\ttgst6" << endl;
	cout << "allIntersect\t" << gst.n_rutas << "\t" << num_queries << "\t" << tNaive << "\t" << tNaivePC << "\t" << tGST6 << "\t[us]" << endl;

	/*****************	allContained 	*****************/

	cout << "Ejecutando allContained Naive..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	tr_allContained(rutas, queries[i]);
	    }
	}
	t1 = clock();
	tNaive = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;
	

	cout << "Ejecutando allContained NaivePC..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	npc.allContained(queries[i]);
	    }
	}
	t1 = clock();
	tNaivePC = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;
	

	cout << "Ejecutando allContained GST6..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	gst.tr_allContained6(queries[i]);
	    }
	}
	t1 = clock();
	tGST6 = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;


	cout << "operacion\trutas\tqueries\ttnaive\ttnpc\ttgst6" << endl;
	cout << "allContained\t" << gst.n_rutas << "\t" << num_queries << "\t" << tNaive << "\t" << tNaivePC << "\t" << tGST6 << "\t[us]" << endl;

	return 0;
}
