#ifndef TopoRel_GST_4
#define TopoRel_GST_4

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sdsl/vectors.hpp>
#include <sdsl/suffix_trees.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/cst_iterators.hpp>
#include "TopoRel_Cons.hpp"

using namespace std;
using namespace sdsl;

// *********************** CLASE TOPORELGST_4 ***********************
//  --------------------Inicio clase TopoRelGST-------------------
class TopoRelGST_4{
public:
    cst_sada<csa_wt<wt_int<rrr_vector<>>>> cst;
    vector<int_vector<>> gstRutas;
    vector<sd_vector<>> gstMarcas;
    vector<cst_sada<>::node_type> gstMapa;
    int n_rutas;
    int n_concat;
    int n_stops;
    int finSec;
    
    TopoRelGST_4(vector<vector<int>>&, int);
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

    //void navega(int);
    void sizeEstructura();

private:
    bool bordesSeg_touches(int, int);
    cst_sada<>::node_type nodoSubseq(cst_sada<>::node_type, int);
};

#endif