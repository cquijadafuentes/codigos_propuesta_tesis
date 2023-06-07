#ifndef TopoRel_GST
#define TopoRel_GST

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_set>
#include <sdsl/vectors.hpp>
#include <sdsl/suffix_trees.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/cst_iterators.hpp>
#include <omp.h>                    // OpenMP: paralelismo en construcción
#include "TopoRel_Cons.hpp"

using namespace std;
using namespace sdsl;

// *********************** CLASE TOPORELGST ***********************
//  --------------------Inicio clase TopoRelGST-------------------
class TopoRelGST{
public:
    int n_rutas;
    int n_concat;
    int n_stops;
    int finSec;
    int len_min;
    int len_max;

    int howManyLCP;
    int howManyNodes;
    int howManyInserts;
    int howManyIfs;
    int howManyidRutaDesdeCeldaDeSecConcat;
    int howManygetLargoRuta;
    int howManyCSTviews;


    cst_sada<csa_wt<wt_int<rrr_vector<>>>> cst;
    vector<cst_sada<>::node_type> gstMapRuta2Nodo;  // Se guarda en la posición que representa la ID de la Ruta, el nodo que la representa
    unordered_multimap<int,int> gstMapNodo2Ruta;    // Las claves serán el id del nodo, y los valores serán las id de la secuencia 
    sd_vector<> gstMFSbv;                       // Marcas de fin de secuencia
    sd_vector<>::rank_1_type gstMFSrank;
    sd_vector<>::select_1_type gstMFSselect;
    sd_vector<> gstMNodos;                         // Marcas de Nodos en Mapa
    sd_vector<> gstMRamas;                         // Marcas de Ramas con hijos en Mapa
    
    TopoRelGST(vector<vector<int>>&, int);    // Constructor Top-down Paralelo
    TopoRelGST(string);

    // Relaciones topológicas en conjunto
    vector<int> tr_allContain(int);
    vector<int> tr_allEqual(int);
    vector<int> tr_allContained(int);
    vector<int> tr_allIntersect(int);
    unordered_map<int,int> tr_allIntersectPP(int, int=1);

    void navega(int);
    void sizeEstructura();
    void sizeToPlot();
    void printRutaYPos(int);
    void printRuta(int);
    int getLargoRuta(int);
    vector<int> getRuta(int);
    bool iguales(TopoRelGST x);
    void statsReset();

    bool save(string);

private:
    cst_sada<>::node_type nodoSubseq(cst_sada<>::node_type, int);
    int idRutaDesdeCeldaDeSecConcat(int);
    vector<int> getSecConPrefijoDelNodo(cst_sada<>::node_type, int);
};

#endif