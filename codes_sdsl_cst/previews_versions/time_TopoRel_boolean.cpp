#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <map>
#include "TopoRel_Naive.hpp"
#include "TopoRel_Naive_PreComp.hpp"
#include "TopoRel_Cons.hpp"
#include "TopoRel_GST_0.hpp"
#include "TopoRel_GST_1.hpp"
#include "TopoRel_GST_2.hpp"
#include "TopoRel_GST_4.hpp"

using namespace std;

void mostrar_rutas(vector<int> &a, vector<int> &b);
vector<vector<double>> tiempos_naive(vector<vector<int>> &routes);
vector<vector<double>> tiempos_naive_precomp(vector<vector<int>> &routes, int n_stops);
vector<vector<double>> tiempos_GST_0(vector<vector<int>> &routes, int n_stops);
vector<vector<double>> tiempos_GST_1(vector<vector<int>> &routes, int n_stops);
vector<vector<double>> tiempos_GST_2(vector<vector<int>> &routes, int n_stops);
vector<vector<double>> tiempos_GST_4(vector<vector<int>> &routes, int n_stops);

int idCoveredby = 0;
int idCovers = 1;
int idEquals = 2;
int idIncludes = 3;
int idInside = 4;
int idOverlaps = 5;
int idTouches = 6;
int idDisjoint = 7;

int idWithin = 8;
int idContains = 9;
int idIntersect = 10;

int minMuestras = 100;

