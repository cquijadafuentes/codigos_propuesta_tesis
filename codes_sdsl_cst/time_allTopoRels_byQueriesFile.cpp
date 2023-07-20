/*
	Toma de tiempo promedio para consultas allContain, allEqual, allContained y allIntersect 
	sobre implementaciones Naive y GST
	Las consultas provienen de un archivo de consulta generado con 
	./extras/generador_consultas
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <set>
#include <cstdlib>
#include "TopoRel_Naive.hpp"
#include "TopoRel_GST.hpp"

using namespace std;

vector<int> generarQueries(int n_rutas, int num_queries);

int main(int argc, char const *argv[]){
	if(argc < 5){
		cout << "Programa para medir tiempo promedio operaciones all*" << endl;
		cout << "Error! faltan argumentos:" << endl;
		cout << argv[0] << " <input_file.txt> <input_file.gst> <queries_file> repeticiones [<min_intersetion>]" << endl;
		return 0;
	}

	int repeticiones = (int) atoi(argv[4]);

	int minIntersection = 1;
	if(argc > 5){
		minIntersection = (int) atoi(argv[5]);
	}

	cout << "Cargando consultas desde " << argv[3] << endl;
	ifstream qFile(argv[3], ifstream::in);
	int num_queries;
	qFile >> num_queries;
	vector<int> queries(num_queries);
	for(int i=0; i<num_queries; i++){
		qFile >> queries[i];
	}
	qFile.close();

	if(num_queries < 1 || repeticiones < 1){
		cout << "Error! en la cantidad de num_queries/repeticiones: " << num_queries << "/" << repeticiones << endl;
		return 0;
	}
	
	cout << "Leyendo rutas desde " << argv[1] << endl;
	ifstream naive(argv[1], ifstream::in);
	int n, x, aux, max;
	naive >> n >> max;
	vector<int_vector<>> rutas(n);
	for(int i = 0; i < n; i++){
		naive >> x;
		rutas[i] = int_vector<>(x);
		for(int j=0; j<x; j++){
			naive >> aux;
			rutas[i][j] = aux;
		}
		util::bit_compress(rutas[i]);
	}
	naive.close();

	cout << "Cargando estructura GST desde " << argv[2] << endl;
	TopoRelGST gst(argv[2]);
	if(num_queries > gst.n_rutas){
		num_queries = gst.n_rutas;
	}

	cout << "Rutas: " << gst.n_rutas << endl;
	cout << "Ruta más corta: " << gst.len_min << endl;
	cout << "Ruta más larga: " << gst.len_max << endl;
	cout << "Consultas a realizar: " << num_queries << endl;
	cout << "Repeticiones a realizar: " << repeticiones << endl;

	unsigned long long t0, t1;
	double tNaive, tNaivePC, tGST;
	int resNaive, resGST;

	//////////////////	allContain 	//////////////////

	resNaive = 0;
	cout << "Ejecutando allContain Naive..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	resNaive += tr_allContain(rutas, queries[i]).size();
	    }
	}
	t1 = clock();
	resNaive /= repeticiones;
	tNaive = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;

	resGST = 0;
	cout << "Ejecutando allContain GST..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	resGST += gst.tr_allContain(queries[i]).size();
	    }
	}
	t1 = clock();
	resGST /= repeticiones;
	tGST = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;
	
	cout << "operacion\trutas\tqueries\ttNaive\ttGST\t[us]\trNaive\trGST" << endl;
	cout << "allContain\t" << gst.n_rutas << "\t" << num_queries << "\t" << tNaive << "\t" << tGST << "\t[us]\t" << resNaive << "\t" << resGST << endl;

	//////////////////	allContained 	//////////////////

	resNaive = 0;
	cout << "Ejecutando allContained Naive..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	resNaive += tr_allContained(rutas, queries[i]).size();
	    }
	}
	t1 = clock();
	resNaive /= repeticiones;
	tNaive = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;	

	resGST = 0;
	cout << "Ejecutando allContained GST6..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	resGST += gst.tr_allContained(queries[i]).size();
	    }
	}
	t1 = clock();
	resGST /= repeticiones;
	tGST = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;


	cout << "operacion\trutas\tqueries\ttNaive\ttGST\t[us]\trNaive\trGST" << endl;
	cout << "allContained\t" << gst.n_rutas << "\t" << num_queries << "\t" << tNaive << "\t" << tGST << "\t[us]\t" << resNaive << "\t" << resGST << endl;

	//////////////////	allEqual 	//////////////////

	resNaive = 0;
	cout << "Ejecutando allEqual Naive..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	resNaive += tr_allEqual(rutas, queries[i]).size();
	    }
	}
	t1 = clock();
	resNaive /= repeticiones;
	tNaive = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;	

	resGST = 0;
	cout << "Ejecutando allEqual GST6..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	resGST += gst.tr_allEqual(queries[i]).size();
	    }
	}
	t1 = clock();
	resGST /= repeticiones;
	tGST = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;

	cout << "operacion\trutas\tqueries\ttNaive\ttGST\t[us]\trNaive\trGST" << endl;
	cout << "allEqual\t" << gst.n_rutas << "\t" << num_queries << "\t" << tNaive << "\t" << tGST << "\t[us]\t" << resNaive << "\t" << resGST << endl;

	//////////////////	allIntersect 	//////////////////
/*
	resNaive = 0;
	cout << "Ejecutando allIntersect Naive..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	resNaive += tr_allIntersect(rutas, queries[i]).size();
	    }
	}
	t1 = clock();
	resNaive /= repeticiones;
	tNaive = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;	

	resGST = 0;
	cout << "Ejecutando allIntersect GST6..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	resGST += gst.tr_allIntersect(queries[i]).size();
	    }
	}
	t1 = clock();
	resGST /= repeticiones;
	tGST = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;

	cout << "operacion\trutas\tqueries\ttNaive\ttGST\t[us]\trNaive\trGST" << endl;
	cout << "allIntersect\t" << gst.n_rutas << "\t" << num_queries << "\t" << tNaive << "\t" << tGST << "\t[us]\t" << resNaive << "\t" << resGST << endl;
*/
	//////////////////	allIntersectPP 	//////////////////
/*
	resNaive = 0;
	cout << "Ejecutando allIntersectPP Naive..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	resNaive += tr_allIntersectPP(rutas, queries[i], minIntersection).size();
	    }
	}
	t1 = clock();
	resNaive /= repeticiones;
	tNaive = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;	

	resGST = 0;
	cout << "Ejecutando allIntersectPP GST6..." << endl;
	t0 = clock();
	for(int j=0; j<repeticiones; j++){
	    for(int i=0; i<queries.size(); i++){
	    	resGST += gst.tr_allIntersectPP(queries[i], minIntersection).size();
	    }
	}
	t1 = clock();
	resGST /= repeticiones;
	tGST = ((((double)(t1 - t0)) / CLOCKS_PER_SEC) / queries.size() / repeticiones)* 1000000;

	cout << "operacion\trutas\tqueries\ttNaive\ttGST\t[us]\trNaive\trGST\tk" << endl;
	cout << "allIntersectPP\t" << gst.n_rutas << "\t" << num_queries << "\t" << tNaive << "\t" << tGST << "\t[us]\t" << resNaive << "\t" << resGST << "\t" << minIntersection << endl;
*/
	return 0;
}
