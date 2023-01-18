/*
    Versión del GST con todas las ideas iniciales disponibles:
    - GST
    - Vector con las rutas
    - Marcas en stops (primer nivel del GST) sd_vector
    - Mapa a las hojas donde terminan las rutas
*/

#include "TopoRel_GST_4.hpp"

TopoRelGST_4::TopoRelGST_4(vector<vector<int>> &rutas, int cant_stops){
    cout << "Constructor TopoRelGST_4" << endl;
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
    bit_vector MFStemporal = bit_vector(n_concat*2, 0);
    int pv = 0;
    int tr = n_rutas;
    // Concatenar rutas
    for(int i = 0; i < n_rutas; i++){
        gstRutas[i] = int_vector<>(rutas[i].size());
        for(int j = 0; j < rutas[i].size(); j++){
            iv[pv++] = rutas[i][j];
            gstRutas[i][j] = rutas[i][j];
        }
        MFStemporal[pv] = 1;
        iv[pv++] = finSec;
        util::bit_compress(gstRutas[i]);
    }

    // Concatenar rutas reversas
    for(int i = 0; i < n_rutas; i++){
        for(int j = rutas[i].size()-1; j >= 0 ; j--){
            iv[pv++] = rutas[i][j];
        }
        MFStemporal[pv] = 1;
        iv[pv++] = finSec;
    }
    gstMFSbv = sd_vector<>(MFStemporal);
    gstMFSrank = sd_vector<>::rank_1_type(&gstMFSbv);
    gstMFSselect = sd_vector<>::select_1_type(&gstMFSbv);

//    cout << "Rutas... concatenadas" << endl;

    //cst_sada<csa_wt<wt_int<rrr_vector<>>>> cst;
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
    int sizePrevCompr = 0;
    int sizePostCompr = 0;    
    gstStops = vector<sd_vector<>>(n_rutas);
    for(int i = 0; i < n_rutas; i++){
        bit_vector bvt = bit_vector(finSec, 0);
        for(int j = 0; j < rutas[i].size(); j++){
            bvt[rutas[i][j]] = 1;
        }
        sizePrevCompr += size_in_bytes(bvt);
        gstStops[i] = sd_vector<>(bvt);
        sizePostCompr += size_in_bytes(gstStops[i]);
    }
    cout << "Tamaño stops: " << sizePrevCompr << " >> " << sizePostCompr << endl;
//    cout << "Marcas en bitvector... OK" << endl;
/*
    cout << "Marcas: " << endl;
    for(int i=0; i<gstStops.size(); i++){
        for(int j=0; j<gstStops[i].size(); j++){
            cout << gstStops[i][j] << " " ;
        }
        cout << endl;
    }
    cout << endl;
*/
    // MAP en un vector
    gstMapa = vector<cst_sada<>::node_type>(n_rutas * 2);
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
    cout << "Fin constructor TopDown (Completo)" << endl;
}


