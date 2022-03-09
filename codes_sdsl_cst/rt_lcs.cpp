#include <iostream>
#include <algorithm>
#include <string>

using namespace std;



int lcs_info(string s, string t, int n, int m, int* vs, int* vt);

void toporel(string s, string t);

string det_relation(int n, int m, int* vs, int* vt, int lcs);


int main(){
    //string s = "etiSweNOldSites:GeeksforGeeks.org";
    //string t = "NewSite:GeeksQuiz.com";
    string s = "ABCD";
    string t = "ABCE";
 
    if(s.length() < t.length()){
        toporel(s,t);
    }else{
        toporel(t,s);
    }

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
            }
            else{
                dp[i % 2][j] = 0;
            }
            cout << " " << dp[i % 2][j] ;
        }
        cout << endl;
    }

    return res;
}



void toporel(string s, string t){
    // Se asume que |s| < |t|
    int n = s.length();
    int m = t.length();
    int* vs = new int[n];
    int* vt = new int[m];

    int res = lcs_info(s, t, n, m, vs, vt);

    string rt = t;
    reverse(rt.begin(), rt.end());
    int* vs2 = new int[n];
    int* vt2 = new int[m];

    int res2 = lcs_info(s, rt, n, m, vs2, vt2);

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