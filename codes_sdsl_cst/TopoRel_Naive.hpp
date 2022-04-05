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
pair<int,bool> lcs_info(vector<int> &s, vector<int> &t);

bool tr_equals(vector<int> &a, vector<int> &b);
bool tr_coveredby(vector<int> &a, vector<int> &b);
bool tr_covers(vector<int> &a, vector<int> &b);
bool tr_disjoint(vector<int> &a, vector<int> &b);
bool tr_includes(vector<int> &a, vector<int> &b);
bool tr_inside(vector<int> &a, vector<int> &b);
bool tr_overlaps(vector<int> &a, vector<int> &b);
bool tr_touches(vector<int> &a, vector<int> &b);
