#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>      // std::fstream
#include <sdsl/vectors.hpp>
#include <sdsl/suffix_trees.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/cst_iterators.hpp>
#include <ctime>
#include "TopoRel_Naive.hpp"
#include "TopoRel_Cons.hpp"

using namespace std;
using namespace sdsl;

void relaciones_nn_naive(vector<vector<int>> &routes);
void relaciones_nn_gst(vector<vector<int>> &routes, int n_stops);
void compara_relaciones(vector<vector<int>> &routes, int n_stops);
void mostrar_rutas(vector<int> &a, vector<int> &b);

// *********************** CLASE TOPORELGST ***********************
//  --------------------Inicio clase TopoRelGST-------------------
class TopoRelGST{
public:
    cst_sct3<csa_wt<wt_int<rrr_vector<>>>> cst;
    vector<vector<int>> routes;
    vector<vector<int>> routes_rev;
    vector<bit_vector> marcas;
    vector<cst_sct3<>::node_type> mapa;
    int n_routes;
    int n_concat;
    int n_stops;
    TopoRelGST(vector<vector<int>>&, int);
    string obtenerRelacion(int, int);
    bool tr_equals(int, int);
    bool tr_coveredby(int, int);
    bool tr_covers(int, int);
    bool tr_inside(int, int);
    bool tr_includes(int, int);
    bool tr_disjoint(int, int);
    bool tr_touches(int, int);
    bool tr_overlaps(int, int);
private:

};

TopoRelGST::TopoRelGST(vector<vector<int>> &rutas, int cant_rutas){
    routes = rutas;
    n_stops = cant_rutas;
    n_concat = 0;
    n_routes = routes.size();
    for(int i = 0; i < n_routes; i++){
        n_concat += routes[i].size();
    }
    int_vector<> iv(n_concat*2);
    int pv = 0;
    int tr = n_routes;
    // Concatenar rutas
    for(int i = 0; i < n_routes; i++){
        for(int j = 0; j < routes[i].size(); j++){
            iv[pv++] = routes[i][j];
        }
    }
//    cout << "Rutas... concatenadas" << endl;

    // Concatenar rutas reversas
    for(int i = 0; i < n_routes; i++){
        routes_rev.push_back(routes[i]);
        reverse(routes_rev[i].begin(), routes_rev[i].end());
        for(int j = routes_rev[i].size() - 1; j >= 0; j--){
            iv[pv++] = routes_rev[i][j];
        }
    }
//    cout << "Rutas reversas... concatenadas" << endl;
/*
    for(int i=0; i<iv.size(); i++){
        cout << iv[i] << " ";
    }
    cout << endl;
*/

    //cst_sct3<csa_wt<wt_int<rrr_vector<>>>> cst;
    construct_im(cst, iv);
/*
    cout << "inner nodes : " << cst.nodes() - cst.csa.size() << endl;
    auto v = cst.select_child(cst.child(cst.root(), 930),1);
    auto d = cst.depth(v);
    cout << "v : " << d << "-[" << cst.lb(v) << "," << cst.rb(v) << "]" << endl;
    cout << "extract(cst, v) : " << extract(cst, v) << endl;

    v = cst.root();
    cout << "root\tid: " << cst.id(v) << endl;
    for(int i=0; i < routes[0].size(); i++){
        v = cst.child(v, routes[0][i]);
        cout << routes[0][i] << "\tid: " << cst.id(v) << endl;
    }
*/
    // Marcas en bitvectors
    marcas = vector<bit_vector>(n_routes * 2, bit_vector(cst.nodes(), 0));
    for(int i = 0; i < n_routes; i++){
        for(int j = 0; j < routes[i].size(); j++){
            // La primera mitad de marcas es para routes
            auto v = cst.child(cst.root(), routes[i][j]);
            while(v != cst.root() && cst.depth(v) <= routes[i].size()-j){
                marcas[i][cst.id(v)] = 1;
                if(cst.depth(v) < routes[i].size()-j){
                    v = cst.child(v, routes[i][cst.depth(v)+j]);
                }else{
                    break;
                }
            }
            // La segunda mitad de marcas es para routes_rev
            v = cst.child(cst.root(), routes_rev[i][j]);
            while(v != cst.root() && cst.depth(v) <= routes_rev[i].size()-j){
                marcas[n_routes + i][cst.id(v)] = 1;
                if(cst.depth(v) < routes_rev[i].size()-j){
                    v = cst.child(v, routes_rev[i][cst.depth(v)+j]);
                }else{
                    break;
                }
            }

        }
    }
//    cout << "Marcas en bitvector... OK" << endl;

    // MAP en un vector
    mapa = vector<cst_sct3<>::node_type>(n_routes * 2);
    for(int i = 0; i < n_routes; i++){
        // La primera mitad de mapa para routes
        auto v = cst.child(cst.root(), routes[i][0]);
        while(v != cst.root() && cst.depth(v) < routes[i].size()){
            v = cst.child(v, routes[i][cst.depth(v)]);
        }
        mapa[i] = v;
        // La segunda mitad de mapa para routes_rev
        v = cst.child(cst.root(), routes_rev[i][0]);
        while(v != cst.root() && cst.depth(v) < routes_rev[i].size()){
            v = cst.child(v, routes_rev[i][cst.depth(v)]);
        }
        mapa[n_routes + i] = v;
    }
//    cout << "Map... OK" << endl;
}

