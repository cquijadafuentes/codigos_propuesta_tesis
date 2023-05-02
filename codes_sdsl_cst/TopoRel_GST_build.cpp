#include <iostream>
#include "TopoRel_GST.hpp"

using namespace std;
using namespace sdsl;

/*
	Construye archivos gst desde rutas en formato txt
*/

void print_bool(bool x){
	if(x){
		cout << "true";
	}else{
		cout << "false";
	}
}

int main(int argc, char const *argv[]){

	if(argc < 3){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <input_filename.txt> <output_filename.gst>" << endl;
		cout << "Input_File format:" << endl;
		cout << "cant_rutas max_stop" << endl;
		cout << "cant_stops_r1 r1_st1 r1_st2 ···" << endl;
		cout << "cant_stops_r2 r2_st1 r2_st2 ···" << endl;
		cout << "···" << endl << endl;
		return 0;
	}

	ifstream txtInput(argv[1], ifstream::in);
	
	int nr, max;
	txtInput >> nr >> max;
	vector<vector<int>> vi;
	for(int i = 0; i < nr; i++){
		int n, x;
		txtInput >> n;
		vector<int> v;
		for(int j = 0; j < n; j++){
			txtInput >> x;
			v.push_back(x);
		}
		vi.push_back(v);
	}
	txtInput.close();

	TopoRelGST gst(vi, max);

	string filename(argv[2]);
	if(gst.save(filename)){
		cout << "Se ha guardado exitosamente " << argv[2] << endl;
	}else{
		cout << "Falla en el guardado." << endl;
	}	

	return 0;
}