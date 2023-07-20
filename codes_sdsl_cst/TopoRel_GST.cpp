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

#include "TopoRel_GST.hpp"

TopoRelGST::TopoRelGST(vector<vector<int>> &rutas, int cant_stops){
    statsReset();
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
    int_vector<> iv(n_concat);
    bit_vector MFStemporal = bit_vector(n_concat, 0);
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
        for(int j = 0; j < rutas[i].size(); j++){
            iv[pv++] = rutas[i][j];
        }
        MFStemporal[pv] = 1;
        iv[pv++] = finSec;
    }
    gstMFSbv = sd_vector<>(MFStemporal);
    gstMFSrank = sd_vector<>::rank_1_type(&gstMFSbv);
    gstMFSselect = sd_vector<>::select_1_type(&gstMFSbv);

    construct_im(cst, iv);
//    cout << "GST construido" << endl;
    bit_vector MNtemporal = bit_vector(cst.nodes(), 0);
    bit_vector MRtemporal = bit_vector(cst.nodes(), 0);


    // MAP en un vector
    int maxThreads = omp_get_max_threads();
    if(maxThreads > n_rutas){
        maxThreads = n_rutas;
    }
    // Vector que contendrá los vectores de cada porción que se genere según la cantidad de threads
    int porciones = n_rutas / maxThreads;
    if((porciones * maxThreads) < n_rutas){
        porciones++;
    }
    vector<vector<cst_sada<>::node_type>> vPorcMap(maxThreads);
    cout << "N° Threads: " << maxThreads << endl;
    #pragma omp parallel for
    for(int iParalelo = 0; iParalelo < maxThreads; iParalelo++){
        vPorcMap[iParalelo] = vector<cst_sada<>::node_type>(porciones);
        int base = iParalelo * porciones;
        int tope = base + porciones;
        if(tope > n_rutas){
            tope = n_rutas;
        }
        for(int i = base; i < tope; i++){
            auto v = cst.root();
            v = cst.child(cst.root(), rutas[i][0]);
            while(v != cst.root() && cst.depth(v) < rutas[i].size()){
                v = cst.child(v, rutas[i][cst.depth(v)]);
            }
            if(cst.depth(v) <= rutas[i].size()){
                v = cst.child(v, finSec);
            }
            vPorcMap[iParalelo][i-base] = v;
            MNtemporal[cst.id(v)] = 1;
            auto mr = cst.parent(v);
            MRtemporal[cst.id(mr)] = 1;
            while(mr != cst.root()){
                mr = cst.parent(mr);
                MRtemporal[cst.id(mr)] = 1;
            }
        }
    }

    // Unir resultados obtenidos en la paralelización en el objeto correspondiente
    int sobrantes = (porciones * maxThreads) - n_rutas;
    for(int i=0; i < maxThreads; i++){
        gstMapRuta2Nodo.insert(gstMapRuta2Nodo.end(), vPorcMap[i].begin(), vPorcMap[i].end());
    }

    if(sobrantes > 0){
        gstMapRuta2Nodo.erase(gstMapRuta2Nodo.end() - sobrantes, gstMapRuta2Nodo.end());
    }

    gstMNodos = sd_vector<>(MNtemporal);
    gstMRamas = sd_vector<>(MRtemporal);

    // Llenar gstMapNodo2Rutas
    //int insertos = 0;
    for(int i=0; i<gstMapRuta2Nodo.size(); i++){
        gstMapNodo2Ruta.insert(make_pair(cst.id(gstMapRuta2Nodo[i]), i));
    }
}