string TopoRelGST::obtenerRelacion(int x, int y){
    // Identificar contención según nodo
    // Equal, inside, includes, coveredBy, covers
    int corto, largo;
    if(routes[x].size() < routes[y].size()){
        corto = x;
        largo = y;
    }else{
        corto = y;
        largo = x;
    }
    if(mapa[largo] == mapa[corto]){
        return EQUALS;
    }
    if(mapa[largo] == mapa[corto+n_routes]){
        return EQUALS;
    }
    int id_corto = cst.id(mapa[corto]);
    if(marcas[largo][id_corto] == 1 || marcas[largo + n_routes][id_corto] == 1){
        // Hay contensión
        bool borde = false;
        borde = borde || routes[x][0] == routes[y][0];
        borde = borde || routes[x][0] == routes_rev[y][0];
        borde = borde || routes_rev[x][0] == routes[y][0];
        borde = borde || routes_rev[x][0] == routes_rev[y][0];
        if(borde){
            if(corto == x){
                return COVEREDBY;
            }
            return COVERS;
        }
        if(corto == x){
            return INSIDE;
        }
        return INCLUDES;
    }
    // Identificar otras
    // Touches, Overlaps, Disjoint
    bool touches = false;
    auto root = cst.root();
    for(int i = 1; i < routes[corto].size() - 1; i++){
        auto ch = cst.child(root, routes[corto][i]);
        if(ch != root && marcas[largo][cst.id(ch)]){
            if(routes[corto][i] == routes[largo][0] ||
                    routes[corto][i] == routes_rev[largo][0]){
                touches = true;
            }else{
                return OVERLAPS;
            }
        }
    }
    int idch = cst.id(cst.child(root, routes[corto][0]));
    if(marcas[largo][idch] == 1){
        touches = true;
    }
    idch = cst.id(cst.child(root, routes_rev[corto][0]));
    if(marcas[largo][idch] == 1 || touches){
        return TOUCHES;
    }
    return DISJOINT;
}

bool TopoRelGST::tr_equals(int x, int y){
    if(mapa[x] == mapa[y] || mapa[x] == mapa[y+n_routes]){
        return true;
    }
    return false;
}

