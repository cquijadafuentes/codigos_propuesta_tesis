/*
	Toma de tiempo promedio para consultas grupales sobre implementaciones
	naive, *naiveprecomp y *gst5
	*= desde el archivo
	Necesita el nombre del archivo para construcción
	pero también necesita la entrada para las consultas naive
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

void t_allContain(vector<vector<int>> &rutas, TopoRelNaivePreComp &trnpc, TopoRelGST_5 &gst, vector<int> &queries, int repeticiones);
void t_allEqual(vector<vector<int>> &rutas, TopoRelNaivePreComp &trnpc, TopoRelGST_5 &gst, vector<int> &queries, int repeticiones);
void t_allContained(vector<vector<int>> &rutas, TopoRelNaivePreComp &trnpc, TopoRelGST_5 &gst, vector<int> &queries, int repeticiones);
vector<int> generarQueries(int n_rutas, int num_queries);

int main(int argc, char const *argv[]){
	if(argc < 6){
		cout << "Error! faltan argumentos:" << endl;
		cout << argv[0] << " <input_file_GST5> <input_file_NaivePC> <input_file_txt> num_queries repeticiones" << endl;
		return 0;
	}
	TopoRelGST_5 trgst(argv[1]);
	TopoRelNaivePreComp trnpc(argv[2]); 
	ifstream ifsNaive(argv[3], ifstream::in); 
	int num_queries = (int) atoi(argv[4]);
	int repeticiones = (int) atoi(argv[5]);
	if(num_queries < 1 || repeticiones < 1){
		cout << "Error! en la cantidad de num_queries/repeticiones: " << num_queries << endl;
		return 0;
	}

	int n_rutas, n_t, n_stops, aux;
	vector<vector<int>> rutas;
    ifsNaive >> n_rutas >> n_stops;
	for(int j = 0; j < n_rutas; j++){
		ifsNaive >> n_t;
        vector<int> vt(n_t, 0);
        for(int i = 0; i < n_t; i++){
            ifsNaive >> vt[i];
        }
        rutas.push_back(vt);
	}

	if(num_queries > n_rutas){
		num_queries = n_rutas;
	}
	cout << "Rutas: " << rutas.size() << endl;
	cout << "Consultas a realizar: " << num_queries << endl;
	cout << "Repeticiones a realizar: " << repeticiones << endl;
	vector<int> queries = generarQueries(n_rutas, num_queries);
	
	cout << "Operacion\trutas\tqueries\ttNaive\ttNaivePC\ttGST5" << endl;
	t_allContain(rutas, trnpc, trgst, queries, repeticiones);
	t_allEqual(rutas, trnpc, trgst, queries, repeticiones);
	t_allContained(rutas, trnpc, trgst, queries, repeticiones);

	

	return 0;
}

void t_allContain(vector<vector<int>> &rutas, TopoRelNaivePreComp &trnpc, TopoRelGST_5 &gst, vector<int> &queries, int repeticiones){
	cout << "Ejecutando Naive ... " << endl;
	unsigned t0, t1;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	tr_allContain(rutas, queries[i]);
	    }
	}
	t1 = clock();
	double tiempoNaive = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;

	cout << "Ejecutando consultas NaivePC..." << endl;
    t0 = clock();    
    for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	trnpc.allContain(queries[i]);
	    }
	}
	t1 = clock();
	double tiempoNaivePC = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;

	cout << "Ejecutando consultas GST5..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	gst.tr_allContain(queries[i]);
	    }
	}
	t1 = clock();
	double tiempoGST = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;

	cout << "allContain\t" << rutas.size() << "\t" << queries.size() << "\t" << tiempoNaive << "\t" << tiempoNaivePC << "\t" << tiempoGST << "\t[us]" << endl;
}

void t_allEqual(vector<vector<int>> &rutas, TopoRelNaivePreComp &trnpc, TopoRelGST_5 &gst, vector<int> &queries, int repeticiones){
	cout << "Ejecutando Naive ... " << endl;
	unsigned t0, t1;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	tr_allEqual(rutas, queries[i]);
	    }
	}
	t1 = clock();
	double tiempoNaive = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;

	cout << "Ejecutando consultas NaivePC..." << endl;
    t0 = clock();    
    for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	trnpc.allEqual(queries[i]);
	    }
	}
	t1 = clock();
	double tiempoNaivePC = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;

	cout << "Ejecutando consultas GST5..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	gst.tr_allEqual(queries[i]);
	    }
	}
	t1 = clock();
	double tiempoGST = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;

	cout << "allEqual\t" << rutas.size() << "\t" << queries.size() << "\t" << tiempoNaive << "\t" << tiempoNaivePC << "\t" << tiempoGST << "\t[us]" << endl;
}

void t_allContained(vector<vector<int>> &rutas, TopoRelNaivePreComp &trnpc, TopoRelGST_5 &gst, vector<int> &queries, int repeticiones){
	cout << "Ejecutando Naive ... " << endl;
	unsigned t0, t1;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	tr_allContained(rutas, queries[i]);
	    }
	}
	t1 = clock();
	double tiempoNaive = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;

	cout << "Ejecutando consultas NaivePC..." << endl;
    t0 = clock();    
    for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	trnpc.allContained(queries[i]);
	    }
	}
	t1 = clock();
	double tiempoNaivePC = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;

	cout << "Ejecutando consultas GST5..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	gst.tr_allContained(queries[i]);
	    }
	}
	t1 = clock();
	double tiempoGST = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;

	cout << "allContained\t" << rutas.size() << "\t" << queries.size() << "\t" << tiempoNaive << "\t" << tiempoNaivePC << "\t" << tiempoGST << "\t[us]" << endl;
}

vector<int> generarQueries(int n_rutas, int num_queries){
	srand((unsigned) time(NULL));
	set<int> ms;
	while(ms.size() < num_queries){
		ms.insert(rand() % n_rutas);
	}

	vector<int> mv(ms.begin(), ms.end());
	return mv;
}