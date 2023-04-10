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
	vector<int> visitedNodesV1(nQueries, 0);
	vector<int> visitedNodesV2(nQueries, 0);
	vector<int> visitedNodesV3(nQueries, 0);
	double promVisitedNodesV1 = 0.0;
	double promVisitedNodesV2 = 0.0;
	double promVisitedNodesV3 = 0.0;
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
		gst.howManyNodes = 0;
		vector<int> resV1 = gst.tr_allContained(x);
		visitedNodesV1[i] = gst.howManyNodes;
		promVisitedNodesV1 += gst.howManyNodes;
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
		gst.howManyNodes = 0;
		vector<int> resV2 = gst.tr_allContained2(x);
		visitedNodesV2[i] = gst.howManyNodes;
		promVisitedNodesV2 += gst.howManyNodes;
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
		gst.howManyNodes = 0;
		vector<int> resV3 = gst.tr_allContained3(x);
		visitedNodesV3[i] = gst.howManyNodes;
		promVisitedNodesV3 += gst.howManyNodes;
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
	sort(visitedNodesV1.begin(), visitedNodesV1.end());
	sort(visitedNodesV2.begin(), visitedNodesV2.end());
	sort(visitedNodesV3.begin(), visitedNodesV3.end());
	promVisitedNodesV1 /= nQueries;
	promVisitedNodesV2 /= nQueries;
	promVisitedNodesV3 /= nQueries;
	cout << "fallas\tnRutas\tnQueries\tfails_{V1}\tfails_{V2}\tfails_{V3}" << endl;
	cout << "fallas\t" << gst.n_rutas << "\t";
	cout << nQueries << "\t";
	cout << cantFallasV1 << "\t";
	cout << cantFallasV2 << "\t";
	cout << cantFallasV3 << endl;
	queries.close();
	results.close();
	cout << "visNods\tnRutas\tpromV1\tpromV2\tpromV3\tminV1\tminV2\tminV3\tmedV1\tmedV2\tmedV3\tmaxV1\tmaxV2\tmaxV3" << endl;
	cout << "visNods\t" << gst.n_rutas << "\t";
	cout << promVisitedNodesV1 << "\t" << promVisitedNodesV2 << "\t" << promVisitedNodesV3 << "\t";
	cout << visitedNodesV1[0] << "\t" << visitedNodesV2[0] << "\t" << visitedNodesV3[0] << "\t";
	cout << visitedNodesV1[nQueries/2] << "\t" << visitedNodesV2[nQueries/2] << "\t" << visitedNodesV3[nQueries/2] << "\t";
	cout << visitedNodesV1[nQueries-1] << "\t" << visitedNodesV2[nQueries-1] << "\t" << visitedNodesV3[nQueries-1] << endl;
	cout << endl;
    return 0;
}