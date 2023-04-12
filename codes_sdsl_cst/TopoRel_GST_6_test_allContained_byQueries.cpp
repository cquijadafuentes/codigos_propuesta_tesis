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
	vector<int> statsV1(5, 0);
	vector<int> statsV2(5, 0);
	vector<int> statsV3(5, 0);
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
		gst.howManyInserts = 0;
		gst.howManyIfs = 0;
		gst.howManyidRutaDesdeCeldaDeSecConcat = 0;
		gst.howManygetLargoRuta = 0;
		vector<int> resV1 = gst.tr_allContained(x);
		visitedNodesV1[i] = gst.howManyNodes;
		promVisitedNodesV1 += gst.howManyNodes;
		statsV1[0] += gst.howManyInserts;
		statsV1[1] += gst.howManyIfs;
		statsV1[2] += gst.howManyidRutaDesdeCeldaDeSecConcat;
		statsV1[3] += gst.howManygetLargoRuta;
		statsV1[4] += resV1.size();
		// Comparar los resultados de la versión 1
		sort(resV1.begin(), resV1.end());
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
		gst.howManyInserts = 0;
		gst.howManyIfs = 0;
		gst.howManyidRutaDesdeCeldaDeSecConcat = 0;
		gst.howManygetLargoRuta = 0;
		vector<int> resV2 = gst.tr_allContained2(x);
		visitedNodesV2[i] = gst.howManyNodes;
		promVisitedNodesV2 += gst.howManyNodes;
		statsV2[0] += gst.howManyInserts;
		statsV2[1] += gst.howManyIfs;
		statsV2[2] += gst.howManyidRutaDesdeCeldaDeSecConcat;
		statsV2[3] += gst.howManygetLargoRuta;
		statsV2[4] += resV1.size();
		// Comparar los resultados de la versión 2
		sort(resV2.begin(), resV2.end());
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
		gst.howManyInserts = 0;
		gst.howManyIfs = 0;
		gst.howManyidRutaDesdeCeldaDeSecConcat = 0;
		gst.howManygetLargoRuta = 0;
		vector<int> resV3 = gst.tr_allContained3(x);
		visitedNodesV3[i] = gst.howManyNodes;
		promVisitedNodesV3 += gst.howManyNodes;
		statsV3[0] += gst.howManyInserts;
		statsV3[1] += gst.howManyIfs;
		statsV3[2] += gst.howManyidRutaDesdeCeldaDeSecConcat;
		statsV3[3] += gst.howManygetLargoRuta;
		statsV3[4] += resV1.size();
		// Comparar los resultados de la versión 3
		sort(resV3.begin(), resV3.end());
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
	queries.close();
	results.close();
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
	cout << "visNods\tnRutas\tpromV1\tpromV2\tpromV3\tminV1\tminV2\tminV3\tmedV1\tmedV2\tmedV3\tmaxV1\tmaxV2\tmaxV3" << endl;
	cout << "visNods\t" << gst.n_rutas << "\t";
	cout << promVisitedNodesV1 << "\t" << promVisitedNodesV2 << "\t" << promVisitedNodesV3 << "\t";
	cout << visitedNodesV1[0] << "\t" << visitedNodesV2[0] << "\t" << visitedNodesV3[0] << "\t";
	cout << visitedNodesV1[nQueries/2] << "\t" << visitedNodesV2[nQueries/2] << "\t" << visitedNodesV3[nQueries/2] << "\t";
	cout << visitedNodesV1[nQueries-1] << "\t" << visitedNodesV2[nQueries-1] << "\t" << visitedNodesV3[nQueries-1] << endl;
	cout << "stats\tnRutas\tnQueries\tins_{V1}\tins_{V2}\tins_{V3}\tifs_{V1}\tifs_{V2}\tifs_{V3}\tidsR_{V1}\tidsR_{V2}\tidsR_{V3}\tlarR_{V1}\tlarR_{V2}\tlarR_{V3}\tcRes_{V1}\tcRes_{V2}\tcRes_{V3}" << endl;
	cout << "visNods\t" << gst.n_rutas << "\t" << nQueries << "\t";
	cout << statsV1[0] << "\t" << statsV2[0] << "\t" << statsV3[0] << "\t";
	cout << statsV1[1] << "\t" << statsV2[1] << "\t" << statsV3[1] << "\t";
	cout << statsV1[2] << "\t" << statsV2[2] << "\t" << statsV3[2] << "\t";
	cout << statsV1[3] << "\t" << statsV2[3] << "\t" << statsV3[3] << "\t";
	cout << statsV1[4] << "\t" << statsV2[4] << "\t" << statsV3[4] << endl;
	cout << endl;
    return 0;
}