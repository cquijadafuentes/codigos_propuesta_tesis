/*
	Toma de tiempo promedio para consultas de allcontain sobre implementaciones
	naive, naiveprecomp y gst5 (desde el archivo)
	Necesita el nombre del archivo para construir el gst5
	pero también necesita la entrada para las consultas naive y naivepc
*/
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <map>
#include "TopoRel_Naive.hpp"
#include "TopoRel_Naive_PreComp.hpp"
#include "TopoRel_GST_5.hpp"

using namespace std;

double tiempos_naive(vector<vector<int>> rutas, int queries);
double tiempos_naivePC(vector<vector<int>> rutas, int n_stops, int queries);
double tiempos_GST_5(string filename, int queries);

int main(int argc, char const *argv[]){
	int queries = 0;
	if(argc < 2){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <input_filename> [<numb_queries>]" << endl;
		return 0;
	}
	string filename(argv[1]);
	if(argc > 2){
		queries = (int) atoi(argv[2]);
	}
	int n_rutas, n_t, n_stops, aux;
	vector<vector<int>> rutas;
    cin >> n_rutas >> n_stops;
	for(int j = 0; j < n_rutas; j++){
		cin >> n_t;
        vector<int> vt(n_t, 0);
        for(int i = 0; i < n_t; i++){
            cin >> vt[i];
        }
        rutas.push_back(vt);
	}
	if(queries == 0){
		queries = n_rutas;
	}
	cout << "Rutas: " << rutas.size() << endl;
	cout << "Consultas a realizar: " << queries << endl;
	double t_naive = tiempos_naive(rutas, queries);
	double t_naivePC = tiempos_naivePC(rutas, n_stops, queries);
	double t_GST_5 = tiempos_GST_5(filename, queries);

	cout << "N°Rutas\ttNaive\ttNaivePC\ttGST5" << endl;
	cout << n_rutas << "\t" << t_naive << "\t" << t_naivePC << "\t" << t_GST_5 << "\t[us]" << endl;
}

double tiempos_naive(vector<vector<int>> rutas, int queries){
	cout << "Ejecutando Naive ... " << endl;
	int cont = 0;
	unsigned t0;
    unsigned t1;
    t0 = clock();
    for(int i=0; i<queries; i++){
    	cont += (tr_allContain(rutas, i)).size();
    }
	t1 = clock();
	double tiempo = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries)* 1000000;
	double promConten = (0.0 + cont) / queries;
	cout << "Naive encontró " << cont << " relaciones contenidas en total, ";
	cout << promConten << " contenidas en promedio." << endl;
	return tiempo;
}

double tiempos_naivePC(vector<vector<int>> rutas, int n_stops, int queries){
	cout << "Creando NaivePC ... " << endl;
	TopoRelNaivePreComp trpc(rutas, n_stops);
	cout << "Ejecutando consultas NaivePC..." << endl;
	int cont = 0;
	unsigned t0;
    unsigned t1;
    t0 = clock();    
    for(int i=0; i<queries; i++){
    	cont += (trpc.allContain(i)).size();
    }
	t1 = clock();
	double tiempo = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries)* 1000000;
	double promConten = (0.0 + cont) / queries;
	cout << "NaivePC encontró " << cont << " relaciones contenidas en total, ";
	cout << promConten << " contenidas en promedio." << endl;
	return tiempo;
}

double tiempos_GST_5(string filename, int queries){
	cout << "Creando GST5 ... " << endl;
	TopoRelGST_5 tt(filename);
	cout << "Ejecutando consultas GST5..." << endl;
	int cont = 0;
	unsigned t0;
    unsigned t1;
    t0 = clock();
    for(int i=0; i<queries; i++){
    	cont += (tt.tr_allContain(i)).size();
    }
	t1 = clock();
	double tiempo = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries)* 1000000;
	double promConten = (0.0 + cont) / queries;
	cout << "GST5 encontró " << cont << " relaciones contenidas en total, ";
	cout << promConten << " contenidas en promedio." << endl;
	return tiempo;
}