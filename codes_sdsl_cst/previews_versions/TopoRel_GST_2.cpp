/*
    Versión del GST reduciendo las marcas y manteniendo las rutas
    - GST
    - Marcas en todos los nodos del GST
    - Mapa a las hojas donde terminan las rutas
*/

#include "TopoRel_GST_2.hpp"

TopoRelGST_2::TopoRelGST_2(vector<vector<int>> &rutas, int cant_stops){
    cout << "Constructor TopoRelGST_2" << endl;
    n_stops = cant_stops;
    n_concat = 0;
    n_rutas = rutas.size();
    int maxID = 0;  // Para obtener símbolo para fin de secuencia
    for(int i = 0; i < n_rutas; i++){
        n_concat += (rutas[i].size()+1);   // largo de ruta + fin_char
        // verificar el max_char del final de stops
        for(int j = 0; j < rutas[i].size(); j++){
            if(rutas[i][j] <= 0){
                cout << "Error! identificadores con valor <= 0 (i:" << i << ", j:" << j <<")" << endl;
                cout << "valor encontrado: " << rutas[i][j] << " en ruta de tamaño " << rutas[i].size() << endl;
                return;
            }
            if(maxID < rutas[i][j]){
                maxID = rutas[i][j];
            }
        }
    }
    finSec = maxID+1;
    int_vector<> iv(n_concat*2);
    gstMStops = vector<bit_vector> (rutas.size());
    gstLargos = int_vector<>(rutas.size());
    int pv = 0;
    int tr = n_rutas;
    // Concatenar rutas
    for(int i = 0; i < n_rutas; i++){
        gstLargos[i] = rutas[i].size();
        gstMStops[i] = bit_vector(n_stops+1, 0);
        for(int j = 0; j < rutas[i].size(); j++){
            iv[pv++] = rutas[i][j];
            gstMStops[i][rutas[i][j]] = 1;
        }
        iv[pv++] = finSec;
    }
    cout << "Tamaño largos: " << size_in_bytes(gstLargos);
    util::bit_compress(gstLargos);
    cout << " >> " << size_in_bytes(gstLargos) << endl;
//    cout << "Rutas... concatenadas" << endl;

    // Concatenar rutas reversas
    for(int i = 0; i < n_rutas; i++){
        for(int j = rutas[i].size()-1; j >= 0 ; j--){
            iv[pv++] = rutas[i][j];
        }
        iv[pv++] = finSec;
    }

    //cst_sct3<csa_wt<wt_int<rrr_vector<>>>> cst;
    construct_im(cst, iv);
//    cout << "GST construido" << endl;
/*
    cout << "inner nodes : " << cst.nodes() - cst.csa.size() << endl;
    auto v = cst.select_child(cst.child(cst.root(), 930),1);
    auto d = cst.depth(v);
    cout << "v : " << d << "-[" << cst.lb(v) << "," << cst.rb(v) << "]" << endl;
    cout << "extract(cst, v) : " << extract(cst, v) << endl;

    v = cst.root();
    cout << "root\tid: " << cst.id(v) << endl;
    for(int i=0; i < rutas[0].size(); i++){
        v = cst.child(v, rutas[0][i]);
        cout << rutas[0][i] << "\tid: " << cst.id(v) << endl;
    }
*/
    // Marcas en bitvectors
    gstMarcas = vector<bit_vector>(n_rutas * 2, bit_vector(cst.nodes(), 0));
    int aux;
    for(int i = 0; i < n_rutas; i++){
        for(int j = 0; j < rutas[i].size(); j++){
            // La primera mitad de marcas es para rutas
            auto v = cst.child(cst.root(), rutas[i][j]);
            while(v != cst.root() && (cst.depth(v) + j) <= rutas[i].size()){
                gstMarcas[i][cst.id(v)] = 1;
                aux = rutas[i][cst.depth(v) + j];
                v = cst.child(v, aux);
            }
        }
        vector<int> vAux(rutas[i]);
        reverse(vAux.begin(), vAux.end());
        for(int j = 0; j < vAux.size(); j++){
            // La primera mitad de marcas es para rutas
            auto v = cst.child(cst.root(), vAux[j]);
            while(v != cst.root() && (cst.depth(v) + j) <= vAux.size()){
                gstMarcas[i+n_rutas][cst.id(v)] = 1;
                aux = vAux[cst.depth(v) + j];
                v = cst.child(v, aux);
            }
        }
    }
//    cout << "Marcas en bitvector... OK" << endl;
/*
    cout << "Marcas: " << endl;
    for(int i=0; i<gstMarcas.size(); i++){
        for(int j=0; j<gstMarcas[i].size(); j++){
            cout << gstMarcas[i][j] << " " ;
        }
        cout << endl;
    }
    cout << endl;
*/
    // MAP en un vector
    gstMapa = vector<cst_sct3<>::node_type>(n_rutas * 2);
    for(int i = 0; i < n_rutas; i++){
        // La primera mitad de mapa para rutas
        auto v = cst.child(cst.root(), rutas[i][0]);
        while(v != cst.root() && cst.depth(v) < rutas[i].size()){
            v = cst.child(v, rutas[i][cst.depth(v)]);
        }
        if(cst.depth(v) <= rutas[i].size()){
            v = cst.child(v, finSec);
        }
        gstMapa[i] = v;
        // La segunda mitad de mapa para rutas reversas
        int pr = rutas[i].size()-1;
        v = cst.child(cst.root(), rutas[i][pr]);
        while(v != cst.root() && cst.depth(v) < rutas[i].size()){
            v = cst.child(v, rutas[i][pr-cst.depth(v)]);
        }
        if(cst.depth(v) <= rutas[i].size()){
            v = cst.child(v, finSec);
        }
        gstMapa[n_rutas + i] = v;
    }
    // Nodos de los bordes de cada ruta
    gstStopBI = int_vector<>(rutas.size());
    gstStopBF = int_vector<>(rutas.size());
    for(int i = 0; i < n_rutas; i++){
        gstStopBI[i] = rutas[i][0];
        gstStopBF[i] = rutas[i][gstLargos[i]-1];
    }
    util::bit_compress(gstStopBI);
    util::bit_compress(gstStopBF);

//    cout << "Map... OK" << endl;
    cout << "Fin constructor" << endl;
}



