/*
	Compara la construcción de un CST con fines de secuencia iguales
	vs uno con fines de secuencia distinto por cada secuencia
	
	Compilar con: 
		g++ -std=c++11 -g -O0 -DNDEBUG -I ~/include -L ~/lib test_cst_marcasFinDistintas.cpp -o test_cst_marcasFinDistintas -lsdsl -ldivsufsort -ldivsufsort64
*/

#include <iostream>
#include <sdsl/vectors.hpp>
#include <sdsl/suffix_trees.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/suffix_trees.hpp>

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){

	if(argc < 2){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << "<input_filename.txt>" << endl;
		cout << "Input_File format:" << endl;
		cout << "cant_rutas max_stop" << endl;
		cout << "cant_stops_r1 r1_st1 r1_st2 ···" << endl;
		cout << "cant_stops_r2 r2_st1 r2_st2 ···" << endl;
		cout << "···" << endl << endl;
		return -1;
	}


	ifstream txtInput(argv[1], ifstream::in);
	
	int n_rutas, max;
	txtInput >> n_rutas >> max;
	vector<vector<int>> rutas;
	for(int i = 0; i < n_rutas; i++){
		int n, x;
		txtInput >> n;
		vector<int> v;
		for(int j = 0; j < n; j++){
			txtInput >> x;
			v.push_back(x);
		}
		rutas.push_back(v);
	}
	txtInput.close();


	int n_concat = 0;
	int maxID = 0;  // Para obtener símbolo para fin de secuencia
    for(int i = 0; i < n_rutas; i++){
        n_concat += (rutas[i].size()+1);   // largo de ruta + fin_char
        // verificar el max_char del final de stops
        for(int j = 0; j < rutas[i].size(); j++){
            if(rutas[i][j] <= 0){
                cout << "Error! identificadores con valor <= 0 (i:" << i << ", j:" << j <<")" << endl;
                cout << "valor encontrado: " << rutas[i][j] << " en ruta de tamaño " << rutas[i].size() << endl;
                return -1;
            }
            if(maxID < rutas[i][j]){
                maxID = rutas[i][j];
            }
        }
    }
    int finSec = maxID+1;
    int_vector<> secConcat_old(n_concat);	// con una marca de fin de secuencia único
    int_vector<> secConcat_new(n_concat);	// con una marca de fin de secuencia distinto por cada ruta

    int pv = 0;
    int tr = n_rutas;
    // Concatenar rutas
    for(int i = 0; i < n_rutas; i++){
        for(int j = 0; j < rutas[i].size(); j++){
            secConcat_old[pv] = rutas[i][j];
            secConcat_new[pv] = rutas[i][j];
            pv++;
        }
        secConcat_old[pv] = finSec;
        secConcat_new[pv] = finSec+i;
        pv++;
    }

    cst_sada<csa_wt<wt_int<rrr_vector<>>>> cst_old;
    construct_im(cst_old, secConcat_old);
    
    cst_sada<csa_wt<wt_int<rrr_vector<>>>> cst_new;
    construct_im(cst_new, secConcat_new);

    cout << "Versio\tNrutas\tNnodos\tNhojas\tSize" << endl;
    cout << "old\t" << n_rutas << "\t" << cst_old.nodes() << "\t" << cst_old.size() << "\t" << size_in_bytes(cst_old) << endl;
    cout << "new\t" << n_rutas << "\t" << cst_new.nodes() << "\t" << cst_new.size() << "\t" << size_in_bytes(cst_new) << endl;

	return 0;
}