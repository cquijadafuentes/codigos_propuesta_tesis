#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <input_file>" << endl;
		return 0;
	}
	// Cargando datos
	ifstream entrada(argv[1], ifstream::in);
	int n, x, aux, max;
	entrada >> n >> max;
	int cantSeqConcat = 0;
	for(int i = 0; i < n; i++){
		entrada >> x;
		for(int j=0; j<x; j++){
			entrada >> aux;
		}
		cantSeqConcat += (x+1); 	// La secuencia completa + separador entre secuencias
	}
	cantSeqConcat += 1; 		// El caracter de fin de secuencia (0)
	entrada.close();
	double logaritmo = log2(cantSeqConcat);
	double cielo = ceil(logaritmo);
	double bytes = (cielo * cantSeqConcat)/8;
	cout << "rutas\tsecConcat\tbitsxNum\tbytes" << endl;
	cout << n << "\t" << cantSeqConcat << "\t" << cielo << "\t" << bytes << endl;
	return 0; 
}