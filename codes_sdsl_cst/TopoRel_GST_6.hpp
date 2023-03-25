#ifndef TopoRel_GST_6
#define TopoRel_GST_6

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sdsl/vectors.hpp>
#include <sdsl/suffix_trees.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/cst_iterators.hpp>
#include <omp.h>                    // OpenMP para el paralelismo
#include "TopoRel_Cons.hpp"

using namespace std;
using namespace sdsl;

// *********************** CLASE TOPORELGST_6 ***********************
//  --------------------Inicio clase TopoRelGST-------------------
class TopoRelGST_6{
public:
    int n_rutas;
    int n_concat;
    int n_stops;
    int finSec;
    int len_min;
    int len_max;

    int howManyLCP;

    cst_sada<csa_wt<wt_int<rrr_vector<>>>> cst;
    vector<cst_sada<>::node_type> gstMapa;
    vector<int_vector<>> gstRutas;
    vector<sd_vector<>> gstStops;
    sd_vector<> gstMFSbv;                       // Marcas de fin de secuencia
    sd_vector<>::rank_1_type gstMFSrank;
    sd_vector<>::select_1_type gstMFSselect;
    sd_vector<> gstMN;                         // Marcas de Nodos en Mapa
    
    TopoRelGST_6(vector<vector<int>>&, int);    // Constructor Top-down Paralelo
    TopoRelGST_6(string);                       // Constructor desde archivo
    string obtenerRelacion(int, int);

    // 8 relaciones topológicas básicas
    bool tr_equals(int, int);
    bool tr_coveredby(int, int);
    bool tr_covers(int, int);
    bool tr_inside(int, int);
    bool tr_includes(int, int);
    bool tr_disjoint(int, int);
    bool tr_touches(int, int);
    bool tr_overlaps(int, int);

    // Relaciones topológicas binarias agregadas
    bool tr_within(int, int);
    bool tr_contains(int, int);
    bool tr_intersect(int, int);

    // Relaciones topológicas en conjunto
    vector<int> tr_allContain(int);
    vector<int> tr_allEqual(int);
    vector<int> tr_allContained(int,bool=false);
    vector<int> tr_allContained2(int,bool=false);   // Recorrido desde la raíz de los distintos sufijos
    vector<int> tr_allContained3(int,bool=false);   // Recorrido desde el nodo de la secuencia usando wl

    void navega(int);
    void sizeEstructura();
    void sizeToPlot();
    void printRutaYPos(int);
    void printRuta(int);
    int getLargoRuta(int);
    bool iguales(TopoRelGST_6 x);

    bool save(string);

private:
    bool bordesSeg_touches(int, int);
    cst_sada<>::node_type nodoSubseq(cst_sada<>::node_type, int);
    int idRutaDesdeCeldaDeSecConcat(int);
    vector<int> getSecConPrefijoDelNodo(cst_sada<>::node_type, int);
};

#endif