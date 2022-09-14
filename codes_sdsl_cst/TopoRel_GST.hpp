#ifndef TopoRel_GST
#define TopoRel_GST

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
private:

};

#endif