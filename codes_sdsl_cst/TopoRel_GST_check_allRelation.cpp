#include <iostream>
#include "TopoRel_GST.hpp"
#include "TopoRel_Naive.hpp"

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
	// Verifica los resultados de allIntersect
	if(argc < 5){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <input_filename.gst> <queries_file> <results_file> <id_operacion>" << endl;
		cout << "Donde <id_operacion> es: ";
		cout << "(1) allContain - (2) allContained - (3) allEqual - (4) allIntersect" << endl;
		cout << endl;
		return 0;
	}

	int id_operacion = (int) atoi(argv[4]);
	if(id_operacion < 1 || id_operacion > 4){
		cout << "Error! <id_operacion> no corresponde." << endl;
		return 0;
	}

	// Creando GST
	TopoRelGST gst(argv[1]);
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
		// Obtener resultados desde GST
		gst.statsReset();
		vector<int> resGST;		
		switch(id_operacion){
			case 1:
				resGST = gst.tr_allContain(x);
				break;
			case 2:
				resGST = gst.tr_allContained(x);
				break;
			case 3:
				resGST = gst.tr_allEqual(x);
				break;
			case 4:
				resGST = gst.tr_allIntersect(x);
				break;
			default:
				cout << "Error! Operación no definida." << endl;
				return 0;
		}
		// Comparar los resultados
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
			cout << "Creando mini-dataset de ruta " << x << " por consulta " << i << endl;
			// Preparando datos...
			unordered_map<int,int> mapRutas;
			int stops = 0;
			for(int j=0; j<resNaive.size(); j++){
				vector<int> lx = gst.getRuta(resNaive[j]);
				for(int k=0; k<lx.size(); k++){
					if(mapRutas[lx[k]] == 0){
						mapRutas[lx[k]] = ++stops;
					}
				}
			}
			// Imprimiendo datos
			cout << resNaive.size() << " " << stops << endl;
			for(int j=0; j<resNaive.size(); j++){
				vector<int> lx = gst.getRuta(resNaive[j]);
				cout << lx.size() << "\t";
				for(int k=0; k<lx.size(); k++){
					cout << mapRutas[lx[k]] << "\t";
				}
				cout << endl;
			}
		}
	}
	cout << "------------------------" << endl;
	cout << "Total fallas allIntersect: " << cantFallas << endl;
    return 0;
}