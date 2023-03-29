#include <iostream>
#include "TopoRel_Naive_PreComp.hpp"

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
		cout << argv[0] << " <output_filename>" << endl;
		return 0;
	}

	string filename(argv[1]);
	TopoRelNaivePreComp tpnPC(filename);

	cout << "Cantidad de rutas: " << tpnPC.n_rutas << endl;
	cout << "Cantidad de stops: " << tpnPC.n_stops << endl;

	tpnPC.sizeEstructura();

	return 0;
}
