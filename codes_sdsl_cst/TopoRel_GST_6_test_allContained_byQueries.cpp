#include <iostream>
#include "TopoRel_GST_6.hpp"

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){

	if(argc < 4){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <input_filename> <queries_file> <results_file>" << endl;
		return 0;
	}
	
	TopoRelGST_6 gst(argv[1]);
	
	int nQueries;
	ifstream queries(argv[2], ifstream::in);
	queries >> nQueries;
	int nResults;
	ifstream results(argv[3], ifstream::in);
	results >> nResults;
	if(nQueries != nResults){
		cout << "Error! El número de consultas del archivo " << argv[1];
		cout << "(" << nQueries << ") no coincide con ";
		cout << "la cantidad de resultados del archivo " << argv[2];
		cout << "(" << nResults << ")" << endl;
		return 0;
	}
	int cantFallasV1 = 0;
	int cantFallasV2 = 0;
	int cantFallasV3 = 0;
	for(int i=0; i<nQueries; i++){
		bool fallaV1 = false;
		bool fallaV2 = false;
		bool fallaV3 = false;
		// Carga de la consulta en x
		int x;
		queries >> x;
		// Carga de los resultados de la consulta i
		int y;
		results >> y;	// Cantidad de elementos del resultado ii
		vector<int> naiveResult(y);
		for(int ii=0; ii<y; ii++){
			results >> naiveResult[ii];
		}
		// Obtener resultados de la versión 1
		vector<int> resV1 = gst.tr_allContained(x);
		// Comparar los resultados de la versión 1
		if(resV1.size() != y){
			fallaV1 = true;
		}else{
			for(int j=0; j<y; j++){
				if(resV1[j] != naiveResult[j]){
					fallaV1 = true;
				}
			}
		}
		// Obtener resultados de la versión 2
		vector<int> resV2 = gst.tr_allContained2(x);
		// Comparar los resultados de la versión 2
		if(resV2.size() != y){
			fallaV2 = true;
		}else{
			for(int j=0; j<y; j++){
				if(resV2[j] != naiveResult[j]){
					fallaV2 = true;
				}
			}
		}
		// Obtener resultados de la versión 3
		vector<int> resV3 = gst.tr_allContained3(x);
		// Comparar los resultados de la versión 3
		if(resV3.size() != y){
			fallaV3 = true;
		}else{
			for(int j=0; j<y; j++){
				if(resV3[j] != naiveResult[j]){
					fallaV3 = true;
				}
			}
		}
		// Contabilizar fallas
		if(fallaV1){
			cantFallasV1++;
		}
		if(fallaV2){
			cantFallasV2++;
		}
		if(fallaV3){
			cantFallasV3++;
		}
	}
	cout << "nRutas\tnQueries\tfails_{V1}\tfails_{V2}\tfails_{V3}" << endl;
	cout << gst.n_rutas << "\t";
	cout << nQueries << "\t";
	cout << cantFallasV1 << "\t";
	cout << cantFallasV2 << "\t";
	cout << cantFallasV3 << endl;
	queries.close();
	results.close();
    return 0;
}