string TopoRelGST_2::obtenerRelacion(int x, int y){
//    cout << endl << "x: " << x << " - y: " << y << endl;
//    cout << "id x: " << cst.id(gstMapa[x]) << "id y: " << cst.id(gstMapa[y]) << endl;
    // Identificar igualdad en cualquiera de los dos sentidos
    if(gstMapa[x] == gstMapa[y]){
        return EQUALS;
    }
    if(gstMapa[x] == gstMapa[y+n_rutas]){
        return EQUALS;
    }
    // Identificar contención según nodo
    // Inside, includes, coveredBy, covers
    int corto, largo, lC, lL;
    int lx = gstLargos[x];
    int ly = gstLargos[y];
    if(lx < ly){
        corto = x;
        lC = lx;
        largo = y;
        lL = ly;
    }else{
        corto = y;
        lC = ly;
        largo = x;
        lL = lx;
    }

    auto C = gstMapa[corto];
    auto Cr = gstMapa[corto+n_rutas];
    auto L = gstMapa[largo];
    auto Lr = gstMapa[largo+n_rutas];
    // LCA entre corta y larga en sus combinaciones
    auto lcaCL = cst.lca(C, L);
    auto lcaCLr = cst.lca(C, Lr);
    auto lcaCrL = cst.lca(Cr, L);
    auto lcaCrLr = cst.lca(Cr, Lr);
    // Para obtener (CV o CB) se debe verificar que el lca sea el mismo nodo de corta.
    if(cst.depth(lcaCL) == lC || cst.depth(lcaCLr) == lC || cst.depth(lcaCrL) == lC || cst.depth(lcaCrLr) == lC){
        if(corto == x){
            return COVEREDBY;
        }
        return COVERS;
    }
    
    // Para obtener (IC o IS) se debe mirar si el nodo padre de corto tiene una marca de largo
    auto padreCorto = cst.parent(gstMapa[corto]);
    int idPaCorto = cst.id(padreCorto);
    if(cst.depth(padreCorto) == gstLargos[corto] && 
            (gstMarcas[largo][idPaCorto] || gstMarcas[largo+n_rutas][idPaCorto])){
        // Existe contención
        if(corto == x){
            return INSIDE;
        }
        return INCLUDES;
    }


    // Determinar si hay disjoint por medio de marcas
    bool bix = false;
    bool bfx = false;
    bool biy = false;
    bool bfy = false;
    int interIB = 0;
    for(int i=0; i < gstMStops[x].size(); i++){
        if(gstMStops[x][i] == 1 && gstMStops[y][i] == 1){
            // Hay intersección
            if(i == gstStopBI[x] || i == gstStopBF[x] || i == gstStopBI[y] || i == gstStopBF[y]){
                interIB++;
                bix = bix || i == gstStopBI[x];
                bfx = bfx || i == gstStopBF[x];
                biy = biy || i == gstStopBI[y];
                bfy = bfy || i == gstStopBF[y];
            }else{
                return OVERLAPS;
            }
        }
    }

    if(interIB == 0){
        return DISJOINT;
    }

    if((!bix && !bfx) || (!biy && !bfy)){
        // Una de las rutas no toca con el borde a la otra.
        return TOUCHES;
    }

    // Cuando hay intersecciones de borde de las 2 secuencias
//    cout << "x: " << x << " - y: " << y << endl;
//   cout << "revisando secuencia original:" << endl;
    auto nAux = gstMapa[x];
    while(bix && cst.depth(nAux) > 2){
//        cout << "id_" << cst.id(nAux) << " - depth: " << cst.depth(nAux) << endl;
        nAux = cst.parent(nAux);
    }
//    cout << "id_" << cst.id(nAux) << " - depth: " << cst.depth(nAux) << endl;
    int idAux = cst.id(nAux);
    if(bix && cst.depth(nAux) == 2 && (gstMarcas[y][idAux] || gstMarcas[y+n_rutas][idAux])){
        return OVERLAPS;
    }
//    cout << "revisando secuencia original:" << endl;
    nAux = gstMapa[x+n_rutas];
    while(bfx && cst.depth(nAux) > 2){
//        cout << "id_" << cst.id(nAux) << " - depth: " << cst.depth(nAux) << endl;
        nAux = cst.parent(nAux);
    }
//    cout << "id_" << cst.id(nAux) << " - depth: " << cst.depth(nAux) << endl;
    idAux = cst.id(nAux);
    if(bfx && cst.depth(nAux) == 2 && (gstMarcas[y][idAux] || gstMarcas[y+n_rutas][idAux])){
        return OVERLAPS;
    }
    return TOUCHES;
}


