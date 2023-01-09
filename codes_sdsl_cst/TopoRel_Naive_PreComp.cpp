#include "TopoRel_Naive_PreComp.hpp"
#include <vector>
#include <map>

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
	if(relaciones[i][j] == 3){
		return true;
	}
	if(relaciones[i][j] == 0){
		return true;
	}
	if(relaciones[i][j] == 5){
		return true;
	}
	return false;
}

bool TopoRelNaivePreComp::contains(int i, int j){	
	if(relaciones[i][j] == 3){
		return true;
	}
	if(relaciones[i][j] == 1){
		return true;
	}
	if(relaciones[i][j] == 4){
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