TopoRelGST::TopoRelGST(string inputFilename){
    statsReset();
    ifstream infile(inputFilename, ofstream::binary);
    if(infile){
        int aux1, aux2;
        // Cargando valores enteros
        infile.read ((char *)&n_rutas,sizeof(int));
        infile.read ((char *)&n_concat,sizeof(int));
        infile.read ((char *)&n_stops,sizeof(int));
        infile.read ((char *)&finSec,sizeof(int));
        infile.read ((char *)&len_min,sizeof(int));
        infile.read ((char *)&len_max,sizeof(int));
        // Cargando CST
        cst.load(infile);
        // Cargando gstMapRuta2Nodo
        infile.read ((char *)&aux1,sizeof(int));
        gstMapRuta2Nodo = vector<cst_sada<>::node_type>(aux1);
        for(int i=0; i<aux1; i++){
            infile.read ((char *)&aux2,sizeof(int));
            gstMapRuta2Nodo[i] = cst.inv_id(aux2);
        }
        // Cargando gstMapNodo2Ruta
        infile.read ((char *)&aux1,sizeof(int));
        for(int i=0; i<aux1; i++){
            int a,b;
            infile.read ((char *)&a,sizeof(int));
            infile.read ((char *)&b,sizeof(int));
            gstMapNodo2Ruta.insert(make_pair(a,b));
        }
        // Cargando gstMFSbv, rank y select.
        gstMFSbv.load(infile);
        gstMNodos.load(infile);
        gstMRamas.load(infile);
        // Cerrando archivo
        infile.close();
        // Construyendo rank y select de MFS
        gstMFSrank = sd_vector<>::rank_1_type(&gstMFSbv);
        gstMFSselect = sd_vector<>::select_1_type(&gstMFSbv);
        return;
    }    
    cout << "Error en la carga!" << endl;
}

bool TopoRelGST::save(string outputFilename){
    int aux1, aux2;
    ofstream outfile(outputFilename, ofstream::binary);
    // Guardando valores enteros
    outfile.write((char const*)&n_rutas, sizeof(int));
    outfile.write((char const*)&n_concat, sizeof(int));
    outfile.write((char const*)&n_stops, sizeof(int));
    outfile.write((char const*)&finSec, sizeof(int));
    outfile.write((char const*)&len_min, sizeof(int));
    outfile.write((char const*)&len_max, sizeof(int));
    // Guardando CST
    cst.serialize(outfile);
    // Guardando gstMapRuta2Nodo
    aux1 = gstMapRuta2Nodo.size();
    outfile.write((char const*)&aux1, sizeof(int));
    for(int i=0; i<aux1; i++){
        aux2 = cst.id(gstMapRuta2Nodo[i]);
        outfile.write((char const*)&aux2, sizeof(int));
    }
    // Guardando gstMapNodo2Ruta
    aux1 = gstMapNodo2Ruta.size();
    outfile.write((char const*)&aux1, sizeof(int));
    for(auto it = gstMapNodo2Ruta.begin(); it != gstMapNodo2Ruta.end(); it++){
        aux2 = it->first;
        outfile.write((char const*)&aux2, sizeof(int));
        aux2 = it->second;
        outfile.write((char const*)&aux2, sizeof(int));
    }
    // Guardando gstMFSbv, rank y select.
    gstMFSbv.serialize(outfile);
    gstMNodos.serialize(outfile);
    gstMRamas.serialize(outfile);
    // Cerrando archivo
    outfile.close();
    return true;
}


/*******************************************************
            Relaciones topológicas en conjunto
*******************************************************/


vector<int> TopoRelGST::tr_allContain(int x){
    vector<int> y;
    if(x > n_rutas) return y;

    auto nX = gstMapRuta2Nodo[x];
    auto idLChST = cst.lb(nX);
    auto idRChST = cst.rb(nX);
    auto nParentX = cst.parent(nX);

    if(cst.depth(nParentX) == getLargoRuta(x)){
        idLChST = cst.lb(nParentX);
        idRChST = cst.rb(nParentX);
    }
    for(int i=idLChST; i <= idRChST; i++){
        int idAux = gstMFSrank(cst.csa[i]); // Equivalente a la función idRutaDesdeCeldaDeSecConcat(cst.csa[i])
        y.push_back(idAux);
    }
    return y;
}


vector<int> TopoRelGST::tr_allEqual(int x){
    vector<int> y;
    if(x > n_rutas) return y;

    int nX = cst.id(gstMapRuta2Nodo[x]);
    int count = gstMapNodo2Ruta.count(nX);
    auto rango = gstMapNodo2Ruta.equal_range(nX);
    for(auto j=rango.first; j!=rango.second; j++){
        y.push_back(j->second);
        howManyInserts++;
        howManyIfs++;
    }
    return y;
}


