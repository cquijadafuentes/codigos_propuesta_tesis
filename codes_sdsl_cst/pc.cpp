#include <sdsl/suffix_trees.hpp>
#include <sdsl/bit_vectors.hpp>
#include <fstream>
#include <vector>
#include <string>

using namespace sdsl;
using namespace std;

void mostrar_marcas(vector<bit_vector> marcas, int n_trips, int cstnodos){
    cout << "Marcas:" << endl;
    for(int i=0; i<n_trips; i++){
        cout << " T_" << i << ":";
        for(int j=0; j<cstnodos; j++){
            if(j%5 == 0) cout << " ";
            cout << marcas[i][j];
        }
        cout << endl;
    }
    for(int i=0; i<n_trips; i++){
        cout << "rT_" << i << ":";
        for(int j=0; j<cstnodos; j++){
            if(j%5 == 0) cout << " ";
            cout << marcas[i+n_trips][j];
        }
        cout << endl;
    }
}

int main(int argc, char const *argv[]){
    if(argc < 2){
        cout << "Error! Faltan argumentos. " << endl;
        cout << argv[0] << " <file_input>" << endl;
        return 0;
    }

    // Leer información
    cout << "Leyendo desde archivo: " << argv[1] << endl;
    ifstream entrada;
    entrada.open(argv[1]);
    int n_nodos, n_trips;
    entrada >> n_nodos;
    vector<char> nodos(n_nodos);
    for(int i=0; i<n_nodos; i++){
        entrada >> nodos[i];
    }
    entrada >> n_trips;
    vector<string> trips(n_trips*2);
    for(int i=0; i<n_trips; i++){
        entrada >> trips[i];
    }

    // Mostrar información
    cout << "Nodos: ";
    for(int i=0; i<n_nodos; i++){
        cout << nodos[i] << " - ";
    }
    cout << endl;

    cout << "Trips:" << endl;
    for(int i=0; i<n_trips; i++){
        cout << "\t" << trips[i] << endl;
    }

    // Preparar string concatenado
    string concatenacion = "";
    int endchar=32;
    for(int i=0; i<n_trips; i++){
        concatenacion += trips[i] + (char)(endchar++);
    }
    string revtrip;
    for(int i=0; i<n_trips; i++){
        revtrip = trips[i];
        reverse(revtrip.begin(), revtrip.end());
        trips[i+n_trips] = revtrip;
        concatenacion += revtrip + (char)(endchar++);
    }
    cout << "Concatenación" << endl << "\t" << concatenacion << endl;

    // Construir GST
    cst_sct3<> cst;
    construct_im(cst, concatenacion, 1);
    
    cout << "Información del CompressedSuffixTree:" << endl;
    cout << "Cantidad de nodos: " << cst.nodes() << endl;
    cout << "Cantidad de hojas: " << cst.size() << endl;
    cout << endl;
    cout << "BFS del CompressedSuffixTree:" << endl;
    cout << "id\ted_1\tdeg\tdep\tndep\tsize\tlb\trb\tsun\tleaf\ttext" << endl;
    typedef cst_bfs_iterator<cst_sct3<>> iterator;
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
            cout << cst.edge(*it, i);
        }
        cout << "\t" << endl;

        if(++count % 5 == 0){
            cout << endl;
        }
    }

    // Marcas en bitvectors
    vector<bit_vector> marcas(n_nodos*2, bit_vector(cst.nodes(), 0));
