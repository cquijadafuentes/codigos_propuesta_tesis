#include <iostream>
#include "TopoRel_GST.hpp"

// Programa para verificar los resultados de las 6 implementaciones de allContained.

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){

	if(argc < 4){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <input_filename> <queries_file> <results_file>" << endl;
		return 0;
	}
	
	TopoRelGST gst(argv[1]);
	
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
	int cantFallas = 0;
	vector<int> visitedNodes(nQueries, 0);
	vector<int> stats(5, 0);
	double promVisitedNodes = 0.0;
	for(int i=0; i<nQueries; i++){
		bool falla = false;
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
		// Obtener resultados
		gst.statsReset();
		vector<int> res = gst.tr_allContained(x);
		visitedNodes[i] = gst.howManyNodes;
		promVisitedNodes += gst.howManyNodes;
		stats[0] += gst.howManyInserts;
		stats[1] += gst.howManyIfs;
		stats[2] += gst.howManyidRutaDesdeCeldaDeSecConcat;
		stats[3] += gst.howManygetLargoRuta;
		stats[4] += res.size();
		// Comparar los resultados de la versión 1 y contar fallas
		sort(res.begin(), res.end());
		if(res.size() != y){
			cantFallas++;
		}else{
			for(int j=0; j<y; j++){
				if(res[j] != naiveResult[j]){
					falla = true;
				}
			}
			if(falla){
				cantFallas++;	
			}
		}
	}
	queries.close();
	results.close();
	sort(visitedNodes.begin(), visitedNodes.end());
	promVisitedNodes /= nQueries;
	// Mostrando conteo de fallas
	cout << "fallas\tnRutas\tnQueries\tfails" << endl;
	cout << "fallas\t";
	cout << gst.n_rutas << "\t";
	cout << nQueries << "\t";
	cout << cantFallas << endl;
	// Mostrando estadísticas de nodos visitados
	cout << "visNods\tnRutas\tprom\tmin\tmed\tmax" << endl;
	cout << "visNods\t" << gst.n_rutas << "\t";
	cout << promVisitedNodes << "\t";
	cout << visitedNodes[0] << "\t";
	cout << visitedNodes[nQueries/2];
	cout << visitedNodes[nQueries-1] << endl;
	// Mostrando conteo de instrucciones
	cout << "stats\tnRutas\tnQueries";
	cout << "\tins\tifs\tidsR\tlarR\tcRes" << endl;
	cout << "visNods\t" << gst.n_rutas << "\t" << nQueries << "\t";
	cout << stats[0] << "\t";
	cout << stats[1] << "\t";
	cout << stats[2] << "\t";
	cout << stats[3] << "\t";
	cout << stats[4] << endl;
	cout << endl;
    return 0;
}