/*******************************************************
            8 Relaciones topológicas básicas
*******************************************************/


bool TopoRelGST_2::tr_equals(int x, int y){
    if(gstMapa[x] == gstMapa[y] || gstMapa[x] == gstMapa[y+n_rutas]){
        return true;
    }
    return false;
}

bool TopoRelGST_2::tr_coveredby(int x, int y){
    // Descarte por largo de secuencias
    int lx = gstLargos[x];
    int ly = gstLargos[y];
    if(lx >= ly){
        return false;
    }
    // Verifica CoveredBy
    /*
    auto lca = cst.lca(gstMapa[x], gstMapa[y]);
    if(cst.depth(lca) == lx){
        return true;
    }
    lca = cst.lca(gstMapa[x], gstMapa[y+n_rutas]);
    if(cst.depth(lca) == lx){
        return true;
    }
    lca = cst.lca(gstMapa[x+n_rutas], gstMapa[y]);
    if(cst.depth(lca) == lx){
        return true;
    }
    lca = cst.lca(gstMapa[x+n_rutas], gstMapa[y+n_rutas]);
    if(cst.depth(lca) == lx){
        return true;
    }
    */
    // Comprobar bordes
    if(gstStopBI[x] == gstStopBI[y] || gstStopBI[x] == gstStopBF[y] ||
            gstStopBF[x] == gstStopBI[y] || gstStopBF[x] == gstStopBF[y]){
        // Comprobrar contención
        auto parentX = cst.parent(gstMapa[x]);
        if(cst.depth(parentX) == gstLargos[x] && (gstMarcas[y][cst.id(parentX)] || gstMarcas[y+n_rutas][cst.id(parentX)])){
            return true;
        }
        parentX = cst.parent(gstMapa[x+n_rutas]);
        if(cst.depth(parentX) == gstLargos[x] && (gstMarcas[y][cst.id(parentX)] || gstMarcas[y+n_rutas][cst.id(parentX)])){
            return true;
        }
    }

    return false;
}

bool TopoRelGST_2::tr_covers(int x, int y){
    return tr_coveredby(y,x);
}

