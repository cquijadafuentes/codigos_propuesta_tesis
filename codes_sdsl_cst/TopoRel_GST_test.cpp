#include <iostream>
#include "TopoRel_GST.hpp"

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
	cout << "Input:" << endl;
	cout << "cant_rutas" << endl;
	cout << "cant_stops_r1 r1_st1 r1_st2 ···" << endl;
	cout << "cant_stops_r2 r2_st1 r2_st2 ···" << endl;
	cout << "···" << endl;
	int nr;
	cin >> nr;
	vector<vector<int>> vi;
	for(int i = 0; i < nr; i++){
		int n, x;
		cin >> n;
		vector<int> v;
		for(int j = 0; j < n; j++){
			cin >> x;
			v.push_back(x);
		}
		vi.push_back(v);
	}
	
	cout << vi.size() << endl;
	for(int i = 0; i < vi.size(); i++){
		cout << vi[i].size() << " -> ";
		for(int j = 0; j < vi[i].size(); j++){
			cout << vi[i][j] << " " ;
		}
		cout << endl;
	}
	TopoRelGST tr_gst(vi, 9);
	for(int i = 0; i < vi.size(); i++){
		for(int j = 0; j < vi.size(); j++){
			cout << tr_gst.obtenerRelacion(i, j) << endl;
		}
	}
	return 0;
}