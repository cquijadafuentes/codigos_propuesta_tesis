#include <iostream>
#include <set>
#include "TopoRel_GST.hpp"

using namespace std;
using namespace sdsl;

void print_bool(bool x){
	if(x){
		cout << "true";
	}else{
		cout << "false";
	}
}

int main(int argc, char const *argv[]){
	cout << "Input:" << endl;
	cout << "cant_rutas" << endl;
	cout << "cant_stops_r1 r1_st1 r1_st2 ···" << endl;
	cout << "cant_stops_r2 r2_st1 r2_st2 ···" << endl;
	cout << "···" << endl << endl;
	
	int nr, maxStop = 0;
	set<int> paradas;
	cin >> nr;
	vector<vector<int>> vi;
	for(int i = 0; i < nr; i++){
		int n, x;
		cin >> n;
		vector<int> v;
		for(int j = 0; j < n; j++){
			cin >> x;
			v.push_back(x);
			paradas.insert(x);
			if(x>maxStop){
				maxStop = x;
			}
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
	cout << endl;
	
	int max = (paradas.size() > maxStop) ? paradas.size() : maxStop;
	TopoRelGST tr_gst(vi, max);
	
	cout << "Relaciones:" << endl;
	for(int i = 0; i < vi.size(); i++){
		for(int j = 0; j < vi.size(); j++){
			cout << tr_gst.obtenerRelacion(i, j) << "  ";
		}
		cout << endl;
	}
	cout << endl;

	cout << COVEREDBY << ": " << endl;
	cout << " _ _ _ _ _" << endl;
	for(int i=0; i < vi.size(); i++){
		cout << "|";
		for(int j=0; j < vi.size(); j++){
			if(tr_gst.tr_coveredby(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << COVERS << ": " << endl;
	cout << " _ _ _ _ _" << endl;
	for(int i=0; i < vi.size(); i++){
		cout << "|";
		for(int j=0; j < vi.size(); j++){
			if(tr_gst.tr_covers(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << DISJOINT << ": " << endl;
	cout << " _ _ _ _ _" << endl;
	for(int i=0; i < vi.size(); i++){
		cout << "|";
		for(int j=0; j < vi.size(); j++){
			if(tr_gst.tr_disjoint(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << EQUALS << ": " << endl;
	cout << " _ _ _ _ _" << endl;
	for(int i=0; i < vi.size(); i++){
		cout << "|";
		for(int j=0; j < vi.size(); j++){
			if(tr_gst.tr_equals(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << INCLUDES << ": " << endl;
	cout << " _ _ _ _ _" << endl;
	for(int i=0; i < vi.size(); i++){
		cout << "|";
		for(int j=0; j < vi.size(); j++){
			if(tr_gst.tr_includes(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << INSIDE << ": " << endl;
	cout << " _ _ _ _ _" << endl;
	for(int i=0; i < vi.size(); i++){
		cout << "|";
		for(int j=0; j < vi.size(); j++){
			if(tr_gst.tr_inside(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << OVERLAPS << ": " << endl;
	cout << " _ _ _ _ _" << endl;
	for(int i=0; i < vi.size(); i++){
		cout << "|";
		for(int j=0; j < vi.size(); j++){
			if(tr_gst.tr_overlaps(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << TOUCHES << ": " << endl;
	cout << " _ _ _ _ _" << endl;
	for(int i=0; i < vi.size(); i++){
		cout << "|";
		for(int j=0; j < vi.size(); j++){
			if(tr_gst.tr_touches(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << "WITHIN: " << endl;
	cout << " _ _ _ _ _" << endl;
	for(int i=0; i < vi.size(); i++){
		cout << "|";
		for(int j=0; j < vi.size(); j++){
			if(tr_gst.tr_within(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << "CONTAINS: " << endl;
	cout << " _ _ _ _ _" << endl;
	for(int i=0; i < vi.size(); i++){
		cout << "|";
		for(int j=0; j < vi.size(); j++){
			if(tr_gst.tr_contains(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << "INTERSECTS: " << endl;
	cout << " _ _ _ _ _" << endl;
	for(int i=0; i < vi.size(); i++){
		cout << "|";
		for(int j=0; j < vi.size(); j++){
			if(tr_gst.tr_intersects(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	return 0;
}