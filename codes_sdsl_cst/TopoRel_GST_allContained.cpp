#include <iostream>
#include "TopoRel_GST.hpp"

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <input_filename> <queries_file>" << endl;
		return 0;
	}
	
	TopoRelGST gst(argv[1]);
	
	int nQueries;
	ifstream queries(argv[2], ifstream::in);
	queries >> nQueries;

	cout << "Resultados de allContained:" << endl;
	for(int i=0; i<nQueries; i++){
		// Carga de la consulta en x
		int x;
		queries >> x;
		// Obtener resultados de allContained
		vector<int> res = gst.tr_allContained(x);

		cout << "Query " << i+1 << " con " << res.size() << " resultados: ";
		for(int j=0; j<res.size(); j++){
			cout << res[j] << " ";
		}
		cout << endl;
	}	
	
	queries.close();
	return 0;
}