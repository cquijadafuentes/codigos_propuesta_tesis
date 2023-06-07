#ifndef TopoRel_Naive
#define TopoRel_Naive

#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>
#include "TopoRel_Cons.hpp"

using namespace std;
using namespace sdsl;

// RELACIONES TOPOLÓGICAS NAIVE
//string toporel(vector<int> &a, vector<int> &b);
pair<int,int> KMPSearch(int_vector<> &pat, int_vector<> &txt);
void computeLPSArray(int_vector<> &pat, int M, int* lps);
pair<int,bool> lcs_info(int_vector<> &s, int_vector<> &t);

// 8 relaciones topológicas binarias básicas
bool tr_equals(int_vector<> &a, int_vector<> &b);
//bool tr_coveredby(vector<int> &a, vector<int> &b);
//bool tr_covers(vector<int> &a, vector<int> &b);
//bool tr_disjoint(vector<int> &a, vector<int> &b);
//bool tr_includes(vector<int> &a, vector<int> &b);
//bool tr_inside(vector<int> &a, vector<int> &b);
//bool tr_overlaps(vector<int> &a, vector<int> &b);
//bool tr_touches(vector<int> &a, vector<int> &b);

// Relaciones topológicas en conjunto 
vector<int> tr_allContain(vector<int_vector<>> &r, int x);
vector<int> tr_allEqual(vector<int_vector<>> &r, int x);
vector<int> tr_allContained(vector<int_vector<>> &r, int x);
vector<int> tr_allIntersect(vector<int_vector<>> &r, int x);
unordered_map<int,int> tr_allIntersectPP(vector<int_vector<>> &r, int x, int minimo=1);

// Relaciones topológicas binarias agregadas
bool tr_within(int_vector<> &a, int_vector<> &b);
bool tr_contains(int_vector<> &a, int_vector<> &b);
bool tr_intersect(int_vector<> &a, int_vector<> &b);
int tr_intersectPP(int_vector<> &a, int_vector<> &b);

#endif