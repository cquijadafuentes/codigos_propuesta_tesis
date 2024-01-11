#include <iostream>
#include "TopoRel_Naive.hpp"

using namespace std;
using namespace sdsl;

/*
	Entrega el tamaño de la representación ingenua y el tamaño 
	de todas las repuestas para las relaciones posibles en ul
	conjunto de datos de entrada.
	Para las relaciones contain, contained y equal se registra la lista 
	con el resultado de la operación.
	Para la relación intersect se guarda el largo de la subesecuencia
	común más larga entre todas las combinaciones (i,j) donde i<=j.
	Esto último debido a que la triangular superior es equivalente a
	la triangular inferior
*/



int main(int argc, char const *argv[]){

	if(argc < 2){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <input_filename.txt>" << endl;
		cout << "Input_File format:" << endl;
		cout << "cant_rutas max_stop" << endl;
		cout << "cant_stops_r1 r1_st1 r1_st2 ···" << endl;
		cout << "cant_stops_r2 r2_st1 r2_st2 ···" << endl;
		cout << "···" << endl << endl;
		return 0;
	}

	// Cargando rutas de entrada
	ifstream entrada(argv[1], ifstream::in);
	int n, x, aux, max;
	int bytes_rutas = 0;
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
		bytes_rutas += size_in_bytes(rutas[i]);
	}
	entrada.close();
	
	// Ejecutando consultas
	
	vector<int> acum;

	cout << "allContain " << endl;
	for(int i=0; i<rutas.size(); i++){
		vector<int> res = tr_allContain(rutas, i);
		acum.push_back(res.size());
		acum.insert(acum.end(), res.begin(), res.end());
	}

	cout << "allContained" << endl;
	for(int i=0; i<rutas.size(); i++){
		vector<int> res = tr_allContained(rutas, i);
		acum.push_back(res.size());
		acum.insert(acum.end(), res.begin(), res.end());
	}

	cout << "allEqual " << endl;
	for(int i=0; i<rutas.size(); i++){
		vector<int> res = tr_allEqual(rutas, i);
		acum.push_back(res.size());
		acum.insert(acum.end(), res.begin(), res.end());
	}

	cout << "allIntersect " << endl;
	for(int i=0; i<rutas.size(); i++){
		for(int j=i; j<rutas.size(); j++){
			int x = tr_intersectPP(rutas[i], rutas[j]);
			acum.push_back(x);
		}
	}

	cout << "calculando tamaño" << endl;
	int_vector<> acumulado(acum.size());
	for(int i=0; i<acum.size(); i++){
		acumulado[i] = acum[i];
	}
	util::bit_compress(acumulado);
	// Mostrando tamaños
	cout << "rutas\tstops\tsnaive\tsrels" << endl;
	cout << n << "\t" << max << "\t" << bytes_rutas << "\t" << size_in_bytes(acumulado) << endl;

	return 0;
}