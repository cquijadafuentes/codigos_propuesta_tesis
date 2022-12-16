#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <sdsl/vectors.hpp>
#include <sdsl/suffix_trees.hpp>

using namespace std;
using namespace sdsl;

int myrandom(int i){
	srand(unsigned (time(0)));
	return rand() % i;
}

void construir_sct3(int_vector<> iv){
	cst_sct3<csa_wt<wt_int<rrr_vector<>>>> cst_sct3;
	unsigned t0 = clock();
	construct_im(cst_sct3, iv);
	unsigned t1 = clock();
    double t = (((double)(t1 - t0))/CLOCKS_PER_SEC) * 1000000;
    long long bytesCST = size_in_bytes(cst_sct3);
    cout << bytesCST << "\t" << t << "\t";
}

void construir_sada(int_vector<> iv){
	cst_sada<csa_wt<wt_int<rrr_vector<>>>> cst_sada;
	unsigned t0 = clock();	
	construct_im(cst_sada, iv);
	unsigned t1 = clock();
    double t = (((double)(t1 - t0))/CLOCKS_PER_SEC) * 1000000;
    long long bytesCST = size_in_bytes(cst_sada);
    cout << bytesCST << "\t" << t << "\t";
}

void print_vector(vector<int> x){
	for(int i=0; i<x.size(); i++){
		cout << x[i] << " ";
	}
	cout << endl;
}

void print_intvector(int_vector<> x){
	for(int i=0; i<x.size(); i++){
		cout << x[i] << " ";
	}
	cout << endl;
}

int main(int argc, char const *argv[]){

	int n_routes, n_t, n_stops, aux;
	vector<vector<int>> routes;
    cin >> n_routes >> n_stops;
	for(int j = 0; j < n_routes; j++){
		cin >> n_t;
        vector<int> vt(n_t, 0);
        for(int i = 0; i < n_t; i++){
            cin >> vt[i];
        }
        routes.push_back(vt);
	}

	int porcion = n_routes / 10;

	cout << "N\tintv-unc{[B]}\t";
	cout << "sct3-unc_{[B]}\tsct3-unc_{[us]}\tsada-unc_{[B]}\tsada-unc_{[us]}\tintv-suc{[B]}\t";
	cout << "sct3-suc_{[B]}\tsct3-suc_{[us]}\tsada-suc_{[B]}\tsada-suc_{[us]}" << endl;
	for(int i=1; i<=10; i++){
		int canRutasUsar = porcion * i;
		int largoRutas = 0;
		for(int j=0; j<canRutasUsar; j++){
			largoRutas += routes[j].size();
		}
		int_vector<> ivParte(largoRutas);
		int piv = 0;
		for(int j=0; j<canRutasUsar; j++){
			for(int k=0; k<routes[j].size(); k++){
				ivParte[piv++] = routes[j][k];
			}
		}

		cout << largoRutas << "\t" << size_in_bytes(ivParte) << "\t";
		
		// construir sin comprimir
		construir_sct3(ivParte);
		construir_sada(ivParte);

		util::bit_compress(ivParte);
		cout << size_in_bytes(ivParte) << "\t";

		// construir comprimidos
		construir_sct3(ivParte);
		construir_sada(ivParte);

		cout << endl;
	}

	
	return 0;
}