#ifndef TopoRel_GST_2
#define TopoRel_GST_2

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

// *********************** CLASE TOPORELGST_2 ***********************
//  --------------------Inicio clase TopoRelGST-------------------
class TopoRelGST_2{
public:
    cst_sct3<csa_wt<wt_int<rrr_vector<>>>> cst;
    vector<int_vector<>> gstRutas;
    vector<bit_vector> gstMarcas;
    vector<cst_sct3<>::node_type> gstMapa;
    int_vector<> gstLargos;
    int n_rutas;
    int n_concat;
    int n_stops;
    int finSec;
    
    TopoRelGST_2(vector<vector<int>>&, int);
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
    bool tr_intersects(int, int);

    void navega(int);
    void sizeEstructura();

private:
    bool bordesSeg_touches(int, int);
    bool interiorInterior(int, int);
    cst_sct3<>::node_type nodoSubseq(cst_sct3<>::node_type, int);
};

#endif