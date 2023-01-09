#ifndef TopoRel_Naive_PreComp
#define TopoRel_Naive_PreComp

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sdsl/vectors.hpp>
#include "TopoRel_Naive.hpp"
#include "TopoRel_Cons.hpp"

using namespace std;
using namespace sdsl;

// *********************** CLASE TOPORELGST ***********************
//  --------------------Inicio clase TopoRelGST-------------------
class TopoRelNaivePreComp{
public:
    vector<int_vector<>> rutas;
    vector<string> nombresRel;
    vector<int_vector<>> relaciones;
    int n_stops;
    int n_rutas;
    
    TopoRelNaivePreComp(vector<vector<int>>&, int);

    string obtenerRelacion(int, int);

    // 8 relaciones topológicas básicas
    bool equals(int, int);
    bool coveredby(int, int);
    bool covers(int, int);
    bool inside(int, int);
    bool includes(int, int);
    bool disjoint(int, int);
    bool touches(int, int);
    bool overlaps(int, int);

    // Relaciones topológicas en conjunto 
    vector<int> allContain(int);

    // Relaciones topológicas binarias agregadas
    bool within(int, int);
    bool contains(int, int);
    bool intersect(int, int);

    void navega();
    void sizeEstructura();
private:

};

#endif