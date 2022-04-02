#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include "TopoRel_Cons.hpp"

using namespace std;

// RELACIONES TOPOLÓGICAS NAIVE
string toporel(vector<int> &a, vector<int> &b);
pair<int,int> KMPSearch(vector<int> &pat, vector<int> &txt);
void computeLPSArray(vector<int> &pat, int M, int* lps);
pair<int,bool> lcs_info(vector<int> &s, vector<int> &t, int n, int m);