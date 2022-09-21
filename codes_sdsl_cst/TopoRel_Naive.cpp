#include "TopoRel_Naive.hpp"

//////////////////////////////////////////////////////////////
// ************* RELACIONES TOPOLÓGICAS NAIVE ************* //
//////////////////////////////////////////////////////////////

bool iguales(vector<int> &a, vector<int> &b){
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

bool bordesSeg_touches(vector<int> &a, vector<int> &b){
    // Comprueba si hay segmentos finales que se intersecan
    // Esto implica una intersección Interior-Interior entre los bordes
    // que se considera como un touches falso-positivo

    int bff = b.size() - 1;
    int bpf = bff - 1;
    if((a[0] == b[1] && a[1] == b[0]) ||
            (a[0] == b[bpf] && a[1] == b[bff])){
        return true;
    }

    int aff = a.size() - 1;
    int apf = aff - 1;
    if((a[aff] == b[1] && a[apf] == b[0]) ||
            (a[aff] == b[bpf] && a[apf] == b[bff])){
        return true;
    }

    return false;
}

string toporel(vector<int> &a, vector<int> &b){
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

    // Se tiene que |s| < |t|
    int n = s.size();
    int m = t.size();
    
    // ------- EQUALS -------

    vector<int> rt = t;
    reverse(rt.begin(), rt.end());
    // "Verificación de Igualdad"
    if(n == m && (iguales(s,t) || iguales(s,rt))){
        return EQUALS;
    }

    // ------- CONTENCION -------
    pair<int,int> kmp = KMPSearch(s, t);
    pair<int,int> kmpR = KMPSearch(s, rt);
    if(kmp.first != -1 || kmpR.first != -1){
        int index = (kmp.first != -1) ? kmp.first : kmpR.first;
        if(index == 0 || index == m-n){
            if(sa){
                return COVEREDBY;
            }else{
                return COVERS;
            }
        }else{
            if(sa){
                return INSIDE;
            }else{
                return INCLUDES;
            }
        }
    }

    if(kmp.second > 2 || kmpR.second > 2){
        return OVERLAPS;
    }


    // ------- OTRAS -------

    pair<int,bool> res = lcs_info(s, t);

    if(res.first == 0){
        return DISJOINT;
    }

    if(res.second || bordesSeg_touches(a,b)){
        // Caso en el que hay intersección interior-interior
        return OVERLAPS;
    }
    return TOUCHES;

}

// Find occurrences of txt[] in pat[]
// Retorna: <posición,largo> de la coincidencia
pair<int,int> KMPSearch(vector<int> &pat, vector<int> &txt){
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
void computeLPSArray(vector<int> &pat, int M, int* lps){
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

// Retorna: <largo_mayor_coindicencia,intersección_interior-interior>;
pair<int,bool> lcs_info(vector<int> &s, vector<int> &t){
    int n = s.size();
    int m = t.size();
    // Create DP table
    int dp[2][m + 1];
    int res = 0;
    bool ii_intersect = false;

    for(int i = 0; i <= m; i++){
        dp[0][i] = 0;
        dp[1][i] = 0;
    }
/*
    cout << "************** Tabla PD **************" << endl;
    cout << endl << " +";
    for (int i=0; i<m; i++){
        cout << " " << t[i];
    }
    cout << endl;
*/
    for (int i = 1; i <= n; i++) {
//        cout << " " << s[i-1];
        for (int j = 1; j <= m; j++) {
            if (s[i - 1] == t[j - 1]) {
                dp[i % 2][j] = dp[(i - 1) % 2][j - 1] + 1;
                if (dp[i % 2][j] == res + 1){
                    res = dp[i % 2][j];
                }
                if(i > 1 && i < n && j > 1 && j < m){
                    ii_intersect = true;
//                    cout << "ii_intersect: true por " << i << " " << j << endl;
                }
            }
            else{
                dp[i % 2][j] = 0;
            }
//            cout << " " << dp[i % 2][j] ;
        }
//        cout << endl;
    }
//    cout << "Resultado devuelto: " << res << endl;
//    cout << "Intersección interior-interior: " << ii_intersect << endl;

    return make_pair(res,ii_intersect);
}


/*******************************************************
            8 Relaciones topológicas básicas
*******************************************************/


bool tr_equals(vector<int> &a, vector<int> &b){
    if(a.size() != b.size()){
        return false;
    }
    int fallasAB = 0;
    int fallasBA = 0;
    int aS = a.size();
    for(int i = 0; i < aS; i++){
        if(a[i] != b[i]){
            fallasAB++;
        }
        if(a[i] != b[aS-1-i]){
            fallasBA++;
        }
    }
    return !(fallasAB && fallasBA);
}

bool tr_coveredby(vector<int> &a, vector<int> &b){
    if(a.size() >= b.size()){
        return false;
    }
    int n = a.size();
    int m = b.size();
    
    // ------- EQUALS -------

    vector<int> rb = b;
    reverse(rb.begin(), rb.end());
    
    pair<int,int> kmp = KMPSearch(a, b);
    pair<int,int> kmpR = KMPSearch(a, rb);
    if(kmp.first != -1 || kmpR.first != -1){
        int index = (kmp.first != -1) ? kmp.first : kmpR.first;
        if(index == 0 || index == m-n){
            return true;
        }
    }
    return false;
}

bool tr_covers(vector<int> &a, vector<int> &b){
    if(b.size() >= a.size()){
        return false;
    }
    int n = b.size();
    int m = a.size();
    
    // ------- EQUALS -------

    vector<int> ra = a;
    reverse(ra.begin(), ra.end());
    
    pair<int,int> kmp = KMPSearch(b, a);
    pair<int,int> kmpR = KMPSearch(b, ra);
    if(kmp.first != -1 || kmpR.first != -1){
        int index = (kmp.first != -1) ? kmp.first : kmpR.first;
        if(index == 0 || index == m-n){
            return true;
        }
    }
    return false;
}

bool tr_disjoint(vector<int> &a, vector<int> &b){
    // Bordes de las secuencias
    int a_bi = a[0];
    int b_bi = b[0];
    int a_bf = a[a.size()-1];
    int b_bf = b[b.size()-1];
    // Verificar intersección bordes-bordes
    if(a_bi == b_bi || a_bi == b_bf){
        return false;
    }
    if(a_bf == b_bi || a_bf == b_bf){
        return false;
    }
    // Copia de los vectores
    vector<int> ca(a.begin(), a.end());
    vector<int> cb(b.begin(), b.end());
    // Ordenar los vectores
    sort(ca.begin(), ca.end());
    sort(cb.begin(), cb.end());
    // Verificar intersección
    int i=0, j=0;
    while(i<ca.size() && j<cb.size()){
        if(ca[i] == cb[j]){
            // Intersección interior-interior detectada
            return false;
        }else if(ca[i] < cb[j]){
            i++;
        }else{
            j++;
        }
    }

    return true;
}

bool tr_includes(vector<int> &a, vector<int> &b){
    if(b.size() >= a.size()){
        return false;
    }
    int n = b.size();
    int m = a.size();
    
    // ------- EQUALS -------

    vector<int> ra = a;
    reverse(ra.begin(), ra.end());
    
    pair<int,int> kmp = KMPSearch(b, a);
    pair<int,int> kmpR = KMPSearch(b, ra);
    if(kmp.first != -1 || kmpR.first != -1){
        int index = (kmp.first != -1) ? kmp.first : kmpR.first;
        if(index != 0 && index != m-n){
            return true;
        }
    }
    return false;
}

bool tr_inside(vector<int> &a, vector<int> &b){
    if(a.size() >= b.size()){
        return false;
    }
    int n = a.size();
    int m = b.size();
    
    // ------- EQUALS -------

    vector<int> rb = b;
    reverse(rb.begin(), rb.end());
    
    pair<int,int> kmp = KMPSearch(a, b);
    pair<int,int> kmpR = KMPSearch(a, rb);
    if(kmp.first != -1 || kmpR.first != -1){
        int index = (kmp.first != -1) ? kmp.first : kmpR.first;
        if(index != 0 && index != m-n){
            return true;
        }
    }
    return false;
}

bool tr_overlaps(vector<int> &a, vector<int> &b){
    // Descartar contención
    int n,m;
    pair<int,int> kmp;
    pair<int,int> kmpR;

    if(a.size() < b.size()){
        n = a.size();   
        m = b.size();
        vector<int> rb = b;
        reverse(rb.begin(), rb.end());
        kmp = KMPSearch(a, b);
        kmpR = KMPSearch(a, rb);
    }else{
        n = b.size();   
        m = a.size();
        vector<int> ra = a;
        reverse(ra.begin(), ra.end());
        kmp = KMPSearch(b, a);
        kmpR = KMPSearch(b, ra);
    }
    if(kmp.first != -1 || kmpR.first != -1){
        int index = (kmp.first != -1) ? kmp.first : kmpR.first;
        // Hay contención
        return false;
    }
    
    /*
        A este punto existe intersección exterior-exterior entre 
        las secuencias ya que no hay contensión entre ellas.
        Si hubiera intersección interior-interior se determina la
        relación overlap.
    */

    // Verificar intersección interior-interior
    // Copia de los vectores sin los extremos
    vector<int> aInt(a.begin()+1, a.end()-1);
    vector<int> bInt(b.begin()+1, b.end()-1);
    // Ordenar los vectores
    sort(aInt.begin(), aInt.end());
    sort(bInt.begin(), bInt.end());
    // Verificar intersección interior-interior
    int i=0, j=0;
    while(i<aInt.size() && j<bInt.size()){
        if(aInt[i] == bInt[j]){
            // Intersección interior-interior detectada
            return true;
        }else if(aInt[i] < bInt[j]){
            i++;
        }else{
            j++;
        }
    }

    return false;
}

bool tr_touches(vector<int> &a, vector<int> &b){
    // Descartar igualdad
    if(a.size() == b.size()){
        vector<int> ra(a);
        reverse(ra.begin(), ra.end());
        if(iguales(a, b) || iguales(ra, b)){
            return false;
        }
    }

    // Copia de los vectores sin los extremos
    vector<int> aInt(a.begin()+1, a.end()-1);
    vector<int> bInt(b.begin()+1, b.end()-1);
    // Ordenar los vectores
    sort(aInt.begin(), aInt.end());
    sort(bInt.begin(), bInt.end());
    // Verificar intersección interior-interior
    int i=0, j=0;
    while(i<aInt.size() && j<bInt.size()){
        if(aInt[i] == bInt[j]){
            // Intersección interior-interior detectada
            return false;
        }else if(aInt[i] < bInt[j]){
            i++;
        }else{
            j++;
        }
    }
    // Bordes de las secuencias
    int a_bi = a[0];
    int b_bi = b[0];
    int a_bf = a[a.size()-1];
    int b_bf = b[b.size()-1];
    // Verificar intersección bordes-bordes
    if(a_bi == b_bi || a_bi == b_bf){
        return true;
    }
    if(a_bf == b_bi || a_bf == b_bf){
        return true;
    }
    // Verificar intersección bordes-interior
    if(binary_search(bInt.begin(), bInt.end(), a_bi)){
        return true;
    }
    if(binary_search(bInt.begin(), bInt.end(), a_bf)){
        return true;
    }
    if(binary_search(aInt.begin(), aInt.end(), b_bi)){
        return true;
    }
    if(binary_search(aInt.begin(), aInt.end(), b_bf)){
        return true;
    }

    return false;
}


/*******************************************************
            Relaciones topológicas agregadas
*******************************************************/


bool tr_within(vector<int> &a, vector<int> &b){
    if(a.size() > b.size()){
        return false;
    }
    int n = a.size();
    int m = b.size();
    
    // ------- EQUALS -------

    vector<int> rb = b;
    reverse(rb.begin(), rb.end());
    
    pair<int,int> kmp = KMPSearch(a, b);
    pair<int,int> kmpR = KMPSearch(a, rb);
    
    return (kmp.first != -1 || kmpR.first != -1);
}

bool tr_contains(vector<int> &a, vector<int> &b){
    if(b.size() > a.size()){
        return false;
    }
    int n = b.size();
    int m = a.size();
    
    // ------- EQUALS -------

    vector<int> ra = a;
    reverse(ra.begin(), ra.end());
    
    pair<int,int> kmp = KMPSearch(b, a);
    pair<int,int> kmpR = KMPSearch(b, ra);
    return (kmp.first != -1 || kmpR.first != -1);
}

bool tr_intersects(vector<int> &a, vector<int> &b){
    pair<int,bool> res = lcs_info(a, b);
    
    return (res.first != 0);
}