bool TopoRelGST_2::tr_inside(int x, int y){
    // Descarte por largo de secuencias
    int lx = gstLargos[x];
    int ly = gstLargos[y];
    if(lx >= ly){
        return false;
    }
    
    // Comprobar bordes
    if(gstStopBI[x] != gstStopBI[y] && gstStopBI[x] != gstStopBF[y] &&
            gstStopBF[x] != gstStopBI[y] && gstStopBF[x] != gstStopBF[y]){
        // Comprobar contención
        auto parentX = cst.parent(gstMapa[x]);
        if(cst.depth(parentX) == gstLargos[x] && (gstMarcas[y][cst.id(parentX)] || gstMarcas[y+n_rutas][cst.id(parentX)])){
            return true;
        }
        parentX = cst.parent(gstMapa[x+n_rutas]);
        if(cst.depth(parentX) == gstLargos[x] && (gstMarcas[y][cst.id(parentX)] || gstMarcas[y+n_rutas][cst.id(parentX)])){
            return true;
        }
    }
    return false;
}

bool TopoRelGST_2::tr_includes(int x, int y){
    return tr_inside(y,x);
}

bool TopoRelGST_2::tr_disjoint(int x, int y){
    // Descartar igualdad
    if(gstMapa[x] == gstMapa[y] || gstMapa[x] == gstMapa[y+n_rutas]){
        return false;
    }
    // Descartar contención por CB o CV o IS o IC
    auto parentX = cst.parent(gstMapa[x]);
    if(gstMarcas[y][cst.id(parentX)] || gstMarcas[y+n_rutas][cst.id(parentX)]){
        return false;
    }
    parentX = cst.parent(gstMapa[x+n_rutas]);
    if(gstMarcas[y][cst.id(parentX)] || gstMarcas[y+n_rutas][cst.id(parentX)]){
        return false;
    }

    // Descartar por los bordes
    if(gstMStops[y][gstStopBI[x]] == 1 || gstMStops[y][gstStopBF[x]] == 1 
            || gstMStops[x][gstStopBI[y]] == 1 || gstMStops[x][gstStopBF[y]] == 1){
        // Uno de los bordes de una secuencia toca un elemento de la otra
        return false;
    }

    // Determinar si hay disjoint por medio de marcas
    int interIB = 0;
    for(int i=0; i < gstMStops[x].size(); i++){
        if(gstMStops[x][i] == 1 && gstMStops[y][i] == 1){
            return false;
        }
    }

    // Hay disjunción
    return true;
}

bool TopoRelGST_2::tr_touches(int x, int y){
    // Descarte por igualdad
    if(gstMapa[x] == gstMapa[y] || gstMapa[x] == gstMapa[y+n_rutas]){
        return false;
    }
    // Verifica candidato por bordes
    int bXi = gstStopBI[x];
    int bXf = gstStopBF[x];
    int bYi = gstStopBI[y];
    int bYf = gstStopBF[y];
    if(gstMStops[y][bXi] != 1 && gstMStops[y][bXf] != 1 && gstMStops[x][bYi] != 1 && gstMStops[x][bYf] != 1){
        // No hay bordes en contacto con la otra secuencia
        return false;
    }
    // Verficar que la coincidencia de bordes no posee intersección I-I
    // Determinar si hay disjoint por medio de marcas
    bool bix = false;
    bool bfx = false;
    bool biy = false;
    bool bfy = false;
    for(int i=0; i < gstMStops[x].size(); i++){
        if(gstMStops[x][i] == 1 && gstMStops[y][i] == 1){
            // Hay intersección
            if(i == gstStopBI[x] || i == gstStopBF[x] || i == gstStopBI[y] || i == gstStopBF[y]){
                bix = bix || i == gstStopBI[x];
                bfx = bfx || i == gstStopBF[x];
                biy = biy || i == gstStopBI[y];
                bfy = bfy || i == gstStopBF[y];
            }else{
                return false;
            }
        }
    }

    if((!bix && !bfx) || (!biy && !bfy)){
        // Una de las rutas no toca con el borde a la otra.
        return true;
    }

    // Cuando hay intersecciones de borde de las 2 secuencias
    auto nAux = gstMapa[x];
    while(bix && cst.depth(nAux) > 2){
        nAux = cst.parent(nAux);
    }
    int idAux = cst.id(nAux);
    if(bix && cst.depth(nAux) == 2 && (gstMarcas[y][idAux] || gstMarcas[y+n_rutas][idAux])){
        return false;
    }
    nAux = gstMapa[x+n_rutas];
    while(bfx && cst.depth(nAux) > 2){
        nAux = cst.parent(nAux);
    }
    idAux = cst.id(nAux);
    if(bfx && cst.depth(nAux) == 2 && (gstMarcas[y][idAux] || gstMarcas[y+n_rutas][idAux])){
        return false;
    }
    return true;
}

