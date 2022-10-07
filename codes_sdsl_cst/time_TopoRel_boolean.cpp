#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <map>
#include "TopoRel_Naive.hpp"
#include "TopoRel_Naive_PreComp.hpp"
#include "TopoRel_GST.hpp"
#include "TopoRel_Cons.hpp"

using namespace std;

void mostrar_rutas(vector<int> &a, vector<int> &b);
void tiempos_naive(vector<vector<int>> &routes);
void tiempos_GST(vector<vector<int>> &routes, int n_stops);
void tiempos_naive_precomp(vector<vector<int>> &routes, int n_stops);

int main(int argc, char const *argv[]){
	int n_routes, n_t, n_stops, aux;
	vector<vector<int>> routes;
    cin >> n_routes >> n_stops;
	for(int j = 0; j < n_routes; j++){
		cin >> n_t;
        vector<int> vt(n_t, 0);
        for(int i = 0; i < n_t; i++){
            cin >> vt[i];
        }
        routes.push_back(vt);
	}

    tiempos_naive(routes);
    tiempos_naive_precomp(routes, n_stops);
    tiempos_GST(routes,  n_stops);
	
    return 0;

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

void tiempos_naive(vector<vector<int>> &routes){
    cout << "********** Naive Algorithm **********" << endl;
    map<string, int> mrt;
    map<string, double> tr_all;
    map<string, double> tr_true;
    int tr = 0;
    // Cantidad de relaciones de cada tipo
    mrt[COVEREDBY] = 0;
    mrt[COVERS] = 0;
    mrt[DISJOINT] = 0;
    mrt[EQUALS] = 0;
    mrt[INCLUDES] = 0;
    mrt[INSIDE] = 0;
    mrt[OVERLAPS] = 0;
    mrt[TOUCHES] = 0;
    // Tiempo de llamadas a relaciones boolean
    tr_all[COVEREDBY] = 0.0;
    tr_all[COVERS] = 0.0;
    tr_all[DISJOINT] = 0.0;
    tr_all[EQUALS] = 0.0;
    tr_all[INCLUDES] = 0.0;
    tr_all[INSIDE] = 0.0;
    tr_all[OVERLAPS] = 0.0;
    tr_all[TOUCHES] = 0.0;
    // Tiempo de llamada a relaciones boolean que son true
    tr_true[COVEREDBY] = 0.0;
    tr_true[COVERS] = 0.0;
    tr_true[DISJOINT] = 0.0;
    tr_true[EQUALS] = 0.0;
    tr_true[INCLUDES] = 0.0;
    tr_true[INSIDE] = 0.0;
    tr_true[OVERLAPS] = 0.0;
    tr_true[TOUCHES] = 0.0;

    vector<int> r_coveredby;
    vector<int> r_covers;
    vector<int> r_disjoint;
    vector<int> r_equals;
    vector<int> r_includes;
    vector<int> r_inside;
    vector<int> r_overlaps;
    vector<int> r_touches;

    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            string r = toporel(routes[i], routes[j]);
            mrt[r]++;
            if(r == COVEREDBY){
                r_coveredby.push_back(i);
                r_coveredby.push_back(j);
            }else if(r == COVERS){
                r_covers.push_back(i);
                r_covers.push_back(j);
            }else if(r == DISJOINT){
                r_disjoint.push_back(i);
                r_disjoint.push_back(j);
            }else if(r == EQUALS){
                r_equals.push_back(i);
                r_equals.push_back(j);
            }else if(r == INCLUDES){
                r_includes.push_back(i);
                r_includes.push_back(j);
            }else if(r == INSIDE){
                r_inside.push_back(i);
                r_inside.push_back(j);
            }else if(r == OVERLAPS){
                r_overlaps.push_back(i);
                r_overlaps.push_back(j);
            }else if(r == TOUCHES){
                r_touches.push_back(i);
                r_touches.push_back(j);
            }else{
                cout << "Error! routas sin relacion topológica: " << i << " " << j << endl;
            }
        }
    }

    unsigned t0;
    unsigned t1;
    double tiempo = 0.0;
    int x, y;

    int minMuestras = 100;
    int mstrsCovB = 0;
    int mstrsCovs = 0;
    int mstrsDisj = 0;
    int mstrsEqls = 0;
    int mstrsIncl = 0;
    int mstrsInsi = 0;
    int mstrsOver = 0;
    int mstrsTouc = 0;
    
    t0 = clock();
    while(mstrsCovB < minMuestras && r_coveredby.size() > 0){
        for(int j = 0; j < r_coveredby.size(); j += 2){
            x = r_coveredby[j];
            y = r_coveredby[j + 1];
            tr_coveredby(routes[x], routes[y]);
            mstrsCovB++;
        }
    }
    t1 = clock();
    tr_true[COVEREDBY] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsCovs < minMuestras && r_covers.size() > 0){
        for(int j = 0; j < r_covers.size(); j += 2){
            x = r_covers[j];
            y = r_covers[j + 1];
            tr_covers(routes[x], routes[y]);
            mstrsCovs++;
        }
    }
    t1 = clock();
    tr_true[COVERS] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsDisj < minMuestras && r_disjoint.size() > 0){
        for(int j = 0; j < r_disjoint.size(); j += 2){
            x = r_disjoint[j];
            y = r_disjoint[j + 1];
            tr_disjoint(routes[x], routes[y]);
            mstrsDisj++;
        }
    }
    t1 = clock();
    tr_true[DISJOINT] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsEqls < minMuestras && r_equals.size() > 0){
        for(int j = 0; j < r_equals.size(); j += 2){
            x = r_equals[j];
            y = r_equals[j + 1];
            tr_equals(routes[x], routes[y]);
            mstrsEqls++;
        }
    }
    t1 = clock();
    tr_true[EQUALS] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsIncl < minMuestras && r_includes.size() > 0){
        for(int j = 0; j < r_includes.size(); j += 2){
            x = r_includes[j];
            y = r_includes[j + 1];
            tr_includes(routes[x], routes[y]);
            mstrsIncl++;
        }
    }
    t1 = clock();
    tr_true[INCLUDES] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsInsi < minMuestras && r_inside.size() > 0){
        for(int j = 0; j < r_inside.size(); j += 2){
            x = r_inside[j];
            y = r_inside[j + 1];
            tr_inside(routes[x], routes[y]);
            mstrsInsi++;
        }
    }
    t1 = clock();
    tr_true[INSIDE] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsOver < minMuestras && r_overlaps.size() > 0){
        for(int j = 0; j < r_overlaps.size(); j += 2){
            x = r_overlaps[j];
            y = r_overlaps[j + 1];
            tr_overlaps(routes[x], routes[y]);
            mstrsOver++;
        }
    }
    t1 = clock();
    tr_true[OVERLAPS] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsTouc < minMuestras && r_touches.size() > 0){
        for(int j = 0; j < r_touches.size(); j += 2){
            x = r_touches[j];
            y = r_touches[j + 1];
            tr_touches(routes[x], routes[y]);
            mstrsTouc++;
        }
    }
    t1 = clock();
    tr_true[TOUCHES] = (double)(t1 - t0);


    tr_true[COVEREDBY] = ((tr_true[COVEREDBY]/CLOCKS_PER_SEC) / mstrsCovB) * 1000000;
    tr_true[COVERS] = ((tr_true[COVERS]/CLOCKS_PER_SEC) / mstrsCovs) * 1000000;
    tr_true[DISJOINT] = ((tr_true[DISJOINT]/CLOCKS_PER_SEC) / mstrsDisj) * 1000000;
    tr_true[EQUALS] = ((tr_true[EQUALS]/CLOCKS_PER_SEC) / mstrsEqls) * 1000000;
    tr_true[INCLUDES] = ((tr_true[INCLUDES]/CLOCKS_PER_SEC) / mstrsIncl) * 1000000;
    tr_true[INSIDE] = ((tr_true[INSIDE]/CLOCKS_PER_SEC) / mstrsInsi) * 1000000;
    tr_true[OVERLAPS] = ((tr_true[OVERLAPS]/CLOCKS_PER_SEC) / mstrsOver) * 1000000;
    tr_true[TOUCHES] = ((tr_true[TOUCHES]/CLOCKS_PER_SEC) / mstrsTouc) * 1000000;

    tr = mrt[COVEREDBY] + mrt[COVERS] + mrt[DISJOINT] + mrt[EQUALS] + mrt[INCLUDES] + mrt[INSIDE] + mrt[OVERLAPS] + mrt[TOUCHES];

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_coveredby(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVEREDBY] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_covers(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVERS] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_disjoint(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[DISJOINT] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_equals(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[EQUALS] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_includes(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INCLUDES] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_inside(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INSIDE] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_overlaps(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[OVERLAPS] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_touches(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[TOUCHES] += tiempo;


    tr_all[COVEREDBY] = ((tr_all[COVEREDBY]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[COVERS] = ((tr_all[COVERS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[DISJOINT] = ((tr_all[DISJOINT]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[EQUALS] = ((tr_all[EQUALS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INCLUDES] = ((tr_all[INCLUDES]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INSIDE] = ((tr_all[INSIDE]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[OVERLAPS] = ((tr_all[OVERLAPS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[TOUCHES] = ((tr_all[TOUCHES]/CLOCKS_PER_SEC) / tr) * 1000000;

    cout << "TopoRelation\tcant\tTime_true\tTime_all" << endl;
    cout << COVEREDBY << "\t" << mrt[COVEREDBY] << "\t" << tr_true[COVEREDBY] << " [us]\t" << tr_all[COVEREDBY] << " [us]" << endl;
    cout << COVERS << "\t\t" << mrt[COVERS] << "\t" << tr_true[COVERS] << " [us]\t" << tr_all[COVERS] << " [us]" << endl;
    cout << DISJOINT << "\t" << mrt[DISJOINT] << "\t" << tr_true[DISJOINT] << " [us]\t" << tr_all[DISJOINT] << " [us]" << endl;
    cout << EQUALS << "\t\t" << mrt[EQUALS] << "\t" << tr_true[EQUALS] << " [us]\t" << tr_all[EQUALS] << " [us]" << endl;
    cout << INCLUDES << "\t" << mrt[INCLUDES] << "\t" << tr_true[INCLUDES] << " [us]\t" << tr_all[INCLUDES] << " [us]" << endl;
    cout << INSIDE << "\t\t" << mrt[INSIDE] << "\t" << tr_true[INSIDE] << " [us]\t" << tr_all[INSIDE] << " [us]" << endl;
    cout << OVERLAPS << "\t" << mrt[OVERLAPS] << "\t" << tr_true[OVERLAPS] << " [us]\t" << tr_all[OVERLAPS] << " [us]" << endl;
    cout << TOUCHES << "\t\t" << mrt[TOUCHES] << "\t" << tr_true[TOUCHES] << " [us]\t" << tr_all[TOUCHES] << " [us]" << endl;
    cout << "Total relaciones: " ;
    cout << tr << endl;
}


void tiempos_GST(vector<vector<int>> &routes, int n_stops){
    unsigned t0;
    unsigned t1;
    double tiempo = 0.0;
    cout << "********** Compressed Suffix Tree **********" << endl;
    t0 = clock();
    TopoRelGST tt(routes, n_stops);
    t1 = clock();
    double tConstruccion = ((t1-t0)/CLOCKS_PER_SEC) * 1000000;
    cout << "Tiempo Construcción: " << tConstruccion << "[ms]" << endl;
    tt.sizeEstructura();
    cout << endl;
    map<string, int> mrt;
    map<string, double> tr_all;
    map<string, double> tr_true;
    int tr = 0;
    // Cantidad de relaciones de cada tipo
    mrt[COVEREDBY] = 0;
    mrt[COVERS] = 0;
    mrt[DISJOINT] = 0;
    mrt[EQUALS] = 0;
    mrt[INCLUDES] = 0;
    mrt[INSIDE] = 0;
    mrt[OVERLAPS] = 0;
    mrt[TOUCHES] = 0;
    // Tiempo de llamadas a relaciones boolean
    tr_all[COVEREDBY] = 0.0;
    tr_all[COVERS] = 0.0;
    tr_all[DISJOINT] = 0.0;
    tr_all[EQUALS] = 0.0;
    tr_all[INCLUDES] = 0.0;
    tr_all[INSIDE] = 0.0;
    tr_all[OVERLAPS] = 0.0;
    tr_all[TOUCHES] = 0.0;
    // Tiempo de llamada a relaciones boolean que son true
    tr_true[COVEREDBY] = 0.0;
    tr_true[COVERS] = 0.0;
    tr_true[DISJOINT] = 0.0;
    tr_true[EQUALS] = 0.0;
    tr_true[INCLUDES] = 0.0;
    tr_true[INSIDE] = 0.0;
    tr_true[OVERLAPS] = 0.0;
    tr_true[TOUCHES] = 0.0;

    vector<int> r_coveredby;
    vector<int> r_covers;
    vector<int> r_disjoint;
    vector<int> r_equals;
    vector<int> r_includes;
    vector<int> r_inside;
    vector<int> r_overlaps;
    vector<int> r_touches;

    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            string r = tt.obtenerRelacion(i, j);
            mrt[r]++;
            if(r == COVEREDBY){
                r_coveredby.push_back(i);
                r_coveredby.push_back(j);
            }else if(r == COVERS){
                r_covers.push_back(i);
                r_covers.push_back(j);
            }else if(r == DISJOINT){
                r_disjoint.push_back(i);
                r_disjoint.push_back(j);
            }else if(r == EQUALS){
                r_equals.push_back(i);
                r_equals.push_back(j);
            }else if(r == INCLUDES){
                r_includes.push_back(i);
                r_includes.push_back(j);
            }else if(r == INSIDE){
                r_inside.push_back(i);
                r_inside.push_back(j);
            }else if(r == OVERLAPS){
                r_overlaps.push_back(i);
                r_overlaps.push_back(j);
            }else if(r == TOUCHES){
                r_touches.push_back(i);
                r_touches.push_back(j);
            }else{
                cout << "Error! routas sin relacion topológica: " << i << " " << j << endl;
            }
        }
    }

    int x, y;

    int minMuestras = 100;
    int mstrsCovB = 0;
    int mstrsCovs = 0;
    int mstrsDisj = 0;
    int mstrsEqls = 0;
    int mstrsIncl = 0;
    int mstrsInsi = 0;
    int mstrsOver = 0;
    int mstrsTouc = 0;
    
    t0 = clock();
    while(mstrsCovB < minMuestras && r_coveredby.size() > 0){
        for(int j = 0; j < r_coveredby.size(); j += 2){
            x = r_coveredby[j];
            y = r_coveredby[j + 1];
            tt.tr_coveredby(x, y);
            mstrsCovB++;
        }
    }
    t1 = clock();
    tr_true[COVEREDBY] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsCovs < minMuestras && r_covers.size() > 0){
        for(int j = 0; j < r_covers.size(); j += 2){
            x = r_covers[j];
            y = r_covers[j + 1];
            tt.tr_covers(x, y);
            mstrsCovs++;
        }
    }
    t1 = clock();
    tr_true[COVERS] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsDisj < minMuestras && r_disjoint.size() > 0){
        for(int j = 0; j < r_disjoint.size(); j += 2){
            x = r_disjoint[j];
            y = r_disjoint[j + 1];
            tt.tr_disjoint(x, y);
            mstrsDisj++;
        }
    }
    t1 = clock();
    tr_true[DISJOINT] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsEqls < minMuestras && r_equals.size() > 0){
        for(int j = 0; j < r_equals.size(); j += 2){
            x = r_equals[j];
            y = r_equals[j + 1];
            tt.tr_equals(x, y);
            mstrsEqls++;
        }
    }
    t1 = clock();
    tr_true[EQUALS] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsIncl < minMuestras && r_includes.size() > 0){
        for(int j = 0; j < r_includes.size(); j += 2){
            x = r_includes[j];
            y = r_includes[j + 1];
            tt.tr_includes(x, y);
            mstrsIncl++;
        }
    }
    t1 = clock();
    tr_true[INCLUDES] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsInsi < minMuestras && r_inside.size() > 0){
        for(int j = 0; j < r_inside.size(); j += 2){
            x = r_inside[j];
            y = r_inside[j + 1];
            tt.tr_inside(x, y);
            mstrsInsi++;
        }
    }
    t1 = clock();
    tr_true[INSIDE] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsOver < minMuestras && r_overlaps.size() > 0){
        for(int j = 0; j < r_overlaps.size(); j += 2){
            x = r_overlaps[j];
            y = r_overlaps[j + 1];
            tt.tr_overlaps(x, y);
            mstrsOver++;
        }
    }
    t1 = clock();
    tr_true[OVERLAPS] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsTouc < minMuestras && r_touches.size() > 0){
        for(int j = 0; j < r_touches.size(); j += 2){
            x = r_touches[j];
            y = r_touches[j + 1];
            tt.tr_touches(x, y);
            mstrsTouc++;
        }
    }
    t1 = clock();
    tr_true[TOUCHES] = (double)(t1 - t0);


    tr_true[COVEREDBY] = ((tr_true[COVEREDBY]/CLOCKS_PER_SEC) / mstrsCovB) * 1000000;
    tr_true[COVERS] = ((tr_true[COVERS]/CLOCKS_PER_SEC) / mstrsCovs) * 1000000;
    tr_true[DISJOINT] = ((tr_true[DISJOINT]/CLOCKS_PER_SEC) / mstrsDisj) * 1000000;
    tr_true[EQUALS] = ((tr_true[EQUALS]/CLOCKS_PER_SEC) / mstrsEqls) * 1000000;
    tr_true[INCLUDES] = ((tr_true[INCLUDES]/CLOCKS_PER_SEC) / mstrsIncl) * 1000000;
    tr_true[INSIDE] = ((tr_true[INSIDE]/CLOCKS_PER_SEC) / mstrsInsi) * 1000000;
    tr_true[OVERLAPS] = ((tr_true[OVERLAPS]/CLOCKS_PER_SEC) / mstrsOver) * 1000000;
    tr_true[TOUCHES] = ((tr_true[TOUCHES]/CLOCKS_PER_SEC) / mstrsTouc) * 1000000;

    tr = mrt[COVEREDBY] + mrt[COVERS] + mrt[DISJOINT] + mrt[EQUALS] + mrt[INCLUDES] + mrt[INSIDE] + mrt[OVERLAPS] + mrt[TOUCHES];

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_coveredby(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVEREDBY] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_covers(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVERS] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_disjoint(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[DISJOINT] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_equals(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[EQUALS] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_includes(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INCLUDES] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_inside(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INSIDE] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_overlaps(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[OVERLAPS] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_touches(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[TOUCHES] += tiempo;


    tr_all[COVEREDBY] = ((tr_all[COVEREDBY]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[COVERS] = ((tr_all[COVERS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[DISJOINT] = ((tr_all[DISJOINT]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[EQUALS] = ((tr_all[EQUALS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INCLUDES] = ((tr_all[INCLUDES]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INSIDE] = ((tr_all[INSIDE]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[OVERLAPS] = ((tr_all[OVERLAPS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[TOUCHES] = ((tr_all[TOUCHES]/CLOCKS_PER_SEC) / tr) * 1000000;

    cout << "TopoRelation\tcant\tTime_true\tTime_all" << endl;
    cout << COVEREDBY << "\t" << mrt[COVEREDBY] << "\t" << tr_true[COVEREDBY] << " [us]\t" << tr_all[COVEREDBY] << " [us]" << endl;
    cout << COVERS << "\t\t" << mrt[COVERS] << "\t" << tr_true[COVERS] << " [us]\t" << tr_all[COVERS] << " [us]" << endl;
    cout << DISJOINT << "\t" << mrt[DISJOINT] << "\t" << tr_true[DISJOINT] << " [us]\t" << tr_all[DISJOINT] << " [us]" << endl;
    cout << EQUALS << "\t\t" << mrt[EQUALS] << "\t" << tr_true[EQUALS] << " [us]\t" << tr_all[EQUALS] << " [us]" << endl;
    cout << INCLUDES << "\t" << mrt[INCLUDES] << "\t" << tr_true[INCLUDES] << " [us]\t" << tr_all[INCLUDES] << " [us]" << endl;
    cout << INSIDE << "\t\t" << mrt[INSIDE] << "\t" << tr_true[INSIDE] << " [us]\t" << tr_all[INSIDE] << " [us]" << endl;
    cout << OVERLAPS << "\t" << mrt[OVERLAPS] << "\t" << tr_true[OVERLAPS] << " [us]\t" << tr_all[OVERLAPS] << " [us]" << endl;
    cout << TOUCHES << "\t\t" << mrt[TOUCHES] << "\t" << tr_true[TOUCHES] << " [us]\t" << tr_all[TOUCHES] << " [us]" << endl;
    cout << "Total relaciones: " ;
    cout << tr << endl;
}

void tiempos_naive_precomp(vector<vector<int>> &routes, int n_stops){
    cout << "********** Naive Pre-Computed **********" << endl;
    unsigned t0 = clock();
    TopoRelNaivePreComp trpc(routes, n_stops);
    unsigned t1 = clock();
    double tConstruccion = ((t1-t0)/CLOCKS_PER_SEC) * 1000000;
    cout << "Tiempo Construcción: " << tConstruccion << "[ms]" << endl;
    trpc.sizeEstructura();
    cout << endl;
    map<string, int> mrt;
    map<string, double> tr_all;
    map<string, double> tr_true;
    int tr = 0;
    // Cantidad de relaciones de cada tipo
    mrt[COVEREDBY] = 0;
    mrt[COVERS] = 0;
    mrt[DISJOINT] = 0;
    mrt[EQUALS] = 0;
    mrt[INCLUDES] = 0;
    mrt[INSIDE] = 0;
    mrt[OVERLAPS] = 0;
    mrt[TOUCHES] = 0;
    // Tiempo de llamadas a relaciones boolean
    tr_all[COVEREDBY] = 0.0;
    tr_all[COVERS] = 0.0;
    tr_all[DISJOINT] = 0.0;
    tr_all[EQUALS] = 0.0;
    tr_all[INCLUDES] = 0.0;
    tr_all[INSIDE] = 0.0;
    tr_all[OVERLAPS] = 0.0;
    tr_all[TOUCHES] = 0.0;
    // Tiempo de llamada a relaciones boolean que son true
    tr_true[COVEREDBY] = 0.0;
    tr_true[COVERS] = 0.0;
    tr_true[DISJOINT] = 0.0;
    tr_true[EQUALS] = 0.0;
    tr_true[INCLUDES] = 0.0;
    tr_true[INSIDE] = 0.0;
    tr_true[OVERLAPS] = 0.0;
    tr_true[TOUCHES] = 0.0;

    vector<int> r_coveredby;
    vector<int> r_covers;
    vector<int> r_disjoint;
    vector<int> r_equals;
    vector<int> r_includes;
    vector<int> r_inside;
    vector<int> r_overlaps;
    vector<int> r_touches;

    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            string r = trpc.obtenerRelacion(i, j);
            mrt[r]++;
            if(r == COVEREDBY){
                r_coveredby.push_back(i);
                r_coveredby.push_back(j);
            }else if(r == COVERS){
                r_covers.push_back(i);
                r_covers.push_back(j);
            }else if(r == DISJOINT){
                r_disjoint.push_back(i);
                r_disjoint.push_back(j);
            }else if(r == EQUALS){
                r_equals.push_back(i);
                r_equals.push_back(j);
            }else if(r == INCLUDES){
                r_includes.push_back(i);
                r_includes.push_back(j);
            }else if(r == INSIDE){
                r_inside.push_back(i);
                r_inside.push_back(j);
            }else if(r == OVERLAPS){
                r_overlaps.push_back(i);
                r_overlaps.push_back(j);
            }else if(r == TOUCHES){
                r_touches.push_back(i);
                r_touches.push_back(j);
            }else{
                cout << "Error! rutas sin relacion topológica: " << i << " " << j << endl;
            }
        }
    }

    int x, y;
    double tiempo = 0.0;
    int minMuestras = 100;
    int mstrsCovB = 0;
    int mstrsCovs = 0;
    int mstrsDisj = 0;
    int mstrsEqls = 0;
    int mstrsIncl = 0;
    int mstrsInsi = 0;
    int mstrsOver = 0;
    int mstrsTouc = 0;
    
    t0 = clock();
    while(mstrsCovB < minMuestras && r_coveredby.size() > 0){
        for(int j = 0; j < r_coveredby.size(); j += 2){
            x = r_coveredby[j];
            y = r_coveredby[j + 1];
            trpc.coveredby(x, y);
            mstrsCovB++;
        }
    }
    t1 = clock();
    tr_true[COVEREDBY] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsCovs < minMuestras && r_covers.size() > 0){
        for(int j = 0; j < r_covers.size(); j += 2){
            x = r_covers[j];
            y = r_covers[j + 1];
            trpc.covers(x, y);
            mstrsCovs++;
        }
    }
    t1 = clock();
    tr_true[COVERS] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsDisj < minMuestras && r_disjoint.size() > 0){
        for(int j = 0; j < r_disjoint.size(); j += 2){
            x = r_disjoint[j];
            y = r_disjoint[j + 1];
            trpc.disjoint(x, y);
            mstrsDisj++;
        }
    }
    t1 = clock();
    tr_true[DISJOINT] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsEqls < minMuestras && r_equals.size() > 0){
        for(int j = 0; j < r_equals.size(); j += 2){
            x = r_equals[j];
            y = r_equals[j + 1];
            trpc.equals(x, y);
            mstrsEqls++;
        }
    }
    t1 = clock();
    tr_true[EQUALS] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsIncl < minMuestras && r_includes.size() > 0){
        for(int j = 0; j < r_includes.size(); j += 2){
            x = r_includes[j];
            y = r_includes[j + 1];
            trpc.includes(x, y);
            mstrsIncl++;
        }
    }
    t1 = clock();
    tr_true[INCLUDES] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsInsi < minMuestras && r_inside.size() > 0){
        for(int j = 0; j < r_inside.size(); j += 2){
            x = r_inside[j];
            y = r_inside[j + 1];
            trpc.inside(x, y);
            mstrsInsi++;
        }
    }
    t1 = clock();
    tr_true[INSIDE] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsOver < minMuestras && r_overlaps.size() > 0){
        for(int j = 0; j < r_overlaps.size(); j += 2){
            x = r_overlaps[j];
            y = r_overlaps[j + 1];
            trpc.overlaps(x, y);
            mstrsOver++;
        }
    }
    t1 = clock();
    tr_true[OVERLAPS] = (double)(t1 - t0);
    
    t0 = clock();
    while(mstrsTouc < minMuestras && r_touches.size() > 0){
        for(int j = 0; j < r_touches.size(); j += 2){
            x = r_touches[j];
            y = r_touches[j + 1];
            trpc.touches(x, y);
            mstrsTouc++;
        }
    }
    t1 = clock();
    tr_true[TOUCHES] = (double)(t1 - t0);

    tr_true[COVEREDBY] = ((tr_true[COVEREDBY]/CLOCKS_PER_SEC) / mstrsCovB) * 1000000;
    tr_true[COVERS] = ((tr_true[COVERS]/CLOCKS_PER_SEC) / mstrsCovs) * 1000000;
    tr_true[DISJOINT] = ((tr_true[DISJOINT]/CLOCKS_PER_SEC) / mstrsDisj) * 1000000;
    tr_true[EQUALS] = ((tr_true[EQUALS]/CLOCKS_PER_SEC) / mstrsEqls) * 1000000;
    tr_true[INCLUDES] = ((tr_true[INCLUDES]/CLOCKS_PER_SEC) / mstrsIncl) * 1000000;
    tr_true[INSIDE] = ((tr_true[INSIDE]/CLOCKS_PER_SEC) / mstrsInsi) * 1000000;
    tr_true[OVERLAPS] = ((tr_true[OVERLAPS]/CLOCKS_PER_SEC) / mstrsOver) * 1000000;
    tr_true[TOUCHES] = ((tr_true[TOUCHES]/CLOCKS_PER_SEC) / mstrsTouc) * 1000000;

    tr = mrt[COVEREDBY] + mrt[COVERS] + mrt[DISJOINT] + mrt[EQUALS] + mrt[INCLUDES] + mrt[INSIDE] + mrt[OVERLAPS] + mrt[TOUCHES];

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.coveredby(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVEREDBY] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.covers(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVERS] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.disjoint(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[DISJOINT] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.equals(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[EQUALS] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.includes(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INCLUDES] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.inside(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INSIDE] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.overlaps(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[OVERLAPS] += tiempo;

    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.touches(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[TOUCHES] += tiempo;


    tr_all[COVEREDBY] = ((tr_all[COVEREDBY]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[COVERS] = ((tr_all[COVERS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[DISJOINT] = ((tr_all[DISJOINT]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[EQUALS] = ((tr_all[EQUALS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INCLUDES] = ((tr_all[INCLUDES]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INSIDE] = ((tr_all[INSIDE]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[OVERLAPS] = ((tr_all[OVERLAPS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[TOUCHES] = ((tr_all[TOUCHES]/CLOCKS_PER_SEC) / tr) * 1000000;

    cout << "TopoRelation\tcant\tTime_true\tTime_all" << endl;
    cout << COVEREDBY << "\t" << mrt[COVEREDBY] << "\t" << tr_true[COVEREDBY] << " [us]\t" << tr_all[COVEREDBY] << " [us]" << endl;
    cout << COVERS << "\t\t" << mrt[COVERS] << "\t" << tr_true[COVERS] << " [us]\t" << tr_all[COVERS] << " [us]" << endl;
    cout << DISJOINT << "\t" << mrt[DISJOINT] << "\t" << tr_true[DISJOINT] << " [us]\t" << tr_all[DISJOINT] << " [us]" << endl;
    cout << EQUALS << "\t\t" << mrt[EQUALS] << "\t" << tr_true[EQUALS] << " [us]\t" << tr_all[EQUALS] << " [us]" << endl;
    cout << INCLUDES << "\t" << mrt[INCLUDES] << "\t" << tr_true[INCLUDES] << " [us]\t" << tr_all[INCLUDES] << " [us]" << endl;
    cout << INSIDE << "\t\t" << mrt[INSIDE] << "\t" << tr_true[INSIDE] << " [us]\t" << tr_all[INSIDE] << " [us]" << endl;
    cout << OVERLAPS << "\t" << mrt[OVERLAPS] << "\t" << tr_true[OVERLAPS] << " [us]\t" << tr_all[OVERLAPS] << " [us]" << endl;
    cout << TOUCHES << "\t\t" << mrt[TOUCHES] << "\t" << tr_true[TOUCHES] << " [us]\t" << tr_all[TOUCHES] << " [us]" << endl;
    cout << "Total relaciones: " ;
    cout << tr << endl;
}

