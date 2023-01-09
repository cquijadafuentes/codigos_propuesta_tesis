#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <map>
#include "TopoRel_Naive.hpp"
#include "TopoRel_Naive_PreComp.hpp"
#include "TopoRel_GST_4.hpp"

using namespace std;

double tiempos_naive(vector<vector<int>> rutas);
double tiempos_naivePC(vector<vector<int>> rutas, int n_stops);
double tiempos_GST_4(vector<vector<int>> rutas, int n_stops);

int main(int argc, char const *argv[]){

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

	double t_naive = tiempos_naive(rutas);
	double t_naivePC = tiempos_naivePC(rutas, n_stops);
	double t_GST_4 = tiempos_GST_4(rutas, n_stops);

	cout << "N°Rutas\ttNaive\ttNaivePC\ttGST4" << endl;
	cout << n_rutas << "\t" << t_naive << "\t" << t_naivePC << "\t" << t_GST_4 << "\t[us]" << endl;
}

double tiempos_naive(vector<vector<int>> rutas){
	int cont = 0;
	unsigned t0;
    unsigned t1;
    t0 = clock();
    for(int i=0; i<rutas.size(); i++){
    	cont += (tr_allContain(rutas, i)).size();
    }
	t1 = clock();
	double tiempo = (((double)(t1 - t0)) / CLOCKS_PER_SEC) * 1000000;
	double promConten = (0.0 + cont) / rutas.size();
	cout << "Naive encontró " << cont << " relaciones contenidas en total, ";
	cout << promConten << " contenidas en promedio." << endl;
	return tiempo;
}

double tiempos_naivePC(vector<vector<int>> rutas, int n_stops){
	TopoRelNaivePreComp trpc(rutas, n_stops);
	int cont = 0;
	unsigned t0;
    unsigned t1;
    t0 = clock();    
    for(int i=0; i<rutas.size(); i++){
    	cont += (trpc.allContain(i)).size();
    }
	t1 = clock();
	double tiempo = (((double)(t1 - t0)) / CLOCKS_PER_SEC) * 1000000;
	double promConten = (0.0 + cont) / rutas.size();
	cout << "NaivePC encontró " << cont << " relaciones contenidas en total, ";
	cout << promConten << " contenidas en promedio." << endl;
	return tiempo;
}

double tiempos_GST_4(vector<vector<int>> rutas, int n_stops){
	TopoRelGST_4 tt(rutas, n_stops);
	int cont = 0;
	unsigned t0;
    unsigned t1;
    t0 = clock();
    for(int i=0; i<rutas.size(); i++){
    	cont += (tt.tr_allContain(i)).size();
    }
	t1 = clock();
	double tiempo = (((double)(t1 - t0)) / CLOCKS_PER_SEC) * 1000000;
	double promConten = (0.0 + cont) / rutas.size();
	cout << "GST4 encontró " << cont << " relaciones contenidas en total, ";
	cout << promConten << " contenidas en promedio." << endl;
	return tiempo;
}
