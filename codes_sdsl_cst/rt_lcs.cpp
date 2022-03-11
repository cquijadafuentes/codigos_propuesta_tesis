#include <iostream>
#include <algorithm>
#include <string>

using namespace std;



int lcs_info(string s, string t, int n, int m, int* vs, int* vt);

void toporel(string a, string b);

string det_relation(int n, int m, int* vs, int* vt, int lcs);


int KMPSearch(string pat, string txt);

void computeLPSArray(string pat, int M, int* lps);


int main(){
    //string s = "etiSweNOldSites:GeeksforGeeks.org";
    //string t = "Geeka";
    string s = "ADHI";
    string t = "ABCEFG";

    cout << "s: " << s << " - t: " << t << endl;

    toporel(s,t);
    KMPSearch(s,t);
    
    return 0;
}


int lcs_info(string s, string t, int n, int m, int* vs, int* vt){
    // Iniciar valores en 0 para vs y vm
    for(int i=0; i<n; i++){
        vs[i] = 0;
    }
    for(int i=0; i<m; i++){
        vt[i] = 0;
    }

    // Create DP table
    int dp[2][m + 1];
    int res = 0;
    bool ii_intersect = false;

    cout << "************** Tabla PD **************" << endl;
    cout << endl << "+ ";
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
                if(dp[i % 2][j] > vs[i-1]){
                    vs[i-1] = dp[i % 2][j];
                }
                if(dp[i % 2][j] > vt[j-1]){
                    vt[j-1] = dp[i % 2][j];
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

    return res;
}



void toporel(string a, string b){
    /*
        Se define cuál es la relación topológica entre dos secuencias representadas por string.
        Las relaciones se comprueban según la complejidad que implica el identificarlas:
        - Equals: comparación lineal de las dos strings (considerando que pudiera estar en inverso)
        - Contención (Covered, Covers, Includes, Inside): comparación mediante algoritmo KMP (Knuth-Morris-Pratt)
        - Otras: Por medio de algoritmo de DP para LCS (Longest Common Substring). En este caso no es relevante
            el orden de las intersecciones. Ya se ha revisado la contención y sólo queda verificar la cantidad
            de coincidencias entre las secuencias para determinar entre Touches, Overlaps y Disjoint.
    */

    string s, t;
    if(a.length() < b.length()){
        s = a;
        t = b;
    }else{
        t = a;
        s = b;
    }

    // Se asume que |s| < |t|
    int n = s.length();
    int m = t.length();
    
    // ------- EQUALS -------

    string rt = t;
    reverse(rt.begin(), rt.end());
    // "Verificación de Igualdad"
    if(n == m && (s == t || s == rt)){
        cout << "Equals " << endl;
        return;
    }

    // ------- CONTENCION -------
    int kmp = KMPSearch(s, t);
    int kmpR = KMPSearch(s, rt);
    if(kmp != -1 || kmpR != -1){
        int index = (kmp != -1) ? kmp : kmpR;
        if(index == 0 || index == m-n){
            if(s == a){
                cout << "CoveredBy" << endl;
            }else{
                cout << "Covers" << endl;
            }
        }else{
            if(s == a){
                cout << "Inside" << endl;
            }else{
                cout << "Includes" << endl;
            }
        }
        return;
    }


    // ------- OTRAS -------

    //

    int* vs = new int[n];
    int* vt = new int[m];

    int res = lcs_info(s, t, n, m, vs, vt);

    if(res == 0){
        cout << "Disjoint" << endl;
        return;
    }
    
    int* vs2 = new int[n];
    int* vt2 = new int[m];

    int res2 = lcs_info(s, rt, n, m, vs2, vt2);

    if(res > 1 || res2 > 1){
        // Caso en que las secuencias de paradas tienen una coincidencia en algún tramo
        // por lo que se puede descartar touches.
        cout << "Overlaps" << endl;
        return;
    }

    cout << "************** Valores VS **************" << endl;
    for(int i=0; i<n; i++){
        cout << " " << s[i];
    }
    cout << endl;
    for(int i=0; i<n; i++){
        cout << " " << vs[i];
    }
    cout << endl;
    for(int i=0; i<n; i++){
        cout << " " << vs2[i];
    }
    cout << "\t reverso" << endl;

    
    cout << "************** Valores VT **************" << endl;
    for(int i=0; i<m; i++){
        cout << " " << t[i];
    }
    cout << endl;
    for(int i=0; i<m; i++){
        cout << " " << vt[i];
    }
    cout << endl;
    for(int i=0; i<m; i++){
        cout << " " << vt2[m-1-i];
    }
    cout << "\t reverso" << endl;


    cout << "************** LCS **************" << endl;
    cout << res << endl;

    cout << "************** LCS 2 **************" << endl;
    cout << res2 << endl;

    cout << "************** RELATION **************" << endl;
    if(res < res2){
        cout << det_relation(n, m, vs2, vt2, res2) << endl;
    }else{
        cout << det_relation(n, m, vs, vt, res) << endl;
    }


    delete vs;
    delete vt;
    delete vs2;
    delete vt2;
}

string det_relation(int n, int m, int* vs, int* vt, int lcs){
    // Se asume que n <= m
    if(lcs == n && n == m){
        return "Equals";
    }

    if(lcs == n){
        // Hay una contención
    }
    return "Unknown";
}

// Prints occurrences of txt[] in pat[]
int KMPSearch(string pat, string txt){
    int M = pat.length();
    int N = txt.length();
  
    // create lps[] that will hold the longest prefix suffix
    // values for pattern
    int lps[M];
  
    // Preprocess the pattern (calculate lps[] array)
    computeLPSArray(pat, M, lps);
  
    int i = 0; // index for txt[]
    int j = 0; // index for pat[]
    while (i < N) {
        if (pat[j] == txt[i]) {
            j++;
            i++;
        }
  
        if (j == M) {
            // cout << "Found pattern at index " << (i - j) << endl;
            return i-j;
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
    return -1;
}
  
// Fills lps[] for given patttern pat[0..M-1]
void computeLPSArray(string pat, int M, int* lps){
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