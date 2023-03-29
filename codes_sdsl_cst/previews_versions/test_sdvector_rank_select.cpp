#include <iostream>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
    int n_rutas, n_t, n_stops, aux;
    vector<vector<int>> rutas;
    cin >> n_rutas >> n_stops;
    int secConcat = 0;
    cout << "Secuencia inicial: " << endl;
    for(int j = 0; j < n_rutas; j++){
        cin >> n_t;
        vector<int> vt(n_t, 0);
        for(int i = 0; i < n_t; i++){
            cin >> vt[i];
            cout << vt[i] << " ";
        }
        cout << n_stops+1 << " ";
        rutas.push_back(vt);
        secConcat += n_t;
    }
    cout << endl;

    secConcat += n_rutas;

    bit_vector bv = bit_vector(secConcat, 0);
    int pBV = 0;
    for(int i=0; i<n_rutas; i++){
        pBV += rutas[i].size();
        bv[pBV++] = 1;
    }

    cout << "bit_vector:" << endl;
    for(int i=0; i<bv.size(); i++){
        cout << bv[i];
    }
    cout << endl;

    rank_support_v<1> bv_rank(&bv);
    cout << "bv_rank_1:" << endl;
    for(int i=0; i <= bv.size(); i++){
        cout << bv_rank(i);
    }
    cout << endl;

    bit_vector::select_1_type bv_select(&bv);
    cout << "bv_select_1:" << endl;
    for(int i=1; i <= n_rutas; i++){
        cout << bv_select(i) << " ";
    }
    cout << endl;


    sd_vector<> sdbv(bv);
    cout << "sd_vector:" << endl;
    for(int i=0; i<sdbv.size(); i++){
        cout << sdbv[i];
    }
    cout << endl;

    sd_vector<>::rank_1_type sdb_rank(&sdbv);
    cout << "sd_rank_1:" << endl;
    for(int i=0; i <= sdbv.size(); i++){
        cout << sdb_rank(i);
    }
    cout << endl;

    sd_vector<>::select_1_type sdb_select(&sdbv);
    cout << "sd_select_1:" << endl;
    for(int i=1; i <= n_rutas; i++){
        cout << sdb_select(i) << " ";
    }    
    cout << endl;

    cout << "==========Tamaños==========" << endl;
    cout << "bits en bitmap: " << bv.size() << endl;
    cout << "bit_vector: " << size_in_bytes(bv) << "[B]" << endl;
    cout << "bv_rank_1: " << size_in_bytes(bv_rank) << "[B]" << endl;
    cout << "bv_select_1: " << size_in_bytes(bv_select) << "[B]" << endl;
    cout << "sd_vector: " << size_in_bytes(sdbv) << "[B]" << endl;
    cout << "sdv_rank_1: " << size_in_bytes(sdb_rank) << "[B]" << endl;
    cout << "sdv_select_1: " << size_in_bytes(sdb_select) << "[B]" << endl;

    return 0;
}