bool TopoRelGST::tr_coveredby(int x, int y){
    if(routes[x].size() >= routes[y].size()){
        return false;
    }
    int id_x = cst.id(mapa[x]);
    if(marcas[y][id_x] == 1 || marcas[y + n_routes][id_x] == 1){
        // Hay contensión
        bool borde = false;
        borde = borde || routes[x][0] == routes[y][0];
        borde = borde || routes[x][0] == routes_rev[y][0];
        borde = borde || routes_rev[x][0] == routes[y][0];
        borde = borde || routes_rev[x][0] == routes_rev[y][0];
        if(borde){
            return true;
        }
    }
    return false;
}

bool TopoRelGST::tr_covers(int x, int y){
    if(routes[x].size() <= routes[y].size()){
        return false;
    }
    int id_y = cst.id(mapa[y]);
    if(marcas[x][id_y] == 1 || marcas[x + n_routes][id_y] == 1){
        // Hay contensión
        bool borde = false;
        borde = borde || routes[x][0] == routes[y][0];
        borde = borde || routes[x][0] == routes_rev[y][0];
        borde = borde || routes_rev[x][0] == routes[y][0];
        borde = borde || routes_rev[x][0] == routes_rev[y][0];
        if(borde){
            return true;
        }
    }
    return false;
}

bool TopoRelGST::tr_inside(int x, int y){
    if(routes[x].size() >= routes[y].size()){
        return false;
    }
    int id_x = cst.id(mapa[x]);
    if(marcas[y][id_x] == 1 || marcas[y + n_routes][id_x] == 1){
        // Hay contensión
        bool borde = false;
        borde = borde || routes[x][0] == routes[y][0];
        borde = borde || routes[x][0] == routes_rev[y][0];
        borde = borde || routes_rev[x][0] == routes[y][0];
        borde = borde || routes_rev[x][0] == routes_rev[y][0];
        if(!borde){
            return true;
        }
    }
    return false;
}

bool TopoRelGST::tr_includes(int x, int y){
    if(routes[x].size() <= routes[y].size()){
        return false;
    }
    int id_y = cst.id(mapa[y]);
    if(marcas[x][id_y] == 1 || marcas[x + n_routes][id_y] == 1){
        // Hay contensión
        bool borde = false;
        borde = borde || routes[x][0] == routes[y][0];
        borde = borde || routes[x][0] == routes_rev[y][0];
        borde = borde || routes_rev[x][0] == routes[y][0];
        borde = borde || routes_rev[x][0] == routes_rev[y][0];
        if(!borde){
            return true;
        }
    }
    return false;
}

bool TopoRelGST::tr_disjoint(int x, int y){
    int corto, largo;
    if(routes[x].size() < routes[y].size()){
        corto = x;
        largo = y;
    }else{
        corto = y;
        largo = x;
    }
    if(mapa[largo] == mapa[corto] || mapa[largo] == mapa[corto+n_routes]){
        return false;
    }
    int id_corto = cst.id(mapa[corto]);
    if(marcas[largo][id_corto] == 1 || marcas[largo + n_routes][id_corto] == 1){
        return false;
    }
    // Identificar otras
    // Touches, Overlaps, Disjoint
    bool touches = false;
    auto root = cst.root();
    for(int i = 1; i < routes[corto].size() - 1; i++){
        auto ch = cst.child(root, routes[corto][i]);
        if(ch != root && marcas[largo][cst.id(ch)]){
            if(routes[corto][i] == routes[largo][0] ||
                    routes[corto][i] == routes_rev[largo][0]){
                touches = true;
            }else{
                return false;
            }
        }
    }
    int idch = cst.id(cst.child(root, routes[corto][0]));
    if(marcas[largo][idch] == 1){
        touches = true;
    }
    idch = cst.id(cst.child(root, routes_rev[corto][0]));
    if(marcas[largo][idch] == 1 || touches){
        return false;
    }
    return true;
}

