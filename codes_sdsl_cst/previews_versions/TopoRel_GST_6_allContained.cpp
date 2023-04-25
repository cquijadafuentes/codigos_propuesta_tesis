#include <iostream>
#include "TopoRel_GST_6.hpp"

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <input_filename> <queries_file> <version_allContained>" << endl;
		return 0;
	}
	
	TopoRelGST_6 gst(argv[1]);
	
	int nQueries;
	ifstream queries(argv[2], ifstream::in);
	queries >> nQueries;

	int version = (int) atoi(argv[3]);
	if(version > 3 || version < 1){
		cout << "Error! Versión ingresada no es correcta (1, 2 ó 3)." << endl;
		return 0;
	}

	cout << "Resultados de allContained con la versión " << version << endl;
	for(int i=0; i<nQueries; i++){
		// Carga de la consulta en x
		int x;
		queries >> x;
		// Obtener resultados de allContained <version_allContained>
		vector<int> res;
		if(version == 1){
			res = gst.tr_allContained(x);
		}else if(version == 2){
			res = gst.tr_allContained2(x);
		}else if(version == 3){
			res = gst.tr_allContained3(x);
		}

		cout << "Query " << i+1 << " con " << res.size() << " resultados: ";
		for(int j=0; j<res.size(); j++){
			cout << res[j] << " ";
		}
		cout << endl;
	}	
	
	queries.close();
	return 0;
}