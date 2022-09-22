#include <iostream>
#include <vector>
#include "TopoRel_Naive.hpp"
#include "TopoRel_GST.hpp"

using namespace std;

void print_bool(bool x){
	if(x){
		cout << "true";
	}else{
		cout << "false";
	}
}


int main(){

	cout << "Input:" << endl;
	cout << "cant_rutas" << endl;
	cout << "cant_stops_r1 r1_st1 r1_st2 ···" << endl;
	cout << "cant_stops_r2 r2_st1 r2_st2 ···" << endl;
	cout << "···" << endl << endl;

	int n, x, aux;
	cin >> n;
	vector<vector<int>> lx(n);
	for(int i = 0; i < n; i++){
		cin >> x;
		for(int j=0; j<x; j++){
			cin >> aux;
			lx[i].push_back(aux);
		}
	}

	cout << lx.size() << endl;
	for(int i=0; i<lx.size(); i++){
		cout << lx[i].size() << ": ";
		for(int j=0; j<lx[i].size(); j++){
			cout << lx[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	cout << "Relaciones:" << endl;
	for(int i = 0; i < lx.size(); i++){
		for(int j = 0; j < lx.size(); j++){
			cout << toporel(lx[i], lx[j]) << "  ";
		}
		cout << endl;
	}
	cout << endl;

	cout << COVEREDBY << ": " << endl;
	cout << " _ _ _ _ _" << endl;
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(tr_coveredby(lx[i],lx[j])){
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
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(tr_covers(lx[i],lx[j])){
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
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(tr_disjoint(lx[i],lx[j])){
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
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(tr_equals(lx[i],lx[j])){
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
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(tr_includes(lx[i],lx[j])){
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
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(tr_inside(lx[i],lx[j])){
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
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(tr_overlaps(lx[i],lx[j])){
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
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(tr_touches(lx[i],lx[j])){
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
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(tr_within(lx[i],lx[j])){
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
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(tr_contains(lx[i],lx[j])){
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
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(tr_intersects(lx[i],lx[j])){
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
