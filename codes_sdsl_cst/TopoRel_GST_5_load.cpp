#include <iostream>
#include "TopoRel_GST_5.hpp"

using namespace std;
using namespace sdsl;

void print_bool(bool x){
	if(x){
		cout << "true";
	}else{
		cout << "false";
	}
}

int main(int argc, char const *argv[]){

	if(argc < 2){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <input_filename>" << endl;
		return 0;
	}
	
	TopoRelGST_5 tr_gst(argv[1]);
	cout << "Información del conjunto:" << endl;
	cout << "Nº de rutas: " << tr_gst.n_rutas << endl;
	cout << "Largo de la secuencia total: " << tr_gst.n_concat << endl;
	cout << "Nº de stops: " << tr_gst.n_stops << endl;
	cout << "Información del CompressedSuffixTree:" << endl;
    cout << "Cantidad de nodos: " << tr_gst.cst.nodes() << endl;
    cout << "Cantidad de hojas: " << tr_gst.cst.size() << endl;
	return 0;
}