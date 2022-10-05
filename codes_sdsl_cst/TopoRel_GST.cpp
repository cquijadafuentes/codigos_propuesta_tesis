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
        for(int j = 0; j < routes[i].size(); j++){
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
    if(bordesSeg_touches(corto, largo)){
        return OVERLAPS;
    }    
    // Touches, Overlaps, Disjoint
    auto root = cst.root();
    bool touches = false;
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
    if(marcas[y][id_x] == 1 || marcas[y + n_routes][id_x] == 1
            || marcas[y][id_x + n_routes] == 1 || marcas[y + n_routes][id_x + n_routes] == 1){
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
    if(marcas[x][id_y] == 1 || marcas[x + n_routes][id_y] == 1
            || marcas[x][id_y + n_routes] == 1 || marcas[x + n_routes][id_y + n_routes] == 1){
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
    if(marcas[y][id_x] == 1 || marcas[y + n_routes][id_x] == 1
            || marcas[y][id_x + n_routes] == 1 || marcas[y + n_routes][id_x + n_routes] == 1){
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
    if(marcas[x][id_y] == 1 || marcas[x + n_routes][id_y] == 1
            || marcas[x][id_y + n_routes] == 1 || marcas[x + n_routes][id_y + n_routes] == 1){
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
    // Descarta igualdad
    if(mapa[largo] == mapa[corto] || mapa[largo] == mapa[corto+n_routes]){
        return false;
    }
    int id_corto = cst.id(mapa[corto]);
    // Descarta contención
    if(marcas[largo][id_corto] == 1 || marcas[largo + n_routes][id_corto] == 1){
        return false;
    }
    // Identificar alguna intersección
    auto root = cst.root();
    for(int i = 0; i < routes[corto].size(); i++){
        auto ch = cst.child(root, routes[corto][i]);
        if(ch != root && marcas[largo][cst.id(ch)]){
            return false;
        }
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
    if(bordesSeg_touches(corto, largo)){
        return false;
    }
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

void TopoRelGST::navega(int x){

    cout << "Información del CompressedSuffixTree:" << endl;
    cout << "Cantidad de nodos: " << cst.nodes() << endl;
    cout << "Cantidad de hojas: " << cst.size() << endl;

    cout << "Recorrido de hijos de root del CompressedSuffixTree:" << endl;
    auto root = cst.root();
    cout << "id\ted_1\tdeg\tdep\tndep\tsize\tlb\trb\tsun\tleaf\ttext" << endl;
    for (auto& child: cst.children(root)) {
        cout << cst.id(child) << "\t";
        cout << "'" << cst.edge(child, 1) << "'" << "\t";       // D-th char of the edge-label
        cout << cst.degree(child) << "\t";      // Number of children
        cout << cst.depth(child) << "\t";       // String depth
        cout << cst.node_depth(child) << "\t";  // 
        cout << cst.size(child) << "\t";        // Number of leaves in the subtree
        cout << cst.lb(child) << "\t";          // Leftmost leaf
        cout << cst.rb(child) << "\t";          // Rightmost leaf
        cout << cst.sn(child) << "\t";          // Suffix number
        cout << cst.is_leaf(child) << "\t";     // IsLeaf
        for(int i=1; i<=cst.depth(child); i++){
            cout << cst.edge(child, i);
        }
        cout << "\t" << endl;
    }

    cout << endl;
    cout << "BFS del CompressedSuffixTree:" << endl;
    cout << "id\ted_1\tdeg\tdep\tndep\tsize\tlb\trb\tsun\tleaf\ttext" << endl;
    typedef cst_bfs_iterator<cst_sct3<csa_wt<wt_int<rrr_vector<>>>>> iterator;
    iterator begin = iterator(&cst, cst.root());
    iterator end   = iterator(&cst, cst.root(), true, true);
    int count = 0;
    for (iterator it = begin; it != end; ++it) {
        cout << cst.id(*it) << "\t";
        cout << "'" << cst.edge(*it, 1) << "'" << "\t";     // D-th char of the edge-label
        cout << cst.degree(*it) << "\t";        // Number of children
        cout << cst.depth(*it) << "\t";         // String depth
        cout << cst.node_depth(*it) << "\t";    // 
        cout << cst.size(*it) << "\t";          // Number of leaves in the subtree
        cout << cst.lb(*it) << "\t";            // Leftmost leaf
        cout << cst.rb(*it) << "\t";            // Rightmost leaf
        cout << cst.sn(*it) << "\t";            // Suffix number
        cout << cst.is_leaf(*it) << "\t";       // IsLeaf
        for(int i=1; i<=cst.depth(*it); i++){
            cout << cst.edge(*it, i) << " ";
        }
        cout << "\t" << endl;

        if(++count % 5 == 0){
            cout << endl;
        }
    }

    cout << endl;
    cout << "Pruebas de Navegación en CompressedSuffixTree: " << endl;
    
    auto nodeAux = cst.root();
    cout << "Root: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
        cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.select_leaf(3);
    cout << "Leaf 3: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
        cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.leftmost_leaf(cst.root());
    cout << "Leftmost: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
        cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.rightmost_leaf(cst.root());
    cout << "Rightmost: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
        cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.parent(cst.rightmost_leaf(cst.root()));
    cout << "Parent: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
        cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.select_child(cst.child(cst.root(), 'l'),1);
    cout << "Sibling de 'lmu': " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
        cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.select_child(cst.root(),6);
    cout << "Select child 6 de root: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
        cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.child(cst.root(),'u');
    cout << "child starts 'u' desde root: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
        cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.inv_id(7);
    cout << "inv_id de 7: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
        cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.sl(cst.inv_id(7));
    cout << "suffix link nodo 'mum': " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
        cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.wl(cst.inv_id(7), 'l');
    cout << "Weiner link nodo 'mum' y 'l': " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
        cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    cout << endl << "mapa: " << endl;
    for(int i=0; i<mapa.size(); i++){
        cout << "(" << i << "): " << cst.id(mapa[i]) << endl;
    }

}

void TopoRelGST::sizeEstructura(){
    cout << "**** Tamaño en bytes ****" << endl;
    cout << "cst_sct3: " << size_in_bytes(cst) << endl;
    // Calculo de los bytes para rutas
    int elementos = 0;
    for(int i=0; i<routes.size(); i++){
        elementos += routes[i].size();
    }
    int bytesRutas = (elementos*4)*2;
    cout << "rutas/rev: " << bytesRutas << endl;
    // Calculo de los bytes para marcas
    int bytesMarcas = 0;
    for(int i=0; i<marcas.size(); i++){
        bytesMarcas += size_in_bytes(marcas[i]);
    }
    cout << "marcas: " << bytesMarcas << endl;
    // Calculo de los bytes para mapa
    int bytesMapa = 0;
    for(int i=0; i<mapa.size(); i++){
        bytesMapa += sizeof(mapa[i]);
    }
    cout << "mapa: " << bytesMapa << endl;

    cout << "**** Elementos ****" << endl;
    cout << "Rutas: " << routes.size() << endl;
    cout << "Nodos cst_sct3: " << cst.nodes() << endl;
    cout << "Hojas cst_sct3: " << cst.size() << endl;
}

bool TopoRelGST::bordesSeg_touches(int i, int j){
    // Comprobar segmentos borde
    auto root = cst.root();
    auto ch1 = cst.child(root, routes[i][0]);
    if(cst.depth(ch1) == 1){
        ch1 = cst.child(ch1, routes[i][1]);
    }
    int cff = routes[i].size() - 1;
    auto ch2 = cst.child(root, routes[i][cff]);
    if(cst.depth(ch2) == 1){
        ch2 = cst.child(ch2, routes[i][cff-1]);
    }

    if(marcas[j][cst.id(ch1)]){
        return true;
    }
    if(marcas[j][cst.id(ch2)]){
        return true;
    }
    if(marcas[j + n_routes][cst.id(ch1)]){
        return true;
    }
    if(marcas[j + n_routes][cst.id(ch2)]){
        return true;
    }
    return false;
}