int main(int argc, char const *argv[]){

    if(argc > 1){
        minMuestras = (int) atoi(argv[1]);
    }

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
    //cout << fixed;  // Mostrar los decimales con notación en digitos

    vector<vector<double>> mNaive = tiempos_naive(routes);
    vector<vector<double>> pCNaive = tiempos_naive_precomp(routes, n_stops);
    vector<vector<double>> mGST0 = tiempos_GST_0(routes,  n_stops);
    vector<vector<double>> mGST1 = tiempos_GST_1(routes,  n_stops);
    vector<vector<double>> mGST2 = tiempos_GST_2(routes,  n_stops);
    vector<vector<double>> mGST4 = tiempos_GST_4(routes,  n_stops);

    vector<string> ns = {"Coveredby", "Covers", "Equals", "Includes", "Inside", "Overlaps", "Touches", "Disjoint", "Whitin", "Contains", "Intersect"};    

    cout << " *********************** Relaciones Básicas *********************** " << endl;
    cout << "%relacion\tnvCant\tnvSamp\tnvTrue\tnvAll\tpcCant\tpcSamp\tpcTrue\tpcAll\tgst0Can\tgst0Sam\tgst0True\tgst0All\tgst1Can\tgst1Sam\tgst1True\tgst1All\tgst2Can\tgst2Sam\tgst2True\tgst2All\tgst4Can\tgst4Sam\tgst4True\tgst4All" << endl;
    for(int i=0; i<8; i++){
        cout << ns[i] << "\t";
        for(int j=0; j<4; j++){
            cout << mNaive[i][j] << "\t";
        }
        for(int j=0; j<4; j++){
            cout << pCNaive[i][j] << "\t";
        }
        for(int j=0; j<4; j++){
            cout << mGST0[i][j] << "\t";
        }
        for(int j=0; j<4; j++){
            cout << mGST1[i][j] << "\t";
        }
        for(int j=0; j<4; j++){
            cout << mGST2[i][j] << "\t";
        }
        for(int j=0; j<4; j++){
            cout << mGST4[i][j] << "\t";
        }
        cout << endl;
    }

    cout << " *********************** Relaciones Agregadas *********************** " << endl;
    cout << "%relacion\tcant\tnvAll\tpcAll\tgst0All\tgst1All\tgst2All\tgst4All" << endl;
    for(int i=8; i<11; i++){
        cout << ns[i] << "\t";
        cout << mNaive[i][0] << "\t";
        cout << mNaive[i][3] << "\t";
        cout << pCNaive[i][3] << "\t";
        cout << mGST0[i][3] << "\t";
        cout << mGST1[i][3] << "\t";
        cout << mGST2[i][3] << "\t";
        cout << mGST4[i][3] << "\t";
        cout << endl;
    }
	
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

vector<vector<double>> tiempos_naive(vector<vector<int>> &routes){
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

    tr_all[WHITIN] = 0.0;
    tr_all[CONTAINS] = 0.0;
    tr_all[INTERSECT] = 0.0;
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

    cout << ">>> Calculando..." << endl << "\tRelación Topológica ... ";
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
    cout << "OK" << endl;

    vector<vector<double>> m(11, vector<double>(4, 0.0));

    // Cantidad de relaciones
    m[idCoveredby][0] = r_coveredby.size()/2;
    m[idCovers][0] = r_covers.size()/2;
    m[idDisjoint][0] = r_disjoint.size()/2;
    m[idEquals][0] = r_equals.size()/2;
    m[idIncludes][0] = r_includes.size()/2;
    m[idInside][0] = r_inside.size()/2;
    m[idOverlaps][0] = r_overlaps.size()/2;
    m[idTouches][0] = r_touches.size()/2;

    m[idWithin][0] = m[idCoveredby][0] + m[idEquals][0] + m[idInside][0];
    m[idContains][0] = m[idCovers][0] + m[idEquals][0] + m[idIncludes][0];
    m[idIntersect][0] = m[idCoveredby][0] + m[idCovers][0] + m[idEquals][0] + m[idIncludes][0] + m[idInside][0] + m[idOverlaps][0] + m[idTouches][0];


    unsigned t0;
    unsigned t1;
    double tiempo = 0.0;
    int x, y;
    int mstrsCovB = 0;
    int mstrsCovs = 0;
    int mstrsDisj = 0;
    int mstrsEqls = 0;
    int mstrsIncl = 0;
    int mstrsInsi = 0;
    int mstrsOver = 0;
    int mstrsTouc = 0;
    
    cout << "\tRelaciones binarias TRUE ... ";
    cout << "CB ... ";
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
    if(r_coveredby.size() == 0){
        tr_true[COVEREDBY] = 0;
    }
    
    cout << "CV ... ";
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
    if(r_covers.size() == 0){
        tr_true[COVERS] = 0;
    }
    
    cout << "DJ ... ";
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
    if(r_disjoint.size() == 0){
        tr_true[DISJOINT] = 0;
    }
    
    cout << "EQ ... ";
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
    if(r_equals.size() == 0){
        tr_true[EQUALS] = 0;
    }
    
    cout << "IC ... ";
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
    if(r_includes.size() == 0){
        tr_true[INCLUDES] = 0;
    }
    
    cout << "IS ... ";
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
    if(r_inside.size() == 0){
        tr_true[INSIDE] = 0;
    }
    
    cout << "OV ... ";
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
    if(r_overlaps.size() == 0){
        tr_true[OVERLAPS] = 0;
    }
    
    cout << "TO ... ";
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
    if(r_touches.size() == 0){
        tr_true[TOUCHES] = 0;
    }

    cout << "OK" << endl;

    // Cantidad de muestras para tiempos_true
    m[idCoveredby][1] = mstrsCovB;
    m[idCovers][1] = mstrsCovs;
    m[idDisjoint][1] = mstrsDisj;
    m[idEquals][1] = mstrsEqls;
    m[idIncludes][1] = mstrsIncl;
    m[idInside][1] = mstrsInsi;
    m[idOverlaps][1] = mstrsOver;
    m[idTouches][1] = mstrsTouc;


    tr_true[COVEREDBY] = ((tr_true[COVEREDBY]/CLOCKS_PER_SEC) / mstrsCovB) * 1000000;
    tr_true[COVERS] = ((tr_true[COVERS]/CLOCKS_PER_SEC) / mstrsCovs) * 1000000;
    tr_true[DISJOINT] = ((tr_true[DISJOINT]/CLOCKS_PER_SEC) / mstrsDisj) * 1000000;
    tr_true[EQUALS] = ((tr_true[EQUALS]/CLOCKS_PER_SEC) / mstrsEqls) * 1000000;
    tr_true[INCLUDES] = ((tr_true[INCLUDES]/CLOCKS_PER_SEC) / mstrsIncl) * 1000000;
    tr_true[INSIDE] = ((tr_true[INSIDE]/CLOCKS_PER_SEC) / mstrsInsi) * 1000000;
    tr_true[OVERLAPS] = ((tr_true[OVERLAPS]/CLOCKS_PER_SEC) / mstrsOver) * 1000000;
    tr_true[TOUCHES] = ((tr_true[TOUCHES]/CLOCKS_PER_SEC) / mstrsTouc) * 1000000;

    // Tiempos de operaciones true
    m[idCoveredby][2] = tr_true[COVEREDBY];
    m[idCovers][2] = tr_true[COVERS];
    m[idDisjoint][2] = tr_true[DISJOINT];
    m[idEquals][2] = tr_true[EQUALS];
    m[idIncludes][2] = tr_true[INCLUDES];
    m[idInside][2] = tr_true[INSIDE];
    m[idOverlaps][2] = tr_true[OVERLAPS];
    m[idTouches][2] = tr_true[TOUCHES];

    tr = mrt[COVEREDBY] + mrt[COVERS] + mrt[DISJOINT] + mrt[EQUALS] + mrt[INCLUDES] + mrt[INSIDE] + mrt[OVERLAPS] + mrt[TOUCHES];

    cout << "\tRelaciones binarias ALL ... ";
    cout << "CB ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_coveredby(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVEREDBY] += tiempo;

    cout << "CV ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_covers(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVERS] += tiempo;

    cout << "DJ ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_disjoint(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[DISJOINT] += tiempo;

    cout << "EQ ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_equals(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[EQUALS] += tiempo;

    cout << "IC ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_includes(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INCLUDES] += tiempo;

    cout << "IS ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_inside(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INSIDE] += tiempo;

    cout << "OV ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_overlaps(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[OVERLAPS] += tiempo;

    cout << "TO ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_touches(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[TOUCHES] += tiempo;

    cout << "_WT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_within(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[WHITIN] += tiempo;

    cout << "_CT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_contains(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[CONTAINS] += tiempo;

    cout << "_IT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tr_intersect(routes[i], routes[j]);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INTERSECT] += tiempo;

    cout << "OK" << endl << ">>> Fin Calculo Tiempo Naive" << endl;


    tr_all[COVEREDBY] = ((tr_all[COVEREDBY]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[COVERS] = ((tr_all[COVERS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[DISJOINT] = ((tr_all[DISJOINT]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[EQUALS] = ((tr_all[EQUALS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INCLUDES] = ((tr_all[INCLUDES]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INSIDE] = ((tr_all[INSIDE]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[OVERLAPS] = ((tr_all[OVERLAPS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[TOUCHES] = ((tr_all[TOUCHES]/CLOCKS_PER_SEC) / tr) * 1000000;

    tr_all[WHITIN] = ((tr_all[WHITIN]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[CONTAINS] = ((tr_all[CONTAINS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INTERSECT] = ((tr_all[INTERSECT]/CLOCKS_PER_SEC) / tr) * 1000000;

    // Tiempos de operaciones all
    m[idCoveredby][3] = tr_all[COVEREDBY];
    m[idCovers][3] = tr_all[COVERS];
    m[idDisjoint][3] = tr_all[DISJOINT];
    m[idEquals][3] = tr_all[EQUALS];
    m[idIncludes][3] = tr_all[INCLUDES];
    m[idInside][3] = tr_all[INSIDE];
    m[idOverlaps][3] = tr_all[OVERLAPS];
    m[idTouches][3] = tr_all[TOUCHES];

    m[idWithin][3] = tr_all[WHITIN];
    m[idContains][3] = tr_all[CONTAINS];
    m[idIntersect][3] = tr_all[INTERSECT];

    cout << "TopoRelation\tcant\tTime_true\tTime_all" << endl;
    cout << COVEREDBY << "\t" << mrt[COVEREDBY] << "\t" << tr_true[COVEREDBY] << " [us]\t" << tr_all[COVEREDBY] << " [us]" << endl;
    cout << COVERS << "\t\t" << mrt[COVERS] << "\t" << tr_true[COVERS] << " [us]\t" << tr_all[COVERS] << " [us]" << endl;
    cout << EQUALS << "\t\t" << mrt[EQUALS] << "\t" << tr_true[EQUALS] << " [us]\t" << tr_all[EQUALS] << " [us]" << endl;
    cout << INCLUDES << "\t" << mrt[INCLUDES] << "\t" << tr_true[INCLUDES] << " [us]\t" << tr_all[INCLUDES] << " [us]" << endl;
    cout << INSIDE << "\t\t" << mrt[INSIDE] << "\t" << tr_true[INSIDE] << " [us]\t" << tr_all[INSIDE] << " [us]" << endl;
    cout << OVERLAPS << "\t" << mrt[OVERLAPS] << "\t" << tr_true[OVERLAPS] << " [us]\t" << tr_all[OVERLAPS] << " [us]" << endl;
    cout << TOUCHES << "\t\t" << mrt[TOUCHES] << "\t" << tr_true[TOUCHES] << " [us]\t" << tr_all[TOUCHES] << " [us]" << endl;
    cout << DISJOINT << "\t" << mrt[DISJOINT] << "\t" << tr_true[DISJOINT] << " [us]\t" << tr_all[DISJOINT] << " [us]" << endl;
    cout << "---" << endl;
    cout << WHITIN << "\t\t\t\t\t" << tr_all[WHITIN] << " [us]" << endl;
    cout << CONTAINS << "\t\t\t\t\t\t" << tr_all[CONTAINS] << " [us]" << endl;
    cout << INTERSECT << "\t\t\t\t\t" << tr_all[INTERSECT] << " [us]" << endl;
    cout << "Total relaciones: " ;
    cout << tr << endl;

    return m;
}

vector<vector<double>> tiempos_naive_precomp(vector<vector<int>> &routes, int n_stops){
    cout << "********** Naive Pre-Computed **********" << endl;
    unsigned t0 = clock();
    TopoRelNaivePreComp trpc(routes, n_stops);
    unsigned t1 = clock();
    double tConstruccion = ((t1-t0)/CLOCKS_PER_SEC) * 1000000;
    cout << "Tiempo Construcción: " << tConstruccion << "[us]" << endl;
    trpc.sizeEstructura();

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

    tr_all[WHITIN] = 0.0;
    tr_all[CONTAINS] = 0.0;
    tr_all[INTERSECT] = 0.0;
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

    cout << ">>> Calculando..." << endl << "\tRelación Topológica ... ";
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
    cout << "OK" << endl;

    vector<vector<double>> m(11, vector<double>(4, 0.0));
    // Cantidad de relaciones
    m[idCoveredby][0] = r_coveredby.size()/2;
    m[idCovers][0] = r_covers.size()/2;
    m[idDisjoint][0] = r_disjoint.size()/2;
    m[idEquals][0] = r_equals.size()/2;
    m[idIncludes][0] = r_includes.size()/2;
    m[idInside][0] = r_inside.size()/2;
    m[idOverlaps][0] = r_overlaps.size()/2;
    m[idTouches][0] = r_touches.size()/2;

    m[idWithin][0] = m[idCoveredby][0] + m[idEquals][0] + m[idInside][0];
    m[idContains][0] = m[idCovers][0] + m[idEquals][0] + m[idIncludes][0];
    m[idIntersect][0] = m[idCoveredby][0] + m[idCovers][0] + m[idEquals][0] + m[idIncludes][0] + m[idInside][0] + m[idOverlaps][0] + m[idTouches][0];

    int x, y;
    double tiempo = 0.0;
    int mstrsCovB = 0;
    int mstrsCovs = 0;
    int mstrsDisj = 0;
    int mstrsEqls = 0;
    int mstrsIncl = 0;
    int mstrsInsi = 0;
    int mstrsOver = 0;
    int mstrsTouc = 0;
    
    cout << "\tRelaciones binarias TRUE ... ";
    cout << "CB ... ";
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
    if(r_coveredby.size() == 0){
        tr_true[COVEREDBY] = 0;
    }
    
    cout << "CV ... ";
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
    if(r_covers.size() == 0){
        tr_true[COVERS] = 0;
    }
    
    cout << "DJ ... ";
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
    if(r_disjoint.size() == 0){
        tr_true[DISJOINT] = 0;
    }
    
    cout << "EQ ... ";
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
    if(r_equals.size() == 0){
        tr_true[EQUALS] = 0;
    }
    
    cout << "IC ... ";
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
    if(r_includes.size() == 0){
        tr_true[INCLUDES] = 0;
    }
    
    cout << "IS ... ";
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
    if(r_inside.size() == 0){
        tr_true[INSIDE] = 0;
    }
    
    cout << "OV ... ";
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
    if(r_overlaps.size() == 0){
        tr_true[OVERLAPS] = 0;
    }
    
    cout << "TO ... ";
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
    if(r_touches.size() == 0){
        tr_true[TOUCHES] = 0;
    }

    cout << "OK" << endl;

    // Cantidad de muestras para tiempos_true
    m[idCoveredby][1] = mstrsCovB;
    m[idCovers][1] = mstrsCovs;
    m[idDisjoint][1] = mstrsDisj;
    m[idEquals][1] = mstrsEqls;
    m[idIncludes][1] = mstrsIncl;
    m[idInside][1] = mstrsInsi;
    m[idOverlaps][1] = mstrsOver;
    m[idTouches][1] = mstrsTouc;

    tr_true[COVEREDBY] = ((tr_true[COVEREDBY]/CLOCKS_PER_SEC) / mstrsCovB) * 1000000;
    tr_true[COVERS] = ((tr_true[COVERS]/CLOCKS_PER_SEC) / mstrsCovs) * 1000000;
    tr_true[DISJOINT] = ((tr_true[DISJOINT]/CLOCKS_PER_SEC) / mstrsDisj) * 1000000;
    tr_true[EQUALS] = ((tr_true[EQUALS]/CLOCKS_PER_SEC) / mstrsEqls) * 1000000;
    tr_true[INCLUDES] = ((tr_true[INCLUDES]/CLOCKS_PER_SEC) / mstrsIncl) * 1000000;
    tr_true[INSIDE] = ((tr_true[INSIDE]/CLOCKS_PER_SEC) / mstrsInsi) * 1000000;
    tr_true[OVERLAPS] = ((tr_true[OVERLAPS]/CLOCKS_PER_SEC) / mstrsOver) * 1000000;
    tr_true[TOUCHES] = ((tr_true[TOUCHES]/CLOCKS_PER_SEC) / mstrsTouc) * 1000000;

    // Tiempos de operaciones true
    m[idCoveredby][2] = tr_true[COVEREDBY];
    m[idCovers][2] = tr_true[COVERS];
    m[idDisjoint][2] = tr_true[DISJOINT];
    m[idEquals][2] = tr_true[EQUALS];
    m[idIncludes][2] = tr_true[INCLUDES];
    m[idInside][2] = tr_true[INSIDE];
    m[idOverlaps][2] = tr_true[OVERLAPS];
    m[idTouches][2] = tr_true[TOUCHES];

    tr = mrt[COVEREDBY] + mrt[COVERS] + mrt[DISJOINT] + mrt[EQUALS] + mrt[INCLUDES] + mrt[INSIDE] + mrt[OVERLAPS] + mrt[TOUCHES];

    cout << "\tRelaciones binarias ALL ... ";
    cout << "CB ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.coveredby(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVEREDBY] += tiempo;

    cout << "CV ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.covers(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVERS] += tiempo;

    cout << "DJ ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.disjoint(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[DISJOINT] += tiempo;

    cout << "EQ ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.equals(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[EQUALS] += tiempo;

    cout << "IC ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.includes(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INCLUDES] += tiempo;

    cout << "IS ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.inside(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INSIDE] += tiempo;

    cout << "OV ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.overlaps(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[OVERLAPS] += tiempo;

    cout << "TO ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.touches(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[TOUCHES] += tiempo;

    cout << "_WT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.within(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[WHITIN] += tiempo;

    cout << "_CT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.contains(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[CONTAINS] += tiempo;

    cout << "_IT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            trpc.intersect(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INTERSECT] += tiempo;

    cout << "OK" << endl << ">>> Fin Calculo Tiempo PreComp" << endl;


    tr_all[COVEREDBY] = ((tr_all[COVEREDBY]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[COVERS] = ((tr_all[COVERS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[DISJOINT] = ((tr_all[DISJOINT]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[EQUALS] = ((tr_all[EQUALS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INCLUDES] = ((tr_all[INCLUDES]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INSIDE] = ((tr_all[INSIDE]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[OVERLAPS] = ((tr_all[OVERLAPS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[TOUCHES] = ((tr_all[TOUCHES]/CLOCKS_PER_SEC) / tr) * 1000000;

    tr_all[WHITIN] = ((tr_all[WHITIN]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[CONTAINS] = ((tr_all[CONTAINS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INTERSECT] = ((tr_all[INTERSECT]/CLOCKS_PER_SEC) / tr) * 1000000;

    // Tiempos de operaciones all
    m[idCoveredby][3] = tr_all[COVEREDBY];
    m[idCovers][3] = tr_all[COVERS];
    m[idDisjoint][3] = tr_all[DISJOINT];
    m[idEquals][3] = tr_all[EQUALS];
    m[idIncludes][3] = tr_all[INCLUDES];
    m[idInside][3] = tr_all[INSIDE];
    m[idOverlaps][3] = tr_all[OVERLAPS];
    m[idTouches][3] = tr_all[TOUCHES];

    m[idWithin][3] = tr_all[WHITIN];
    m[idContains][3] = tr_all[CONTAINS];
    m[idIntersect][3] = tr_all[INTERSECT];

    cout << "TopoRelation\tcant\tTime_true\tTime_all" << endl;
    cout << COVEREDBY << "\t" << mrt[COVEREDBY] << "\t" << tr_true[COVEREDBY] << " [us]\t" << tr_all[COVEREDBY] << " [us]" << endl;
    cout << COVERS << "\t\t" << mrt[COVERS] << "\t" << tr_true[COVERS] << " [us]\t" << tr_all[COVERS] << " [us]" << endl;
    cout << EQUALS << "\t\t" << mrt[EQUALS] << "\t" << tr_true[EQUALS] << " [us]\t" << tr_all[EQUALS] << " [us]" << endl;
    cout << INCLUDES << "\t" << mrt[INCLUDES] << "\t" << tr_true[INCLUDES] << " [us]\t" << tr_all[INCLUDES] << " [us]" << endl;
    cout << INSIDE << "\t\t" << mrt[INSIDE] << "\t" << tr_true[INSIDE] << " [us]\t" << tr_all[INSIDE] << " [us]" << endl;
    cout << OVERLAPS << "\t" << mrt[OVERLAPS] << "\t" << tr_true[OVERLAPS] << " [us]\t" << tr_all[OVERLAPS] << " [us]" << endl;
    cout << TOUCHES << "\t\t" << mrt[TOUCHES] << "\t" << tr_true[TOUCHES] << " [us]\t" << tr_all[TOUCHES] << " [us]" << endl;
    cout << DISJOINT << "\t" << mrt[DISJOINT] << "\t" << tr_true[DISJOINT] << " [us]\t" << tr_all[DISJOINT] << " [us]" << endl;
    cout << "---" << endl;
    cout << WHITIN << "\t\t\t\t\t" << tr_all[WHITIN] << " [us]" << endl;
    cout << CONTAINS << "\t\t\t\t\t\t" << tr_all[CONTAINS] << " [us]" << endl;
    cout << INTERSECT << "\t\t\t\t\t" << tr_all[INTERSECT] << " [us]" << endl;
    cout << "Total relaciones: " ;
    cout << tr << endl;

    return m;
}

vector<vector<double>> tiempos_GST_0(vector<vector<int>> &routes, int n_stops){
    cout << "********** Compressed Suffix Tree GST_0 **********" << endl;
    unsigned t0 = clock();
    TopoRelGST_0 tt(routes, n_stops);
    unsigned t1 = clock();
    double tConstruccion = ((t1-t0)/CLOCKS_PER_SEC) * 1000000;
    cout << "Tiempo Construcción: " << tConstruccion << "[us]" << endl;
    tt.sizeEstructura();

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

    tr_all[WHITIN] = 0.0;
    tr_all[CONTAINS] = 0.0;
    tr_all[INTERSECT] = 0.0;
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

    cout << ">>> Calculando..." << endl << "\tRelación Topológica ... ";
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
    cout << "OK" << endl;

    vector<vector<double>> m(11, vector<double>(4, 0.0));
    // Cantidad de relaciones
    m[idCoveredby][0] = r_coveredby.size()/2;
    m[idCovers][0] = r_covers.size()/2;
    m[idDisjoint][0] = r_disjoint.size()/2;
    m[idEquals][0] = r_equals.size()/2;
    m[idIncludes][0] = r_includes.size()/2;
    m[idInside][0] = r_inside.size()/2;
    m[idOverlaps][0] = r_overlaps.size()/2;
    m[idTouches][0] = r_touches.size()/2;

    m[idWithin][0] = m[idCoveredby][0] + m[idEquals][0] + m[idInside][0];
    m[idContains][0] = m[idCovers][0] + m[idEquals][0] + m[idIncludes][0];
    m[idIntersect][0] = m[idCoveredby][0] + m[idCovers][0] + m[idEquals][0] + m[idIncludes][0] + m[idInside][0] + m[idOverlaps][0] + m[idTouches][0];

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

    cout << "\tRelaciones binarias TRUE ... ";
    cout << "CB ... ";
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
    if(r_coveredby.size() == 0){
        tr_true[COVEREDBY] = 0;
    }

    cout << "CV ... ";
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
    if(r_covers.size() == 0){
        tr_true[COVERS] = 0;
    }

    cout << "DJ ... ";
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
    if(r_disjoint.size() == 0){
        tr_true[DISJOINT] = 0;
    }

    cout << "EQ ... ";
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
    if(r_equals.size() == 0){
        tr_true[EQUALS] = 0;
    }

    cout << "IC ... ";
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
    if(r_includes.size() == 0){
        tr_true[INCLUDES] = 0;
    }

    cout << "IS ... ";
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
    if(r_inside.size() == 0){
        tr_true[INSIDE] = 0;
    }

    cout << "OV ... ";
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
    if(r_overlaps.size() == 0){
        tr_true[OVERLAPS] = 0;
    }

    cout << "TO ... ";
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
    if(r_touches.size() == 0){
        tr_true[TOUCHES] = 0;
    }

    cout << "OK" << endl;

    // Cantidad de muestras para tiempos_true
    m[idCoveredby][1] = mstrsCovB;
    m[idCovers][1] = mstrsCovs;
    m[idDisjoint][1] = mstrsDisj;
    m[idEquals][1] = mstrsEqls;
    m[idIncludes][1] = mstrsIncl;
    m[idInside][1] = mstrsInsi;
    m[idOverlaps][1] = mstrsOver;
    m[idTouches][1] = mstrsTouc;


    tr_true[COVEREDBY] = ((tr_true[COVEREDBY]/CLOCKS_PER_SEC) / mstrsCovB) * 1000000;
    tr_true[COVERS] = ((tr_true[COVERS]/CLOCKS_PER_SEC) / mstrsCovs) * 1000000;
    tr_true[DISJOINT] = ((tr_true[DISJOINT]/CLOCKS_PER_SEC) / mstrsDisj) * 1000000;
    tr_true[EQUALS] = ((tr_true[EQUALS]/CLOCKS_PER_SEC) / mstrsEqls) * 1000000;
    tr_true[INCLUDES] = ((tr_true[INCLUDES]/CLOCKS_PER_SEC) / mstrsIncl) * 1000000;
    tr_true[INSIDE] = ((tr_true[INSIDE]/CLOCKS_PER_SEC) / mstrsInsi) * 1000000;
    tr_true[OVERLAPS] = ((tr_true[OVERLAPS]/CLOCKS_PER_SEC) / mstrsOver) * 1000000;
    tr_true[TOUCHES] = ((tr_true[TOUCHES]/CLOCKS_PER_SEC) / mstrsTouc) * 1000000;

    // Tiempos de operaciones true
    m[idCoveredby][2] = tr_true[COVEREDBY];
    m[idCovers][2] = tr_true[COVERS];
    m[idDisjoint][2] = tr_true[DISJOINT];
    m[idEquals][2] = tr_true[EQUALS];
    m[idIncludes][2] = tr_true[INCLUDES];
    m[idInside][2] = tr_true[INSIDE];
    m[idOverlaps][2] = tr_true[OVERLAPS];
    m[idTouches][2] = tr_true[TOUCHES];

    tr = mrt[COVEREDBY] + mrt[COVERS] + mrt[DISJOINT] + mrt[EQUALS] + mrt[INCLUDES] + mrt[INSIDE] + mrt[OVERLAPS] + mrt[TOUCHES];

    cout << "\tRelaciones binarias ALL ... ";
    cout << "CB ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_coveredby(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVEREDBY] += tiempo;

    cout << "CV ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_covers(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVERS] += tiempo;

    cout << "DJ ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_disjoint(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[DISJOINT] += tiempo;

    cout << "EQ ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_equals(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[EQUALS] += tiempo;

    cout << "IC ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_includes(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INCLUDES] += tiempo;

    cout << "IS ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_inside(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INSIDE] += tiempo;

    cout << "OV ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_overlaps(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[OVERLAPS] += tiempo;

    cout << "TO ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_touches(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[TOUCHES] += tiempo;

    cout << "_WT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_within(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[WHITIN] += tiempo;

    cout << "_CT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_contains(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[CONTAINS] += tiempo;

    cout << "_IT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_intersect(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INTERSECT] += tiempo;

    cout << "OK" << endl << ">>> Fin Calculo Tiempo GST_0" << endl;


    tr_all[COVEREDBY] = ((tr_all[COVEREDBY]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[COVERS] = ((tr_all[COVERS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[DISJOINT] = ((tr_all[DISJOINT]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[EQUALS] = ((tr_all[EQUALS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INCLUDES] = ((tr_all[INCLUDES]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INSIDE] = ((tr_all[INSIDE]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[OVERLAPS] = ((tr_all[OVERLAPS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[TOUCHES] = ((tr_all[TOUCHES]/CLOCKS_PER_SEC) / tr) * 1000000;

    tr_all[WHITIN] = ((tr_all[WHITIN]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[CONTAINS] = ((tr_all[CONTAINS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INTERSECT] = ((tr_all[INTERSECT]/CLOCKS_PER_SEC) / tr) * 1000000;

    // Tiempos de operaciones all
    m[idCoveredby][3] = tr_all[COVEREDBY];
    m[idCovers][3] = tr_all[COVERS];
    m[idDisjoint][3] = tr_all[DISJOINT];
    m[idEquals][3] = tr_all[EQUALS];
    m[idIncludes][3] = tr_all[INCLUDES];
    m[idInside][3] = tr_all[INSIDE];
    m[idOverlaps][3] = tr_all[OVERLAPS];
    m[idTouches][3] = tr_all[TOUCHES];

    m[idWithin][3] = tr_all[WHITIN];
    m[idContains][3] = tr_all[CONTAINS];
    m[idIntersect][3] = tr_all[INTERSECT];

    cout << "TopoRelation\tcant\tTime_true\tTime_all" << endl;
    cout << COVEREDBY << "\t" << mrt[COVEREDBY] << "\t" << tr_true[COVEREDBY] << " [us]\t" << tr_all[COVEREDBY] << " [us]" << endl;
    cout << COVERS << "\t\t" << mrt[COVERS] << "\t" << tr_true[COVERS] << " [us]\t" << tr_all[COVERS] << " [us]" << endl;
    cout << EQUALS << "\t\t" << mrt[EQUALS] << "\t" << tr_true[EQUALS] << " [us]\t" << tr_all[EQUALS] << " [us]" << endl;
    cout << INCLUDES << "\t" << mrt[INCLUDES] << "\t" << tr_true[INCLUDES] << " [us]\t" << tr_all[INCLUDES] << " [us]" << endl;
    cout << INSIDE << "\t\t" << mrt[INSIDE] << "\t" << tr_true[INSIDE] << " [us]\t" << tr_all[INSIDE] << " [us]" << endl;
    cout << OVERLAPS << "\t" << mrt[OVERLAPS] << "\t" << tr_true[OVERLAPS] << " [us]\t" << tr_all[OVERLAPS] << " [us]" << endl;
    cout << TOUCHES << "\t\t" << mrt[TOUCHES] << "\t" << tr_true[TOUCHES] << " [us]\t" << tr_all[TOUCHES] << " [us]" << endl;
    cout << DISJOINT << "\t" << mrt[DISJOINT] << "\t" << tr_true[DISJOINT] << " [us]\t" << tr_all[DISJOINT] << " [us]" << endl;
    cout << "---" << endl;
    cout << WHITIN << "\t\t\t\t\t" << tr_all[WHITIN] << " [us]" << endl;
    cout << CONTAINS << "\t\t\t\t\t\t" << tr_all[CONTAINS] << " [us]" << endl;
    cout << INTERSECT << "\t\t\t\t\t" << tr_all[INTERSECT] << " [us]" << endl;
    cout << "Total relaciones: " ;
    cout << tr << endl;

    return m;
}

vector<vector<double>> tiempos_GST_1(vector<vector<int>> &routes, int n_stops){
    cout << "********** Compressed Suffix Tree GST_1 **********" << endl;
    unsigned t0 = clock();
    TopoRelGST_1 tt(routes, n_stops);
    unsigned t1 = clock();
    double tConstruccion = ((t1-t0)/CLOCKS_PER_SEC) * 1000000;
    cout << "Tiempo Construcción: " << tConstruccion << "[us]" << endl;
    tt.sizeEstructura();
    
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

    tr_all[WHITIN] = 0.0;
    tr_all[CONTAINS] = 0.0;
    tr_all[INTERSECT] = 0.0;
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

    cout << ">>> Calculando..." << endl << "\tRelación Topológica ... ";
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
    cout << "OK" << endl;

    vector<vector<double>> m(11, vector<double>(4, 0.0));
    // Cantidad de relaciones
    m[idCoveredby][0] = r_coveredby.size()/2;
    m[idCovers][0] = r_covers.size()/2;
    m[idDisjoint][0] = r_disjoint.size()/2;
    m[idEquals][0] = r_equals.size()/2;
    m[idIncludes][0] = r_includes.size()/2;
    m[idInside][0] = r_inside.size()/2;
    m[idOverlaps][0] = r_overlaps.size()/2;
    m[idTouches][0] = r_touches.size()/2;

    m[idWithin][0] = m[idCoveredby][0] + m[idEquals][0] + m[idInside][0];
    m[idContains][0] = m[idCovers][0] + m[idEquals][0] + m[idIncludes][0];
    m[idIntersect][0] = m[idCoveredby][0] + m[idCovers][0] + m[idEquals][0] + m[idIncludes][0] + m[idInside][0] + m[idOverlaps][0] + m[idTouches][0];

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
    
    cout << "\tRelaciones binarias TRUE ... ";
    cout << "CB ... ";
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
    if(r_coveredby.size() == 0){
        tr_true[COVEREDBY] = 0;
    }
    
    cout << "CV ... ";
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
    if(r_covers.size() == 0){
        tr_true[COVERS] = 0;
    }
    
    cout << "DJ ... ";
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
    if(r_disjoint.size() == 0){
        tr_true[DISJOINT] = 0;
    }
    
    cout << "EQ ... ";
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
    if(r_equals.size() == 0){
        tr_true[EQUALS] = 0;
    }
    
    cout << "IC ... ";
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
    if(r_includes.size() == 0){
        tr_true[INCLUDES] = 0;
    }
    
    cout << "IS ... ";
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
    if(r_inside.size() == 0){
        tr_true[INSIDE] = 0;
    }
    
    cout << "OV ... ";
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
    if(r_overlaps.size() == 0){
        tr_true[OVERLAPS] = 0;
    }
    
    cout << "TO ... ";
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
    if(r_touches.size() == 0){
        tr_true[TOUCHES] = 0;
    }

    cout << "OK" << endl;

    // Cantidad de muestras para tiempos_true
    m[idCoveredby][1] = mstrsCovB;
    m[idCovers][1] = mstrsCovs;
    m[idDisjoint][1] = mstrsDisj;
    m[idEquals][1] = mstrsEqls;
    m[idIncludes][1] = mstrsIncl;
    m[idInside][1] = mstrsInsi;
    m[idOverlaps][1] = mstrsOver;
    m[idTouches][1] = mstrsTouc;


    tr_true[COVEREDBY] = ((tr_true[COVEREDBY]/CLOCKS_PER_SEC) / mstrsCovB) * 1000000;
    tr_true[COVERS] = ((tr_true[COVERS]/CLOCKS_PER_SEC) / mstrsCovs) * 1000000;
    tr_true[DISJOINT] = ((tr_true[DISJOINT]/CLOCKS_PER_SEC) / mstrsDisj) * 1000000;
    tr_true[EQUALS] = ((tr_true[EQUALS]/CLOCKS_PER_SEC) / mstrsEqls) * 1000000;
    tr_true[INCLUDES] = ((tr_true[INCLUDES]/CLOCKS_PER_SEC) / mstrsIncl) * 1000000;
    tr_true[INSIDE] = ((tr_true[INSIDE]/CLOCKS_PER_SEC) / mstrsInsi) * 1000000;
    tr_true[OVERLAPS] = ((tr_true[OVERLAPS]/CLOCKS_PER_SEC) / mstrsOver) * 1000000;
    tr_true[TOUCHES] = ((tr_true[TOUCHES]/CLOCKS_PER_SEC) / mstrsTouc) * 1000000;

    // Tiempos de operaciones true
    m[idCoveredby][2] = tr_true[COVEREDBY];
    m[idCovers][2] = tr_true[COVERS];
    m[idDisjoint][2] = tr_true[DISJOINT];
    m[idEquals][2] = tr_true[EQUALS];
    m[idIncludes][2] = tr_true[INCLUDES];
    m[idInside][2] = tr_true[INSIDE];
    m[idOverlaps][2] = tr_true[OVERLAPS];
    m[idTouches][2] = tr_true[TOUCHES];

    tr = mrt[COVEREDBY] + mrt[COVERS] + mrt[DISJOINT] + mrt[EQUALS] + mrt[INCLUDES] + mrt[INSIDE] + mrt[OVERLAPS] + mrt[TOUCHES];

    cout << "\tRelaciones binarias ALL ... ";
    cout << "CB ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_coveredby(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVEREDBY] += tiempo;

    cout << "CV ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_covers(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVERS] += tiempo;

    cout << "DJ ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_disjoint(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[DISJOINT] += tiempo;

    cout << "EQ ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_equals(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[EQUALS] += tiempo;

    cout << "IC ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_includes(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INCLUDES] += tiempo;

    cout << "IS ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_inside(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INSIDE] += tiempo;

    cout << "OV ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_overlaps(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[OVERLAPS] += tiempo;

    cout << "TO ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_touches(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[TOUCHES] += tiempo;

    cout << "_WT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_within(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[WHITIN] += tiempo;

    cout << "_CT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_contains(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[CONTAINS] += tiempo;

    cout << "_IT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_intersect(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INTERSECT] += tiempo;

    cout << "OK" << endl << ">>> Fin Calculo Tiempo GST_1" << endl;


    tr_all[COVEREDBY] = ((tr_all[COVEREDBY]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[COVERS] = ((tr_all[COVERS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[DISJOINT] = ((tr_all[DISJOINT]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[EQUALS] = ((tr_all[EQUALS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INCLUDES] = ((tr_all[INCLUDES]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INSIDE] = ((tr_all[INSIDE]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[OVERLAPS] = ((tr_all[OVERLAPS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[TOUCHES] = ((tr_all[TOUCHES]/CLOCKS_PER_SEC) / tr) * 1000000;

    tr_all[WHITIN] = ((tr_all[WHITIN]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[CONTAINS] = ((tr_all[CONTAINS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INTERSECT] = ((tr_all[INTERSECT]/CLOCKS_PER_SEC) / tr) * 1000000;

    // Tiempos de operaciones all
    m[idCoveredby][3] = tr_all[COVEREDBY];
    m[idCovers][3] = tr_all[COVERS];
    m[idDisjoint][3] = tr_all[DISJOINT];
    m[idEquals][3] = tr_all[EQUALS];
    m[idIncludes][3] = tr_all[INCLUDES];
    m[idInside][3] = tr_all[INSIDE];
    m[idOverlaps][3] = tr_all[OVERLAPS];
    m[idTouches][3] = tr_all[TOUCHES];

    m[idWithin][3] = tr_all[WHITIN];
    m[idContains][3] = tr_all[CONTAINS];
    m[idIntersect][3] = tr_all[INTERSECT];

    cout << "TopoRelation\tcant\tTime_true\tTime_all" << endl;
    cout << COVEREDBY << "\t" << mrt[COVEREDBY] << "\t" << tr_true[COVEREDBY] << " [us]\t" << tr_all[COVEREDBY] << " [us]" << endl;
    cout << COVERS << "\t\t" << mrt[COVERS] << "\t" << tr_true[COVERS] << " [us]\t" << tr_all[COVERS] << " [us]" << endl;
    cout << EQUALS << "\t\t" << mrt[EQUALS] << "\t" << tr_true[EQUALS] << " [us]\t" << tr_all[EQUALS] << " [us]" << endl;
    cout << INCLUDES << "\t" << mrt[INCLUDES] << "\t" << tr_true[INCLUDES] << " [us]\t" << tr_all[INCLUDES] << " [us]" << endl;
    cout << INSIDE << "\t\t" << mrt[INSIDE] << "\t" << tr_true[INSIDE] << " [us]\t" << tr_all[INSIDE] << " [us]" << endl;
    cout << OVERLAPS << "\t" << mrt[OVERLAPS] << "\t" << tr_true[OVERLAPS] << " [us]\t" << tr_all[OVERLAPS] << " [us]" << endl;
    cout << TOUCHES << "\t\t" << mrt[TOUCHES] << "\t" << tr_true[TOUCHES] << " [us]\t" << tr_all[TOUCHES] << " [us]" << endl;
    cout << DISJOINT << "\t" << mrt[DISJOINT] << "\t" << tr_true[DISJOINT] << " [us]\t" << tr_all[DISJOINT] << " [us]" << endl;
    cout << "---" << endl;
    cout << WHITIN << "\t\t\t\t\t" << tr_all[WHITIN] << " [us]" << endl;
    cout << CONTAINS << "\t\t\t\t\t\t" << tr_all[CONTAINS] << " [us]" << endl;
    cout << INTERSECT << "\t\t\t\t\t" << tr_all[INTERSECT] << " [us]" << endl;
    cout << "Total relaciones: " ;
    cout << tr << endl;

    return m;
}

vector<vector<double>> tiempos_GST_2(vector<vector<int>> &routes, int n_stops){
    cout << "********** Compressed Suffix Tree GST_2 **********" << endl;
    unsigned t0 = clock();
    TopoRelGST_2 tt(routes, n_stops);
    unsigned t1 = clock();
    double tConstruccion = ((t1-t0)/CLOCKS_PER_SEC) * 1000000;
    cout << "Tiempo Construcción: " << tConstruccion << "[us]" << endl;
    tt.sizeEstructura();
    
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

    tr_all[WHITIN] = 0.0;
    tr_all[CONTAINS] = 0.0;
    tr_all[INTERSECT] = 0.0;
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

    cout << ">>> Calculando..." << endl << "\tRelación Topológica ... ";
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
    cout << "OK" << endl;

    vector<vector<double>> m(11, vector<double>(4, 0.0));
    // Cantidad de relaciones
    m[idCoveredby][0] = r_coveredby.size()/2;
    m[idCovers][0] = r_covers.size()/2;
    m[idDisjoint][0] = r_disjoint.size()/2;
    m[idEquals][0] = r_equals.size()/2;
    m[idIncludes][0] = r_includes.size()/2;
    m[idInside][0] = r_inside.size()/2;
    m[idOverlaps][0] = r_overlaps.size()/2;
    m[idTouches][0] = r_touches.size()/2;

    m[idWithin][0] = m[idCoveredby][0] + m[idEquals][0] + m[idInside][0];
    m[idContains][0] = m[idCovers][0] + m[idEquals][0] + m[idIncludes][0];
    m[idIntersect][0] = m[idCoveredby][0] + m[idCovers][0] + m[idEquals][0] + m[idIncludes][0] + m[idInside][0] + m[idOverlaps][0] + m[idTouches][0];

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
    
    cout << "\tRelaciones binarias TRUE ... ";
    cout << "CB ... ";
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
    if(r_coveredby.size() == 0){
        tr_true[COVEREDBY] = 0;
    }
    
    cout << "CV ... ";
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
    if(r_covers.size() == 0){
        tr_true[COVERS] = 0;
    }
    
    cout << "DJ ... ";
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
    if(r_disjoint.size() == 0){
        tr_true[DISJOINT] = 0;
    }
    
    cout << "EQ ... ";
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
    if(r_equals.size() == 0){
        tr_true[EQUALS] = 0;
    }
    
    cout << "IC ... ";
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
    if(r_includes.size() == 0){
        tr_true[INCLUDES] = 0;
    }
    
    cout << "IS ... ";
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
    if(r_inside.size() == 0){
        tr_true[INSIDE] = 0;
    }
    
    cout << "OV ... ";
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
    if(r_overlaps.size() == 0){
        tr_true[OVERLAPS] = 0;
    }
    
    cout << "TO ... ";
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
    if(r_touches.size() == 0){
        tr_true[TOUCHES] = 0;
    }

    cout << "OK" << endl;

    // Cantidad de muestras para tiempos_true
    m[idCoveredby][1] = mstrsCovB;
    m[idCovers][1] = mstrsCovs;
    m[idDisjoint][1] = mstrsDisj;
    m[idEquals][1] = mstrsEqls;
    m[idIncludes][1] = mstrsIncl;
    m[idInside][1] = mstrsInsi;
    m[idOverlaps][1] = mstrsOver;
    m[idTouches][1] = mstrsTouc;


    tr_true[COVEREDBY] = ((tr_true[COVEREDBY]/CLOCKS_PER_SEC) / mstrsCovB) * 1000000;
    tr_true[COVERS] = ((tr_true[COVERS]/CLOCKS_PER_SEC) / mstrsCovs) * 1000000;
    tr_true[DISJOINT] = ((tr_true[DISJOINT]/CLOCKS_PER_SEC) / mstrsDisj) * 1000000;
    tr_true[EQUALS] = ((tr_true[EQUALS]/CLOCKS_PER_SEC) / mstrsEqls) * 1000000;
    tr_true[INCLUDES] = ((tr_true[INCLUDES]/CLOCKS_PER_SEC) / mstrsIncl) * 1000000;
    tr_true[INSIDE] = ((tr_true[INSIDE]/CLOCKS_PER_SEC) / mstrsInsi) * 1000000;
    tr_true[OVERLAPS] = ((tr_true[OVERLAPS]/CLOCKS_PER_SEC) / mstrsOver) * 1000000;
    tr_true[TOUCHES] = ((tr_true[TOUCHES]/CLOCKS_PER_SEC) / mstrsTouc) * 1000000;

    // Tiempos de operaciones true
    m[idCoveredby][2] = tr_true[COVEREDBY];
    m[idCovers][2] = tr_true[COVERS];
    m[idDisjoint][2] = tr_true[DISJOINT];
    m[idEquals][2] = tr_true[EQUALS];
    m[idIncludes][2] = tr_true[INCLUDES];
    m[idInside][2] = tr_true[INSIDE];
    m[idOverlaps][2] = tr_true[OVERLAPS];
    m[idTouches][2] = tr_true[TOUCHES];

    tr = mrt[COVEREDBY] + mrt[COVERS] + mrt[DISJOINT] + mrt[EQUALS] + mrt[INCLUDES] + mrt[INSIDE] + mrt[OVERLAPS] + mrt[TOUCHES];

    cout << "\tRelaciones binarias ALL ... ";
    cout << "CB ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_coveredby(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVEREDBY] += tiempo;

    cout << "CV ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_covers(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVERS] += tiempo;

    cout << "DJ ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_disjoint(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[DISJOINT] += tiempo;

    cout << "EQ ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_equals(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[EQUALS] += tiempo;

    cout << "IC ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_includes(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INCLUDES] += tiempo;

    cout << "IS ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_inside(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INSIDE] += tiempo;

    cout << "OV ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_overlaps(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[OVERLAPS] += tiempo;

    cout << "TO ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_touches(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[TOUCHES] += tiempo;

    cout << "_WT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_within(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[WHITIN] += tiempo;

    cout << "_CT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_contains(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[CONTAINS] += tiempo;

    cout << "_IT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_intersect(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INTERSECT] += tiempo;

    cout << "OK" << endl << ">>> Fin Calculo Tiempo GST_2" << endl;


    tr_all[COVEREDBY] = ((tr_all[COVEREDBY]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[COVERS] = ((tr_all[COVERS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[DISJOINT] = ((tr_all[DISJOINT]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[EQUALS] = ((tr_all[EQUALS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INCLUDES] = ((tr_all[INCLUDES]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INSIDE] = ((tr_all[INSIDE]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[OVERLAPS] = ((tr_all[OVERLAPS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[TOUCHES] = ((tr_all[TOUCHES]/CLOCKS_PER_SEC) / tr) * 1000000;

    tr_all[WHITIN] = ((tr_all[WHITIN]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[CONTAINS] = ((tr_all[CONTAINS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INTERSECT] = ((tr_all[INTERSECT]/CLOCKS_PER_SEC) / tr) * 1000000;

    // Tiempos de operaciones all
    m[idCoveredby][3] = tr_all[COVEREDBY];
    m[idCovers][3] = tr_all[COVERS];
    m[idDisjoint][3] = tr_all[DISJOINT];
    m[idEquals][3] = tr_all[EQUALS];
    m[idIncludes][3] = tr_all[INCLUDES];
    m[idInside][3] = tr_all[INSIDE];
    m[idOverlaps][3] = tr_all[OVERLAPS];
    m[idTouches][3] = tr_all[TOUCHES];

    m[idWithin][3] = tr_all[WHITIN];
    m[idContains][3] = tr_all[CONTAINS];
    m[idIntersect][3] = tr_all[INTERSECT];

    cout << "TopoRelation\tcant\tTime_true\tTime_all" << endl;
    cout << COVEREDBY << "\t" << mrt[COVEREDBY] << "\t" << tr_true[COVEREDBY] << " [us]\t" << tr_all[COVEREDBY] << " [us]" << endl;
    cout << COVERS << "\t\t" << mrt[COVERS] << "\t" << tr_true[COVERS] << " [us]\t" << tr_all[COVERS] << " [us]" << endl;
    cout << EQUALS << "\t\t" << mrt[EQUALS] << "\t" << tr_true[EQUALS] << " [us]\t" << tr_all[EQUALS] << " [us]" << endl;
    cout << INCLUDES << "\t" << mrt[INCLUDES] << "\t" << tr_true[INCLUDES] << " [us]\t" << tr_all[INCLUDES] << " [us]" << endl;
    cout << INSIDE << "\t\t" << mrt[INSIDE] << "\t" << tr_true[INSIDE] << " [us]\t" << tr_all[INSIDE] << " [us]" << endl;
    cout << OVERLAPS << "\t" << mrt[OVERLAPS] << "\t" << tr_true[OVERLAPS] << " [us]\t" << tr_all[OVERLAPS] << " [us]" << endl;
    cout << TOUCHES << "\t\t" << mrt[TOUCHES] << "\t" << tr_true[TOUCHES] << " [us]\t" << tr_all[TOUCHES] << " [us]" << endl;
    cout << DISJOINT << "\t" << mrt[DISJOINT] << "\t" << tr_true[DISJOINT] << " [us]\t" << tr_all[DISJOINT] << " [us]" << endl;
    cout << "---" << endl;
    cout << WHITIN << "\t\t\t\t\t" << tr_all[WHITIN] << " [us]" << endl;
    cout << CONTAINS << "\t\t\t\t\t\t" << tr_all[CONTAINS] << " [us]" << endl;
    cout << INTERSECT << "\t\t\t\t\t" << tr_all[INTERSECT] << " [us]" << endl;
    cout << "Total relaciones: " ;
    cout << tr << endl;

    return m;
}

vector<vector<double>> tiempos_GST_4(vector<vector<int>> &routes, int n_stops){
    cout << "********** Compressed Suffix Tree GST_4 **********" << endl;
    unsigned t0 = clock();
    TopoRelGST_4 tt(routes, n_stops);
    unsigned t1 = clock();
    double tConstruccion = ((t1-t0)/CLOCKS_PER_SEC) * 1000000;
    cout << "Tiempo Construcción: " << tConstruccion << "[us]" << endl;
    tt.sizeEstructura();
    
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

    tr_all[WHITIN] = 0.0;
    tr_all[CONTAINS] = 0.0;
    tr_all[INTERSECT] = 0.0;
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

    cout << ">>> Calculando..." << endl << "\tRelación Topológica ... ";
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
    cout << "OK" << endl;

    vector<vector<double>> m(11, vector<double>(4, 0.0));
    // Cantidad de relaciones
    m[idCoveredby][0] = r_coveredby.size()/2;
    m[idCovers][0] = r_covers.size()/2;
    m[idDisjoint][0] = r_disjoint.size()/2;
    m[idEquals][0] = r_equals.size()/2;
    m[idIncludes][0] = r_includes.size()/2;
    m[idInside][0] = r_inside.size()/2;
    m[idOverlaps][0] = r_overlaps.size()/2;
    m[idTouches][0] = r_touches.size()/2;

    m[idWithin][0] = m[idCoveredby][0] + m[idEquals][0] + m[idInside][0];
    m[idContains][0] = m[idCovers][0] + m[idEquals][0] + m[idIncludes][0];
    m[idIntersect][0] = m[idCoveredby][0] + m[idCovers][0] + m[idEquals][0] + m[idIncludes][0] + m[idInside][0] + m[idOverlaps][0] + m[idTouches][0];

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
    
    cout << "\tRelaciones binarias TRUE ... ";
    cout << "CB ... ";
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
    if(r_coveredby.size() == 0){
        tr_true[COVEREDBY] = 0;
    }
    
    cout << "CV ... ";
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
    if(r_covers.size() == 0){
        tr_true[COVERS] = 0;
    }
    
    cout << "DJ ... ";
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
    if(r_disjoint.size() == 0){
        tr_true[DISJOINT] = 0;
    }
    
    cout << "EQ ... ";
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
    if(r_equals.size() == 0){
        tr_true[EQUALS] = 0;
    }
    
    cout << "IC ... ";
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
    if(r_includes.size() == 0){
        tr_true[INCLUDES] = 0;
    }
    
    cout << "IS ... ";
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
    if(r_inside.size() == 0){
        tr_true[INSIDE] = 0;
    }
    
    cout << "OV ... ";
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
    if(r_overlaps.size() == 0){
        tr_true[OVERLAPS] = 0;
    }
    
    cout << "TO ... ";
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
    if(r_touches.size() == 0){
        tr_true[TOUCHES] = 0;
    }

    cout << "OK" << endl;

    // Cantidad de muestras para tiempos_true
    m[idCoveredby][1] = mstrsCovB;
    m[idCovers][1] = mstrsCovs;
    m[idDisjoint][1] = mstrsDisj;
    m[idEquals][1] = mstrsEqls;
    m[idIncludes][1] = mstrsIncl;
    m[idInside][1] = mstrsInsi;
    m[idOverlaps][1] = mstrsOver;
    m[idTouches][1] = mstrsTouc;


    tr_true[COVEREDBY] = ((tr_true[COVEREDBY]/CLOCKS_PER_SEC) / mstrsCovB) * 1000000;
    tr_true[COVERS] = ((tr_true[COVERS]/CLOCKS_PER_SEC) / mstrsCovs) * 1000000;
    tr_true[DISJOINT] = ((tr_true[DISJOINT]/CLOCKS_PER_SEC) / mstrsDisj) * 1000000;
    tr_true[EQUALS] = ((tr_true[EQUALS]/CLOCKS_PER_SEC) / mstrsEqls) * 1000000;
    tr_true[INCLUDES] = ((tr_true[INCLUDES]/CLOCKS_PER_SEC) / mstrsIncl) * 1000000;
    tr_true[INSIDE] = ((tr_true[INSIDE]/CLOCKS_PER_SEC) / mstrsInsi) * 1000000;
    tr_true[OVERLAPS] = ((tr_true[OVERLAPS]/CLOCKS_PER_SEC) / mstrsOver) * 1000000;
    tr_true[TOUCHES] = ((tr_true[TOUCHES]/CLOCKS_PER_SEC) / mstrsTouc) * 1000000;

    // Tiempos de operaciones true
    m[idCoveredby][2] = tr_true[COVEREDBY];
    m[idCovers][2] = tr_true[COVERS];
    m[idDisjoint][2] = tr_true[DISJOINT];
    m[idEquals][2] = tr_true[EQUALS];
    m[idIncludes][2] = tr_true[INCLUDES];
    m[idInside][2] = tr_true[INSIDE];
    m[idOverlaps][2] = tr_true[OVERLAPS];
    m[idTouches][2] = tr_true[TOUCHES];

    tr = mrt[COVEREDBY] + mrt[COVERS] + mrt[DISJOINT] + mrt[EQUALS] + mrt[INCLUDES] + mrt[INSIDE] + mrt[OVERLAPS] + mrt[TOUCHES];

    cout << "\tRelaciones binarias ALL ... ";
    cout << "CB ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_coveredby(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVEREDBY] += tiempo;

    cout << "CV ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_covers(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[COVERS] += tiempo;

    cout << "DJ ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_disjoint(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[DISJOINT] += tiempo;

    cout << "EQ ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_equals(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[EQUALS] += tiempo;

    cout << "IC ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_includes(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INCLUDES] += tiempo;

    cout << "IS ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_inside(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INSIDE] += tiempo;

    cout << "OV ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_overlaps(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[OVERLAPS] += tiempo;

    cout << "TO ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_touches(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[TOUCHES] += tiempo;

    cout << "_WT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_within(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[WHITIN] += tiempo;

    cout << "_CT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_contains(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[CONTAINS] += tiempo;

    cout << "_IT ... ";
    t0 = clock();
    for(int i = 0; i < routes.size(); i++){
        for(int j = 0; j < routes.size(); j++){
            tt.tr_intersect(i, j);
        }
    }
    t1 = clock();
    tiempo = (double)(t1 - t0);
    tr_all[INTERSECT] += tiempo;

    cout << "OK" << endl << ">>> Fin Calculo Tiempo GST_4" << endl;


    tr_all[COVEREDBY] = ((tr_all[COVEREDBY]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[COVERS] = ((tr_all[COVERS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[DISJOINT] = ((tr_all[DISJOINT]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[EQUALS] = ((tr_all[EQUALS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INCLUDES] = ((tr_all[INCLUDES]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INSIDE] = ((tr_all[INSIDE]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[OVERLAPS] = ((tr_all[OVERLAPS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[TOUCHES] = ((tr_all[TOUCHES]/CLOCKS_PER_SEC) / tr) * 1000000;

    tr_all[WHITIN] = ((tr_all[WHITIN]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[CONTAINS] = ((tr_all[CONTAINS]/CLOCKS_PER_SEC) / tr) * 1000000;
    tr_all[INTERSECT] = ((tr_all[INTERSECT]/CLOCKS_PER_SEC) / tr) * 1000000;

    // Tiempos de operaciones all
    m[idCoveredby][3] = tr_all[COVEREDBY];
    m[idCovers][3] = tr_all[COVERS];
    m[idDisjoint][3] = tr_all[DISJOINT];
    m[idEquals][3] = tr_all[EQUALS];
    m[idIncludes][3] = tr_all[INCLUDES];
    m[idInside][3] = tr_all[INSIDE];
    m[idOverlaps][3] = tr_all[OVERLAPS];
    m[idTouches][3] = tr_all[TOUCHES];

    m[idWithin][3] = tr_all[WHITIN];
    m[idContains][3] = tr_all[CONTAINS];
    m[idIntersect][3] = tr_all[INTERSECT];

    cout << "TopoRelation\tcant\tTime_true\tTime_all" << endl;
    cout << COVEREDBY << "\t" << mrt[COVEREDBY] << "\t" << tr_true[COVEREDBY] << " [us]\t" << tr_all[COVEREDBY] << " [us]" << endl;
    cout << COVERS << "\t\t" << mrt[COVERS] << "\t" << tr_true[COVERS] << " [us]\t" << tr_all[COVERS] << " [us]" << endl;
    cout << EQUALS << "\t\t" << mrt[EQUALS] << "\t" << tr_true[EQUALS] << " [us]\t" << tr_all[EQUALS] << " [us]" << endl;
    cout << INCLUDES << "\t" << mrt[INCLUDES] << "\t" << tr_true[INCLUDES] << " [us]\t" << tr_all[INCLUDES] << " [us]" << endl;
    cout << INSIDE << "\t\t" << mrt[INSIDE] << "\t" << tr_true[INSIDE] << " [us]\t" << tr_all[INSIDE] << " [us]" << endl;
    cout << OVERLAPS << "\t" << mrt[OVERLAPS] << "\t" << tr_true[OVERLAPS] << " [us]\t" << tr_all[OVERLAPS] << " [us]" << endl;
    cout << TOUCHES << "\t\t" << mrt[TOUCHES] << "\t" << tr_true[TOUCHES] << " [us]\t" << tr_all[TOUCHES] << " [us]" << endl;
    cout << DISJOINT << "\t" << mrt[DISJOINT] << "\t" << tr_true[DISJOINT] << " [us]\t" << tr_all[DISJOINT] << " [us]" << endl;
    cout << "---" << endl;
    cout << WHITIN << "\t\t\t\t\t" << tr_all[WHITIN] << " [us]" << endl;
    cout << CONTAINS << "\t\t\t\t\t\t" << tr_all[CONTAINS] << " [us]" << endl;
    cout << INTERSECT << "\t\t\t\t\t" << tr_all[INTERSECT] << " [us]" << endl;
    cout << "Total relaciones: " ;
    cout << tr << endl;

    return m;
}
