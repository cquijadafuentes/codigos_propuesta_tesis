/*
    Versión del GST con todas las ideas iniciales disponibles:
    - GST
    - Vector con las rutas
    - Marcas en stops (primer nivel del GST) sd_vector
    - Mapa a las hojas donde terminan las rutas
*/

#include "TopoRel_GST_1.hpp"

TopoRelGST_1::TopoRelGST_1(vector<vector<int>> &rutas, int cant_stops){
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
    gstRutas = vector<int_vector<>>(rutas.size());
    int pv = 0;
    int tr = n_rutas;
    // Concatenar rutas
    for(int i = 0; i < n_rutas; i++){
        gstRutas[i] = int_vector<>(rutas[i].size());
        for(int j = 0; j < rutas[i].size(); j++){
            iv[pv++] = rutas[i][j];
            gstRutas[i][j] = rutas[i][j];
        }
        iv[pv++] = finSec;
        util::bit_compress(gstRutas[i]);
    }
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
    gstMarcas = vector<sd_vector<>>(n_rutas);
    for(int i = 0; i < n_rutas; i++){
        bit_vector bvt = bit_vector(finSec, 0);
        for(int j = 0; j < rutas[i].size(); j++){
            bvt[rutas[i][j]] = 1;
        }
        gstMarcas[i] = sd_vector<>(bvt);
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
//    cout << "Map... OK" << endl;
}



string TopoRelGST_1::obtenerRelacion(int x, int y){
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
    int lx = gstRutas[x].size();
    int ly = gstRutas[y].size();
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
    // Para obtener (IC o IS) se debe ir reduciendo el tamaño de larga
//    cout << endl << "x: " << x << " - y: " << y << endl;
//    cout << "C id: " << cst.id(C) << " - L id: " << cst.id(L) << endl;
    int tempL = gstRutas[largo].size();
    do{
        // Acortar la secuencia Larga
        L = cst.sl(L);
        tempL--;
        // Calcular LCA con corta
        lcaCL = cst.lca(C, L);
        lcaCrL = cst.lca(Cr, L);
//        cout << "L id: " << cst.id(L) << " - lcaCL id: " << cst.id(lcaCL) << " - lcaCrL id: " << cst.id(lcaCrL) << endl;
//        cout << "if: " << cst.depth(L) << ">" << lC << " && " << cst.depth(lcaCL) << "<" << lC << " && " << cst.depth(lcaCrL) << "<" << lC << endl;
    }while(tempL > lC && cst.depth(lcaCL) < lC && cst.depth(lcaCrL) < lC);

    if(cst.depth(lcaCL) >= lC || cst.depth(lcaCrL) >= lC){
        // Existe contención
        if(corto == x){
            return INSIDE;
        }
        return INCLUDES;
    }
    tempL = gstRutas[largo].size();
    do{
        // Acortar la secuencia LargaReversa
        Lr = cst.sl(Lr);
        tempL--;
        // Calcular LCA con corta
        lcaCLr = cst.lca(C, Lr);
        lcaCrLr = cst.lca(Cr, Lr);
//        cout << "Lr id: " << cst.id(Lr) << " - lcaCLr id: " << cst.id(lcaCLr) << " - lcaCrLr id: " << cst.id(lcaCrLr) << endl;
    }while(tempL > lC && cst.depth(lcaCLr) < lC && cst.depth(lcaCrLr) < lC);

    if(cst.depth(lcaCLr) >= lC || cst.depth(lcaCrLr) >= lC){
        // Existe contención
        if(corto == x){
            return INSIDE;
        }
        return INCLUDES;
    }


    // Verificar si TO es posible
    bool posibleTo = false;
    int bCi = gstRutas[corto][0];
    int bCf = gstRutas[corto][gstRutas[corto].size()-1];
    int bLi = gstRutas[largo][0];
    int bLf = gstRutas[largo][gstRutas[largo].size()-1];
    if(gstMarcas[largo][bCi] == 1 || gstMarcas[largo][bCf] == 1 || gstMarcas[corto][bLi] == 1 || gstMarcas[corto][bLf] == 1){
//        cout << endl << "x: " << x << " - y: " << y << endl;
        // Verficar que la coincidencia de bordes no posee intersección I-I
        if(bordesSeg_touches(x, y)){
            return OVERLAPS;
        }
        posibleTo = true;
    }
    // Verificar OV (intersección interior-interior)
    // Se recorre corto para identificar cualquier coincidencia con largo
    int aux;
//    cout << "lC: " << lC << endl;
    for(int i=2; i<lC; i++){
        aux = gstRutas[corto][i-1];
//        cout << "aux: " << aux << endl;
        if(aux != bLi && aux != bLf && gstMarcas[largo][aux]){
            return OVERLAPS;
        }
    }

    if(posibleTo){
        return TOUCHES;
    }

    return DISJOINT;
}


/*******************************************************
            8 Relaciones topológicas básicas
*******************************************************/


bool TopoRelGST_1::tr_equals(int x, int y){
    if(gstMapa[x] == gstMapa[y] || gstMapa[x] == gstMapa[y+n_rutas]){
        return true;
    }
    return false;
}

bool TopoRelGST_1::tr_coveredby(int x, int y){
    // Descarte por largo de secuencias
    int lx = gstRutas[x].size();
    int ly = gstRutas[y].size();
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
    
    return false;
}

bool TopoRelGST_1::tr_covers(int x, int y){
    return tr_coveredby(y,x);
}

bool TopoRelGST_1::tr_inside(int x, int y){
    // Descarte por largo de secuencias
    int lx = gstRutas[x].size();
    int ly = gstRutas[y].size();
    if(lx >= ly){
        return false;
    }
    // Descarte por CoveredBy
    int l = gstRutas[x].size();
    auto lca1 = cst.lca(gstMapa[x], gstMapa[y]);
    auto lca2 = cst.lca(gstMapa[x], gstMapa[y+n_rutas]);
    auto lca3 = cst.lca(gstMapa[x+n_rutas], gstMapa[y]);
    auto lca4 = cst.lca(gstMapa[x+n_rutas], gstMapa[y+n_rutas]);
    if(cst.depth(lca1) == l || cst.depth(lca2) == l || cst.depth(lca3) == l || cst.depth(lca4) == l){
        return false;
    }
    // Descarte por SegmentosBorde
    if(bordesSeg_touches(x,y)){
        return false;
    }
    // Verifica Inside
    auto L = gstMapa[y];
    auto C = gstMapa[x];
    auto Lr = gstMapa[y+n_rutas];
    auto Cr = gstMapa[x+n_rutas];
    auto lcaCL = cst.root();
    auto lcaCLr = cst.root();
    auto lcaCrL = cst.root();
    auto lcaCrLr = cst.root();

    int tempL = gstRutas[y].size();
    do{
        // Acortar la secuencia Larga
        L = cst.sl(L);
        tempL--;
        // Calcular LCA con corta
        lcaCL = cst.lca(C, L);
        lcaCrL = cst.lca(Cr, L);
    }while(tempL > l && cst.depth(lcaCL) < l && cst.depth(lcaCrL) < l);
    if(cst.depth(lcaCL) >= l || cst.depth(lcaCrL) >= l){
        // Existe contención
        return true;
    }
    tempL = gstRutas[y].size();
    do{
        // Acortar la secuencia LargaReversa
        Lr = cst.sl(Lr);
        tempL--;
        // Calcular LCA con corta
        lcaCLr = cst.lca(C, Lr);
        lcaCrLr = cst.lca(Cr, Lr);
    }while(tempL > l && cst.depth(lcaCLr) < l && cst.depth(lcaCrLr) < l);

    if(cst.depth(lcaCLr) >= l || cst.depth(lcaCrLr) >= l){
        return true;
    }
    return false;
}

bool TopoRelGST_1::tr_includes(int x, int y){
    return tr_inside(y,x);
}

bool TopoRelGST_1::tr_disjoint(int x, int y){
    // Descartar igualdad
    if(gstMapa[x] == gstMapa[y] || gstMapa[x] == gstMapa[y+n_rutas]){
        return false;
    }
    // Descartar contención por CB o CV
    if(tr_coveredby(x,y) || tr_covers(x,y)){
        return false;
    }
    // Descartar posible TO
    int bXi = gstRutas[x][0];
    int bXf = gstRutas[x][gstRutas[x].size()-1];
    int bYi = gstRutas[y][0];
    int bYf = gstRutas[y][gstRutas[y].size()-1];
    if(gstMarcas[y][bXi] == 1 || gstMarcas[y][bXf] == 1 || gstMarcas[x][bYi] == 1 || gstMarcas[x][bYf] == 1){
        // Hay bordes en contacto con la otra secuencia
        return false;
    }
    // Comprobar cualquier intersección
    int lx = gstRutas[x].size();
    int ly = gstRutas[y].size();

    for(int i=2; i<lx; i++){
        if(gstMarcas[y][gstRutas[x][i-1]] == 1){
            // Hay intersección del interior de X con Y
            return false;
        }
    }
    for(int i=2; i<ly; i++){
        if(gstMarcas[x][gstRutas[y][i-1]] == 1){
            // Hay intersección del interior de X con Y
            return false;
        }
    }
    // Hay disjunción
    return true;
}

bool TopoRelGST_1::tr_touches(int x, int y){
    // Descarte por igualdad
    if(gstMapa[x] == gstMapa[y] || gstMapa[x] == gstMapa[y+n_rutas]){
        return false;
    }
    // Verifica candidato por bordes
    int bXi = gstRutas[x][0];
    int bXf = gstRutas[x][gstRutas[x].size()-1];
    int bYi = gstRutas[y][0];
    int bYf = gstRutas[y][gstRutas[y].size()-1];
    if(gstMarcas[y][bXi] != 1 && gstMarcas[y][bXf] != 1 && gstMarcas[x][bYi] != 1 && gstMarcas[x][bYf] != 1){
        // No hay bordes en contacto con la otra secuencia
        return false;
    }
    // Verficar que la coincidencia de bordes no posee intersección I-I
    if(bordesSeg_touches(x, y)){
        return false;
    }
    // Descartar intersección I-I
    // Verificar OV (intersección interior-interior)
    // Se recorre x para identificar cualquier coincidencia con y
    int aux;
    int lx = gstRutas[x].size();
    for(int i=2; i<lx; i++){
        aux = gstRutas[x][i-1];
        if(aux != bYi && aux != bYf && gstMarcas[y][aux]){
            return false;
        }
    }
    return true;
}

bool TopoRelGST_1::tr_overlaps(int x, int y){
    // Descartar igualdad
    if(gstMapa[x] == gstMapa[y] || gstMapa[x] == gstMapa[y+n_rutas]){
        return false;
    }
    // Descartar contención por CB o CV
    if(tr_coveredby(x,y) || tr_covers(x,y)){
        return false;
    }
    // Bordes de las secuencias
    int bXi = gstRutas[x][0];
    int bXf = gstRutas[x][gstRutas[x].size()-1];
    int bYi = gstRutas[y][0];
    int bYf = gstRutas[y][gstRutas[y].size()-1];
    // Comprobar intersección Ix-Iy y Ix-Ey (al menos una de cada una)
    int iXiY = 0;
    int iXeY = 0;
    int lx = gstRutas[x].size();
    int ly = gstRutas[y].size();
    int px = 2;
    int ed;
    if(gstMarcas[y][bXi] == 0 || gstMarcas[y][bXf] == 0){
        // Un borde de X está en el exterior de Y, entonces su arco adyacente también
        // Hay intersección X interior con exterior Y
        iXeY++;
    }
    if(bordesSeg_touches(x,y)){
        // Si los bordes solapan, hay interseción interior-interior
        iXiY++;
    }
    while(px < lx && (iXiY == 0 || iXeY == 0)){
        ed = gstRutas[x][px-1];
        if(gstMarcas[y][ed] == 0){
            // Hay intersección de interior X con exterior Y
            iXeY++;
        }else if(ed != bYi && ed != bYf){
            // No es un borde de Y
            // Hay intersección de interior X con interior Y
            iXiY++;
        }

        px++;
    }
    if(iXiY == 0 || iXeY == 0){
        return false;
    }
    // Sólo queda identificar intersección del interior Y con exterior X
    if(gstMarcas[x][bYi] == 0 || gstMarcas[x][bYf] == 0){
        // Un borde de Y está en el exterior de X, entonces su arco adyacente también
        // Hay intersección Y interior con exterior X
        return true;
    }
    int py = 2;
    while(py < ly){
        ed = gstRutas[y][py-1];
        if(gstMarcas[x][ed] == 0){
            // Hay intersección exterior X con interior Y
            return true;
        }      
        py++;
    }
    return false;
}


/*******************************************************
            Relaciones topológicas agregadas
*******************************************************/


bool TopoRelGST_1::tr_within(int x, int y){
    // Debe ser EQUALS, COVEREDBY o INSIDE
    // Descarte por largo de secuencia
    int lx = gstRutas[x].size();
    int ly = gstRutas[y].size();
    if(lx > ly){
        return false;
    }
    // Comprobando Equals
    if(gstMapa[x] == gstMapa[y] || gstMapa[x] == gstMapa[y+n_rutas]){
        return true;
    }
    // Comprobando COVEREDBY
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
    // Comprobando INSIDE
    auto L = gstMapa[y];
    auto C = gstMapa[x];
    auto Lr = gstMapa[y+n_rutas];
    auto Cr = gstMapa[x+n_rutas];
    auto lcaCL = cst.root();
    auto lcaCLr = cst.root();
    auto lcaCrL = cst.root();
    auto lcaCrLr = cst.root();
    int l = gstRutas[x].size();
    int tempL = gstRutas[y].size();
    do{
        // Acortar la secuencia Larga
        L = cst.sl(L);
        tempL--;
        // Calcular LCA con corta
        lcaCL = cst.lca(C, L);
        lcaCrL = cst.lca(Cr, L);
    }while(tempL > l && cst.depth(lcaCL) < l && cst.depth(lcaCrL) < l);
    if(cst.depth(lcaCL) >= l || cst.depth(lcaCrL) >= l){
        // Existe contención
        return true;
    }

    tempL = gstRutas[y].size();
    do{
        // Acortar la secuencia LargaReversa
        Lr = cst.sl(Lr);
        tempL--;
        // Calcular LCA con corta
        lcaCLr = cst.lca(C, Lr);
        lcaCrLr = cst.lca(Cr, Lr);
    }while(tempL > l && cst.depth(lcaCLr) < l && cst.depth(lcaCrLr) < l);
    if(cst.depth(lcaCLr) >= l || cst.depth(lcaCrLr) >= l){
        return true;
    }
    return false;
}

bool TopoRelGST_1::tr_contains(int x, int y){
    return tr_within(y, x);
}    

bool TopoRelGST_1::tr_intersect(int x, int y){
    return !tr_disjoint(x, y);
}


/*******************************************************
            Otras funcionalidades
*******************************************************/


void TopoRelGST_1::navega(int x){

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
    for(int i=0; i<gstMapa.size(); i++){
        cout << "(" << i << "): " << cst.id(gstMapa[i]) << endl;
    }
    cout << endl;
}

// Funciones private

void TopoRelGST_1::sizeEstructura(){
    cout << "**** Tamaño en bytes ****" << endl;
    cout << "cst_sct3 [B]: " << size_in_bytes(cst) << endl;
    // Calculo de los bytes para RUTAS
    int bytesRutas = 0;
    for(int i=0; i<gstRutas.size(); i++){
        bytesRutas += size_in_bytes(gstRutas[i]);
    }
    cout << "rutas [B]: " << bytesRutas << endl;
    // Calculo de los bytes para MARCAS
    int bytesMarcas = 0;
    int bitsUno = 0;
    int bitsTotal = 0;
    for(int i=0; i<gstMarcas.size(); i++){
        bytesMarcas += size_in_bytes(gstMarcas[i]);
        bitsTotal += gstMarcas[i].size();
        for(int j=0; j<gstMarcas[i].size(); j++){
            bitsUno += gstMarcas[i][j];
        }
    }
    double porcentaje = (bitsUno+0.0)/bitsTotal*100;
    cout << "marcas [B]: " << bytesMarcas << endl;
    // Calculo de los bytes para MAPA
    int bytesMapa = 0;
    for(int i=0; i<gstMapa.size(); i++){
        bytesMapa += sizeof(gstMapa[i]);
    }
    cout << "mapa [B]: " << bytesMapa << endl;

    cout << "**** Elementos ****" << endl;
    cout << "Nº Rutas: " << gstMarcas.size() << endl;
    cout << "Nº Nodos cst_sct3: " << cst.nodes() << endl;
    cout << "Nº Hojas cst_sct3: " << cst.size() << endl;
    cout << "Nº 1s/length en marcas: " << bitsUno << "/" << bitsTotal;
    cout << " (" << porcentaje << "%)" << endl;
}

bool TopoRelGST_1::bordesSeg_touches(int t1, int t2){
    // Comprueba si hay segmentos finales que se intersecan
    // Esto implica una intersección Interior-Interior entre los bordes
    // que se considera como un touches falso-positivo

    int bff = gstRutas[t2].size() - 1;
    int bpf = bff - 1;
    if((gstRutas[t1][0] == gstRutas[t2][1] && gstRutas[t1][1] == gstRutas[t2][0]) ||
            (gstRutas[t1][0] == gstRutas[t2][bpf] && gstRutas[t1][1] == gstRutas[t2][bff])){
        return true;
    }

    int aff = gstRutas[t1].size() - 1;
    int apf = aff - 1;
    if((gstRutas[t1][aff] == gstRutas[t2][1] && gstRutas[t1][apf] == gstRutas[t2][0]) ||
            (gstRutas[t1][aff] == gstRutas[t2][bpf] && gstRutas[t1][apf] == gstRutas[t2][bff])){
        return true;
    }

    return false;
}

cst_sct3<>::node_type TopoRelGST_1::nodoSubseq(cst_sct3<>::node_type n, int x){
    // Retorna el nodo con la subsequencia de largo x desde el nodo n
    auto r = cst.root();
    if(x <= 0 || x > cst.depth(n)){
        return r;
    }
    int d = 0;
    do{
        r = cst.child(r, cst.edge(n, d+1));
        d = cst.depth(r);
    }while(d < x);
    return r;
}