bool TopoRelGST::tr_touches(int x, int y){
    int corto, largo;
    if(routes[x].size() < routes[y].size()){
        corto = x;
        largo = y;
    }else{
        corto = y;
        largo = x;
    }
    if(mapa[largo] == mapa[corto] || mapa[largo] == mapa[corto+n_routes]){
        return false;
    }
    int id_corto = cst.id(mapa[corto]);
    if(marcas[largo][id_corto] == 1 || marcas[largo + n_routes][id_corto] == 1){
        return false;
    }
    // Identificar otras
    // Touches, Overlaps, Disjoint
    bool touches = false;
    auto root = cst.root();
    for(int i = 1; i < routes[corto].size() - 1; i++){
        auto ch = cst.child(root, routes[corto][i]);
        if(ch != root && marcas[largo][cst.id(ch)]){
            if(routes[corto][i] == routes[largo][0] ||
                    routes[corto][i] == routes_rev[largo][0]){
                touches = true;
            }else{
                return false;
            }
        }
    }
    if(touches){
        return true;
    }
    int idch = cst.id(cst.child(root, routes[corto][0]));
    if(marcas[largo][idch] == 1){
        return true;
    }
    idch = cst.id(cst.child(root, routes_rev[corto][0]));
    if(marcas[largo][idch] == 1){
        return true;
    }
    return false;
}

bool TopoRelGST::tr_overlaps(int x, int y){
    int corto, largo;
    if(routes[x].size() < routes[y].size()){
        corto = x;
        largo = y;
    }else{
        corto = y;
        largo = x;
    }
    if(mapa[largo] == mapa[corto] || mapa[largo] == mapa[corto+n_routes]){
        return false;
    }
    int id_corto = cst.id(mapa[corto]);
    if(marcas[largo][id_corto] == 1 || marcas[largo + n_routes][id_corto] == 1){
        return false;
    }
    // Identificar otras
    // Touches, Overlaps, Disjoint
    bool touches = false;
    auto root = cst.root();
    for(int i = 1; i < routes[corto].size() - 1; i++){
        auto ch = cst.child(root, routes[corto][i]);
        if(ch != root && marcas[largo][cst.id(ch)]){
            if(routes[corto][i] == routes[largo][0] ||
                    routes[corto][i] == routes_rev[largo][0]){
                touches = true;
            }else{
                return true;
            }
        }
    }

    return false;
}

//  ---------------------Fin clase TopoRelGST--------------------

