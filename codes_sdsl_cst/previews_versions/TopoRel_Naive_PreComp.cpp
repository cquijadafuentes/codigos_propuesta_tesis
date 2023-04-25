#include "TopoRel_Naive_PreComp.hpp"
#include <vector>
#include <map>

// Construcción previa: No paralela
TopoRelNaivePreComp::TopoRelNaivePreComp(vector<vector<int>> &rs, int cant_stops, bool x=false){
	rutas = vector<int_vector<>>(rs.size());
	for(int i=0; i<rs.size(); i++){
		rutas[i] = int_vector<>(rs[i].size());
		for(int j=0; j<rs[i].size(); j++){
			rutas[i][j] = rs[i][j];
		}
		util::bit_compress(rutas[i]);
	}
	n_stops = cant_stops;
	n_rutas = rs.size();
	relaciones = vector<int_vector<>>(n_rutas, int_vector<>(n_rutas,0));
	map<string, int> mapNameIdRel;
	
	nombresRel = vector<string>(8, "");
	
	mapNameIdRel[COVEREDBY] = 0;
	nombresRel[0] = COVEREDBY;
	mapNameIdRel[COVERS] = 1;
	nombresRel[1] = COVERS;
	mapNameIdRel[DISJOINT] = 2;
	nombresRel[2] = DISJOINT;
	mapNameIdRel[EQUALS] = 3;
	nombresRel[3] = EQUALS;
	mapNameIdRel[INCLUDES] = 4;
	nombresRel[4] = INCLUDES;
	mapNameIdRel[INSIDE] = 5;
	nombresRel[5] = INSIDE;
	mapNameIdRel[OVERLAPS] = 6;
	nombresRel[6] = OVERLAPS;
	mapNameIdRel[TOUCHES] = 7;
	nombresRel[7] = TOUCHES;

	for(int i=0; i<n_rutas; i++){
		for(int j=0; j<n_rutas; j++){
			relaciones[i][j] = mapNameIdRel[toporel(rs[i],rs[j])];
		}
		util::bit_compress(relaciones[i]);
	}
}

// Csontrucción paralela
TopoRelNaivePreComp::TopoRelNaivePreComp(vector<vector<int>> &rs, int cant_stops){
	rutas = vector<int_vector<>>(rs.size());
	for(int i=0; i<rs.size(); i++){
		rutas[i] = int_vector<>(rs[i].size());
		for(int j=0; j<rs[i].size(); j++){
			rutas[i][j] = rs[i][j];
		}
		util::bit_compress(rutas[i]);
	}
	n_stops = cant_stops;
	n_rutas = rs.size();
	map<string, int> mapNameIdRel;
	
	nombresRel = vector<string>(8, "");
	
	mapNameIdRel[COVEREDBY] = 0;
	nombresRel[0] = COVEREDBY;
	mapNameIdRel[COVERS] = 1;
	nombresRel[1] = COVERS;
	mapNameIdRel[DISJOINT] = 2;
	nombresRel[2] = DISJOINT;
	mapNameIdRel[EQUALS] = 3;
	nombresRel[3] = EQUALS;
	mapNameIdRel[INCLUDES] = 4;
	nombresRel[4] = INCLUDES;
	mapNameIdRel[INSIDE] = 5;
	nombresRel[5] = INSIDE;
	mapNameIdRel[OVERLAPS] = 6;
	nombresRel[6] = OVERLAPS;
	mapNameIdRel[TOUCHES] = 7;
	nombresRel[7] = TOUCHES;

	int maxThreads = omp_get_max_threads();
    if(maxThreads > n_rutas){
        maxThreads = n_rutas;
    }
    // Vector que contendrá los vectores de cada porción que se genere según la cantidad de threads
    int porciones = n_rutas / maxThreads;
    if((porciones * maxThreads) < (n_rutas * 2)){
        porciones++;
    }
    vector<vector<int_vector<>>> relXThread(maxThreads, vector<int_vector<>>(porciones, int_vector<>(n_rutas,0)));
    cout << "N° Threads: " << maxThreads << endl;
    #pragma omp parallel for
    for(int iParalelo =0; iParalelo < maxThreads; iParalelo++ ){
    	int base = iParalelo * porciones;
		for(int i=0; i<porciones && base+i < n_rutas; i++){
			for(int j=0; j<n_rutas; j++){
				relXThread[iParalelo][i][j] = mapNameIdRel[toporel(rs[base+i],rs[j])];
			}
			util::bit_compress(relXThread[iParalelo][i]);
		}
    }

    relaciones = vector<int_vector<>>(n_rutas);
    int pos = 0;
    for(int i=0; i<maxThreads; i++){
    	for(int j=0; j<relXThread[i].size() && pos < n_rutas; j++){
    		relaciones[pos++] = relXThread[i][j];
    	}
    }

}

TopoRelNaivePreComp::TopoRelNaivePreComp(string inputFilename){
	ifstream infile(inputFilename, ofstream::binary);
	if(infile){
		infile.read ((char *)&n_rutas,sizeof(int));
		infile.read ((char *)&n_stops,sizeof(int));
		rutas = vector<int_vector<>>(n_rutas);
		for(int i=0; i<n_rutas; i++){
            rutas[i].load(infile);
        }
		relaciones = vector<int_vector<>>(n_rutas);
		for(int i=0; i<n_rutas; i++){
            relaciones[i].load(infile);
        }

        nombresRel = vector<string>(8, "");
		nombresRel[0] = COVEREDBY;
		nombresRel[1] = COVERS;
		nombresRel[2] = DISJOINT;
		nombresRel[3] = EQUALS;
		nombresRel[4] = INCLUDES;
		nombresRel[5] = INSIDE;
		nombresRel[6] = OVERLAPS;
		nombresRel[7] = TOUCHES;

		return;
	}
	cout << "Error! en la carga del archivo." << endl;
}

