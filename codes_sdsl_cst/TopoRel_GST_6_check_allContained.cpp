#include <iostream>
#include "TopoRel_GST_6.hpp"
#include "TopoRel_Naive.hpp"

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){

	if(argc < 4){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <input_filename.gst6> <queries_file> <results_file>" << endl;
		cout << endl;
		return 0;
	}

	// Creando GST
	TopoRelGST_6 gst(argv[1]);
	// Cargando consultas
	int nQueries;
	ifstream queries(argv[2], ifstream::in);
	queries >> nQueries;
	// Cargando resultados
	ifstream results(argv[3], ifstream::in);
	int nResults;
	results >> nResults;
	if(nQueries != nResults){
		cout << "Error! El número de consultas del archivo " << argv[1];
		cout << "(" << nQueries << ") no coincide con ";
		cout << "la cantidad de resultados del archivo " << argv[2];
		cout << "(" << nResults << ")" << endl;
		return 0;
	}
	// Realizando consultas y comparando
	int cantFallas = 0;
	for(int i=0; i<nQueries; i++){
		bool falla = false;
		// Carga de la consulta en x
		int x; 
		queries >> x;
		// Carga de los resultados de la consulta i
		int y;
		results >> y;	// Cantidad de elementos del resultado ii
		vector<int> resNaive(y);
		for(int ii=0; ii<y; ii++){
			results >> resNaive[ii];
		}
		// Obtener resultados desde GST versión 4
		gst.statsReset();
		vector<int> resGST = gst.tr_allContained5(x);
		// Comparar los resultados de la versión 4
		sort(resGST.begin(), resGST.end());
		if(resGST.size() != resNaive.size()){
			falla = true;
		}else{
			for(int j=0; j<resNaive.size(); j++){
				if(resGST[j] != resNaive[j]){
					falla = true;
				}
			}
		}
		// Contabilizar fallas
		if(falla){
			cantFallas++;
			cout << "------------------------" << endl;
			cout << "Resultados naive (" << resNaive.size() << "):\t";
			for(int j=0; j<resNaive.size(); j++){
				cout << resNaive[j] << "\t";
			}
			cout << endl;
			cout << "Resultados GST (" << resGST.size() << "):\t";
			for(int j=0; j<resGST.size(); j++){
				cout << resGST[j] << "\t";
			}
			cout << endl;
			cout << "Creando mini-dataset de ruta " << i << endl;
			cout << resNaive.size() << " " << gst.n_stops << endl;
			for(int j=0; j<resNaive.size(); j++){
				vector<int> lx = gst.getRuta(resNaive[j]);
				cout << lx.size() << "\t";
				for(int k=0; k<lx.size(); k++){
					cout << lx[k] << "\t";
				}
				cout << endl;
			}
		}
	}
	cout << "------------------------" << endl;
	cout << "Total fallas: " << cantFallas << endl;
    return 0;
}