#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>      // std::fstream
#include <map>
#include <vector>

using namespace std;

// RELACIONES TOPOLÓGICAS NAIVE
void toporel(vector<int> a, vector<int> b);
pair<int,int> KMPSearch(vector<int> pat, vector<int> txt);
void computeLPSArray(vector<int> pat, int M, int* lps);
pair<int,bool> lcs_info(vector<int> s, vector<int> t, int n, int m);


int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! falta nombre archivo a procesar como entrada." << endl;
		return -1;
	}

	fstream entrada;
	entrada.open(argv[1], fstream::in);
	int n_stops;
	entrada >> n_stops;
//	cout << n_stops << endl;

	map<string,int> mapa_stops;
	for(int i = 1; i <= n_stops; i++){
		string id_stop;
		entrada >> id_stop;
		mapa_stops[id_stop] = i;
//		cout << id_stop << " - " << i << endl;
	}

	int n_routes, n_t;
	string l, t;
	entrada >> n_routes;
//	cout << n_routes << endl;
	vector<vector<int>> routes;

	for(int j = 0; j < n_routes; j++){
		entrada >> l;
//		cout << "-----------------" << endl;
//		cout << l << endl;
		n_t = 1;
		for(int i = 0; i < l.length(); i++){
			if(l[i] == ','){
				l[i] = ' ';
				n_t++;
			}
		}

		vector<int> vr(n_t, 0);
//		cout << "-----------------" << endl;
//		cout << l << endl;
		stringstream ss(l);
//		cout << "-----------------" << endl;
		for(int i = 0; i < n_t; i++){
			ss >> t;
//			cout << t << " ";
			vr[i] = mapa_stops[t];
		}
//		cout << endl << "-----------------" << endl;
//		for(int i = 0; i < n_t; i++){
//			cout << vr[i] << " ";
//		}
//		cout << endl << "-----------------" << endl;
		routes.push_back(vr);
	}
	cout << "Total rutas: " << routes.size() << endl;
	cout << "Ruta 1: ";
	for(int i = 0; i < routes[0].size(); i++){
		cout << routes[0][i] << " ";
	}
	cout << endl;
	cout << "Ruta 2: ";
	for(int i = 0; i < routes[n_routes-1].size(); i++){
		cout << routes[n_routes-1][i] << " ";
	}
	cout << endl;
	toporel(routes[0],routes[n_routes-1]);
    return 0;

	
}



//////////////////////////////////////////////////////////////
// ************* RELACIONES TOPOLÓGICAS NAIVE ************* //
//////////////////////////////////////////////////////////////

bool iguales(vector<int> a, vector<int> b){
	if(a.size() != b.size()){
		return false;
	}

	for(int i = 0; i < a.size(); i++){
		if(a[i] != b[i]){
			return false;
		}
	}

	return true;
}

void toporel(vector<int> a, vector<int> b){
    /*
        Se define cuál es la relación topológica entre dos secuencias representadas por vector<int>.
        Las relaciones se comprueban según la complejidad que implica el identificarlas:
        - Equals: comparación lineal de las dos secuencias (considerando que pudiera estar en inverso)
        - Contención (Covered, Covers, Includes, Inside): comparación mediante algoritmo KMP (Knuth-Morris-Pratt)
        - Otras: Por medio de algoritmo de DP para LCS (Longest Common Substring). En este caso no es relevante
            el orden de las intersecciones. Ya se ha revisado la contención y sólo queda verificar la cantidad
            de coincidencias entre las secuencias para determinar entre Touches, Overlaps y Disjoint.
    */

    vector<int> s, t;
    bool sa = false;
    bool sb = false;
    if(a.size() < b.size()){
        s = a;
        t = b;
        sa = true;
    }else{
        t = a;
        s = b;
        sb = true;
    }

    // Se asume que |s| < |t|
    int n = s.size();
    int m = t.size();
    
    // ------- EQUALS -------

    vector<int> rt = t;
    reverse(rt.begin(), rt.end());
    // "Verificación de Igualdad"
    if(n == m && (iguales(s,t) || iguales(s,rt))){
        cout << "Equals " << endl;
        return;
    }

    // ------- CONTENCION -------
    pair<int,int> kmp = KMPSearch(s, t);
    pair<int,int> kmpR = KMPSearch(s, rt);
    if(kmp.first != -1 || kmpR.first != -1){
        int index = (kmp.first != -1) ? kmp.first : kmpR.first;
        if(index == 0 || index == m-n){
            if(sa){
                cout << "CoveredBy" << endl;
            }else{
                cout << "Covers" << endl;
            }
        }else{
            if(sa){
                cout << "Inside" << endl;
            }else{
                cout << "Includes" << endl;
            }
        }
        return;
    }

    if(kmp.second > 1 || kmpR.second > 1){
        cout << "Overlaps" << endl;
        return;
    }


    // ------- OTRAS -------

    pair<int,bool> res = lcs_info(s, t, n, m);

    if(res.first == 0){
        cout << "Disjoint" << endl;
        return;
    }

    if(res.second){
        // Caso en el que hay intersección interior-interior
        cout << "Overlaps" << endl;
        return;
    }

    cout << "Touches" << endl;

    cout << "************** LCS **************" << endl;
    cout << res.first << endl;

}

