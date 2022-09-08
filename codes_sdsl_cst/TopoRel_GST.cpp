#include "TopoRel_GST.hpp"

using namespace std;
using namespace sdsl;

TopoRelGST::TopoRelGST(vector<vector<int>> &rutas, int cant_stops){
    routes = rutas;
    n_stops = cant_stops;
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


/*******************************************************
            8 Relaciones topológicas básicas
*******************************************************/


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


/*******************************************************
            Relaciones topológicas agregadas
*******************************************************/


bool TopoRelGST::tr_within(int x, int y){
    if(routes[x].size() > routes[y].size()){
        return false;
    }
    int id_x = cst.id(mapa[x]);
    if(marcas[y][id_x] == 1 || marcas[y + n_routes][id_x] == 1){
        // Hay contensión
        return true;
    }
    return false;
}

bool TopoRelGST::tr_contains(int x, int y){
    if(routes[x].size() < routes[y].size()){
        return false;
    }
    int id_y = cst.id(mapa[y]);
    if(marcas[x][id_y] == 1 || marcas[x + n_routes][id_y] == 1){
        // Hay contensión
        return true;
    }
    return false;
}

bool TopoRelGST::tr_intersects(int x, int y){
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
    
    auto root = cst.root();
    for(int i = 1; i < routes[corto].size() - 1; i++){
        auto ch = cst.child(root, routes[corto][i]);
        if(ch != root && marcas[largo][cst.id(ch)]){
            return true;
        }
    }

    return false;
}
