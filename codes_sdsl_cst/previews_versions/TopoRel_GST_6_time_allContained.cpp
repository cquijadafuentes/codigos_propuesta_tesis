#include <iostream>
#include <ctime>
#include "TopoRel_GST_6.hpp"

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
	// Toma el tiempo de obtener los resultados de la versión de allContained que se especifique
	if(argc < 4){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <input_filename.gst6> <queries_file> <version>" << endl;
		cout << endl;
		return 0;
	}

	// Cargando versión
	int version = (int) atoi(argv[3]);
	if(version < 1 || version > 6){
		cout << "Error! la versión indicada no es correcta. Se usará la versión 6" << endl;
		version = 6;
	}
	// Creando GST
	TopoRelGST_6 gst(argv[1]);
	// Cargando consultas
	int nQueries;
	ifstream queries(argv[2], ifstream::in);
	queries >> nQueries;

	// Realizando consultas y comparando
	int cantRutasRetorna = 0;
	int q;
	vector<int> resGST;
	auto t0 = clock();
	switch(version){
		case 1:
			for(int i=0; i<nQueries; i++){
				queries >> q;
				resGST = gst.tr_allContained(q);
				cantRutasRetorna += resGST.size();
			}
			break;
		case 2:
			for(int i=0; i<nQueries; i++){
				queries >> q;
				resGST = gst.tr_allContained2(q);
				cantRutasRetorna += resGST.size();
			}
			break;
		case 3:
			for(int i=0; i<nQueries; i++){
				queries >> q;
				resGST = gst.tr_allContained3(q);
				cantRutasRetorna += resGST.size();
			}
			break;
		case 4:
			for(int i=0; i<nQueries; i++){
				queries >> q;
				resGST = gst.tr_allContained4(q);
				cantRutasRetorna += resGST.size();
			}
			break;
		case 5:
			for(int i=0; i<nQueries; i++){
				queries >> q;
				resGST = gst.tr_allContained5(q);
				cantRutasRetorna += resGST.size();
			}
			break;
		case 6:
			for(int i=0; i<nQueries; i++){
				queries >> q;
				resGST = gst.tr_allContained6(q);
				cantRutasRetorna += resGST.size();
			}
			break;
		default:
			cout << "Opción sin definir" << endl;
	}
	auto t1 = clock();
	double time = ((double)(t1 - t0) / CLOCKS_PER_SEC)* 1000000;
	double timeProm = time / nQueries;
	double cantPromResp = (cantRutasRetorna + 0.0) / nQueries;
	cout << "************************** RESULTADOS GST6 **************************" << endl;
	cout << "Archivo entrada: " << argv[1] << endl;
	cout << "Archivo consultas: " << argv[2] << endl;
	cout << "Usando allContained versión " << version << endl;
	cout << "Rutas del conjunto: " << gst.n_rutas << endl;
	cout << "Stops del conjunto: " << gst.n_stops << endl;
	cout << "Ruta más corta: " << gst.len_min << endl;
	cout << "Ruta más larga: " << gst.len_max << endl;
	cout << "Cantidad de consultas: " << nQueries << endl;
	cout << "+ Tiempo total de consultas: " << time << "[us]" << endl;
	cout << "+ Tiempo promedio de consulta: " << timeProm << "[us]" << endl;
	cout << "Total de rutas retornadas: " << cantRutasRetorna << endl;
	cout << "Promedio de rutas retornadas: " << cantPromResp << endl;
	// Mostrando estadísticas de la versión usada en allContained
	double promAux;
	promAux = (gst.howManyNodes + 0.0) / nQueries;
	cout << "Total de nodos visitados: " << gst.howManyNodes << endl;
	cout << "Promedio de nodos visitados: " << promAux << endl;
	promAux = (gst.howManyInserts + 0.0) / nQueries;
	cout << "Total de inserciones al resultado: " << gst.howManyInserts << endl;
	cout << "Promedio de inserciones al resultado: " << promAux << endl;
	promAux = (gst.howManyIfs + 0.0) / nQueries;
	cout << "Total de condicionales IF: " << gst.howManyIfs << endl;
	cout << "Promedio de condicionales IF: " << promAux << endl;
	promAux = (gst.howManyCSTviews + 0.0) / nQueries;
	cout << "Total de celdas CST revisadas: " << gst.howManyCSTviews << endl;
	cout << "Promedio de celdas CST revisadas: " << promAux << endl;

    return 0;
}