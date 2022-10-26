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
#include "TopoRel_GST_0.hpp"
#include "TopoRel_Cons.hpp"

using namespace std;
using namespace sdsl;

void relaciones_nn_naive(vector<vector<int>> &routes);
void relaciones_nn_gst(vector<vector<int>> &routes, int n_stops);
void compara_relaciones(vector<vector<int>> &routes, int n_stops);
void mostrar_rutas(vector<int> &a, vector<int> &b);

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
//  cout << n_stops << endl;

    map<string,int> mapa_stops;
    for(int i = 1; i <= n_stops; i++){
        string id_stop;
        entrada >> id_stop;
        mapa_stops[id_stop] = i;
//      cout << id_stop << " - " << i << endl;
    }

    int n_rutas, n_t;
    string l, t;
    entrada >> n_rutas;
//  cout << n_rutas << endl;
    vector<vector<int>> routes;

    for(int j = 0; j < n_rutas; j++){
        entrada >> l;
//      cout << "-----------------" << endl;
//      cout << l << endl;
        n_t = 1;
        for(int i = 0; i < l.length(); i++){
            if(l[i] == ','){
                l[i] = ' ';
                n_t++;
            }
        }

        vector<int> vr(n_t, 0);
//      cout << "-----------------" << endl;
//      cout << l << endl;
        stringstream ss(l);
//      cout << "-----------------" << endl;
        for(int i = 0; i < n_t; i++){
            ss >> t;
//          cout << t << " ";
            vr[i] = mapa_stops[t];
        }
//      cout << endl << "-----------------" << endl;
//      for(int i = 0; i < n_t; i++){
//          cout << vr[i] << " ";
//      }
//      cout << endl << "-----------------" << endl;
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
    for(int i = 0; i < routes[n_rutas-1].size(); i++){
        cout << routes[n_rutas-1][i] << " ";
    }
    cout << endl;
    string res = toporel(routes[0],routes[n_rutas-1]);
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
    for(int x = 0; x < tt.n_rutas; x++){
        for(int y = 0; y < tt.n_rutas; y++){
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
    for(int x = 0; x < tt.n_rutas; x++){
        for(int y = 0; y < tt.n_rutas; y++){
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
    for(int x = 0; x < tt.n_rutas; x++){
        for(int y = 0; y < tt.n_rutas; y++){
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