bool verificaRuta(vector<int> v){
    for(int j = 0; j < v.size() - 1; j++){
        for(int k = j + 1; k < v.size(); k++){
            if(v[j] == v[k]){
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! falta nombre archivo a procesar como entrada." << endl;
		return -1;
	}
    
	fstream entrada;
	entrada.open(argv[1], fstream::in);
	int n_stops;
	entrada >> n_stops;
//	cout << n_stops << endl;

	map<string,int> mapa_stops;
	for(int i = 1; i <= n_stops; i++){
		string id_stop;
		entrada >> id_stop;
		mapa_stops[id_stop] = i;
//		cout << id_stop << " - " << i << endl;
	}

	int n_routes, n_t;
	string l, t;
	entrada >> n_routes;
//	cout << n_routes << endl;
	vector<vector<int>> routes;

	for(int j = 0; j < n_routes; j++){
		entrada >> l;
//		cout << "-----------------" << endl;
//		cout << l << endl;
		n_t = 1;
		for(int i = 0; i < l.length(); i++){
			if(l[i] == ','){
				l[i] = ' ';
				n_t++;
			}
		}

		vector<int> vr(n_t, 0);
//		cout << "-----------------" << endl;
//		cout << l << endl;
		stringstream ss(l);
//		cout << "-----------------" << endl;
		for(int i = 0; i < n_t; i++){
			ss >> t;
//			cout << t << " ";
			vr[i] = mapa_stops[t];
		}
//		cout << endl << "-----------------" << endl;
//		for(int i = 0; i < n_t; i++){
//			cout << vr[i] << " ";
//		}
//		cout << endl << "-----------------" << endl;
		if(verificaRuta(vr)){
            routes.push_back(vr);
        }
	}
/*
	cout << "Total rutas: " << routes.size() << endl;
	cout << "Ruta 1: ";
	for(int i = 0; i < routes[0].size(); i++){
		cout << routes[0][i] << " ";
	}
	cout << endl;
	cout << "Ruta 2: ";
	for(int i = 0; i < routes[n_routes-1].size(); i++){
		cout << routes[n_routes-1][i] << " ";
	}
	cout << endl;
	string res = toporel(routes[0],routes[n_routes-1]);
	cout << res << endl;
*/

	relaciones_nn_naive(routes);

    relaciones_nn_gst(routes, n_stops);

    return 0;

}


void relaciones_nn_naive(vector<vector<int>> &routes){
    cout << "********** Naive Algorithm **********" << endl;
    unsigned t0 = clock();
	map<string, int> mrt;
	for(int i = 0; i < routes.size(); i++){
		for(int j = 0; j < routes.size(); j++){
			string r = toporel(routes[i], routes[j]);
			mrt[r]++;
		}
	}
    unsigned t1 = clock();
	
    cout << COVEREDBY << ": " << mrt[COVEREDBY] << endl;
	cout << COVERS << ": " << mrt[COVERS] << endl;
	cout << DISJOINT << ": " << mrt[DISJOINT] << endl;
	cout << EQUALS << ": " << mrt[EQUALS] << endl;
	cout << INCLUDES << ": " << mrt[INCLUDES] << endl;
	cout << INSIDE << ": " << mrt[INSIDE] << endl;
	cout << OVERLAPS << ": " << mrt[OVERLAPS] << endl;
	cout << TOUCHES << ": " << mrt[TOUCHES] << endl;
    cout << "Total relaciones: " ;
    int tr = mrt[COVEREDBY] + mrt[COVERS] + mrt[DISJOINT] + mrt[EQUALS] + mrt[INCLUDES] + mrt[INSIDE] + mrt[OVERLAPS] + mrt[TOUCHES];
    cout << tr << endl;
    double tiempo = (double)(t1 - t0)/CLOCKS_PER_SEC;
    cout << "Tiempo total: " << tiempo << " segs." << endl;

    cout << "=== Relaciones individuales ===" << endl;
    map<string, double> trt;
    mrt[COVEREDBY] = 0;
    mrt[COVERS] = 0;
    mrt[DISJOINT] = 0;
    mrt[EQUALS] = 0;
    mrt[INCLUDES] = 0;
    mrt[INSIDE] = 0;
    mrt[OVERLAPS] = 0;
    mrt[TOUCHES] = 0;
    trt[COVEREDBY] = 0.0;
    trt[COVERS] = 0.0;
    trt[DISJOINT] = 0.0;
    trt[EQUALS] = 0.0;
    trt[INCLUDES] = 0.0;
    trt[INSIDE] = 0.0;
    trt[OVERLAPS] = 0.0;
    trt[TOUCHES] = 0.0;
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            int cant_relaciones = 0;
            string resultados = "";
            t0 = clock();
            if(tr_coveredby(routes[i], routes[j])){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[COVEREDBY] += tiempo;
                mrt[COVEREDBY]++;
                cant_relaciones++;
                resultados += COVEREDBY + " ";
            }
            t0 = clock();
            if(tr_covers(routes[i], routes[j])){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[COVERS] += tiempo;
                mrt[COVERS]++;
                cant_relaciones++;
                resultados += COVERS + " ";
            }
            t0 = clock();
            if(tr_disjoint(routes[i], routes[j])){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[DISJOINT] += tiempo;
                mrt[DISJOINT]++;
                cant_relaciones++;
                resultados += DISJOINT + " ";
            }
            t0 = clock();
            if(tr_equals(routes[i], routes[j])){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[EQUALS] += tiempo;
                mrt[EQUALS]++;
                cant_relaciones++;
                resultados += EQUALS + " ";
            }
            t0 = clock();
            if(tr_includes(routes[i], routes[j])){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[INCLUDES] += tiempo;
                mrt[INCLUDES]++;
                cant_relaciones++;
                resultados += INCLUDES + " ";
            }
            t0 = clock();
            if(tr_inside(routes[i], routes[j])){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[INSIDE] += tiempo;
                mrt[INSIDE]++;
                cant_relaciones++;
                resultados += INSIDE + " ";
            }
            t0 = clock();
            if(tr_overlaps(routes[i], routes[j])){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[OVERLAPS] += tiempo;
                mrt[OVERLAPS]++;
                cant_relaciones++;
                resultados += OVERLAPS + " ";
            }
            t0 = clock();
            if(tr_touches(routes[i], routes[j])){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[TOUCHES] += tiempo;
                mrt[TOUCHES]++;
                cant_relaciones++;
                resultados += TOUCHES + " ";
            }
            if(cant_relaciones != 1){
                cout << "Cantidad de relaciones: " << cant_relaciones << endl;
                cout << "Relación Topológica: " << toporel(routes[i], routes[j]) << endl;
                cout << "Resultados: " << resultados << endl;
                mostrar_rutas(routes[i], routes[j]);
            }
        }
    }

    trt[COVEREDBY] = ((trt[COVEREDBY]/CLOCKS_PER_SEC) / mrt[COVEREDBY]) * 1000000;
    trt[COVERS] = ((trt[COVERS]/CLOCKS_PER_SEC) / mrt[COVERS]) * 1000000;
    trt[DISJOINT] = ((trt[DISJOINT]/CLOCKS_PER_SEC) / mrt[DISJOINT]) * 1000000;
    trt[EQUALS] = ((trt[EQUALS]/CLOCKS_PER_SEC) / mrt[EQUALS]) * 1000000;
    trt[INCLUDES] = ((trt[INCLUDES]/CLOCKS_PER_SEC) / mrt[INCLUDES]) * 1000000;
    trt[INSIDE] = ((trt[INSIDE]/CLOCKS_PER_SEC) / mrt[INSIDE]) * 1000000;
    trt[OVERLAPS] = ((trt[OVERLAPS]/CLOCKS_PER_SEC) / mrt[OVERLAPS]) * 1000000;
    trt[TOUCHES] = ((trt[TOUCHES]/CLOCKS_PER_SEC) / mrt[TOUCHES]) * 1000000;

    cout << COVEREDBY << "\t" << mrt[COVEREDBY] << "\t" << trt[COVEREDBY] << "[us]" << endl;
    cout << COVERS << "\t\t" << mrt[COVERS] << "\t" << trt[COVERS] << "[us]" << endl;
    cout << DISJOINT << "\t" << mrt[DISJOINT] << "\t" << trt[DISJOINT] << "[us]" << endl;
    cout << EQUALS << "\t\t" << mrt[EQUALS] << "\t" << trt[EQUALS] << "[us]" << endl;
    cout << INCLUDES << "\t" << mrt[INCLUDES] << "\t" << trt[INCLUDES] << "[us]" << endl;
    cout << INSIDE << "\t\t" << mrt[INSIDE] << "\t" << trt[INSIDE] << "[us]" << endl;
    cout << OVERLAPS << "\t" << mrt[OVERLAPS] << "\t" << trt[OVERLAPS] << "[us]" << endl;
    cout << TOUCHES << "\t\t" << mrt[TOUCHES] << "\t" << trt[TOUCHES] << "[us]" << endl;
    cout << "Total relaciones: " ;
    tr = mrt[COVEREDBY] + mrt[COVERS] + mrt[DISJOINT] + mrt[EQUALS] + mrt[INCLUDES] + mrt[INSIDE] + mrt[OVERLAPS] + mrt[TOUCHES];
    cout << tr << endl;
}


