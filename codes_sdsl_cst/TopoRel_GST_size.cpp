#include <iostream>
#include "TopoRel_GST.hpp"

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
	
	TopoRelGST tr_gst(argv[1]);
	tr_gst.sizeToPlot();

	return 0;
}
