#include <iostream>
#include <vector>
#include <fstream>
#include "TopoRel_Naive.hpp"

/*
	Calcula los resultados de la operación <id_operacion> para 
	las rutas en el conjunto del archivo <input_file> usando 
	las consultas de <querires_file>
*/

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <input_file> <queries_file> <id_operacion>" << endl;
		cout << "Donde <id_operacion> es: ";
		cout << "(1) allContain - (2) allContained - (3) allEqual - (4) allIntersect" << endl;
		return 0;
	}

	int id_operacion = (int) atoi(argv[3]);
	if(id_operacion < 1 || id_operacion > 4){
		cout << "Error! <id_operacion> no corresponde." << endl;
		return 0;
	}

	ifstream entrada(argv[1], ifstream::in);

	int n, x, aux, max;
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
	}
	entrada.close();

	ifstream consultas(argv[2], ifstream::in);
	int n_consultas;
	consultas >> n_consultas;
	cout << n_consultas << endl;
	for(int i=0; i<n_consultas; i++){
		consultas >> x;
		vector<int> res;
		switch(id_operacion){
			case 1:
				res = tr_allContain(rutas, x);
				break;
			case 2:
				res = tr_allContained(rutas, x);
				break;
			case 3:
				res = tr_allEqual(rutas, x);
				break;
			case 4:
				res = tr_allIntersect(rutas, x);
				break;
			default:
				cout << "Error! Operación no definida." << endl;
				return 0;
		}
		cout << res.size() << " ";
		for(int j=0; j<res.size(); j++){
			cout << res[j] << " ";
		}
		cout << endl;
	}
	consultas.close();
	return 0;
}