void relaciones_nn_gst(vector<vector<int>> &routes, int n_stops){
    cout << "********** Compressed Suffix Tree **********" << endl;
    unsigned t0 = clock();
    TopoRelGST tt(routes, n_stops);   
    unsigned t1 = clock();
    map<string, int> mrt;
    // Relaciones topológicas
    for(int x = 0; x < tt.n_routes; x++){
        for(int y = 0; y < tt.n_routes; y++){
            string r = tt.obtenerRelacion(x, y);
            mrt[r]++;
        }
    }
    unsigned t2 = clock();

    cout << COVEREDBY << ": " << mrt[COVEREDBY] << endl;
    cout << COVERS << ": " << mrt[COVERS] << endl;
    cout << DISJOINT << ": " << mrt[DISJOINT] << endl;
    cout << EQUALS << ": " << mrt[EQUALS] << endl;
    cout << INCLUDES << ": " << mrt[INCLUDES] << endl;
    cout << INSIDE << ": " << mrt[INSIDE] << endl;
    cout << OVERLAPS << ": " << mrt[OVERLAPS] << endl;
    cout << TOUCHES << ": " << mrt[TOUCHES] << endl;

    double tiempoTotal = (double)(t2 - t0)/CLOCKS_PER_SEC;
    double tiempoCons = (double)(t1 - t0)/CLOCKS_PER_SEC;
    double tiempoOps = (double)(t2 - t1)/CLOCKS_PER_SEC;
    cout << "Tiempo total: " << tiempoTotal << " segs." << endl;
    cout << "Construccion: " << tiempoCons << " segs." << endl;
    cout << "Operaciones: " << tiempoOps << " segs." << endl;


    cout << "=== Relaciones individuales ===" << endl;
    map<string, double> trt;
    double tiempo;
    mrt[COVEREDBY] = 0;
    mrt[COVERS] = 0;
    mrt[DISJOINT] = 0;
    mrt[EQUALS] = 0;
    mrt[INCLUDES] = 0;
    mrt[INSIDE] = 0;
    mrt[OVERLAPS] = 0;
    mrt[TOUCHES] = 0;
    trt[COVEREDBY] = 0.0;
    trt[COVERS] = 0.0;
    trt[DISJOINT] = 0.0;
    trt[EQUALS] = 0.0;
    trt[INCLUDES] = 0.0;
    trt[INSIDE] = 0.0;
    trt[OVERLAPS] = 0.0;
    trt[TOUCHES] = 0.0;
    int tr = 0;
    for(int x = 0; x < tt.n_routes; x++){
        for(int y = 0; y < tt.n_routes; y++){
            t0 = clock();
            if(tt.tr_equals(x, y)){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[EQUALS] += tiempo;
                mrt[EQUALS]++;
            }
            t0 = clock();
            if(tt.tr_coveredby(x, y)){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[COVEREDBY] += tiempo;
                mrt[COVEREDBY]++;
            }
            t0 = clock();
            if(tt.tr_covers(x, y)){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[COVERS] += tiempo;
                mrt[COVERS]++;
            }
            t0 = clock();
            if(tt.tr_inside(x, y)){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[INSIDE] += tiempo;
                mrt[INSIDE]++;
            }
            t0 = clock();
            if(tt.tr_includes(x, y)){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[INCLUDES] += tiempo;
                mrt[INCLUDES]++;
            }
            t0 = clock();
            if(tt.tr_disjoint(x, y)){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[DISJOINT] += tiempo;
                mrt[DISJOINT]++;
            }
            t0 = clock();
            if(tt.tr_touches(x, y)){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[TOUCHES] += tiempo;
                mrt[TOUCHES]++;
            }
            t0 = clock();
            if(tt.tr_overlaps(x, y)){
                t1 = clock();
                tiempo = (double)(t1 - t0);
                trt[OVERLAPS] += tiempo;
                mrt[OVERLAPS]++;
            }
        }
    }
    trt[COVEREDBY] = ((trt[COVEREDBY]/CLOCKS_PER_SEC) / mrt[COVEREDBY]) * 1000000;
    trt[COVERS] = ((trt[COVERS]/CLOCKS_PER_SEC) / mrt[COVERS]) * 1000000;
    trt[DISJOINT] = ((trt[DISJOINT]/CLOCKS_PER_SEC) / mrt[DISJOINT]) * 1000000;
    trt[EQUALS] = ((trt[EQUALS]/CLOCKS_PER_SEC) / mrt[EQUALS]) * 1000000;
    trt[INCLUDES] = ((trt[INCLUDES]/CLOCKS_PER_SEC) / mrt[INCLUDES]) * 1000000;
    trt[INSIDE] = ((trt[INSIDE]/CLOCKS_PER_SEC) / mrt[INSIDE]) * 1000000;
    trt[OVERLAPS] = ((trt[OVERLAPS]/CLOCKS_PER_SEC) / mrt[OVERLAPS]) * 1000000;
    trt[TOUCHES] = ((trt[TOUCHES]/CLOCKS_PER_SEC) / mrt[TOUCHES]) * 1000000;

    cout << COVEREDBY << "\t" << mrt[COVEREDBY] << "\t" << trt[COVEREDBY] << "[us]" << endl;
    cout << COVERS << "\t\t" << mrt[COVERS] << "\t" << trt[COVERS] << "[us]" << endl;
    cout << DISJOINT << "\t" << mrt[DISJOINT] << "\t" << trt[DISJOINT] << "[us]" << endl;
    cout << EQUALS << "\t\t" << mrt[EQUALS] << "\t" << trt[EQUALS] << "[us]" << endl;
    cout << INCLUDES << "\t" << mrt[INCLUDES] << "\t" << trt[INCLUDES] << "[us]" << endl;
    cout << INSIDE << "\t\t" << mrt[INSIDE] << "\t" << trt[INSIDE] << "[us]" << endl;
    cout << OVERLAPS << "\t" << mrt[OVERLAPS] << "\t" << trt[OVERLAPS] << "[us]" << endl;
    cout << TOUCHES << "\t\t" << mrt[TOUCHES] << "\t" << trt[TOUCHES] << "[us]" << endl;
    cout << "Total relaciones: " ;
    tr = mrt[COVEREDBY] + mrt[COVERS] + mrt[DISJOINT] + mrt[EQUALS] + mrt[INCLUDES] + mrt[INSIDE] + mrt[OVERLAPS] + mrt[TOUCHES];
    cout << tr << endl;
}



