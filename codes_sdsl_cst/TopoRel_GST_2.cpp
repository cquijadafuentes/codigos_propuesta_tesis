/*
    Versión del GST reduciendo las marcas y manteniendo las rutas
    - Todas las rutas
    - Marcas en todos los stops (1er nivel gst)
    - Mapa a las hojas donde terminan las rutas
*/

#include "TopoRel_GST.hpp"

TopoRelGST::TopoRelGST(vector<vector<int>> &rutas, int cant_stops){
    n_stops = cant_stops;
    n_concat = 0;
    n_rutas = rutas.size();
    int maxID = 0;  // Para obtener símbolo para fin de secuencia
    for(int i = 0; i < n_rutas; i++){
        n_concat += (rutas[i].size()+1);   // largo de ruta + fin_char
        // verificar el max_char del final de stops
        for(int j = 0; j < rutas[i].size(); j++){
            if(rutas[i][j] <= 0){
                cout << "Error! identificadores con valor <= 0" << endl;
                return;
            }
            if(maxID < rutas[i][j]){
                maxID = rutas[i][j];
            }
        }
    }
    finSec = maxID+1;
    int_vector<> iv(n_concat*2);
    int pv = 0;
    int tr = n_rutas;
    // Concatenar rutas
    for(int i = 0; i < n_rutas; i++){
        for(int j = 0; j < rutas[i].size(); j++){
            iv[pv++] = rutas[i][j];
        }
        iv[pv++] = finSec;
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
    marcas = vector<bit_vector>(n_rutas, bit_vector(finSec, 0));
    for(int i = 0; i < n_rutas; i++){
        for(int j = 0; j < rutas[i].size(); j++){
            marcas[i][rutas[i][j]] = 1;
        }
    }
//    cout << "Marcas en bitvector... OK" << endl;
/*
    cout << "Marcas: " << endl;
    for(int i=0; i<marcas.size(); i++){
        for(int j=0; j<marcas[i].size(); j++){
            cout << marcas[i][j] << " " ;
        }
        cout << endl;
    }
    cout << endl;
*/
    // MAP en un vector
    mapa = vector<cst_sct3<>::node_type>(n_rutas * 2);
    for(int i = 0; i < n_rutas; i++){
        // La primera mitad de mapa para rutas
        auto v = cst.child(cst.root(), rutas[i][0]);
        while(v != cst.root() && cst.depth(v) < rutas[i].size()){
            v = cst.child(v, rutas[i][cst.depth(v)]);
        }
        if(cst.depth(v) <= rutas[i].size()){
            v = cst.child(v, finSec);
        }
        mapa[i] = v;
        // La segunda mitad de mapa para rutas reversas
        int pr = rutas[i].size()-1;
        v = cst.child(cst.root(), rutas[i][pr]);
        while(v != cst.root() && cst.depth(v) < rutas[i].size()){
            v = cst.child(v, rutas[i][pr-cst.depth(v)]);
        }
        if(cst.depth(v) <= rutas[i].size()){
            v = cst.child(v, finSec);
        }
        mapa[n_rutas + i] = v;
    }
//    cout << "Map... OK" << endl;
}



string TopoRelGST::obtenerRelacion(int x, int y){
//    cout << endl << "x: " << x << " - y: " << y << endl;
//    cout << "id x: " << cst.id(mapa[x]) << "id y: " << cst.id(mapa[y]) << endl;
    // Identificar igualdad en cualquiera de los dos sentidos
    if(mapa[x] == mapa[y]){
        return EQUALS;
    }
    if(mapa[x] == mapa[y+n_rutas]){
        return EQUALS;
    }
    // Identificar contención según nodo
    // Inside, includes, coveredBy, covers
    int corto, largo, lC, lL;
    int lX = largoSec(mapa[x]);
    int lY = largoSec(mapa[y]);
    if(lX < lY){
        corto = x;
        lC = lX;
        largo = y;
        lL = lY;
    }else{
        corto = y;
        lC = lY;
        largo = x;
        lL = lX;
    }

    auto C = mapa[corto];
    auto Cr = mapa[corto+n_rutas];
    auto L = mapa[largo];
    auto Lr = mapa[largo+n_rutas];
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
    do{
        // Acortar la secuencia Larga
        L = cst.sl(L);
        // Calcular LCA con corta
        lcaCL = cst.lca(C, L);
        lcaCrL = cst.lca(Cr, L);
//        cout << "L id: " << cst.id(L) << " - lcaCL id: " << cst.id(lcaCL) << " - lcaCrL id: " << cst.id(lcaCrL) << endl;
//        cout << "if: " << cst.depth(L) << ">" << lC << " && " << cst.depth(lcaCL) << "<" << lC << " && " << cst.depth(lcaCrL) << "<" << lC << endl;
    }while(cst.depth(L) > lC && cst.edge(L,1) != finSec && cst.depth(lcaCL) < lC && cst.depth(lcaCrL) < lC);

    if(cst.depth(lcaCL) >= lC || cst.depth(lcaCrL) >= lC){
        // Existe contención
        if(corto == x){
            return INSIDE;
        }
        return INCLUDES;
    }

    do{
        // Acortar la secuencia LargaReversa
        Lr = cst.sl(Lr);
        // Calcular LCA con corta
        lcaCLr = cst.lca(C, Lr);
        lcaCrLr = cst.lca(Cr, Lr);
//        cout << "Lr id: " << cst.id(Lr) << " - lcaCLr id: " << cst.id(lcaCLr) << " - lcaCrLr id: " << cst.id(lcaCrLr) << endl;
    }while(cst.depth(Lr) > lC && cst.edge(Lr,1) != finSec && cst.depth(lcaCLr) < lC && cst.depth(lcaCrLr) < lC);

    if(cst.depth(lcaCLr) >= lC || cst.depth(lcaCrLr) >= lC){
        // Existe contención
        if(corto == x){
            return INSIDE;
        }
        return INCLUDES;
    }


    // Verificar si TO es posible
    bool posibleTo = false;
    int bCi = cst.edge(mapa[corto], 1);
    int bCf = cst.edge(mapa[corto+n_rutas], 1);
    int bLi = cst.edge(mapa[largo], 1);
    int bLf = cst.edge(mapa[largo+n_rutas], 1);
    if(marcas[largo][bCi] == 1 || marcas[largo][bCf] == 1 || marcas[corto][bLi] == 1 || marcas[corto][bLf] == 1){
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
        aux = cst.edge(mapa[corto], i);
//        cout << "aux: " << aux << endl;
        if(aux != bLi && aux != bLf && marcas[largo][aux]){
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


bool TopoRelGST::tr_equals(int x, int y){
    if(mapa[x] == mapa[y] || mapa[x] == mapa[y+n_rutas]){
        return true;
    }
    return false;
}

bool TopoRelGST::tr_coveredby(int x, int y){
    // Descarte por largo de secuencias
    int lx = largoSec(mapa[x]);
    int ly = largoSec(mapa[y]);
    if(lx >= ly){
        return false;
    }
    // Verifica CoveredBy
    auto lca1 = cst.lca(mapa[x], mapa[y]);
    auto lca2 = cst.lca(mapa[x], mapa[y+n_rutas]);
    auto lca3 = cst.lca(mapa[x+n_rutas], mapa[y]);
    auto lca4 = cst.lca(mapa[x+n_rutas], mapa[y+n_rutas]);
    if(cst.depth(lca1) == lx || cst.depth(lca2) == lx || cst.depth(lca3) == lx || cst.depth(lca4) == lx){
        return true;
    }
    return false;
}

bool TopoRelGST::tr_covers(int x, int y){
    return tr_coveredby(y,x);
}

bool TopoRelGST::tr_inside(int x, int y){
    // Descarte por largo de secuencias
    int lx = largoSec(mapa[x]);
    int ly = largoSec(mapa[y]);
    if(lx >= ly){
        return false;
    }
    // Descarte por CoveredBy
    int l = largoSec(mapa[x]);
    auto lca1 = cst.lca(mapa[x], mapa[y]);
    auto lca2 = cst.lca(mapa[x], mapa[y+n_rutas]);
    auto lca3 = cst.lca(mapa[x+n_rutas], mapa[y]);
    auto lca4 = cst.lca(mapa[x+n_rutas], mapa[y+n_rutas]);
    if(cst.depth(lca1) == l || cst.depth(lca2) == l || cst.depth(lca3) == l || cst.depth(lca4) == l){
        return false;
    }
    // Descarte por SegmentosBorde
    if(bordesSeg_touches(x,y)){
        return false;
    }
    // Verifica Inside
    auto L = mapa[y];
    auto C = mapa[x];
    auto Lr = mapa[y+n_rutas];
    auto Cr = mapa[x+n_rutas];
    auto lcaCL = cst.root();
    auto lcaCLr = cst.root();
    auto lcaCrL = cst.root();
    auto lcaCrLr = cst.root();

    do{
        // Acortar la secuencia Larga
        L = cst.sl(L);
        // Calcular LCA con corta
        lcaCL = cst.lca(C, L);
        lcaCrL = cst.lca(Cr, L);
    }while(cst.depth(L) > l && cst.edge(L,1) != finSec && cst.depth(lcaCL) < l && cst.depth(lcaCrL) < l);
    if(cst.depth(lcaCL) >= l || cst.depth(lcaCrL) >= l){
        // Existe contención
        return true;
    }
    do{
        // Acortar la secuencia LargaReversa
        Lr = cst.sl(Lr);
        // Calcular LCA con corta
        lcaCLr = cst.lca(C, Lr);
        lcaCrLr = cst.lca(Cr, Lr);
    }while(cst.depth(Lr) > l && cst.edge(Lr,1) != finSec && cst.depth(lcaCLr) < l && cst.depth(lcaCrLr) < l);

    if(cst.depth(lcaCLr) >= l || cst.depth(lcaCrLr) >= l){
        return true;
    }
    return false;
}

bool TopoRelGST::tr_includes(int x, int y){
    return tr_inside(y,x);
}

bool TopoRelGST::tr_disjoint(int x, int y){
    // Descartar igualdad
    if(mapa[x] == mapa[y] || mapa[x] == mapa[y+n_rutas]){
        return false;
    }
    // Descartar contención por CB o CV
    if(tr_coveredby(x,y) || tr_covers(x,y)){
        return false;
    }
    // Descartar posible TO
    int bXi = cst.edge(mapa[x], 1);
    int bXf = cst.edge(mapa[x+n_rutas], 1);
    int bYi = cst.edge(mapa[y], 1);
    int bYf = cst.edge(mapa[y+n_rutas], 1);
    if(marcas[y][bXi] == 1 || marcas[y][bXf] == 1 || marcas[x][bYi] == 1 || marcas[x][bYf] == 1){
        // Hay bordes en contacto con la otra secuencia
        return false;
    }
    // Comprobar cualquier intersección
    int lx = largoSec(mapa[x]);
    for(int i=2; i<lx; i++){
        if(marcas[y][cst.edge(mapa[x], i)] == 1){
            // Hay intersección del interior de X con Y
            return false;
        }
    }
    int ly = largoSec(mapa[y]);
    for(int i=2; i<ly; i++){
        if(marcas[x][cst.edge(mapa[y], i)] == 1){
            // Hay intersección del interior de X con Y
            return false;
        }
    }
    // Hay disjunción
    return true;
}

bool TopoRelGST::tr_touches(int x, int y){
    // Descarte por igualdad
    if(mapa[x] == mapa[y] || mapa[x] == mapa[y+n_rutas]){
        return false;
    }
    // Verifica candidato por bordes
    int bXi = cst.edge(mapa[x], 1);
    int bXf = cst.edge(mapa[x+n_rutas], 1);
    int bYi = cst.edge(mapa[y], 1);
    int bYf = cst.edge(mapa[y+n_rutas], 1);
    if(marcas[y][bXi] != 1 && marcas[y][bXf] != 1 && marcas[x][bYi] != 1 && marcas[x][bYf] != 1){
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
    int lx = largoSec(mapa[x]);
    for(int i=2; i<lx; i++){
        aux = cst.edge(mapa[x], i);
        if(aux != bYi && aux != bYf && marcas[y][aux]){
            return false;
        }
    }
    return true;
}

bool TopoRelGST::tr_overlaps(int x, int y){
    // Descartar igualdad
    if(mapa[x] == mapa[y] || mapa[x] == mapa[y+n_rutas]){
        return false;
    }
    // Descartar contención por CB o CV
    if(tr_coveredby(x,y) || tr_covers(x,y)){
        return false;
    }
    // Bordes de las secuencias
    int bYi = cst.edge(mapa[y], 1);
    int bYf = cst.edge(mapa[y+n_rutas], 1);
    int bXi = cst.edge(mapa[x], 1);
    int bXf = cst.edge(mapa[x+n_rutas], 1);
    // Comprobar intersección Ix-Iy y Ix-Ey (al menos una de cada una)
    int iXiY = 0;
    int iXeY = 0;
    int lx = largoSec(mapa[x]);
    int px = 2;
    int ed;
    if(marcas[y][bXi] == 0 || marcas[y][bXf] == 0){
        // Un borde de X está en el exterior de Y, entonces su arco adyacente también
        // Hay intersección X interior con exterior Y
        iXeY++;
    }
    if(bordesSeg_touches(x,y)){
        // Si los bordes solapan, hay interseción interior-interior
        iXiY++;
    }
    while(px < lx && (iXiY == 0 || iXeY == 0)){
        ed = cst.edge(mapa[x], px);
        if(marcas[y][ed] == 0){
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
    if(marcas[x][bYi] == 0 || marcas[x][bYf] == 0){
        // Un borde de Y está en el exterior de X, entonces su arco adyacente también
        // Hay intersección Y interior con exterior X
        return true;
    }
    int ly = largoSec(mapa[y]);
    int py = 2;
    while(py < ly){
        ed = cst.edge(mapa[y], py);
        if(marcas[x][ed] == 0){
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


bool TopoRelGST::tr_within(int x, int y){
    // Debe ser EQUALS, COVEREDBY o INSIDE
    // Descarte por largo de secuencia
    int lx = largoSec(mapa[x]);
    int ly = largoSec(mapa[y]);
    if(lx > ly){
        return false;
    }
    // Comprobando Equals
    if(mapa[x] == mapa[y] || mapa[x] == mapa[y+n_rutas]){
        return true;
    }
    // Comprobando COVEREDBY
    auto lca1 = cst.lca(mapa[x], mapa[y]);
    auto lca2 = cst.lca(mapa[x], mapa[y+n_rutas]);
    auto lca3 = cst.lca(mapa[x+n_rutas], mapa[y]);
    auto lca4 = cst.lca(mapa[x+n_rutas], mapa[y+n_rutas]);
    if(cst.depth(lca1) == lx || cst.depth(lca2) == lx || cst.depth(lca3) == lx || cst.depth(lca4) == lx){
        return true;
    }
    // Comprobando INSIDE
    auto L = mapa[y];
    auto C = mapa[x];
    auto Lr = mapa[y+n_rutas];
    auto Cr = mapa[x+n_rutas];
    auto lcaCL = cst.root();
    auto lcaCLr = cst.root();
    auto lcaCrL = cst.root();
    auto lcaCrLr = cst.root();
    int l = largoSec(mapa[x]);
    do{
        // Acortar la secuencia Larga
        L = cst.sl(L);
        // Calcular LCA con corta
        lcaCL = cst.lca(C, L);
        lcaCrL = cst.lca(Cr, L);
    }while(cst.depth(L) > l && cst.edge(L,1) != finSec && cst.depth(lcaCL) < l && cst.depth(lcaCrL) < l);
    if(cst.depth(lcaCL) >= l || cst.depth(lcaCrL) >= l){
        // Existe contención
        return true;
    }

    do{
        // Acortar la secuencia LargaReversa
        Lr = cst.sl(Lr);
        // Calcular LCA con corta
        lcaCLr = cst.lca(C, Lr);
        lcaCrLr = cst.lca(Cr, Lr);
    }while(cst.depth(Lr) > l && cst.edge(Lr,1) != finSec && cst.depth(lcaCLr) < l && cst.depth(lcaCrLr) < l);
    if(cst.depth(lcaCLr) >= l || cst.depth(lcaCrLr) >= l){
        return true;
    }
    return false;
}

bool TopoRelGST::tr_contains(int x, int y){
    return tr_within(y, x);
}

bool TopoRelGST::tr_intersects(int x, int y){
    // Descartes en tiempo constante
    // Igualdad
    if(mapa[x] == mapa[y] || mapa[x] == mapa[y+n_rutas]){
        return true;
    }
    // Bordes
    int bXi = cst.edge(mapa[x], 1);
    int bXf = cst.edge(mapa[x+n_rutas], 1);
    int bYi = cst.edge(mapa[y], 1);
    int bYf = cst.edge(mapa[y+n_rutas], 1);
    if(marcas[y][bXi] == 1 || marcas[y][bXf] == 1 || marcas[x][bYi] == 1 || marcas[x][bYf] == 1){
        // Hay bordes en contacto con la otra secuencia
        return true;
    }
    // Verficar segmentos de los bordes
    if(bordesSeg_touches(x, y)){
        return true;
    }
    int lx = largoSec(mapa[x]);
    int ly = largoSec(mapa[y]);
    // Verificar COVEREDBY
    auto lca1 = cst.lca(mapa[x], mapa[y]);
    auto lca2 = cst.lca(mapa[x], mapa[y+n_rutas]);
    auto lca3 = cst.lca(mapa[x+n_rutas], mapa[y]);
    auto lca4 = cst.lca(mapa[x+n_rutas], mapa[y+n_rutas]);
    if(cst.depth(lca1) == lx || cst.depth(lca2) == lx || cst.depth(lca3) == lx || cst.depth(lca4) == lx){
        return true;
    }
    if(cst.depth(lca1) == ly || cst.depth(lca2) == ly || cst.depth(lca3) == ly || cst.depth(lca4) == ly){
        return true;
    }
    // Comprobación en lineal
    int lC, lL;
    auto corto = mapa[x];
    auto largo = mapa[y];
    int iC, iL;
    if(lx < ly){
        lC = lx;
        iC = x;
        lL = ly;
        iL = y;
    }else{
        corto = mapa [y];
        lC = ly;
        iC = y;
        largo = mapa[x];
        lL = lx;
        iL = x;
    }
    int aux;
    for(int i=1; i<=lC; i++){
        aux = cst.edge(corto, i);
        if(marcas[iL][aux] == 1){
            return true;
        }
    }
    return false;
}


/*******************************************************
            Otras funcionalidades
*******************************************************/


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

// Funciones private

void TopoRelGST::sizeEstructura(){
    cout << "**** Tamaño en bytes ****" << endl;
    cout << "cst_sct3 [B]: " << size_in_bytes(cst) << endl;
    // Calculo de los bytes para marcas
    int bytesMarcas = 0;
    int bitsUno = 0;
    int bitsTotal = 0;
    for(int i=0; i<marcas.size(); i++){
        bytesMarcas += size_in_bytes(marcas[i]);
        bitsTotal += marcas[i].size();
        for(int j=0; j<marcas[i].size(); j++){
            bitsUno += marcas[i][j];
        }
    }
    cout << "marcas [B]: " << bytesMarcas << endl;
    // Calculo de los bytes para mapa
    int bytesMapa = 0;
    for(int i=0; i<mapa.size(); i++){
        bytesMapa += sizeof(mapa[i]);
    }
    cout << "mapa [B]: " << bytesMapa << endl;

    cout << "**** Elementos ****" << endl;
    cout << "Nº Rutas: " << marcas.size() << endl;
    cout << "Nº Nodos cst_sct3: " << cst.nodes() << endl;
    cout << "Nº Hojas cst_sct3: " << cst.size() << endl;
    cout << "Nº 1s/length en marcas: " << bitsUno << "/" << bitsTotal << endl;

}

bool TopoRelGST::bordesSeg_touches(int t1, int t2){
    auto t1ab = nodoXSeqInicial(cst.edge(mapa[t1], 1), cst.edge(mapa[t1], 2));
    auto t1ba = nodoXSeqInicial(cst.edge(mapa[t1], 2), cst.edge(mapa[t1], 1));
    auto t1zy = nodoXSeqInicial(cst.edge(mapa[t1+n_rutas], 1), cst.edge(mapa[t1+n_rutas], 2));
    auto t1yz = nodoXSeqInicial(cst.edge(mapa[t1+n_rutas], 2), cst.edge(mapa[t1+n_rutas], 1));
    auto t2ab = nodoXSeqInicial(cst.edge(mapa[t2], 1), cst.edge(mapa[t2], 2));
    auto t2ba = nodoXSeqInicial(cst.edge(mapa[t2], 2), cst.edge(mapa[t2], 1));
    auto t2zy = nodoXSeqInicial(cst.edge(mapa[t2+n_rutas], 1), cst.edge(mapa[t2+n_rutas], 2));
    auto t2yz = nodoXSeqInicial(cst.edge(mapa[t2+n_rutas], 2), cst.edge(mapa[t2+n_rutas], 1));
    bool r1 = t1ab == t2ab || t1ab == t2ba || t1ab == t2yz || t1ab == t2yz;
    bool r2 = t1ba == t2ab || t1ba == t2ba || t1ba == t2yz || t1ba == t2yz;
    bool r3 = t1yz == t2ab || t1yz == t2ba || t1yz == t2yz || t1yz == t2yz;
    bool r4 = t1zy == t2ab || t1zy == t2ba || t1zy == t2yz || t1zy == t2yz;
//    cout << "r1: " << r1 << " - r2: " << r2 << " - r3: " << r3 << " - r4: " << r4 << endl;
    return (r1 || r2 || r3 || r4);
}

int TopoRelGST::largoSec(cst_sct3<>::node_type n){
    if(n == cst.root()){
        return 0;
    }
    auto pn = cst.parent(n);
    int x = cst.depth(pn);
    while(cst.edge(n, x+1) != finSec){
        x++;
    }
    return x;
}

cst_sct3<>::node_type TopoRelGST::nodoSubseq(cst_sct3<>::node_type n, int x){
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

cst_sct3<>::node_type TopoRelGST::nodoXSeqInicial(int a, int b){
    auto r = cst.root();
    r = cst.child(r, a);
    if(cst.depth(r) == 1){
        r = cst.child(r, b);
    }else if(cst.depth(r) > 1){
        if(cst.edge(r, 2) != b){
            r = cst.root();
        }
    }
    return r;
}