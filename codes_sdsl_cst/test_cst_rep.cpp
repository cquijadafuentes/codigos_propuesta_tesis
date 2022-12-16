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
	if(argc < 3){
		cout << "ERROR! Faltan argumentos. Ejemplo: " << endl;
		cout << argv[0] << " <largo_permutacion> <cantidad_concatenaciones_max>" << endl;
		return -1;
	}

	int sigma = (int) atoi(argv[1]);
	int rep = (int) atoi(argv[2]);
	cout << "largo alfabeto: " << sigma << endl;
	cout << "repeticiones exp: " << rep << endl;

	vector<int> x_shuffled(sigma);
	for(int i=0; i<sigma; i++){
		x_shuffled[i] = i+1;
	}
	random_shuffle(begin(x_shuffled), end(x_shuffled), myrandom);

	cout << "N\tintv-unc{[B]}\t";
	cout << "sct3-unc_{[B]}\tsct3-unc_{[us]}\tsada-unc_{[B]}\tsada-unc_{[us]}\tintv-suc{[B]}\t";
	cout << "sct3-suc_{[B]}\tsct3-suc_{[us]}\tsada-suc_{[B]}\tsada-suc_{[us]}" << endl;
	for(int i=1; i<=rep; i++){
		int lExp = sigma * i;
		int_vector<> ivShuffle(lExp);
		for(int j=0; j<lExp; j++){
			ivShuffle[j] = x_shuffled[j%sigma];
		}
		cout << lExp << "\t" << size_in_bytes(ivShuffle) << "\t";

		// construir sin comprimir
		construir_sct3(ivShuffle);
		construir_sada(ivShuffle);

		util::bit_compress(ivShuffle);
		cout << size_in_bytes(ivShuffle) << "\t";

		// construir comprimidos
		construir_sct3(ivShuffle);
		construir_sada(ivShuffle);

		cout << endl;
	}

	
	return 0;
}