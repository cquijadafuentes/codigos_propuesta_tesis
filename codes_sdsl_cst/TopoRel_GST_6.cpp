/*
    Versión del GST que se construye de modo paralelo el MAPA:
    - GST
    - Vector con las rutas
    - Marcas en stops (primer nivel del GST) sd_vector
    - Mapa a las hojas donde terminan las rutas
    - Marcas de los nodos que estan en marcas
    - Marcas de las ramas que poseen nodos en marcas

    Constructor:
            Versión en paralelo que recorre por cada ruta
            desde la raíz a los nodos que representan la secuencia
*/

#include "TopoRel_GST_6.hpp"

TopoRelGST_6::TopoRelGST_6(vector<vector<int>> &rutas, int cant_stops){
    statsReset();
    cout << "Constructor TopoRelGST_6 (parallel top-down)" << endl;
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
    len_min = rutas[0].size();
    len_max = rutas[0].size();
    int_vector<> iv(n_concat*2);
    gstRutas = vector<int_vector<>>(rutas.size());
    bit_vector MFStemporal = bit_vector(n_concat*2, 0);
    int pv = 0;
    int tr = n_rutas;
    // Concatenar rutas
    for(int i = 0; i < n_rutas; i++){
        if(rutas[i].size() < len_min){
            len_min = rutas[i].size();
        }
        if(rutas[i].size() > len_max){
            len_max = rutas[i].size();
        }
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

    bit_vector MNtemporal = bit_vector(cst.nodes(), 0);
    bit_vector MRtemporal = bit_vector(cst.nodes(), 0);

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
    cout << "N° Threads: " << maxThreads << endl;
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
            MNtemporal[cst.id(v)] = 1;
            auto mr = cst.parent(v);
            MRtemporal[cst.id(mr)] = 1;
            while(mr != cst.root()){
                mr = cst.parent(mr);
                MRtemporal[cst.id(mr)] = 1;
            }
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
        gstMapRuta2Nodo.insert(gstMapRuta2Nodo.end(), vPorcMap[i].begin(), vPorcMap[i].end());
    }

    if(sobrantes > 0){
        gstMapRuta2Nodo.erase(gstMapRuta2Nodo.end() - sobrantes, gstMapRuta2Nodo.end());
    }
//    cout << "Map... OK" << endl;

    gstMNodos = sd_vector<>(MNtemporal);
    gstMRamas = sd_vector<>(MRtemporal);

    // Llenar gstMapNodo2Rutas
    //int insertos = 0;
    for(int i=0; i<gstMapRuta2Nodo.size(); i++){
        gstMapNodo2Ruta.insert(make_pair(cst.id(gstMapRuta2Nodo[i]), i));
    //    insertos++;
    }
    
    //cout << "Elementos en gstMapNodo2Ruta vs insertos " << gstMapNodo2Ruta.size() << " / " << insertos << endl;
    // Mostrar contenido de gstMapNodo2Rutas
    //for(int i=0; i<gstMNodos.size(); i++){
    //    //cout << gstMNodos[i] << endl;
    //    if(gstMNodos[i] == 1){
    //        /*if(gstMapNodo2Ruta.count(i) > 1){
    //            auto rango = gstMapNodo2Ruta.equal_range(i);
    //            for(auto j=rango.first; j!=rango.second; j++){
    //                cout << "Nodo " << j->first << " contiene la ruta " << j->second << endl;
    //            } 
    //       }else{
    //            cout << "Nodo " << i << " contiene la ruta " << gstMapNodo2Ruta.find(i)->second << endl;
    //        }*/
    //        auto rango = gstMapNodo2Ruta.equal_range(i);
    //        for(auto j=rango.first; j!=rango.second; j++){
    //            cout << "Nodo " << j->first << " contiene la ruta " << j->second << endl;
    //        } 
    //    }
    //}

    cout << "Fin constructor (parallel top-down  -)." << endl;
}

TopoRelGST_6::TopoRelGST_6(string inputFilename){
    statsReset();
    cout << "Cargando estructura desde archivo" << endl;
    ifstream infile(inputFilename, ofstream::binary);
    if(infile){
        int aux1, aux2;
//        cout << "El archivo existe." << endl;
        // Cargando valores enteros
        infile.read ((char *)&n_rutas,sizeof(int));
        infile.read ((char *)&n_concat,sizeof(int));
        infile.read ((char *)&n_stops,sizeof(int));
        infile.read ((char *)&finSec,sizeof(int));
        infile.read ((char *)&len_min,sizeof(int));
        infile.read ((char *)&len_max,sizeof(int));

//        cout << n_rutas << endl;
//        cout << n_concat << endl;
//        cout << n_stops << endl;
//        cout << finSec << endl;
        // Cargando CST
        cst.load(infile);
//        cout << cst.nodes() << endl;
//        cout << cst.size() << endl;    
        // Cargando gstMapRuta2Nodo
        infile.read ((char *)&aux1,sizeof(int));
        gstMapRuta2Nodo = vector<cst_sada<>::node_type>(aux1);
        for(int i=0; i<aux1; i++){
            infile.read ((char *)&aux2,sizeof(int));
            gstMapRuta2Nodo[i] = cst.inv_id(aux2);
//            cout << aux2 << " ";
        }
//        cout << endl;
        // Cargando gstRutas
        infile.read ((char *)&aux1,sizeof(int));
        gstRutas = vector<int_vector<>>(aux1);
        for(int i=0; i<aux1; i++){
            gstRutas[i].load(infile);
/*            for(int j=0; j<gstRutas[i].size(); j++){
                cout << gstRutas[i][j] << " ";
            }
            cout << endl;
*/
        }
        // Cargando gstStops
        infile.read ((char *)&aux1,sizeof(int));
        gstStops = vector<sd_vector<>>(aux1);
        for(int i=0; i<aux1; i++){
            gstStops[i].load(infile);
/*            for(int j=0; j<gstStops[i].size(); j++){
                cout << gstStops[i][j] << " ";
            }
            cout << endl;
*/
        }
        // Cargando gstMFSbv, rank y select.
        gstMFSbv.load(infile);
        gstMNodos.load(infile);
        gstMRamas.load(infile);
        // Cargando gstMapNodo2Ruta
        infile.read ((char *)&aux1,sizeof(int));
        for(int i=0; i<aux1; i++){
            int a,b;
            infile.read ((char *)&a,sizeof(int));
            infile.read ((char *)&b,sizeof(int));
            gstMapNodo2Ruta.insert(make_pair(a,b));
        }
        // Cerrando archivo
        infile.close();
        gstMFSrank = sd_vector<>::rank_1_type(&gstMFSbv);
        gstMFSselect = sd_vector<>::select_1_type(&gstMFSbv);
        return;
    }    
    cout << "Error en la carga!" << endl;
}