//    mostrar_marcas(marcas, n_trips, cst.nodes());

    for(int i=0; i<trips.size(); i++){
//        cout << "Marcando para trip: " << trips[i] << endl;
        for(int j=0; j < trips[i].length(); j++){
//            cout << " - " << endl;
            auto v = cst.child(cst.root(), trips[i][0+j]);
//            cout << cst.id(v) << " -> " << cst.depth(v) << " < " << trips[i].length() << endl;
            while(v != cst.root() && cst.depth(v) <= trips[i].length()-j){
                marcas[i][cst.id(v)] = 1;
//                cout << "Marcando nodo " << i << "," << cst.id(v) << endl;
                if(cst.depth(v) < trips[i].length()-j){
                    v = cst.child(v, trips[i][cst.depth(v)+j]);
                }else{
                    break;
                }
            }
        }
    }
    mostrar_marcas(marcas, n_trips, cst.nodes());

    // MAP en un vector
    vector<cst_sct3<>::node_type> mapa(n_trips*2);
    for(int i=0; i<trips.size(); i++){
        auto v = cst.child(cst.root(), trips[i][0]);
        while(v != cst.root() && cst.depth(v) < trips[i].length()){
            v = cst.child(v, trips[i][cst.depth(v)]);
        }
        mapa[i] = v;
    }
    cout << "Mapa de trayectorias:" << endl;
    for(int i=0; i<n_trips; i++){
        cout << " T_" << i << " " << cst.id(mapa[i]) << endl;
        cout << "rT_" << i << " " << cst.id(mapa[i+n_trips]) << endl;
    }

    // Determinar relación topológica entre dos trayectorias
    cout << "trayectorias: " << endl;
    for(int i=0; i<n_trips; i++){
        cout << i << "\t" << "T_" << i << ": " << trips[i] << endl;
    }


    cout << "Ingrese dos índices entre 0 y " << n_trips-1 << ": ";
    int x, y;
    while(cin >> x >> y && x >= 0 && x < n_trips && y >= 0 && y < n_trips){
        cout << "Calculando relación topológica entre " << trips[x] << " y " << trips[y] << endl;
        // Identificar contención según nodo
        // Equal, inside, includes, coveredBy, covers
        int corto, largo;
        if(trips[x].length() < trips[y].length()){
            corto = x;
            largo = y;
        }else{
            corto = y;
            largo = x;
        }
//        cout << "id_corto: " << cst.id(mapa[corto]) << " / " << cst.id(mapa[corto+n_trips]) << endl;
//        cout << "id_largo: " << cst.id(mapa[largo]) << " / " << cst.id(mapa[largo+n_trips]) << endl;
        if(mapa[largo] == mapa[corto]){
            cout << "Equals" << endl;
        }else if(mapa[largo] == mapa[corto+n_trips]){
            cout << "Equals (reverse)" << endl;
        }else{
            int id_corto = cst.id(mapa[corto]);
            if(marcas[largo][id_corto] == 1 || marcas[largo+n_trips][id_corto] == 1){
                // Hay contensión
                auto v1 = cst.lca(mapa[corto], mapa[largo]);
                auto v2 = cst.lca(mapa[corto], mapa[largo+n_trips]);
                auto v3 = cst.lca(mapa[corto+n_trips], mapa[largo]);
                auto v4 = cst.lca(mapa[corto+n_trips], mapa[largo+n_trips]);
                auto v12 = (cst.depth(v1) > cst.depth(v2)) ? v1 : v2;
                auto v34 = (cst.depth(v3) > cst.depth(v4)) ? v3 : v4;
                auto v1234 = (cst.depth(v12) > cst.depth(v34)) ? v12 : v34;

                if(v1234 == mapa[corto] || v1234 == mapa[corto+n_trips]){
                    if(corto == x){
                        cout << "CoveredBy" << endl;
                    }else{
                        cout << "Covers" << endl;
                    }
                }else{
                    if(corto == x){
                        cout << "Inside" << endl;
                    }else{
                        cout << "Includes" << endl;
                    }
                }
            }else{
                // Identificar otras
                // Touches, Overlaps, Disjoint
                bool touches = false;
                bool overlaps = false;
                auto root = cst.root();
                for(int i=1; i<trips[corto].length() -1; i++){
                    auto ch = cst.child(root, trips[corto][i]);
                    if(ch != root && marcas[largo][cst.id(ch)]){
                        if(trips[corto][i] == trips[largo][0] ||
                                trips[corto][i] == trips[largo+n_trips][0]){
                            touches = true;
                        }else{
                            cout << "Overlaps" << endl;
                            overlaps = true;
                        }
                    }
                }
                int idch = cst.id(cst.child(root, trips[corto][0]));
                if(marcas[largo][idch] == 1){
                    touches = true;
                }
                idch = cst.id(cst.child(root, trips[corto+n_trips][0]));
                if(marcas[largo][idch] == 1){
                    touches = true;
                }
                if(!overlaps && touches){
                    cout << "Touches" << endl;
                }else if(!overlaps && !touches){
                    cout << "Disjoint" << endl;
                }
            }
        }
        cout << "Ingrese dos índices entre 0 y " << n_trips-1 << ": ";
    }
    cout << "Fin!" << endl;
	return 0;
}