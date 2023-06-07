#include "TopoRel_Naive.hpp"

//////////////////////////////////////////////////////////////
// ************* RELACIONES TOPOLÓGICAS NAIVE ************* //
//////////////////////////////////////////////////////////////

bool bordesSeg_touches(int_vector<> &a, int_vector<> &b){
    // Comprueba si hay segmentos finales que se intersecan
    // Esto implica una intersección Interior-Interior entre los bordes
    // que se considera como un touches falso-positivo
    if(a.size() == 1 || b.size() == 1){
    //  Rutas de largo 1 no pueden tener touch del segmento final.
        return false;
    }
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

// Find occurrences of txt[] in pat[]
// Retorna: <posición,largo> de la coincidencia
pair<int,int> KMPSearch(int_vector<> &pat, int_vector<> &txt){
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
void computeLPSArray(int_vector<> &pat, int M, int* lps){
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
pair<int,bool> lcs_info(int_vector<> &s, int_vector<> &t){
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


bool tr_equals(int_vector<> &a, int_vector<> &b){
    if(a.size() != b.size()){
        return false;
    }
    int aS = a.size();
    for(int i = 0; i < aS; i++){
        if(a[i] != b[i]){
            return false;
        }            
    }
    return true;
}
/*******************************************************
            Relaciones topológicas agregadas
*******************************************************/


bool tr_within(int_vector<> &a, int_vector<> &b){
    if(a.size() > b.size()){
        return false;
    }    
    pair<int,int> kmp = KMPSearch(a, b);    
    return (kmp.first != -1);
}

bool tr_contains(int_vector<> &a, int_vector<> &b){
    if(b.size() > a.size()){
        return false;
    }
    pair<int,int> kmp = KMPSearch(b, a);
    return (kmp.first != -1);
}

bool tr_intersect(int_vector<> &a, int_vector<> &b){
    // Verificacición de bordes O(c)
    if(a[0] == b[0] || a[0] == b[b.size()-1] 
        || a[a.size()-1] == b[0] || a[a.size()-1] == b[b.size()-1]){
        return true;
    }
    if(bordesSeg_touches(a, b)){
        return true;
    }
    // Verificación de cualquier intersección
    int max = 0;
    for(int i=0; i<a.size(); i++){
        if(a[i] > max){
            max = a[i];
        }
    }
    for(int i=0; i<b.size(); i++){
        if(b[i] > max){
            max = b[i];
        }
    }
    bit_vector sA = bit_vector(max+1, 0);
    for(int i=0; i<a.size(); i++){
        sA[a[i]] = 1;
    }
    for(int i=0; i<b.size(); i++){
        if(sA[b[i]] == 1){
            return true;
        }
    }
    return false;
}

int tr_intersectPP(int_vector<> &a, int_vector<> &b){
    // Indica el largo de la subsecuencia común más extensa
    int m = a.size();
    int n = b.size();

    vector<vector<int>> mat(m + 1, vector<int>(n + 1, 0));
    int maxLen = 0;
    int endIndex = 0;

    for (int i = 1; i<=m; i++) {
        for (int j = 1; j<=n; j++) {
            if (a[i-1] == b[j-1]) {
                mat[i][j] = mat[i-1][j-1] + 1;
                if (mat[i][j] > maxLen) {
                    maxLen = mat[i][j];
                    endIndex = i-1;
                }
            }
        }
    }

    return maxLen;
}

vector<int> tr_allContain(vector<int_vector<>> &r, int x){
    vector<int> res;
    for(int i=0; i<r.size(); i++){
        if(tr_within(r[x], r[i])){
            res.push_back(i);
        }
    }
    return res;
}

vector<int> tr_allEqual(vector<int_vector<>> &r, int x){
    vector<int> res;
    for(int i=0; i<r.size(); i++){
        if(tr_equals(r[x], r[i])){
            res.push_back(i);
        }
    }
    return res;
}

vector<int> tr_allContained(vector<int_vector<>> &r, int x){
    vector<int> res;
    for(int i=0; i<r.size(); i++){
        if(tr_contains(r[x], r[i])){
            res.push_back(i);
        }
    }
    return res;
}

vector<int> tr_allIntersect(vector<int_vector<>> &r, int x){
    vector<int> res;
    for(int i=0; i<r.size(); i++){
        if(tr_intersect(r[x], r[i])){
            res.push_back(i);
        }
    }
    return res;
}

unordered_map<int,int> tr_allIntersectPP(vector<int_vector<>> &r, int x, int minimo){
    unordered_map<int,int> res;
    for(int i=0; i<r.size(); i++){
        int k = tr_intersectPP(r[x], r[i]);
        if(k >= minimo){
            res[i] = k;
        }
    }
    return res;   
}
