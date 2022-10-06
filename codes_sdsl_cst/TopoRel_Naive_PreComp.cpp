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
	n_routes = rs.size();
	relaciones = vector<int_vector<>>(n_routes, int_vector<>(n_routes,0,3));
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

	for(int i=0; i<n_routes; i++){
		for(int j=0; j<n_routes; j++){
			relaciones[i][j] = mapNameIdRel[toporel(rs[i],rs[j])];
		}
	}
}

string TopoRelNaivePreComp::obtenerRelacion(int x, int y){
	return nombresRel[relaciones[x][y]];
}


bool TopoRelNaivePreComp::equals(int i, int j){
	return (nombresRel[relaciones[i][j]] == EQUALS);
}

bool TopoRelNaivePreComp::coveredby(int i, int j){
	return (nombresRel[relaciones[i][j]] == COVEREDBY);
}

bool TopoRelNaivePreComp::covers(int i, int j){
	return (nombresRel[relaciones[i][j]] == COVERS);
}

bool TopoRelNaivePreComp::inside(int i, int j){
	return (nombresRel[relaciones[i][j]] == INSIDE);
}

bool TopoRelNaivePreComp::includes(int i, int j){
	return (nombresRel[relaciones[i][j]] == INCLUDES);
}

bool TopoRelNaivePreComp::disjoint(int i, int j){
	return (nombresRel[relaciones[i][j]] == DISJOINT);
}

bool TopoRelNaivePreComp::touches(int i, int j){
	return (nombresRel[relaciones[i][j]] == TOUCHES);
}

bool TopoRelNaivePreComp::overlaps(int i, int j){
	return (nombresRel[relaciones[i][j]] == OVERLAPS);
}

bool TopoRelNaivePreComp::within(int i, int j){
	if(nombresRel[relaciones[i][j]] == EQUALS){
		return true;
	}
	if(nombresRel[relaciones[i][j]] == COVEREDBY){
		return true;
	}
	if(nombresRel[relaciones[i][j]] == INSIDE){
		return true;
	}
	return false;
}

bool TopoRelNaivePreComp::contains(int i, int j){	
	if(nombresRel[relaciones[i][j]] == EQUALS){
		return true;
	}
	if(nombresRel[relaciones[i][j]] == COVERS){
		return true;
	}
	if(nombresRel[relaciones[i][j]] == INCLUDES){
		return true;
	}
	return false;
}

bool TopoRelNaivePreComp::intersects(int i, int j){
	return nombresRel[relaciones[i][j]] != DISJOINT;
}



void TopoRelNaivePreComp::navega(){
    cout << "navega" << endl;
}

void TopoRelNaivePreComp::sizeEstructura(){
    cout << "**** Tamaño en bytes ****" << endl;
    // Calculo de los bytes para rutas
    int bytesRutas = 0;
    int bytesRelaciones = 0;
    for(int i=0; i < rutas.size(); i++){
        bytesRutas += size_in_bytes(rutas[i]);
        bytesRelaciones += size_in_bytes(relaciones[i]);
    }
    cout << "rutas [B]: " << bytesRutas << endl;
    cout << "relaciones [B]: " << bytesRelaciones << endl;
    cout << "**** Elementos ****" << endl;
    cout << "Nº Rutas: " << rutas.size() << endl;
}