// Versión de constructor Top-down utilizando paralelismo
TopoRelGST_4::TopoRelGST_4(vector<vector<int>> &rutas, int cant_stops, bool bb){
    cout << "Constructor TopoRelGST_4" << endl;
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
    bit_vector MFStemporal = bit_vector(n_concat*2, 0);
    int pv = 0;
    int tr = n_rutas;
    // Concatenar rutas
    for(int i = 0; i < n_rutas; i++){
        gstRutas[i] = int_vector<>(rutas[i].size());
        for(int j = 0; j < rutas[i].size(); j++){
            iv[pv++] = rutas[i][j];
            gstRutas[i][j] = rutas[i][j];
        }
        MFStemporal[pv] = 1;
        iv[pv++] = finSec;
        util::bit_compress(gstRutas[i]);
    }

    // Concatenar rutas reversas
    for(int i = 0; i < n_rutas; i++){
        for(int j = rutas[i].size()-1; j >= 0 ; j--){
            iv[pv++] = rutas[i][j];
        }
        MFStemporal[pv] = 1;
        iv[pv++] = finSec;
    }
    gstMFSbv = sd_vector<>(MFStemporal);
    gstMFSrank = sd_vector<>::rank_1_type(&gstMFSbv);
    gstMFSselect = sd_vector<>::select_1_type(&gstMFSbv);

//    cout << "Rutas... concatenadas" << endl;

    //cst_sada<csa_wt<wt_int<rrr_vector<>>>> cst;
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
    int sizePrevCompr = 0;
    int sizePostCompr = 0;    
    gstStops = vector<sd_vector<>>(n_rutas);
    for(int i = 0; i < n_rutas; i++){
        bit_vector bvt = bit_vector(finSec, 0);
        for(int j = 0; j < rutas[i].size(); j++){
            bvt[rutas[i][j]] = 1;
        }
        sizePrevCompr += size_in_bytes(bvt);
        gstStops[i] = sd_vector<>(bvt);
        sizePostCompr += size_in_bytes(gstStops[i]);
    }
    cout << "Tamaño stops: " << sizePrevCompr << " >> " << sizePostCompr << endl;
//    cout << "Marcas en bitvector... OK" << endl;
/*
    cout << "Marcas: " << endl;
    for(int i=0; i<gstStops.size(); i++){
        for(int j=0; j<gstStops[i].size(); j++){
            cout << gstStops[i][j] << " " ;
        }
        cout << endl;
    }
    cout << endl;
*/
    // MAP en un vector
    int maxThreads = omp_get_max_threads();
    if(maxThreads > (n_rutas * 2)){
        maxThreads = n_rutas * 2;
    }
    // Vector que contendrá los vectores de cada porción que se genere según la cantidad de threads
    int porciones = (n_rutas * 2) / maxThreads;
    if((porciones * maxThreads) < (n_rutas * 2)){
        porciones++;
    }
    vector<vector<cst_sada<>::node_type>> vPorcMap(maxThreads);

    #pragma omp parallel for
    for(int iParalelo = 0; iParalelo < maxThreads; iParalelo++){
        vPorcMap[iParalelo] = vector<cst_sada<>::node_type>(porciones);
        int base = iParalelo * porciones;
        int tope = base + porciones;
        if(tope > n_rutas * 2){
            tope = n_rutas * 2;
        }
        for(int i = base; i < tope; i++){
            auto v = cst.root();
            if(i < n_rutas){
                // La primera mitad de mapa para rutas
                v = cst.child(cst.root(), rutas[i%n_rutas][0]);
                while(v != cst.root() && cst.depth(v) < rutas[i%n_rutas].size()){
                    v = cst.child(v, rutas[i%n_rutas][cst.depth(v)]);
                }
                if(cst.depth(v) <= rutas[i%n_rutas].size()){
                    v = cst.child(v, finSec);
                }
            }else{
                // La segunda mitad de mapa para rutas reversas
                int pr = rutas[i%n_rutas].size()-1;
                v = cst.child(cst.root(), rutas[i%n_rutas][pr]);
                while(v != cst.root() && cst.depth(v) < rutas[i%n_rutas].size()){
                    v = cst.child(v, rutas[i%n_rutas][pr-cst.depth(v)]);
                }
                if(cst.depth(v) <= rutas[i%n_rutas].size()){
                    v = cst.child(v, finSec);
                }
            }
            vPorcMap[iParalelo][i-base] = v;
            /*
            string st = "ruta: ";
            st += to_string(i);
            st += " id_nodo: ";
            st += to_string((int)cst.id(v));
            st += "\n";
            cout << st;
            */
        }
    }

    // Unir resultados obtenidos en la paralelización en el objeto correspondiente
    int sobrantes = (porciones * maxThreads) - (n_rutas * 2);
    for(int i=0; i < maxThreads; i++){
        gstMapa.insert(gstMapa.end(), vPorcMap[i].begin(), vPorcMap[i].end());
    }
    if(sobrantes > 0){
        gstMapa.erase(gstMapa.end() - sobrantes, gstMapa.end());
    }
//    cout << "Map... OK" << endl;
    cout << "Fin constructor TopDown paralelo (Incompleto)" << endl;
}


