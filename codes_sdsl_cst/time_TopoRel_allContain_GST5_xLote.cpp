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
double tiempos_naivePC(TopoRelNaivePreComp trpc, int queries);
double tiempos_GST_5(TopoRelGST_5, int queries);

int main(int argc, char const *argv[]){
	int queries = 0;
	if(argc < 4){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <inputGST5_filename> <tam_salto_queries> <cant_saltos_queries>" << endl;
		return 0;
	}
	string filename(argv[1]);
	int tamSalto = (int) atoi(argv[2]);
	int cantSaltos = (int) atoi(argv[3]);

	cout << "Lectura de rutas..." << endl;
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
	cout << "Fin lectura de rutas." << endl;	
	cout << "Rutas: " << rutas.size() << endl;
	cout << "Stops: " << n_stops << endl;
	cout << "Construyendo NaivePC..." << endl;
	TopoRelNaivePreComp trpc(rutas, n_stops);
	cout << "NaivePC construido." << endl;
	cout << "Construyendo GST5..." << endl;
	TopoRelGST_5 tgst5(filename);
	cout << "GST5 construido." << endl;
	cout << "Se realizarán " << cantSaltos << " de tamaño " << tamSalto << " cada uno." << endl;
	cout << "****************************************************" << endl;
	cout << "NºQ\ttNaive\ttNaivePC\ttGST5" << endl;
	for(int i=1; i<=cantSaltos; i++){
		int queries = i * tamSalto;
		double t_naive = tiempos_naive(rutas, queries);
		double t_naivePC = tiempos_naivePC(trpc, queries);
		double t_GST_5 = tiempos_GST_5(tgst5, queries);
		cout << queries << "\t" << t_naive << "\t" << t_naivePC << "\t" << t_GST_5 << "\t[us]" << endl;
	}
}

double tiempos_naive(vector<vector<int>> rutas, int queries){
	unsigned t0;
    unsigned t1;
    t0 = clock();
    for(int i=0; i<queries; i++){
    	tr_allContain(rutas, i);
    }
	t1 = clock();
	double tiempo = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries)* 1000000;
	return tiempo;
}

double tiempos_naivePC(TopoRelNaivePreComp trpc, int queries){
	unsigned t0;
    unsigned t1;
    t0 = clock();    
    for(int i=0; i<queries; i++){
    	trpc.allContain(i);
    }
	t1 = clock();
	double tiempo = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries)* 1000000;
	return tiempo;
}

double tiempos_GST_5(TopoRelGST_5 tt, int queries){
	unsigned t0;
    unsigned t1;
    t0 = clock();
    for(int i=0; i<queries; i++){
    	tt.tr_allContain(i);
    }
	t1 = clock();
	double tiempo = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries)* 1000000;
	return tiempo;
}
