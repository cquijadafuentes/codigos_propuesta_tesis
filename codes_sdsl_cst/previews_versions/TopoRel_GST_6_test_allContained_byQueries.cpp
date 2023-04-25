#include <iostream>
#include "TopoRel_GST_6.hpp"

// Programa para verificar los resultados de las 6 implementaciones de allContained.

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
	int cantFallasV4 = 0;
	int cantFallasV5 = 0;
	int cantFallasV6 = 0;
	vector<int> visitedNodesV1(nQueries, 0);
	vector<int> visitedNodesV2(nQueries, 0);
	vector<int> visitedNodesV3(nQueries, 0);
	vector<int> visitedNodesV4(nQueries, 0);
	vector<int> visitedNodesV5(nQueries, 0);
	vector<int> visitedNodesV6(nQueries, 0);
	vector<int> statsV1(5, 0);
	vector<int> statsV2(5, 0);
	vector<int> statsV3(5, 0);
	vector<int> statsV4(5, 0);
	vector<int> statsV5(5, 0);
	vector<int> statsV6(5, 0);
	double promVisitedNodesV1 = 0.0;
	double promVisitedNodesV2 = 0.0;
	double promVisitedNodesV3 = 0.0;
	double promVisitedNodesV4 = 0.0;
	double promVisitedNodesV5 = 0.0;
	double promVisitedNodesV6 = 0.0;
	for(int i=0; i<nQueries; i++){
		bool fallaV1 = false;
		bool fallaV2 = false;
		bool fallaV3 = false;
		bool fallaV4 = false;
		bool fallaV5 = false;
		bool fallaV6 = false;
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
		gst.statsReset();
		vector<int> resV1 = gst.tr_allContained(x);
		visitedNodesV1[i] = gst.howManyNodes;
		promVisitedNodesV1 += gst.howManyNodes;
		statsV1[0] += gst.howManyInserts;
		statsV1[1] += gst.howManyIfs;
		statsV1[2] += gst.howManyidRutaDesdeCeldaDeSecConcat;
		statsV1[3] += gst.howManygetLargoRuta;
		statsV1[4] += resV1.size();
		// Comparar los resultados de la versión 1 y contar fallas
		sort(resV1.begin(), resV1.end());
		if(resV1.size() != y){
			cantFallasV1++;
		}else{
			for(int j=0; j<y; j++){
				if(resV1[j] != naiveResult[j]){
					fallaV1 = true;
				}
			}
			if(fallaV1){
				cantFallasV1++;	
			}
		}
		// Obtener resultados de la versión 2
		gst.statsReset();
		vector<int> resV2 = gst.tr_allContained2(x);
		visitedNodesV2[i] = gst.howManyNodes;
		promVisitedNodesV2 += gst.howManyNodes;
		statsV2[0] += gst.howManyInserts;
		statsV2[1] += gst.howManyIfs;
		statsV2[2] += gst.howManyidRutaDesdeCeldaDeSecConcat;
		statsV2[3] += gst.howManygetLargoRuta;
		statsV2[4] += resV2.size();
		// Comparar los resultados de la versión 2 y contar fallas
		sort(resV2.begin(), resV2.end());
		if(resV2.size() != y){
			cantFallasV2++;
		}else{
			for(int j=0; j<y; j++){
				if(resV2[j] != naiveResult[j]){
					fallaV2 = true;
				}
			}
			if(fallaV2){
				cantFallasV2++;	
			}
		}
		// Obtener resultados de la versión 3
		gst.statsReset();
		vector<int> resV3 = gst.tr_allContained3(x);
		visitedNodesV3[i] = gst.howManyNodes;
		promVisitedNodesV3 += gst.howManyNodes;
		statsV3[0] += gst.howManyInserts;
		statsV3[1] += gst.howManyIfs;
		statsV3[2] += gst.howManyidRutaDesdeCeldaDeSecConcat;
		statsV3[3] += gst.howManygetLargoRuta;
		statsV3[4] += resV3.size();
		// Comparar los resultados de la versión 3 y contar fallas
		sort(resV3.begin(), resV3.end());
		if(resV3.size() != y){
			cantFallasV3++;
		}else{
			for(int j=0; j<y; j++){
				if(resV3[j] != naiveResult[j]){
					fallaV3 = true;
				}
			}
			if(fallaV3){
				cantFallasV3++;	
			}
		}
		// Obtener resultados de la versión 4
		gst.statsReset();
		vector<int> resV4 = gst.tr_allContained4(x);
		visitedNodesV4[i] = gst.howManyNodes;
		promVisitedNodesV4 += gst.howManyNodes;
		statsV4[0] += gst.howManyInserts;
		statsV4[1] += gst.howManyIfs;
		statsV4[2] += gst.howManyidRutaDesdeCeldaDeSecConcat;
		statsV4[3] += gst.howManygetLargoRuta;
		statsV4[4] += resV4.size();
		// Comparar los resultados de la versión 4 y contar fallas
		sort(resV4.begin(), resV4.end());
		if(resV4.size() != y){
			cantFallasV4++;
		}else{
			for(int j=0; j<y; j++){
				if(resV4[j] != naiveResult[j]){
					fallaV4 = true;
				}
			}
			if(fallaV4){
				cantFallasV4++;	
			}
		}
		// Obtener resultados de la versión 5
		gst.statsReset();
		vector<int> resV5 = gst.tr_allContained5(x);
		visitedNodesV5[i] = gst.howManyNodes;
		promVisitedNodesV5 += gst.howManyNodes;
		statsV5[0] += gst.howManyInserts;
		statsV5[1] += gst.howManyIfs;
		statsV5[2] += gst.howManyidRutaDesdeCeldaDeSecConcat;
		statsV5[3] += gst.howManygetLargoRuta;
		statsV5[4] += resV5.size();
		// Comparar los resultados de la versión 5 y contar fallas
		sort(resV5.begin(), resV5.end());
		if(resV5.size() != y){
			cantFallasV5++;
		}else{
			for(int j=0; j<y; j++){
				if(resV5[j] != naiveResult[j]){
					fallaV5 = true;
				}
			}
			if(fallaV5){
				cantFallasV5++;	
			}
		}
		// Obtener resultados de la versión 6
		gst.statsReset();
		vector<int> resV6 = gst.tr_allContained6(x);
		visitedNodesV6[i] = gst.howManyNodes;
		promVisitedNodesV6 += gst.howManyNodes;
		statsV6[0] += gst.howManyInserts;
		statsV6[1] += gst.howManyIfs;
		statsV6[2] += gst.howManyidRutaDesdeCeldaDeSecConcat;
		statsV6[3] += gst.howManygetLargoRuta;
		statsV6[4] += resV6.size();
		// Comparar los resultados de la versión 6 y contar fallas
		sort(resV6.begin(), resV6.end());
		if(resV6.size() != y){
			cantFallasV6++;
		}else{
			for(int j=0; j<y; j++){
				if(resV6[j] != naiveResult[j]){
					fallaV6 = true;
				}
			}
			if(fallaV6){
				cantFallasV6++;	
			}
		}
	}
	queries.close();
	results.close();
	sort(visitedNodesV1.begin(), visitedNodesV1.end());
	sort(visitedNodesV2.begin(), visitedNodesV2.end());
	sort(visitedNodesV3.begin(), visitedNodesV3.end());
	sort(visitedNodesV4.begin(), visitedNodesV4.end());
	sort(visitedNodesV5.begin(), visitedNodesV5.end());
	sort(visitedNodesV6.begin(), visitedNodesV6.end());
	promVisitedNodesV1 /= nQueries;
	promVisitedNodesV2 /= nQueries;
	promVisitedNodesV3 /= nQueries;
	promVisitedNodesV4 /= nQueries;
	promVisitedNodesV5 /= nQueries;
	promVisitedNodesV6 /= nQueries;
	// Mostrando conteo de fallas
	cout << "fallas\tnRutas\tnQueries\tfails_{V1}\tfails_{V2}\tfails_{V3}\tfails_{V4}\tfails_{V5}\tfails_{V6}" << endl;
	cout << "fallas\t" << gst.n_rutas << "\t";
	cout << nQueries << "\t";
	cout << cantFallasV1 << "\t";
	cout << cantFallasV2 << "\t";
	cout << cantFallasV3 << "\t";
	cout << cantFallasV4 << "\t";
	cout << cantFallasV5 << "\t";
	cout << cantFallasV6 << endl;
	// Mostrando estadísticas de nodos visitados
	cout << "visNods\tnRutas\tpromV1\tpromV2\tpromV3\tpromV4\tpromV5\tpromV6";
	cout << "\tminV1\tminV2\tminV3\tminV4\tminV5\tminV6";
	cout << "\tmedV1\tmedV2\tmedV3\tmedV4\tmedV5\tmedV6";
	cout << "\tmaxV1\tmaxV2\tmaxV3\tmaxV4\tmaxV5\tmaxV6" << endl;
	cout << "visNods\t" << gst.n_rutas << "\t";
	cout << promVisitedNodesV1 << "\t" << promVisitedNodesV2 << "\t" << promVisitedNodesV3 << "\t" << promVisitedNodesV4 << "\t" << promVisitedNodesV5 << "\t" << promVisitedNodesV6 << "\t";
	cout << visitedNodesV1[0] << "\t" << visitedNodesV2[0] << "\t" << visitedNodesV3[0] << "\t" << visitedNodesV4[0] << "\t" << visitedNodesV5[0] << "\t" << visitedNodesV6[0] << "\t";
	cout << visitedNodesV1[nQueries/2] << "\t" << visitedNodesV2[nQueries/2] << "\t" << visitedNodesV3[nQueries/2] << "\t" << visitedNodesV4[nQueries/2] << "\t" << visitedNodesV5[nQueries/2] << "\t" << visitedNodesV6[nQueries/2] << "\t";
	cout << visitedNodesV1[nQueries-1] << "\t" << visitedNodesV2[nQueries-1] << "\t" << visitedNodesV3[nQueries-1] << "\t" << visitedNodesV4[nQueries-1] << "\t" << visitedNodesV5[nQueries-1] << "\t" << visitedNodesV6[nQueries-1] << endl;
	// Mostrando conteo de instrucciones
	cout << "stats\tnRutas\tnQueries";
	cout << "\tins_{V1}\tins_{V2}\tins_{V3}\tins_{V4}\tins_{V5}\tins_{V6}";
	cout << "\tifs_{V1}\tifs_{V2}\tifs_{V3}\tifs_{V4}\tifs_{V5}\tifs_{V6}";
	cout << "\tidsR_{V1}\tidsR_{V2}\tidsR_{V3}\tidsR_{V4}\tidsR_{V5}\tidsR_{V6}";
	cout << "\tlarR_{V1}\tlarR_{V2}\tlarR_{V3}\tlarR_{V4}\tlarR_{V5}\tlarR_{V6}";
	cout << "\tcRes_{V1}\tcRes_{V2}\tcRes_{V3}\tcRes_{V4}\tcRes_{V5}\tcRes_{V6}" << endl;
	cout << "visNods\t" << gst.n_rutas << "\t" << nQueries << "\t";
	cout << statsV1[0] << "\t" << statsV2[0] << "\t" << statsV3[0] << "\t" << statsV4[0] << "\t" << statsV5[0] << "\t" << statsV6[0] << "\t";
	cout << statsV1[1] << "\t" << statsV2[1] << "\t" << statsV3[1] << "\t" << statsV4[1] << "\t" << statsV5[1] << "\t" << statsV6[1] << "\t";
	cout << statsV1[2] << "\t" << statsV2[2] << "\t" << statsV3[2] << "\t" << statsV4[2] << "\t" << statsV5[2] << "\t" << statsV6[2] << "\t";
	cout << statsV1[3] << "\t" << statsV2[3] << "\t" << statsV3[3] << "\t" << statsV4[3] << "\t" << statsV5[3] << "\t" << statsV6[3] << "\t";
	cout << statsV1[4] << "\t" << statsV2[4] << "\t" << statsV3[4] << "\t" << statsV4[4] << "\t" << statsV5[4] << "\t" << statsV6[4] << endl;
	cout << endl;
    return 0;
}