string TopoRelGST_6::obtenerRelacion(int x, int y){
//    cout << endl << "x: " << x << " - y: " << y << endl;
//    cout << "id x: " << cst.id(gstMapRuta2Nodo[x]) << "id y: " << cst.id(gstMapRuta2Nodo[y]) << endl;
    // Identificar igualdad en cualquiera de los dos sentidos
    if(gstMapRuta2Nodo[x] == gstMapRuta2Nodo[y]){
        return EQUALS;
    }
    if(gstMapRuta2Nodo[x] == gstMapRuta2Nodo[y+n_rutas]){
        return EQUALS;
    }
    // Identificar contención según nodo
    // Inside, includes, coveredBy, covers
    int corto, largo, lC, lL;
    int lx = getLargoRuta(x);
    int ly = getLargoRuta(y);
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

    auto C = gstMapRuta2Nodo[corto];
    auto Cr = gstMapRuta2Nodo[corto+n_rutas];
    auto L = gstMapRuta2Nodo[largo];
    auto Lr = gstMapRuta2Nodo[largo+n_rutas];
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


bool TopoRelGST_6::tr_equals(int x, int y){
    if(gstMapRuta2Nodo[x] == gstMapRuta2Nodo[y] || gstMapRuta2Nodo[x] == gstMapRuta2Nodo[y+n_rutas]){
        return true;
    }
    return false;
}

bool TopoRelGST_6::tr_coveredby(int x, int y){
    // Descarte por largo de secuencias
    int lx = getLargoRuta(x);
    int ly = getLargoRuta(y);
    if(lx >= ly){
        return false;
    }
    
    // Verifica CoveredBy
    auto lca = cst.lca(gstMapRuta2Nodo[x], gstMapRuta2Nodo[y]);
    if(cst.depth(lca) == lx){
        return true;
    }
    lca = cst.lca(gstMapRuta2Nodo[x], gstMapRuta2Nodo[y+n_rutas]);
    if(cst.depth(lca) == lx){
        return true;
    }
    lca = cst.lca(gstMapRuta2Nodo[x+n_rutas], gstMapRuta2Nodo[y]);
    if(cst.depth(lca) == lx){
        return true;
    }
    lca = cst.lca(gstMapRuta2Nodo[x+n_rutas], gstMapRuta2Nodo[y+n_rutas]);
    if(cst.depth(lca) == lx){
        return true;
    }
    
    return false;
}

bool TopoRelGST_6::tr_covers(int x, int y){
    return tr_coveredby(y,x);
}

bool TopoRelGST_6::tr_inside(int x, int y){
    // Descarte por largo de secuencias
    int lx = getLargoRuta(x);
    int ly = getLargoRuta(y);
    if(lx >= ly){
        return false;
    }
    // Descarte por CoveredBy
    int l = getLargoRuta(x);
    auto lca1 = cst.lca(gstMapRuta2Nodo[x], gstMapRuta2Nodo[y]);
    auto lca2 = cst.lca(gstMapRuta2Nodo[x], gstMapRuta2Nodo[y+n_rutas]);
    auto lca3 = cst.lca(gstMapRuta2Nodo[x+n_rutas], gstMapRuta2Nodo[y]);
    auto lca4 = cst.lca(gstMapRuta2Nodo[x+n_rutas], gstMapRuta2Nodo[y+n_rutas]);
    if(cst.depth(lca1) == l || cst.depth(lca2) == l || cst.depth(lca3) == l || cst.depth(lca4) == l){
        return false;
    }
    // Descarte por SegmentosBorde
    if(bordesSeg_touches(x,y)){
        return false;
    }
    // Verifica Inside
    auto L = gstMapRuta2Nodo[y];
    auto C = gstMapRuta2Nodo[x];
    auto Lr = gstMapRuta2Nodo[y+n_rutas];
    auto Cr = gstMapRuta2Nodo[x+n_rutas];
    auto lcaCL = cst.root();
    auto lcaCLr = cst.root();
    auto lcaCrL = cst.root();
    auto lcaCrLr = cst.root();

    int tempL = getLargoRuta(y);
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
    tempL = getLargoRuta(y);
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

bool TopoRelGST_6::tr_includes(int x, int y){
    return tr_inside(y,x);
}

bool TopoRelGST_6::tr_disjoint(int x, int y){
    // Descartar igualdad
    if(gstMapRuta2Nodo[x] == gstMapRuta2Nodo[y] || gstMapRuta2Nodo[x] == gstMapRuta2Nodo[y+n_rutas]){
        return false;
    }
    // Descartar contención por CB o CV
    if(tr_coveredby(x,y) || tr_covers(x,y)){
        return false;
    }
    // Descartar posible TO
    int bXi = gstRutas[x][0];
    int bXf = gstRutas[x][getLargoRuta(x)-1];
    int bYi = gstRutas[y][0];
    int bYf = gstRutas[y][getLargoRuta(y)-1];
    if(gstStops[y][bXi] == 1 || gstStops[y][bXf] == 1 || gstStops[x][bYi] == 1 || gstStops[x][bYf] == 1){
        // Hay bordes en contacto con la otra secuencia
        return false;
    }
    // Comprobar cualquier intersección
    int lx = getLargoRuta(x);
    int ly = getLargoRuta(y);

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

bool TopoRelGST_6::tr_touches(int x, int y){
    // Descarte por igualdad
    if(gstMapRuta2Nodo[x] == gstMapRuta2Nodo[y] || gstMapRuta2Nodo[x] == gstMapRuta2Nodo[y+n_rutas]){
        return false;
    }
    // Verifica candidato por bordes
    int bXi = gstRutas[x][0];
    int bXf = gstRutas[x][getLargoRuta(x)-1];
    int bYi = gstRutas[y][0];
    int bYf = gstRutas[y][getLargoRuta(y)-1];
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
    int lx = getLargoRuta(x);
    for(int i=2; i<lx; i++){
        aux = gstRutas[x][i-1];
        if(aux != bYi && aux != bYf && gstStops[y][aux]){
            return false;
        }
    }
    return true;
}

bool TopoRelGST_6::tr_overlaps(int x, int y){
    // Descartar igualdad
    if(gstMapRuta2Nodo[x] == gstMapRuta2Nodo[y] || gstMapRuta2Nodo[x] == gstMapRuta2Nodo[y+n_rutas]){
        return false;
    }
    // Descartar contención por CB o CV
    if(tr_coveredby(x,y) || tr_covers(x,y)){
        return false;
    }
    // Bordes de las secuencias
    int bXi = gstRutas[x][0];
    int bXf = gstRutas[x][getLargoRuta(x)-1];
    int bYi = gstRutas[y][0];
    int bYf = gstRutas[y][getLargoRuta(y)-1];
    // Comprobar intersección Ix-Iy y Ix-Ey (al menos una de cada una)
    int iXiY = 0;
    int iXeY = 0;
    int lx = getLargoRuta(x);
    int ly = getLargoRuta(y);
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


bool TopoRelGST_6::tr_within(int x, int y){
    // Debe ser EQUALS, COVEREDBY o INSIDE
    // Descarte por largo de secuencia
    int lx = getLargoRuta(x);
    int ly = getLargoRuta(y);
    if(lx > ly){
        return false;
    }
    // Comprobando Equals
    if(gstMapRuta2Nodo[x] == gstMapRuta2Nodo[y] || gstMapRuta2Nodo[x] == gstMapRuta2Nodo[y+n_rutas]){
        return true;
    }
    // Comprobando COVEREDBY
    auto lca1 = cst.lca(gstMapRuta2Nodo[x], gstMapRuta2Nodo[y]);
    auto lca2 = cst.lca(gstMapRuta2Nodo[x], gstMapRuta2Nodo[y+n_rutas]);
    auto lca3 = cst.lca(gstMapRuta2Nodo[x+n_rutas], gstMapRuta2Nodo[y]);
    auto lca4 = cst.lca(gstMapRuta2Nodo[x+n_rutas], gstMapRuta2Nodo[y+n_rutas]);
    if(cst.depth(lca1) == lx || cst.depth(lca2) == lx || cst.depth(lca3) == lx || cst.depth(lca4) == lx){
        return true;
    }
    // Comprobando INSIDE
    auto L = gstMapRuta2Nodo[y];
    auto C = gstMapRuta2Nodo[x];
    auto Lr = gstMapRuta2Nodo[y+n_rutas];
    auto Cr = gstMapRuta2Nodo[x+n_rutas];
    auto lcaCL = cst.root();
    auto lcaCLr = cst.root();
    auto lcaCrL = cst.root();
    auto lcaCrLr = cst.root();
    int l = getLargoRuta(x);
    int tempL = getLargoRuta(y);
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

    tempL = getLargoRuta(y);
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

bool TopoRelGST_6::tr_contains(int x, int y){
    return tr_within(y, x);
}    

bool TopoRelGST_6::tr_intersect(int x, int y){
    return !tr_disjoint(x, y);
}


/*******************************************************
            Relaciones topológicas en conjunto
*******************************************************/


vector<int> TopoRelGST_6::tr_allContain(int x){
    vector<int> y;
    if(x > n_rutas) return y;

    auto nX = gstMapRuta2Nodo[x];
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
//        auto hoja = cst.inv_id(i);
//        cout << "-----" << endl;
//        cout << "id_GST: " << i << " - " << extract(cst, hoja) << endl;;
//        cout << "p_CSA: " << cst.csa[i] << " - ";
        int idAux = idRutaDesdeCeldaDeSecConcat(cst.csa[i]);
//        cout << "id_Rutas: " << idAux << " - Ruta: ";
//        printRuta(idAux);
//        cout << endl;
//        cout << "-----" << endl;
        y.push_back(idAux);
    }
    return y;
}


vector<int> TopoRelGST_6::tr_allEqual(int x){
    vector<int> y;
    if(x > n_rutas) return y;

    auto nX = gstMapRuta2Nodo[x];
    auto idLChST = cst.leftmost_leaf(nX);
    auto idRChST = cst.rightmost_leaf(nX);

    for(int i=cst.id(idLChST); i <= cst.id(idRChST); i++){
        int idAux = gstMFSrank(cst.csa[i]);  // Esto entrega el id de la secuencia desde el arreglo con marcas de fin de secuencia
                                                // Equivalente al retorno de la función idRutaDesdeCeldaDeSecConcat()
        if(gstMapRuta2Nodo[x] == gstMapRuta2Nodo[idAux]){
            y.push_back(idAux % n_rutas);
        }
    }
    return y;
}


vector<int> TopoRelGST_6::tr_allContained(int x, bool verbose){
    if(verbose){
        cout << "\n++++++++++++++++ Operación ALLCONTAINED ++++++++++++++++" << endl;
    }
    unordered_set<int> y;
    int ls = getLargoRuta(x);
    auto nodo = gstMapRuta2Nodo[x];
    howManyIfs++;
    if(!cst.is_leaf(nodo)){
        nodo = cst.rightmost_leaf(nodo);
        howManyNodes++;
    }
    for(int i=0; ls - i >= len_min; i++){
        int idHoja = cst.id(nodo);
        howManyIfs++;
        if(verbose){
            cout << "Revisando hoja " << idHoja << endl;
        }
        int pos = cst.csa[idHoja];
        howManyIfs++;
        if((pos == 0 || gstMFSbv[pos-1] == 1)){
            // En la celda pos inicia una secuencia
            howManyIfs++;
            if(verbose){
                cout << "insertando posición " << pos << endl;
            }
            int idSecCand = idRutaDesdeCeldaDeSecConcat(pos);
            howManyIfs++;
            if(getLargoRuta(idSecCand) <= ls-i){
                y.insert(idSecCand);
                howManyInserts++;
            }
        }
        // Se realizan operaciones de suffix link mientras el largo 
        // del sufijo sea mayor que el largo de la secuencia más corta
        // Revisión del SA hacia el inicio
        int idNav = idHoja;
        int min_coincidencia = cst.lcp[idNav];
        howManyLCP++;
        while(min_coincidencia >= len_min){
            howManyIfs++;
            if(verbose){
                cout << "IdNav: " << idNav << endl;
            }
            int pos = cst.csa[idNav-1];
            howManyIfs++;
            if(pos == 0 || gstMFSbv[pos-1] == 1){
                // Corresponde a una secuencia completamente contenida
                int idSecCand = idRutaDesdeCeldaDeSecConcat(pos);
                int largoSecCand = getLargoRuta(idSecCand);
                if(largoSecCand <= min_coincidencia && largoSecCand <= (ls - i)){
                    y.insert(idSecCand);
                    howManyInserts++;
                }
            }
            idNav--;
            howManyIfs++;
            if(cst.lcp[idNav] < min_coincidencia){
                min_coincidencia = cst.lcp[idNav];
                howManyLCP++;
            }
            howManyLCP++;
            howManyIfs++;
        }

        // Revisión del SA hacia el final
        idNav = idHoja+1;
        min_coincidencia = cst.lcp[idNav];
        howManyLCP++;
        while(min_coincidencia >= len_min){
            howManyIfs++;
            if(verbose){
                cout << "IdNav: " << idNav << endl;
            }
            int pos = cst.csa[idNav];
            howManyIfs++;
            if(pos == 0 || gstMFSbv[pos-1] == 1){
                // Corresponde a una secuencia completamente contenida
                int idSecCand = idRutaDesdeCeldaDeSecConcat(pos);
                int largoSecCand = getLargoRuta(idSecCand);
                if(largoSecCand <= min_coincidencia && largoSecCand <= (ls - i)){
                    y.insert(idSecCand);
                    howManyInserts++;
                }
            }
            idNav++;
            howManyIfs++;
            if(cst.lcp[idNav] < min_coincidencia){
                min_coincidencia = cst.lcp[idNav];
                howManyLCP++;
            }
            howManyLCP++;
            howManyIfs++;
        }

        // Suffix link para el siguiente ciclo
        nodo = cst.sl(nodo);
        howManyNodes++;
        howManyIfs++;
    }

    vector<int> res(y.begin(), y.end());
    return res;
}


vector<int> TopoRelGST_6::tr_allContained2(int x, bool verbose){
    // Versión de la operación allContained que determina el resultado
    // por un recorrido del GST desde la raíz
    unordered_set<int> setRes;
    setRes.insert(x);
    howManyInserts++;
    vector<int> gstRutaX = getRuta(x);

    // Primer ciclo recorre los sub-árboles con los sufijos
    // que corresponden a la secuencia posible.
    int topeSec = getLargoRuta(x) - len_min;
    auto raiz = cst.root();
    for(int i=0; i<=topeSec; i++){
        auto nodo = cst.child(raiz, gstRutaX[i]);
        howManyNodes++;
        int ii = cst.depth(nodo);
        howManyIfs++;
        if(verbose){
            cout << "Iniciando en " << cst.id(nodo) << endl;
            cout << "i: " << i << " ii: " << ii << " depth: " << cst.depth(nodo) << endl;
        }
        // Comenzar con un sufijo de largo al menos len_min
        while(nodo != raiz && i+ii <= getLargoRuta(x) && (gstMRamas[cst.id(nodo)] == 1 || gstMNodos[cst.id(nodo)] == 1)){
            howManyIfs++;
            if(cst.depth(nodo) >= len_min && (gstMRamas[cst.id(nodo)] == 1 || gstMNodos[cst.id(nodo)] == 1)){
                // Son nodos que representan sufijos cuyo largo 
                // supera el de la secuencia menor del conjunto
                auto nodoAux = cst.child(nodo, finSec);
                howManyNodes++;
                howManyIfs++;
                if(verbose){
                    cout << "\tEvaluando rama desde " << cst.id(nodoAux) << endl;
                }
                howManyIfs++;
                if(nodoAux != raiz){ // && gstMNodos[cst.id(nodoAux)] == 1
                    // Revisar hojas de esta subsecuencia
                    int pi = cst.lb(nodoAux);
                    int pf = cst.rb(nodoAux);
                    howManyIfs++;
                    if(verbose){
                        cout << "\t\tVerificando hojas desde " << pi << " hasta " << pf << endl;
                    }
                    howManyCSTviews += (pf - pi + 1);
                    for(int j=pi; j<= pf; j++){
                        howManyIfs++;
                        if(cst.csa[j] == 0 || gstMFSbv[cst.csa[j]-1] == 1){
                            if(verbose){
                                cout << "\t\t\tInsertando " << idRutaDesdeCeldaDeSecConcat(cst.csa[j]) << endl;
                            }
                            // Corresponde a una secuencia completa
                            setRes.insert(idRutaDesdeCeldaDeSecConcat(cst.csa[j]));
                            howManyInserts++;
                        }
                        howManyIfs++;
                    }
                }
            }

            nodo = cst.child(nodo, gstRutaX[i+ii]);
            howManyNodes++;
            ii = cst.depth(nodo);
            howManyIfs++;
            if(verbose){
                cout << "\tLlegando a por " << cst.id(nodo) << endl;
            }
            howManyIfs++;
        }
        howManyIfs++;
        if(verbose){
            cout << "\tEn nodo " << cst.id(nodo) << endl;
        }
        // Comenzar a verificar si en el nodo que resta por recorrer
        // existen secuencias que contengan el resultado
        

        // Verificando el último nodo
        howManyIfs++;
        if(nodo != raiz && cst.size(nodo) > 1 && (gstMRamas[cst.id(nodo)] == 1 || gstMNodos[cst.id(nodo)] == 1)){
            // Existe un nodo que es sufijo de la secuencia
            // Verificando sub-árbol
            int pi = cst.lb(nodo);
            int pf = cst.rb(nodo);
            howManyIfs++;
            if(verbose){
                cout << "\t\tVerificando hojas desde " << pi << " hasta " << pf << endl;
            }
            howManyCSTviews += (pf - pi + 1);
            for(int j=pi; j<= pf; j++){
                howManyIfs++;
                if(cst.csa[j] == 0 || gstMFSbv[cst.csa[j]-1] == 1){
                    // Corresponde a una secuencia completa
                    setRes.insert(idRutaDesdeCeldaDeSecConcat(cst.csa[j]));
                    if(verbose){
                        cout << "\t\t\tInsertando " << idRutaDesdeCeldaDeSecConcat(cst.csa[j]) << endl;
                    }
                    howManyInserts++;
                }
                howManyIfs++;
            }
        }
        howManyIfs++;
    }

    vector<int> res(setRes.begin(), setRes.end());
    return res;
}


vector<int> TopoRelGST_6::tr_allContained3(int x, bool verbose){
    // Versión de la operación allContained que determina el resultado
    // iniciando en el nodo del sufijo de largo <minlen> de la secuencia
    // recorriendo hacia la raíz usando wl para agregar elementos
    unordered_set<int> setRes;
    setRes.insert(x);
    howManyInserts++;
    vector<int> gstRutaX = getRuta(x);

    // Calculando el nodo inicial con el sufijo de la secuencia de largo len_min
    // Fase 0
    auto raiz = cst.root();
    howManyNodes++;
    auto nodo = raiz;
    int pISec = getLargoRuta(x) - len_min;
    int ps;
    while(cst.depth(nodo) < len_min ){
        ps = pISec + cst.depth(nodo);
        nodo = cst.child(nodo, gstRutaX[ps]);
        howManyNodes++;
        howManyIfs++;
        if(verbose){
            cout << "Fase 0 por nodo: " << cst.id(nodo) << " por elemento " << gstRutaX[ps] << endl;
        }
        howManyIfs++;
    }
    bool conFS = cst.depth(nodo) > len_min;

    // Nodo esta ubicado en el sufijo más corto de la secuencia.
    for(int i=1; i<=pISec+1; i++){
        // Recorrido de las ramas por medio de WL
        auto nodoAux = nodo;
        howManyIfs++;
        if(verbose){
            cout << "\tDesde el nodo " << cst.id(nodoAux) << endl;
        }
        while(cst.depth(nodoAux) >= len_min){
            auto nodoExp = nodoAux;
            howManyIfs++;
            if(len_min + i - 1 >= cst.depth(nodoAux)){  // Conocer si la secuenci contiene caracter de fin
                nodoExp = cst.child(nodoAux, finSec);
                howManyNodes++;
            }
            howManyIfs++;
            if(verbose){
                cout << "\tExplorando nodo: " << cst.id(nodoExp) << endl;
            }
            // Verificando rama por candidato
            howManyIfs++;
            if(nodoExp != raiz && (gstMRamas[cst.id(nodoExp)] == 1 || 
                gstMNodos[cst.id(nodoExp)] == 1)){
                // Existe un nodo que es sufijo de la secuencia y en la rama hay nodos 
                // de secuencia completa
                // Verificando sub-árbol
                int pi = cst.lb(nodoExp);
                int pf = cst.rb(nodoExp);
                howManyIfs++;
                if(verbose){
                    cout << "\t\tVerificando hojas desde " << pi << " hasta " << pf << endl;
                }
                howManyCSTviews += (pf - pi + 1);
                for(int j=pi; j<= pf; j++){
                    howManyIfs++;
                    if(cst.csa[j] == 0 || gstMFSbv[cst.csa[j]-1] == 1){
                        // Corresponde a una secuencia completa
                        setRes.insert(idRutaDesdeCeldaDeSecConcat(cst.csa[j]));
                        howManyInserts++;
                        howManyIfs++;
                        if(verbose){
                            cout << "\t\t\tInsertando " << cst.csa[j] << endl;
                        }
                    }
                    howManyIfs++;
                }
            }
            nodoAux = cst.parent(nodoAux);
            howManyNodes++;
            howManyIfs++;
            if(verbose){
                cout << "Hacia el padre " << cst.id(nodoAux) << endl;
            }
            howManyIfs++;
        }
        howManyIfs++;
        if(verbose){
            cout << "Usando wl desde " << cst.id(nodo);
        }
        howManyIfs++;
        if(i!=pISec+1){
            nodo = cst.wl(nodo, gstRutaX[pISec - i]);
            conFS = (cst.depth(nodo) - i) > len_min;
            howManyNodes++;
            howManyIfs++;
            if(verbose){
                cout << " hasta " << cst.id(nodo) << " por elemento " << gstRutaX[pISec - i] << endl;
            }
        }
        howManyIfs++;
    }

    vector<int> res(setRes.begin(), setRes.end());
    return res;
}


vector<int> TopoRelGST_6::tr_allContained4(int x, bool verbose){
    // Versión de la operación allContained que determina el resultado
    // iniciando en el nodo del sufijo de largo <minlen> de la secuencia
    // recorriendo hacia la raíz usando wl para agregar elementos
    // Esta versión usa gstMapNodo2Ruta para agregar resultados.
    unordered_set<int> setRes;
    setRes.insert(x);
    howManyInserts++;
    vector<int> gstRutaX = getRuta(x);

    // Calculando el nodo inicial con el sufijo de la secuencia de largo len_min
    // Fase 0
    auto raiz = cst.root();
    howManyNodes++;
    auto nodo = raiz;
    int pISec = getLargoRuta(x) - len_min;
    int ps;
    while(cst.depth(nodo) < len_min ){
        ps = pISec + cst.depth(nodo);
        nodo = cst.child(nodo, gstRutaX[ps]);
        howManyNodes++;
        howManyIfs++;
        if(verbose){
            cout << "Fase 0 por nodo: " << cst.id(nodo) << " por elemento " << gstRutaX[ps] << endl;
        }
        howManyIfs++;
    }
    bool conFS = cst.depth(nodo) > len_min;

    // Nodo esta ubicado en el sufijo más corto de la secuencia.
    for(int i=1; i<=pISec+1; i++){
        // Recorrido de las ramas por medio de WL
        auto nodoAux = nodo;
        howManyIfs++;
        if(verbose){
            cout << "\tDesde el nodo " << cst.id(nodoAux) << endl;
        }
        while(cst.depth(nodoAux) >= len_min){
            auto nodoExp = nodoAux;
            howManyIfs++;
            if(len_min + i - 1 >= cst.depth(nodoAux)){  // Conocer si la secuenci contiene caracter de fin
                nodoExp = cst.child(nodoAux, finSec);
                howManyNodes++;
            }
            howManyIfs++;
            if(verbose){
                cout << "\tExplorando nodo: " << cst.id(nodoExp) << endl;
            }
            // Verificando rama por candidato
            howManyIfs++;
            if(nodoExp != raiz && gstMNodos[cst.id(nodoExp)] == 1){
                // Existe un nodo que es sufijo de la secuencia y en la rama hay nodos 
                // de secuencia completa
                int nodoExpID = cst.id(nodoExp);
                int count = gstMapNodo2Ruta.count(nodoExpID);
                howManyIfs++;
                if(verbose){
                    cout << "\t\tInsertando " << count << " rutas desde nodo " << nodoExpID << endl;
                }
                auto rango = gstMapNodo2Ruta.equal_range(nodoExpID);
                for(auto j=rango.first; j!=rango.second; j++){
                    howManyIfs++;
                    if(verbose){
                        cout << "\t\t\tInsertando " << (j->second)%n_rutas << endl;
                    }
                    setRes.insert((j->second)%n_rutas);
                    howManyInserts++;
                    howManyIfs++;
                }
            }
            nodoAux = cst.parent(nodoAux);
            howManyNodes++;
            howManyIfs++;
            if(verbose){
                cout << "Hacia el padre " << cst.id(nodoAux) << endl;
            }
            howManyIfs++;
        }
        howManyIfs++;
        if(verbose){
            cout << "Usando wl desde " << cst.id(nodo);
        }
        howManyIfs++;
        if(i!=pISec+1){
            nodo = cst.wl(nodo, gstRutaX[pISec - i]);
            conFS = (cst.depth(nodo) - i) > len_min;
            howManyNodes++;
            howManyIfs++;
            if(verbose){
                cout << " hasta " << cst.id(nodo) << " por elemento " << gstRutaX[pISec - i] << endl;
            }
        }
        howManyIfs++;
    }

    vector<int> res(setRes.begin(), setRes.end());
    return res;
}


vector<int> TopoRelGST_6::tr_allContained5(int x, bool verbose){
    // Versión de la operación allContained que determina el resultado
    // por un recorrido del GST desde la raíz usando mapNodo2Ruta
    unordered_set<int> setRes;
    setRes.insert(x);
    howManyInserts++;
    vector<int> gstRutaX = getRuta(x);

    // Primer ciclo recorre los sub-árboles con los sufijos
    // que corresponden a la secuencia posible.
    int topeSec = getLargoRuta(x) - len_min;
    auto raiz = cst.root();
    int raizID = cst.id(raiz);
    int raizDepth = cst.depth(raiz);
    for(int i=0; i<=topeSec; i++){
        auto nodo = cst.child(raiz, gstRutaX[i]);
        howManyNodes++;
        int nodoID = cst.id(nodo);
        int nodoDepth = cst.depth(nodo);
        howManyIfs++;
        if(verbose){
            cout << "Iniciando en " << cst.id(nodo) << endl;
            cout << "i: " << i << " nodoDepth: " << nodoDepth << endl;
        }
        // Comenzar con un sufijo de largo al menos len_min
        do{
            howManyIfs++;
            if(nodoDepth >= len_min && (gstMRamas[nodoID] == 1 || gstMNodos[nodoID] == 1)){
                // Son nodos que representan sufijos cuyo largo 
                // supera el de la secuencia menor del conjunto
                auto nodoAux = nodo;
                int nodoAuxID = nodoID;
                int nodoAuxDepth = nodoDepth;
                howManyIfs++;
                if(nodoDepth + i <= gstRutaX.size()){
                     nodoAux = cst.child(nodo, finSec);
                     howManyNodes++;
                     nodoAuxID = cst.id(nodoAux);
                     nodoAuxDepth = cst.depth(nodoAux);
                }
                howManyIfs++;
                if(gstMNodos[nodoAuxID] == 1){
                    int count = gstMapNodo2Ruta.count(nodoAuxID);
                    howManyIfs++;
                    if(verbose){
                        cout << "\t\tInsertando+ " << count << " rutas desde nodo " << nodoAuxID << endl;
                    }
                    howManyIfs++;
                    if(nodoAux != raiz && gstMNodos[nodoAuxID] == 1){ 
                        // El nodo con el fin de secuencia existe y tiene al menos una secuencia
                        auto rango = gstMapNodo2Ruta.equal_range(nodoAuxID);
                        for(auto j=rango.first; j!=rango.second; j++){
                            howManyIfs++;
                            if(verbose){
                                cout << "\t\t\tInsertando " << (j->second)%n_rutas << endl;
                            }
                            setRes.insert((j->second)%n_rutas);
                            howManyInserts++;
                            howManyIfs++;
                        }
                    }
                }
            }
            howManyIfs++;
            if(nodoDepth + i < gstRutaX.size()){
                nodo = cst.child(nodo, gstRutaX[i+nodoDepth]);
                howManyNodes++;
                nodoID = cst.id(nodo);
                nodoDepth = cst.depth(nodo);
            }else{
                nodo = raiz;
            }
            howManyIfs++;
            if(verbose){
                cout << "\tLlegando por " << nodoID << endl;
            }
            howManyIfs++;
        }while(nodo != raiz && (gstMRamas[nodoID] == 1 || gstMNodos[nodoID] == 1));
        howManyIfs++;
    }
    vector<int> res(setRes.begin(), setRes.end());
    return res;
}


vector<int> TopoRelGST_6::tr_allContained6(int x, bool verbose){
    // Versión de la operación allContained que determina el resultado
    // por un recorrido del GST desde el nodo obtenido de mapRuta2Nodo
    // y luego por medio de operaciones parent para subir, y 
    // suffixlink para moverse de rama
    unordered_set<int> setRes;
    int sizeX = getLargoRuta(x);
    int topeSec = sizeX - len_min;
    auto raiz = cst.root();
    howManyNodes++;
    auto nodo = gstMapRuta2Nodo[x]; // Se usa para navegación por suffix-link
    int nodoID = cst.id(nodo);
    int nodoDepth = cst.depth(nodo);
    for(int i=0; i<=topeSec; i++){
        // For para navegación entre ramas
        howManyIfs++;
        if(verbose){
            cout << "Iniciando en " << cst.id(nodo) << endl;
            cout << "i: " << i << " nodoDepth: " << nodoDepth << endl;
        }
        // Verificar si hay secuencia desde nodo en bottom-up
        auto nodoAux = nodo;        // Se usa para navegación por cst.parent
        int nodoAuxID = nodoID;
        int nodoAuxDepth = nodoDepth;
        if (verbose){
            cout << "nodoAux id: " << nodoAuxID << " - peso: " << nodoAuxDepth << endl;
        }
        while(nodoAuxDepth >= len_min){
            // El largo permite una secuencia
            auto nodoExp = nodoAux;     // Se usa para verificar los nodos con secuencia
            int nodoExpID = nodoAuxID;
            int nodoExpDepth = nodoAuxDepth;
            howManyIfs++;
            if(verbose){
                cout << "NodoExp id: " << nodoExpID << " - peso: " << nodoExpDepth << endl;
            }
            howManyIfs++;
            if(gstMNodos[nodoExpID] != 1){
                nodoExp = cst.child(nodoAux, finSec);
                howManyNodes++;
                nodoExpID = cst.id(nodoExp);
                nodoExpDepth = cst.depth(nodoExp);
            }
            howManyIfs++;
            if (verbose){
                cout << "\t\tRevisando " << nodoAuxID << endl;
            }
            howManyIfs++;
            if(nodoExp != raiz && gstMNodos[nodoExpID]){
                // Hay nodos marcados en nodoExp
                int count = gstMapNodo2Ruta.count(nodoExpID);
                auto rango = gstMapNodo2Ruta.equal_range(nodoExpID);
                for(auto j=rango.first; j!=rango.second; j++){
                    howManyIfs++;
                    if(verbose){
                        cout << "\t\t\tInsertando " << (j->second)%n_rutas << endl;
                    }
                    setRes.insert((j->second)%n_rutas);
                    howManyInserts++;
                    howManyIfs++;
                }
            }
            nodoAux = cst.parent(nodoAux);
            howManyNodes++;
            nodoAuxID = cst.id(nodoAux);
            nodoAuxDepth = cst.depth(nodoAux);
            howManyIfs++;
        }
        // Cambio de rama con suffix link
        howManyIfs++;
        if (verbose){
            cout << "SuffixLink desde " << nodoID;
        }
        nodo = cst.sl(nodo);
        howManyNodes++;
        nodoID = cst.id(nodo);
        nodoDepth = cst.depth(nodo);
        if (verbose){
            cout << " hasta " << nodoID << endl;
        }
        howManyIfs++;
    }
    vector<int> res(setRes.begin(), setRes.end());
    return res;
}


vector<int> TopoRelGST_6::tr_allIntersect(int x, bool verbose){
    unordered_set<int> setRes;
    setRes.insert(x);
    vector<int> gstRutaX = getRuta(x);

    // Calculando el nodo inicial con el sufijo de la secuencia de largo len_min
    auto raiz = cst.root();
    for(int i=0; i<getLargoRuta(x); i++){
        auto nodo = cst.child(raiz, gstRutaX[i]);
        int idL = cst.lb(nodo);
        int idR = cst.rb(nodo);
        for(int j=idL; j<=idR; j++){
            setRes.insert(idRutaDesdeCeldaDeSecConcat(cst.csa[j]));
            howManyIfs++;
        }
        howManyIfs++;
    }

    vector<int> res(setRes.begin(), setRes.end());
    return res;
}


/*******************************************************
            Otras funcionalidades
*******************************************************/


void TopoRelGST_6::navega(int x){

    cout << "Información del CompressedSuffixTree:" << endl;
    cout << "Cantidad de nodos: " << cst.nodes() << endl;
    cout << "Cantidad de hojas: " << cst.size() << endl;

    cout << "Recorrido de hijos de root del CompressedSuffixTree:" << endl;
    auto root = cst.root();
    cout << "id\ted_1\tdeg\tdep\tndep\tsize\tlb\trb\tsun\tleaf\ttext" << endl;
    for (auto child: cst.children(root)) {
        cout << cst.id(child) << "\t";
        cout << "'" << cst.edge(child, 1) << "'" << "\t";   // D-th char of the edge-label
        cout << cst.degree(child) << "\t";  // Number of children
        cout << cst.depth(child) << "\t";   // String depth
        cout << cst.node_depth(child) << "\t";  // 
        cout << cst.size(child) << "\t";    // Number of leaves in the subtree
        cout << cst.lb(child) << "\t";      // Leftmost leaf
        cout << cst.rb(child) << "\t";      // Rightmost leaf
        cout << cst.sn(child) << "\t";      // Suffix number
        cout << cst.is_leaf(child) << "\t"; // IsLeaf
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
        cout << "'" << cst.edge(*it, 1) << "'" << "\t"; // D-th char of the edge-label
        cout << cst.degree(*it) << "\t";    // Number of children
        cout << cst.depth(*it) << "\t";     // String depth
        cout << cst.node_depth(*it) << "\t";// 
        cout << cst.size(*it) << "\t";      // Number of leaves in the subtree
        cout << cst.lb(*it) << "\t";        // Leftmost leaf
        cout << cst.rb(*it) << "\t";        // Rightmost leaf
        cout << cst.sn(*it) << "\t";        // Suffix number
        cout << cst.is_leaf(*it) << "\t";   // IsLeaf
        cout << extract(cst, *it) << "\t";  // Text

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
    for(int i=0; i<gstMapRuta2Nodo.size(); i++){
        cout << "(" << i << "): " << cst.id(gstMapRuta2Nodo[i]) << endl;
    }
    cout << endl;

    cout << endl << "Id Secuencia de la posición 2 en concatenación: ";
    int idd = idRutaDesdeCeldaDeSecConcat(2);
    cout << idd << " - ";
    printRutaYPos(idd);
    cout << endl << endl;

    cout << endl << "Id Secuencia de la posición 18 en concatenación: ";
    idd = idRutaDesdeCeldaDeSecConcat(18);
    cout << idd << " - ";
    printRutaYPos(idd);
    cout << endl << endl;
}

void TopoRelGST_6::sizeEstructura(){
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
    // Calculo de los bytes para gstMapRuta2Nodo
    unsigned long long bytesMapaRuta2Nodo = 0;
    for(int i=0; i<gstMapRuta2Nodo.size(); i++){
        bytesMapaRuta2Nodo += sizeof(gstMapRuta2Nodo[i]);
    }
    // Calculo de los bytes para gstMapNodo2Ruta
    unsigned long long bytesMapaNodo2Ruta = 0;
    for(int i=0; i<gstMapNodo2Ruta.size(); i++){
        bytesMapaNodo2Ruta += sizeof(int)*2;
    }
    cout << "gstMapRuta2Nodo [B]: " << bytesMapaRuta2Nodo << endl;
    cout << "gstMapNodo2Ruta [B]: " << bytesMapaNodo2Ruta << endl;
    cout << "gstMFSbv [B]: " << size_in_bytes(gstMFSbv) << endl;
    cout << "gstMFS_rank_1 [B]: " << size_in_bytes(gstMFSrank) << endl;
    cout << "gstMFS_select_1 [B]: " << size_in_bytes(gstMFSselect) << endl;
    cout << "gstMNodos [B]: " << size_in_bytes(gstMNodos) << endl;
    cout << "gstMRamas [B]: " << size_in_bytes(gstMRamas) << endl;

    cout << "**** Elementos ****" << endl;
    cout << "Nº Rutas: " << gstStops.size() << endl;
    cout << "Nº Nodos cst_sada: " << cst.nodes() << endl;
    cout << "Nº Hojas cst_sada: " << cst.size() << endl;
    cout << "Nº 1s/length en marcas: " << bitsUno << "/" << bitsTotal;
    cout << " (" << porcentaje << "%)" << endl;
}

void TopoRelGST_6::sizeToPlot(){
    // Calculo de los bytes para gstMapRuta2Nodo
    unsigned long long bytesMapaRuta2Nodo = 0;
    for(int i=0; i<gstMapRuta2Nodo.size(); i++){
        bytesMapaRuta2Nodo += sizeof(gstMapRuta2Nodo[i]);
    }
    // Calculo de los bytes para gstMapNodo2Ruta
    unsigned long long bytesMapaNodo2Ruta = 0;
    for(int i=0; i<gstMapNodo2Ruta.size(); i++){
        bytesMapaNodo2Ruta += sizeof(int)*2;
    }
    
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
    // Calculo de los bytes para MAPA
    unsigned long long bytesMapa = 0;
    for(int i=0; i<gstMapRuta2Nodo.size(); i++){
        bytesMapa += sizeof(gstMapRuta2Nodo[i]);
    }
    cout << "rutas\tstops\tcstsada\trutas\tstops\tmapR2N\tmapN2R\tgstMFSbv\tgstMNodos\tgstMRamas" << endl;
    cout << n_rutas << "\t"; 
    cout << n_stops << "\t"; 
    cout << size_in_bytes(cst) << "\t";
    cout << bytesMapaRuta2Nodo << "\t";
    cout << bytesMapaNodo2Ruta << "\t";
    cout << bytesStops << "\t";
    cout << bytesMapa << "\t";
    cout << size_in_bytes(gstMFSbv) << "\t";
    cout << size_in_bytes(gstMNodos) << "\t";
    cout << size_in_bytes(gstMRamas) << "\t";
    cout << endl;
}

void TopoRelGST_6::printRutaYPos(int x){
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

void TopoRelGST_6::printRuta(int x){
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

int TopoRelGST_6::getLargoRuta(int x){
    howManygetLargoRuta++;
    //cout << "consultando largo ruta x:" << x << endl;
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

vector<int> TopoRelGST_6::getRuta(int x){
    vector<long unsigned int> iv = extract(cst, gstMapRuta2Nodo[x]);
    vector<int> r(iv.begin(), iv.begin() + getLargoRuta(x));
    return r;
}

bool TopoRelGST_6::iguales(TopoRelGST_6 x){
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

    // Comparando gstMapRuta2Nodo
    if(gstMapRuta2Nodo.size() != x.gstMapRuta2Nodo.size()){
        cout << "gstMapRuta2Nodo tiene distinto tamaño." << endl;
        return false;
    }
    for(int i=0; i<gstMapRuta2Nodo.size(); i++){
        if(gstMapRuta2Nodo[i] != x.gstMapRuta2Nodo[i]){
            cout << "gstMapRuta2Nodo tiene distinto contenido en ruta " << i << endl;
            return false;
        }
    }

    return true;
}

void TopoRelGST_6::statsReset(){
    howManyLCP = 0;
    howManyNodes = 0;
    howManyInserts = 0;
    howManyIfs = 0;
    howManyidRutaDesdeCeldaDeSecConcat = 0;
    howManygetLargoRuta = 0;
    howManyCSTviews = 0;    
}


bool TopoRelGST_6::save(string outputFilename){
    int aux1, aux2;
//    cout << "Guardando el archivo " << outputFilename << endl;
    ofstream outfile(outputFilename, ofstream::binary);
    // Guardando valores enteros
//    cout << n_rutas << endl;
//    cout << n_concat << endl;
//    cout << n_stops << endl;
//    cout << finSec << endl;
    outfile.write((char const*)&n_rutas, sizeof(int));
    outfile.write((char const*)&n_concat, sizeof(int));
    outfile.write((char const*)&n_stops, sizeof(int));
    outfile.write((char const*)&finSec, sizeof(int));
    outfile.write((char const*)&len_min, sizeof(int));
    outfile.write((char const*)&len_max, sizeof(int));
    // Guardando CST
//    cout << cst.nodes() << endl;
//    cout << cst.size() << endl;
    cst.serialize(outfile);
    // Guardando gstMapRuta2Nodo
    aux1 = gstMapRuta2Nodo.size();
    outfile.write((char const*)&aux1, sizeof(int));
    for(int i=0; i<aux1; i++){
        aux2 = cst.id(gstMapRuta2Nodo[i]);
        outfile.write((char const*)&aux2, sizeof(int));
//        cout << aux2 << " ";
    }
//    cout << endl;
    // Guardando gstRutas
    aux1 = gstRutas.size();
    outfile.write((char const*)&aux1, sizeof(int));
    for(int i=0; i<aux1; i++){
        gstRutas[i].serialize(outfile);
/*        for(int j=0; j<gstRutas[i].size(); j++){
            cout << gstRutas[i][j] << " ";
        }
        cout << endl;
*/
    }
    // Guardando gstStops
    aux1 = gstStops.size();
    outfile.write((char const*)&aux1, sizeof(int));
    for(int i=0; i<aux1; i++){
        gstStops[i].serialize(outfile);
/*        for(int j=0; j<gstStops[i].size(); j++){
            cout << gstStops[i][j] << " ";
        }
        cout << endl;
*/
    }
    // Guardando gstMFSbv, rank y select.
    gstMFSbv.serialize(outfile);
    gstMNodos.serialize(outfile);
    gstMRamas.serialize(outfile);
    // Guardando gst
    aux1 = gstMapNodo2Ruta.size();
    outfile.write((char const*)&aux1, sizeof(int));
    for(auto it = gstMapNodo2Ruta.begin(); it != gstMapNodo2Ruta.end(); it++){
        aux2 = it->first;
        outfile.write((char const*)&aux2, sizeof(int));
        aux2 = it->second;
        outfile.write((char const*)&aux2, sizeof(int));
    }
    // Cerrando archivo
    outfile.close();
    return true;
}


/*******************************************************
            FUNCIONALIDADES PRIVADAS
*******************************************************/

bool TopoRelGST_6::bordesSeg_touches(int t1, int t2){
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

cst_sada<>::node_type TopoRelGST_6::nodoSubseq(cst_sada<>::node_type n, int x){
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

int TopoRelGST_6::idRutaDesdeCeldaDeSecConcat(int x){
    howManyidRutaDesdeCeldaDeSecConcat++;
    // Recibe una posición referente a la secuencia concatenada e indica el 
    // id de la secuencia a la que corresponde dicha posición
    // Utiliza operaciones de rank sobre el bitmap de fin de secuencia
    if(x >= gstMFSbv.size()){
        return -1;
    }
    int idT = gstMFSrank(x);
    return (idT % n_rutas);
}

vector<int> TopoRelGST_6::getSecConPrefijoDelNodo(cst_sada<>::node_type nodo, int d){
    // Dado un nodo determina todas las secuencias que tienen prefijo
    // la secuencia del nodo y tienen a lo más su mismo largo
    auto idLChST = cst.leftmost_leaf(nodo);
    auto idRChST = cst.rightmost_leaf(nodo);
    vector<int> y;
    for(int i=cst.id(idLChST); i <= cst.id(idRChST); i++){
        int pos = cst.csa[i];
        if(pos == 0 || gstMFSbv[pos-1] == 1){
            // Es una ruta con prefijo igual al texto del nodo
            int idRuta = gstMFSrank(pos);
            if(getLargoRuta(idRuta) == d){
                y.push_back(idRuta);
//                cout << "Insertando en prefijos: " << idRuta << endl;
            }
        }
    }
    return y;
}