TopoRelGST_4::TopoRelGST_4(vector<vector<int>> &rutas, int cant_stops, int x){
    cout << "Constructor TopoRelGST_4 (versión recorrido CSA)" << endl;
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
    bit_vector MFStemporal = bit_vector(n_concat*2, 0);
    int pv = 0;
    int tr = n_rutas;
    // Concatenar rutas
    for(int i = 0; i < n_rutas; i++){
        gstRutas[i] = int_vector<>(rutas[i].size());
        for(int j = 0; j < rutas[i].size(); j++){
            iv[pv++] = rutas[i][j];
            gstRutas[i][j] = rutas[i][j];
        }
        MFStemporal[pv] = 1;
        iv[pv++] = finSec;
        util::bit_compress(gstRutas[i]);
    }

    // Concatenar rutas reversas
    for(int i = 0; i < n_rutas; i++){
        for(int j = rutas[i].size()-1; j >= 0 ; j--){
            iv[pv++] = rutas[i][j];
        }
        MFStemporal[pv] = 1;
        iv[pv++] = finSec;
    }
    gstMFSbv = sd_vector<>(MFStemporal);
    gstMFSrank = sd_vector<>::rank_1_type(&gstMFSbv);
    gstMFSselect = sd_vector<>::select_1_type(&gstMFSbv);

//    cout << "Rutas... concatenadas" << endl;

    //cst_sada<csa_wt<wt_int<rrr_vector<>>>> cst;
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
    int sizePrevCompr = 0;
    int sizePostCompr = 0;    
    gstStops = vector<sd_vector<>>(n_rutas);
    for(int i = 0; i < n_rutas; i++){
        bit_vector bvt = bit_vector(finSec, 0);
        for(int j = 0; j < rutas[i].size(); j++){
            bvt[rutas[i][j]] = 1;
        }
        sizePrevCompr += size_in_bytes(bvt);
        gstStops[i] = sd_vector<>(bvt);
        sizePostCompr += size_in_bytes(gstStops[i]);
    }
    cout << "Tamaño stops: " << sizePrevCompr << " >> " << sizePostCompr << endl;

//    cout << "nueva construcción..." << endl;
    gstMapa = vector<cst_sada<>::node_type>(n_rutas * 2);
    // Generar rutas para recorrer
    // 1- Identificar casillas del CSA que coinciden con un inicio de ruta.
    for(int i=1; i<cst.csa.size(); i++){
        int valCSA = cst.csa[i];
        auto nodo = cst.root();
        int rankV = 0;
        if(valCSA == 0){
            // Apunta a primera ruta
            nodo = cst.inv_id(i);
//            cout << "ruta 0: " << extract(cst, nodo) << endl;
        }else{
            rankV = gstMFSrank(valCSA);
            if(rankV > gstMFSrank(valCSA-1)){
                nodo = cst.inv_id(i);
//                cout << "ruta " << rankV << ": " << extract(cst, nodo) << endl;  
            }
        }
        if(nodo != cst.root()){
            int sizeR = rutas[rankV % n_rutas].size();
//            cout << "Buscando nodo de ruta " << rankV << " tamaño " << sizeR << ": ";
            auto nodoAux = cst.parent(nodo);
//            cout << "id:" << cst.id(nodo) << "-padre:" << cst.id(nodoAux) << " ";
            while(cst.depth(nodoAux) > sizeR){
                nodo = nodoAux;
                nodoAux = cst.parent(nodo);
//                cout << "id:" << cst.id(nodo) << "-padre:" << cst.id(nodoAux) << " ";
            }
//            cout << " -> " << extract(cst, nodo) << endl;
            gstMapa[rankV] = nodo;
        }
    }

    cout << "Fin constructor BottomUp (Completo)" << endl;
}


