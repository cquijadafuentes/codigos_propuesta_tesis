#include <iostream>
#include <vector>
#include <fstream>
#include "TopoRel_Naive.hpp"

/*
	Calcula los resultados de la operación allContained para 
	las rutas en el conjunto del archivo <input_file> usando 
	las consultas de <querires_file>
*/

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <input_file> <queries_file>" << endl;
		return 0;
	}

	ifstream entrada(argv[1], ifstream::in);

	int n, x, aux, max;
	entrada >> n >> max;
	vector<vector<int>> lx(n);
	for(int i = 0; i < n; i++){
		entrada >> x;
		for(int j=0; j<x; j++){
			entrada >> aux;
			lx[i].push_back(aux);
		}
	}
	entrada.close();

	ifstream consultas(argv[2], ifstream::in);
	int n_consultas;
	consultas >> n_consultas;
	cout << n_consultas << endl;
	for(int i=0; i<n_consultas; i++){
		consultas >> x;
		vector<int> res = tr_allContained(lx, x);
		cout << res.size() << " ";
		for(int j=0; j<res.size(); j++){
			cout << res[j] << " ";
		}
		cout << endl;
	}
	consultas.close();
	return 0;
}