bool TopoRelGST_2::tr_overlaps(int x, int y){
    // Descartar igualdad
    if(gstMapa[x] == gstMapa[y] || gstMapa[x] == gstMapa[y+n_rutas]){
        return false;
    }
    // Descartar contención por CB o CV
    if(tr_coveredby(x,y) || tr_covers(x,y)){
        return false;
    }
    // Descartar contención por IC o IS
    if(tr_includes(x,y) || tr_inside(x,y)){
        return false;
    }
    // Comprobar intersección interior-interior
    bool bix = false;
    bool bfx = false;
    bool biy = false;
    bool bfy = false;
    for(int i=0; i < gstMStops[x].size(); i++){
        if(gstMStops[x][i] == 1 && gstMStops[y][i] == 1){
            // Hay intersección
            if(i == gstStopBI[x] || i == gstStopBF[x] || i == gstStopBI[y] || i == gstStopBF[y]){
                // Intersección con algún borde
                bix = bix || i == gstStopBI[x];
                bfx = bfx || i == gstStopBF[x];
                biy = biy || i == gstStopBI[y];
                bfy = bfy || i == gstStopBF[y];
            }else{
                // Intersección interior-interior
                return true;
            }
        }
    }
    // Descartar por bordes
    if((!bix && !bfx) || (!biy && !bfy)){
        // Sólo una de las rutas toca con el borde a la otra.
        return false;
    }
    // Puede haber un segmento en común por los dos bordes en contacto
    auto nAux = gstMapa[x];
    while(bix && cst.depth(nAux) > 2){
        nAux = cst.parent(nAux);
    }
    int idAux = cst.id(nAux);
    if(bix && cst.depth(nAux) == 2 && (gstMarcas[y][idAux] || gstMarcas[y+n_rutas][idAux])){
        return true;
    }
    nAux = gstMapa[x+n_rutas];
    while(bfx && cst.depth(nAux) > 2){
        nAux = cst.parent(nAux);
    }
    idAux = cst.id(nAux);
    if(bfx && cst.depth(nAux) == 2 && (gstMarcas[y][idAux] || gstMarcas[y+n_rutas][idAux])){
        return true;
    }
    return false;
}


/*******************************************************
            Relaciones topológicas agregadas
*******************************************************/


bool TopoRelGST_2::tr_within(int x, int y){
    // Debe ser EQUALS, COVEREDBY o INSIDE
    if(gstMapa[x] == gstMapa[y] || gstMapa[x] == gstMapa[y+n_rutas]){
        return true;
    }
    // Descarte por largo de secuencias
    int lx = gstLargos[x];
    int ly = gstLargos[y];
    if(lx >= ly){
        return false;
    }
    // Verifica CoveredBy
    auto lca = cst.lca(gstMapa[x], gstMapa[y]);
    if(cst.depth(lca) == lx){
        return true;
    }
    lca = cst.lca(gstMapa[x], gstMapa[y+n_rutas]);
    if(cst.depth(lca) == lx){
        return true;
    }
    lca = cst.lca(gstMapa[x+n_rutas], gstMapa[y]);
    if(cst.depth(lca) == lx){
        return true;
    }
    lca = cst.lca(gstMapa[x+n_rutas], gstMapa[y+n_rutas]);
    if(cst.depth(lca) == lx){
        return true;
    }
    // Verificación de Inside por gstMarcas
    auto padreX = cst.parent(gstMapa[x]);
    int idPaX = cst.id(padreX);
    if(cst.depth(padreX) == lx && gstMarcas[y][idPaX]){
        return true;
    }
    return false;
}

bool TopoRelGST_2::tr_contains(int x, int y){
    return tr_within(y, x);
}

bool TopoRelGST_2::tr_intersect(int x, int y){
    return !tr_disjoint(x, y);
}


/*******************************************************
            Otras funcionalidades
*******************************************************/


