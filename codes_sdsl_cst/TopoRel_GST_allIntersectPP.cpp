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

	for(int i=0; i<nQueries; i++){
		// Carga de la consulta en x
		int x;
		queries >> x;
		// Obtener resultados de allIntersect
		unordered_map<int,int> res = gst.tr_allIntersectPP(x);
		cout << "id:" << x << "(" << res.size() << ")" << endl;
		for(const auto& pair:res){
	        cout << pair.first << ": " << pair.second << endl;
	    }
	}	
	
	queries.close();
	return 0;
}