vector<int> TopoRelGST::tr_allContained(int x){
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
        // Verificar si hay secuencia desde nodo en bottom-up
        auto nodoAux = nodo;        // Se usa para navegación por cst.parent
        int nodoAuxID = nodoID;
        int nodoAuxDepth = nodoDepth;
        while(nodoAuxDepth >= len_min){
            // El largo permite una secuencia
            auto nodoExp = nodoAux;     // Se usa para verificar los nodos con secuencia
            int nodoExpID = nodoAuxID;
            int nodoExpDepth = nodoAuxDepth;
            howManyIfs++;
            if(gstMNodos[nodoExpID] != 1){
                nodoExp = cst.child(nodoAux, finSec);
                howManyNodes++;
                nodoExpID = cst.id(nodoExp);
                nodoExpDepth = cst.depth(nodoExp);
            }
            howManyIfs++;
            if(nodoExp != raiz && gstMNodos[nodoExpID]){
                // Hay nodos marcados en nodoExp
                int count = gstMapNodo2Ruta.count(nodoExpID);
                auto rango = gstMapNodo2Ruta.equal_range(nodoExpID);
                for(auto j=rango.first; j!=rango.second; j++){
                    setRes.insert(j->second);
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
        nodo = cst.sl(nodo);
        howManyNodes++;
        nodoID = cst.id(nodo);
        nodoDepth = cst.depth(nodo);
        howManyIfs++;
    }
    vector<int> res(setRes.begin(), setRes.end());
    return res;
}


vector<int> TopoRelGST::tr_allIntersect(int x){
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
            setRes.insert(gstMFSrank(cst.csa[j]));
            howManyIfs++;
        }
        howManyIfs++;
    }

    vector<int> res(setRes.begin(), setRes.end());
    return res;
}