void compara_relaciones(vector<vector<int>> &routes, int n_stops){
    vector<vector<string>> res_naive(routes.size(), vector<string>(routes.size(), ""));
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            string r = toporel(routes[i], routes[j]);
            res_naive[i][j] = r;
        }
    }

    TopoRelGST tt(routes, n_stops);   
    map<string, int> mrt;
    for(int x = 0; x < tt.n_routes; x++){
        for(int y = 0; y < tt.n_routes; y++){
            string r = tt.obtenerRelacion(x, y);
            if(r != res_naive[x][y]){
                cout << x << " - " << y << " - Naive: " << res_naive[x][y] << " - GST: " << r << endl;
                cout << x << " |" << routes[x].size() << "| : ";
                for(int k = 0; k < routes[x].size(); k++) {
                    cout << routes[x][k] << " ";
                }
                cout << endl;
                cout << y << " |" << routes[y].size() << "| : ";
                for(int k = 0; k < routes[y].size(); k++) {
                    cout << routes[y][k] << " ";
                }
                cout << endl;
            }
        }
    }
}


void mostrar_rutas(vector<int> &a, vector<int> &b){
    cout << "------------" << endl;
    cout << a.size() << " -> ";
    for(int i = 0; i < a.size(); i++){
        cout << a[i] << " ";
    }
    cout << endl << b.size() << " -> ";
    for(int i = 0; i < b.size(); i++){
        cout << b[i] << " ";
    }
    cout << endl << "------------" << endl;
}