string TopoRelGST_4::obtenerRelacion(int x, int y){
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
    if(gstStops[largo][bCi] == 1 || gstStops[largo][bCf] == 1 || gstStops[corto][bLi] == 1 || gstStops[corto][bLf] == 1){
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
        if(aux != bLi && aux != bLf && gstStops[largo][aux]){
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


bool TopoRelGST_4::tr_equals(int x, int y){
    if(gstMapa[x] == gstMapa[y] || gstMapa[x] == gstMapa[y+n_rutas]){
        return true;
    }
    return false;
}

bool TopoRelGST_4::tr_coveredby(int x, int y){
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

bool TopoRelGST_4::tr_covers(int x, int y){
    return tr_coveredby(y,x);
}

bool TopoRelGST_4::tr_inside(int x, int y){
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

bool TopoRelGST_4::tr_includes(int x, int y){
    return tr_inside(y,x);
}

bool TopoRelGST_4::tr_disjoint(int x, int y){
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
    if(gstStops[y][bXi] == 1 || gstStops[y][bXf] == 1 || gstStops[x][bYi] == 1 || gstStops[x][bYf] == 1){
        // Hay bordes en contacto con la otra secuencia
        return false;
    }
    // Comprobar cualquier intersección
    int lx = gstRutas[x].size();
    int ly = gstRutas[y].size();

    for(int i=2; i<lx; i++){
        if(gstStops[y][gstRutas[x][i-1]] == 1){
            // Hay intersección del interior de X con Y
            return false;
        }
    }
    for(int i=2; i<ly; i++){
        if(gstStops[x][gstRutas[y][i-1]] == 1){
            // Hay intersección del interior de X con Y
            return false;
        }
    }
    // Hay disjunción
    return true;
}

bool TopoRelGST_4::tr_touches(int x, int y){
    // Descarte por igualdad
    if(gstMapa[x] == gstMapa[y] || gstMapa[x] == gstMapa[y+n_rutas]){
        return false;
    }
    // Verifica candidato por bordes
    int bXi = gstRutas[x][0];
    int bXf = gstRutas[x][gstRutas[x].size()-1];
    int bYi = gstRutas[y][0];
    int bYf = gstRutas[y][gstRutas[y].size()-1];
    if(gstStops[y][bXi] != 1 && gstStops[y][bXf] != 1 && gstStops[x][bYi] != 1 && gstStops[x][bYf] != 1){
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
        if(aux != bYi && aux != bYf && gstStops[y][aux]){
            return false;
        }
    }
    return true;
}

bool TopoRelGST_4::tr_overlaps(int x, int y){
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
    if(gstStops[y][bXi] == 0 || gstStops[y][bXf] == 0){
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
        if(gstStops[y][ed] == 0){
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
    if(gstStops[x][bYi] == 0 || gstStops[x][bYf] == 0){
        // Un borde de Y está en el exterior de X, entonces su arco adyacente también
        // Hay intersección Y interior con exterior X
        return true;
    }
    int py = 2;
    while(py < ly){
        ed = gstRutas[y][py-1];
        if(gstStops[x][ed] == 0){
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


bool TopoRelGST_4::tr_within(int x, int y){
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
    auto lca1 = cst.lca(gstMapa[x], gstMapa[y]);
    auto lca2 = cst.lca(gstMapa[x], gstMapa[y+n_rutas]);
    auto lca3 = cst.lca(gstMapa[x+n_rutas], gstMapa[y]);
    auto lca4 = cst.lca(gstMapa[x+n_rutas], gstMapa[y+n_rutas]);
    if(cst.depth(lca1) == lx || cst.depth(lca2) == lx || cst.depth(lca3) == lx || cst.depth(lca4) == lx){
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

bool TopoRelGST_4::tr_contains(int x, int y){
    return tr_within(y, x);
}    

bool TopoRelGST_4::tr_intersect(int x, int y){
    return !tr_disjoint(x, y);
}


/*******************************************************
            Relaciones topológicas en conjunto
*******************************************************/


vector<int> TopoRelGST_4::tr_allContain(int x){
    vector<int> y;
    if(x > n_rutas) return y;

    auto nX = gstMapa[x];
    auto idLChST = cst.leftmost_leaf(nX);
    auto idRChST = cst.rightmost_leaf(nX);
    auto nParentX = cst.parent(nX);
//    cout << "id_gst_x: " << cst.id(nX) << " - " << extract(cst, nX) << endl;
//    cout << "id_gst_parent_x: " << cst.id(nParentX) << " - " << extract(cst, nParentX) << endl;
//    cout << "Ruta [" << x << "] = ";
//    printRuta(x);
//    cout << endl;

    if(cst.depth(nParentX) == getLargoRuta(x)){
//        cout << "Trabajando con padre" << endl;
        idLChST = cst.leftmost_leaf(nParentX);
        idRChST = cst.rightmost_leaf(nParentX);
    }
//    else{
//        cout << "Trabajando con nodo" << endl;        
//    }
    for(int i=cst.id(idLChST); i <= cst.id(idRChST); i++){
        auto hoja = cst.inv_id(i);
//        cout << "-----" << endl;
//        cout << "id_GST: " << i << " - " << extract(cst, hoja) << endl;;
//        cout << "p_CSA: " << cst.csa[i] << " - ";
        int idAux = getIdRutaSegunPosConcat(cst.csa[i]);
//        cout << "id_Rutas: " << idAux << " - Ruta: ";
//        printRuta(idAux);
//        cout << endl;
//        cout << "-----" << endl;
        y.push_back(idAux);
    }
    return y;
}


/*******************************************************
            Otras funcionalidades
*******************************************************/


void TopoRelGST_4::navega(int x){

    cout << "Información del CompressedSuffixTree:" << endl;
    cout << "Cantidad de nodos: " << cst.nodes() << endl;
    cout << "Cantidad de hojas: " << cst.size() << endl;

    cout << "Recorrido de hijos de root del CompressedSuffixTree:" << endl;
    auto root = cst.root();
    cout << "id\ted_1\tdeg\tdep\tndep\tsize\tlb\trb\tsun\tleaf\ttext" << endl;
    for (auto child: cst.children(root)) {
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
    typedef cst_bfs_iterator<cst_sada<csa_wt<wt_int<rrr_vector<>>>>> iterator;
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
    cout << cst.csa << endl << endl;

    cout << "Marcas de Fin de Secuencia del CompressedSuffixTree: " << endl;
    for(int i=0; i < gstMFSbv.size(); i++){
        cout << gstMFSbv[i];
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

    cout << endl << "Id Secuencia de la posición 2 en concatenación: ";
    int idd = getIdRutaSegunPosConcat(2);
    cout << idd << " - ";
    printRutaYPos(idd);
    cout << endl << endl;

    cout << endl << "Id Secuencia de la posición 18 en concatenación: ";
    idd = getIdRutaSegunPosConcat(18);
    cout << idd << " - ";
    printRutaYPos(idd);
    cout << endl << endl;
}

void TopoRelGST_4::sizeEstructura(){
    cout << "**** Tamaño en bytes ****" << endl;
    cout << "cst_sada [B]: " << size_in_bytes(cst) << endl;
    // Calculo de los bytes para RUTAS
    unsigned long long bytesRutas = 0;
    for(int i=0; i<gstRutas.size(); i++){
        bytesRutas += size_in_bytes(gstRutas[i]);
    }
    cout << "rutas [B]: " << bytesRutas << endl;
    // Calculo de los bytes para STOPS
    unsigned long long bytesStops = 0;
    int bitsUno = 0;
    int bitsTotal = 0;
    for(int i=0; i<gstStops.size(); i++){
        bytesStops += size_in_bytes(gstStops[i]);
        bitsTotal += gstStops[i].size();
        for(int j=0; j<gstStops[i].size(); j++){
            bitsUno += gstStops[i][j];
        }
    }
    double porcentaje = (bitsUno+0.0)/bitsTotal*100;
    cout << "stops [B]: " << bytesStops << endl;
    // Calculo de los bytes para MAPA
    unsigned long long bytesMapa = 0;
    for(int i=0; i<gstMapa.size(); i++){
        bytesMapa += sizeof(gstMapa[i]);
    }
    cout << "mapa [B]: " << bytesMapa << endl;
    cout << "gstMFSbv [B]: " << size_in_bytes(gstMFSbv) << endl;
    cout << "gstMFS_rank_1 [B]: " << size_in_bytes(gstMFSrank) << endl;
    cout << "gstMFS_select_1 [B]: " << size_in_bytes(gstMFSselect) << endl;

    cout << "**** Elementos ****" << endl;
    cout << "Nº Rutas: " << gstStops.size() << endl;
    cout << "Nº Nodos cst_sada: " << cst.nodes() << endl;
    cout << "Nº Hojas cst_sada: " << cst.size() << endl;
    cout << "Nº 1s/length en marcas: " << bitsUno << "/" << bitsTotal;
    cout << " (" << porcentaje << "%)" << endl;
}

void TopoRelGST_4::printRutaYPos(int x){
    if(x > n_rutas){
        return;
    }

    int pI = 0;
    if(x > 0){
        pI = gstMFSselect(x) + 1;
    }
    int pF = gstMFSselect(x+1) - 1;
    cout << "posiciones: " << pI << " - " << pF;
    cout << " (" << getLargoRuta(x) << ") " << extract(cst.csa,pI, pF) << endl;

}

void TopoRelGST_4::printRuta(int x){
    if(x > n_rutas){
        return;
    }

    int pI = 0;
    if(x > 0){
        pI = gstMFSselect(x) + 1;
    }
    int pF = gstMFSselect(x+1) - 1;
    cout << extract(cst.csa,pI, pF);    
}

int TopoRelGST_4::getLargoRuta(int x){
    if(x > n_rutas){
        return 0;
    }

    int pI = 0;
    if(x > 0){
        pI = gstMFSselect(x) + 1;
    }
    int pF = gstMFSselect(x+1) - 1;
    return pF - pI + 1;
}

bool TopoRelGST_4::iguales(TopoRelGST_4 x){
    // Compara constantes
    if(n_rutas != x.n_rutas){
        return false;
    }
    if(n_concat != x.n_concat){
        return false;
    }
    if(n_stops != x.n_stops){
        return false;
    }
    if(finSec != x.finSec){
        return false;
    }

    // Compara CSA
    if(cst.csa.size() != x.cst.csa.size()){        
        cout << "cst.csa tiene distinto tamaño." << endl;
        return false;
    }
    for(int i=0; i<cst.csa.size(); i++){
        if(cst.csa[i] != x.cst.csa[i]){
            cout << "cst.csa tiene distinto contenido en ruta " << i << endl;
            return false;
        }
    }

    // Comparando gstRutas
    for(int i=0; i<n_rutas; i++){
        if(gstRutas[i] != x.gstRutas[i]){
            cout << "gstRutas tiene distinto contenido en ruta " << i << endl;
            return false;
        }
    }

    // Compararndo gstStops
    if(gstStops.size() != x.gstStops.size()){
        cout << "gstStops tiene distinto tamaño." << endl;
        return false;
    }
    for(int i=0; i<gstStops.size(); i++){
        if(gstStops[i].size() != x.gstStops[i].size()){
        cout << "gstStops[i] tiene distinto tamaño." << endl;
            return false;
        }
        for(int j=0; j<gstStops[i].size(); j++){
            if(gstStops[i][j] != x.gstStops[i][j]){
                cout << "gstStops tiene distinto contenido en ruta " << i << " en la parada " << j << endl;
                return false;
            }
        }
    }

    // Comparando gstMFSbv
    if(gstMFSbv.size() != x.gstMFSbv.size()){
        cout << "gstMFSbv tiene distinto tamaño." << endl;
        return false;
    }
    for(int i=0; i<gstMFSbv.size(); i++){
        if(gstMFSbv[i] != x.gstMFSbv[i]){
            cout << "gstMFSbv tiene distinto contenido en ruta " << i << endl;
            return false;
        }
    }

    // Comparando gstMapa
    if(gstMapa.size() != x.gstMapa.size()){
        cout << "gstMapa tiene distinto tamaño." << endl;
        return false;
    }
    for(int i=0; i<gstMapa.size(); i++){
        if(gstMapa[i] != x.gstMapa[i]){
            cout << "gstMapa tiene distinto contenido en ruta " << i << endl;
            return false;
        }
    }

    return true;
}


/*******************************************************
            FUNCIONALIDADES PRIVADAS
*******************************************************/

bool TopoRelGST_4::bordesSeg_touches(int t1, int t2){
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

cst_sada<>::node_type TopoRelGST_4::nodoSubseq(cst_sada<>::node_type n, int x){
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

int TopoRelGST_4::getIdRutaSegunPosConcat(int x){
    // Recibe una posición referente a la secuencia concatenada e indica el 
    // id de la secuencia a la que corresponde dicha posición
    // Utiliza operaciones de rank sobre el bitmap de fin de secuencia
    if(x >= gstMFSbv.size()){
        return -1;
    }
    int idT = gstMFSrank(x);
    return (idT % n_rutas);
}