unordered_map<int,int> TopoRelGST::tr_allIntersectPP(int x, int minimo){
    unordered_map<int,int> res;
    int ls = getLargoRuta(x);
    auto nodoSL = gstMapRuta2Nodo[x];
    if(ls >= minimo){
        res[x] = ls;
    }
    int lcpAux;
    for(int i=0; i <= ls - minimo; i++){
        // Se realizan operaciones de suffix link para recorrer intersecciones
        auto nodo = cst.rightmost_leaf(nodoSL);
        int idHoja = cst.id(nodo);
        // Revisión del SA hacia el inicio
        int idNav = idHoja;
        int coincidencia = ls-i;
        while(coincidencia >= minimo){
            int pos = cst.csa[idNav];
            int idSecCand = gstMFSrank(pos); // Para determinar el ID de la ruta
            if(res[idSecCand] < coincidencia){
                res[idSecCand] = coincidencia;
            }
            howManyLCP++;
            lcpAux = cst.lcp[idNav];
            if(lcpAux < coincidencia){
                coincidencia = lcpAux;
            }
            idNav--;
        }

        // Revisión del SA hacia el final
        idNav = idHoja+1;
        howManyLCP++;
        coincidencia = cst.lcp[idNav];
        if(coincidencia > ls-i){
            coincidencia = ls-i;
        }
        while(coincidencia >= minimo){
            int pos = cst.csa[idNav];
            int idSecCand = gstMFSrank(pos); // Para determinar el ID de la ruta
            if(res[idSecCand] < coincidencia){
                res[idSecCand] = coincidencia;
            }
            idNav++;
            howManyLCP++;
            lcpAux = cst.lcp[idNav];
            if(lcpAux < coincidencia){
                coincidencia = lcpAux;
            }
        }

        // Suffix link para el siguiente ciclo
        nodoSL = cst.sl(nodoSL);
    }
    return res;
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

void TopoRelGST::sizeEstructura(){
    cout << "**** Tamaño en bytes ****" << endl;
    cout << "cst_sada [B]: " << size_in_bytes(cst) << endl;
    // Calculo de los bytes para gstMapRuta2Nodo
    unsigned long long bytesMapaRuta2Nodo = sizeof(int)*gstMapRuta2Nodo.size();
    cout << "gstMapRuta2Nodo [B]: " << bytesMapaRuta2Nodo << endl;
    // Calculo de los bytes para gstMapNodo2Ruta
    unsigned long long bytesMapaNodo2Ruta = 0;
    for(int i=0; i<gstMapNodo2Ruta.size(); i++){
        bytesMapaNodo2Ruta += sizeof(int)*2;
    }
    cout << "gstMapNodo2Ruta [B]: " << bytesMapaNodo2Ruta << endl;
    cout << "gstMFSbv [B]: " << size_in_bytes(gstMFSbv) << endl;
    cout << "gstMFS_rank_1 [B]: " << size_in_bytes(gstMFSrank) << endl;
    cout << "gstMFS_select_1 [B]: " << size_in_bytes(gstMFSselect) << endl;
    cout << "gstMNodos [B]: " << size_in_bytes(gstMNodos) << endl;
    cout << "gstMRamas [B]: " << size_in_bytes(gstMRamas) << endl;

    cout << "**** Elementos ****" << endl;
    cout << "Nº Rutas: " << n_rutas << endl;
    cout << "Nº Nodos cst_sada: " << cst.nodes() << endl;
    cout << "Nº Hojas cst_sada: " << cst.size() << endl;
}

void TopoRelGST::sizeToPlot(){
    // Calculo de los bytes para gstMapRuta2Nodo
    unsigned long long bytesMapaRuta2Nodo = sizeof(int)*gstMapRuta2Nodo.size();
    // Calculo de los bytes para gstMapNodo2Ruta
    unsigned long long bytesMapaNodo2Ruta = 0;
    for(int i=0; i<gstMapNodo2Ruta.size(); i++){
        bytesMapaNodo2Ruta += sizeof(int)*2;
    }
    int bytesTotales = 0;
    bytesTotales += size_in_bytes(cst);
    bytesTotales += bytesMapaRuta2Nodo;
    bytesTotales += bytesMapaNodo2Ruta;
    bytesTotales += size_in_bytes(gstMFSbv);
    bytesTotales += size_in_bytes(gstMNodos);
    bytesTotales += size_in_bytes(gstMRamas);

    cout << "rutas\tstops\tcstsada\tmapR2N\tmapN2R\tgstMFSbv\tgstMNodos\tgstMRamas\tTOTAL" << endl;
    cout << n_rutas << "\t"; 
    cout << n_stops << "\t"; 
    cout << size_in_bytes(cst) << "\t";
    cout << bytesMapaRuta2Nodo << "\t";
    cout << bytesMapaNodo2Ruta << "\t";
    cout << size_in_bytes(gstMFSbv) << "\t";
    cout << size_in_bytes(gstMNodos) << "\t";
    cout << size_in_bytes(gstMRamas) << "\t";
    cout << bytesTotales << "\t";
    cout << endl;
}

void TopoRelGST::printRutaYPos(int x){
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

void TopoRelGST::printRuta(int x){
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

int TopoRelGST::getLargoRuta(int x){
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

vector<int> TopoRelGST::getRuta(int x){
    vector<long unsigned int> iv = extract(cst, gstMapRuta2Nodo[x]);
    vector<int> r(iv.begin(), iv.begin() + getLargoRuta(x));
    return r;
}

bool TopoRelGST::iguales(TopoRelGST x){
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

    // Comparando gstMapNodo2Ruta
    // ToDo

    return true;
}

void TopoRelGST::statsReset(){
    howManyLCP = 0;
    howManyNodes = 0;
    howManyInserts = 0;
    howManyIfs = 0;
    howManyidRutaDesdeCeldaDeSecConcat = 0;
    howManygetLargoRuta = 0;
    howManyCSTviews = 0;    
}





/*******************************************************
            FUNCIONALIDADES PRIVADAS
*******************************************************/

cst_sada<>::node_type TopoRelGST::nodoSubseq(cst_sada<>::node_type n, int x){
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

int TopoRelGST::idRutaDesdeCeldaDeSecConcat(int x){
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

vector<int> TopoRelGST::getSecConPrefijoDelNodo(cst_sada<>::node_type nodo, int d){
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