void TopoRelGST_2::navega(int x){

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
        cout << extract(cst, *it) << "\t";      // Text

        cout << endl;

        if(++count % 5 == 0){
            cout << endl;
        }
    }
    cout << endl;

    cout << "CSA del CompressedSuffixTree:" << endl;
    for(int i=0; i<cst.csa.size(); i++){
        cout << cst.csa[i] << " ";
    }
    cout << endl << endl;

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
    for(int i=0; i<gstMapa.size(); i++){
        cout << "(" << i << "): " << cst.id(gstMapa[i]) << endl;
    }
    cout << endl;

    cout << endl << "Marcas Nodos: \n\t";
    for(int i=0; i<gstMarcas[0].size()/10; i++){
        cout << i << "         ";
    }
    cout << "\n\t";
    for(int i=0; i<gstMarcas[0].size(); i++){
        cout << i%10;
    }
    cout << endl;
    for(int i=0; i<gstMarcas.size(); i++){
        cout << "(" << i << "): \t";
        for(int j=0; j<gstMarcas[i].size(); j++){
            cout << gstMarcas[i][j];
        }
        cout << endl;
    }
    cout << endl << "Marcas Stops: \n\t";
    for(int i=0; i<gstMStops[0].size()/10; i++){
        cout << i << "         ";
    }
    cout << "\n\t";
    for(int i=0; i<gstMStops[0].size(); i++){
        cout << i%10;
    }
    cout << endl;
    for(int i=0; i<gstMStops.size(); i++){
        cout << "(" << i << "): \t";
        for(int j=0; j<gstMStops[i].size(); j++){
            cout << gstMStops[i][j];
        }
        cout << endl;
    }

}

void TopoRelGST_2::sizeEstructura(){
    cout << "**** Tamaño en bytes ****" << endl;
    cout << "cst_sct3 [B]: " << size_in_bytes(cst) << endl;
    // Calculo de los bytes para Marcas
    unsigned long long bytesMarcas = 0;
    int bitsUnoMarcas = 0;
    int bitsTotalMarcas = 0;
    for(int i=0; i<gstMarcas.size(); i++){
        bytesMarcas += size_in_bytes(gstMarcas[i]);
        bitsTotalMarcas += gstMarcas[i].size();
        for(int j=0; j<gstMarcas[i].size(); j++){
            bitsUnoMarcas += gstMarcas[i][j];
        }
    }
    double porcentajeMarcas = (bitsUnoMarcas+0.0)/bitsTotalMarcas*100;
    cout << "marcas [B]: " << bytesMarcas << endl;
    // Calculo de los bytes para Marcas
    unsigned long long bytesMStops = 0;
    int bitsUnoMStops = 0;
    int bitsTotalMStops = 0;
    for(int i=0; i<gstMStops.size(); i++){
        bytesMStops += size_in_bytes(gstMStops[i]);
        bitsTotalMStops += gstMStops[i].size();
        for(int j=0; j<gstMStops[i].size(); j++){
            bitsUnoMStops += gstMStops[i][j];
        }
    }
    double porcentajeMStops = (bitsUnoMStops+0.0)/bitsTotalMStops*100;
    cout << "stops [B]: " << bytesMStops << endl;
    // Calculo de los bytes para MAPA
    unsigned long long bytesMapa = 0;
    for(int i=0; i<gstMapa.size(); i++){
        bytesMapa += sizeof(gstMapa[i]);
    }
    cout << "mapa [B]: " << bytesMapa << endl;
    cout << "largos [B]: " << size_in_bytes(gstLargos) << endl;
    cout << "bordes [B]: " << (size_in_bytes(gstStopBI)+size_in_bytes(gstStopBF)) << endl;

    cout << "**** Elementos ****" << endl;
    cout << "Nº Rutas: " << gstMarcas.size() << endl;
    cout << "Nº Nodos cst_sct3: " << cst.nodes() << endl;
    cout << "Nº Hojas cst_sct3: " << cst.size() << endl;
    cout << "Nº 1s/length en marcas: " << bitsUnoMarcas << "/" << bitsTotalMarcas;
    cout << " (" << porcentajeMarcas << "%)" << endl;
    cout << "Nº 1s/length en stops: " << bitsUnoMStops << "/" << bitsTotalMStops;
    cout << " (" << porcentajeMStops << "%)" << endl;
}