// Prints occurrences of txt[] in pat[]
pair<int,int> KMPSearch(vector<int> pat, vector<int> txt){
    int M = pat.size();
    int N = txt.size();
  
    // create lps[] that will hold the longest prefix suffix
    // values for pattern
    int lps[M];
  
    // Preprocess the pattern (calculate lps[] array)
    computeLPSArray(pat, M, lps);
  
    int i = 0; // index for txt[]
    int j = 0; // index for pat[]
    int maxJ = 0;
    while (i < N) {
        if (pat[j] == txt[i]) {
            j++;
            i++;
            if(j > maxJ){
                maxJ = j;
            }
        }
  
        if (j == M) {
            // cout << "Found pattern at index " << (i - j) << endl;
            return make_pair(i-j, maxJ);
            //j = lps[j - 1];
        }
  
        // mismatch after j matches
        else if (i < N && pat[j] != txt[i]) {
            // Do not match lps[0..lps[j-1]] characters,
            // they will match anyway
            if (j != 0)
                j = lps[j - 1];
            else
                i = i + 1;
        }
    }
    return make_pair(-1,maxJ);
}
  
// Fills lps[] for given patttern pat[0..M-1]
void computeLPSArray(vector<int> pat, int M, int* lps){
    // length of the previous longest prefix suffix
    int len = 0;
  
    lps[0] = 0; // lps[0] is always 0
  
    // the loop calculates lps[i] for i = 1 to M-1
    int i = 1;
    while (i < M) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        }
        else // (pat[i] != pat[len])
        {
            // This is tricky. Consider the example.
            // AAACAAAA and i = 7. The idea is similar
            // to search step.
            if (len != 0) {
                len = lps[len - 1];
  
                // Also, note that we do not increment
                // i here
            }
            else // if (len == 0)
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

pair<int,bool> lcs_info(vector<int> s, vector<int> t, int n, int m){

    // Create DP table
    int dp[2][m + 1];
    int res = 0;
    bool ii_intersect = false;

    cout << "************** Tabla PD **************" << endl;
    cout << endl << " +";
    for (int i=0; i<m; i++){
        cout << " " << t[i];
    }
    cout << endl;
    for (int i = 1; i <= n; i++) {
        cout << " " << s[i-1];
        for (int j = 1; j <= m; j++) {
            if (s[i - 1] == t[j - 1]) {
                dp[i % 2][j] = dp[(i - 1) % 2][j - 1] + 1;
                if (dp[i % 2][j] > res){
                    res = dp[i % 2][j];
                }
                if(i > 1 && i < n && j > 1 && j < m){
                    ii_intersect = true;
                }
            }
            else{
                dp[i % 2][j] = 0;
            }
            cout << " " << dp[i % 2][j] ;
        }
        cout << endl;
    }

    cout << "Intersección interior-interior: " << ii_intersect << endl;

    return make_pair(res,ii_intersect);
}




/////////////////////////////////////////////////////////////
// ************* RELACIONES TOPOLÓGICAS GST_ ************* //
/////////////////////////////////////////////////////////////