string TopoRelNaivePreComp::obtenerRelacion(int x, int y){
	return nombresRel[relaciones[x][y]];
}


bool TopoRelNaivePreComp::equals(int i, int j){
	//return (nombresRel[relaciones[i][j]] == EQUALS);
	return relaciones[i][j] == 3;
}

bool TopoRelNaivePreComp::coveredby(int i, int j){
	//return (nombresRel[relaciones[i][j]] == COVEREDBY);
	return relaciones[i][j] == 0;
}

bool TopoRelNaivePreComp::covers(int i, int j){
	//return (nombresRel[relaciones[i][j]] == COVERS);
	return relaciones[i][j] == 1;
}

bool TopoRelNaivePreComp::inside(int i, int j){
	//return (nombresRel[relaciones[i][j]] == INSIDE);
	return relaciones[i][j] == 5;
}

bool TopoRelNaivePreComp::includes(int i, int j){
	//return (nombresRel[relaciones[i][j]] == INCLUDES);
	return relaciones[i][j] == 4;
}

bool TopoRelNaivePreComp::disjoint(int i, int j){
	//return (nombresRel[relaciones[i][j]] == DISJOINT);
	return relaciones[i][j] == 2;
}

bool TopoRelNaivePreComp::touches(int i, int j){
	//return (nombresRel[relaciones[i][j]] == TOUCHES);
	return relaciones[i][j] == 7;
}

bool TopoRelNaivePreComp::overlaps(int i, int j){
	//return (nombresRel[relaciones[i][j]] == OVERLAPS);
	return relaciones[i][j] == 6;
}

bool TopoRelNaivePreComp::within(int i, int j){
	int rel = relaciones[i][j];
	if(rel == 3){
		return true;
	}
	if(rel == 0){
		return true;
	}
	if(rel == 5){
		return true;
	}
	return false;
}

bool TopoRelNaivePreComp::contains(int i, int j){	
	int rel = relaciones[i][j];
	if(rel == 3){
		return true;
	}
	if(rel == 1){
		return true;
	}
	if(rel == 4){
		return true;
	}
	return false;
}

bool TopoRelNaivePreComp::intersect(int i, int j){
	return relaciones[i][j] != 2;
}

vector<int> TopoRelNaivePreComp::allContain(int x){
	vector<int> res;
	for(int i=0; i<n_rutas; i++){
		if(within(x, i)){
			res.push_back(i);
		}
	}
	return res;
}

vector<int> TopoRelNaivePreComp::allEqual(int x){
	vector<int> res;
	for(int i=0; i<n_rutas; i++){
		if(equals(x, i)){
			res.push_back(i);
		}
	}
	return res;
}

vector<int> TopoRelNaivePreComp::allContained(int x){
	vector<int> res;
	for(int i=0; i<n_rutas; i++){
		if(contains(x, i)){
			res.push_back(i);
		}
	}
	return res;
}

vector<int> TopoRelNaivePreComp::allIntersect(int x){
	vector<int> res;
	for(int i=0; i<n_rutas; i++){
		if(intersect(x, i)){
			res.push_back(i);
		}
	}
	return res;
}

void TopoRelNaivePreComp::navega(){
    cout << "navega" << endl;
}

void TopoRelNaivePreComp::sizeEstructura(){
    cout << "**** Tamaño en bytes ****" << endl;
    // Calculo de los bytes para rutas
    unsigned long long bytesRutas = 0;
    unsigned long long bytesRelaciones = 0;
    for(int i=0; i < rutas.size(); i++){
        bytesRutas += size_in_bytes(rutas[i]);
        bytesRelaciones += size_in_bytes(relaciones[i]);
    }
    cout << "rutas [B]: " << bytesRutas << endl;
    cout << "relaciones [B]: " << bytesRelaciones << endl;
    cout << "**** Elementos ****" << endl;
    cout << "Nº Rutas: " << rutas.size() << endl;
}

bool TopoRelNaivePreComp::iguales(TopoRelNaivePreComp trnpc2){
	if(n_rutas != trnpc2.n_rutas){
		cout << "Diferencia en el número de rutas." << endl;
		return false;
	}
	if(n_stops != trnpc2.n_stops){
		cout << "Diferencia en el número de stops." << endl;
		return false;
	}
	for(int i=0; i<n_rutas; i++){
		if(rutas[i] != trnpc2.rutas[i]){
			cout << "Diferencia en el contenido de rutas." << endl;
			return false;
		}
	}
	for(int i=0; i<n_rutas; i++){
		if(relaciones[i] != trnpc2.relaciones[i]){
			cout << "Diferencia en el contenido de relaciones." << endl;
			return false;
		}
		for(int i=0; i<nombresRel.size(); i++){
			if(nombresRel[i] != trnpc2.nombresRel[i]){
				cout << "Diferencia en el contenido de nombresRel." << endl;
				return false;
			}
		}
	}
	return true;
}

bool TopoRelNaivePreComp::save(string outputFilename){
	ofstream outfile(outputFilename, ofstream::binary);
	// Guardando argumentos enteros
	outfile.write((char const*)&n_rutas, sizeof(int));
	outfile.write((char const*)&n_stops, sizeof(int));
	// Guardando rutas
	for(int i=0; i<n_rutas; i++){
        rutas[i].serialize(outfile);
    }
    // Guardando relaciones
	for(int i=0; i<n_rutas; i++){
        relaciones[i].serialize(outfile);
    }
    outfile.close();
    return true;
}