#include <iostream>
#include "TopoRel_Naive_PreComp.hpp"

using namespace std;

int main(int argc, char const *argv[]){
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
	int max = 0;
	cout << lx.size() << endl;
	for(int i=0; i<lx.size(); i++){
		cout << lx[i].size() << ": ";
		for(int j=0; j<lx[i].size(); j++){
			cout << lx[i][j] << " ";
			if(lx[i][j] > max){
				max = lx[i][j];
			}
		}
		cout << endl;
	}
	cout << endl;

	TopoRelNaivePreComp trnpc(lx, max);

	cout << "Relaciones:" << endl;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			cout << trnpc.obtenerRelacion(i,j) << "  ";
		}
		cout << endl;
	}
	cout << endl;

	cout << COVEREDBY << ": " << endl;
	for(int i=0; i < lx.size(); i++){
		cout << " _";
	}
	cout << endl;
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(trnpc.coveredby(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << COVERS << ": " << endl;
	for(int i=0; i < lx.size(); i++){
		cout << " _";
	}
	cout << endl;
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(trnpc.covers(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << DISJOINT << ": " << endl;
	for(int i=0; i < lx.size(); i++){
		cout << " _";
	}
	cout << endl;
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(trnpc.disjoint(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << EQUALS << ": " << endl;
	for(int i=0; i < lx.size(); i++){
		cout << " _";
	}
	cout << endl;
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(trnpc.equals(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << INCLUDES << ": " << endl;
	for(int i=0; i < lx.size(); i++){
		cout << " _";
	}
	cout << endl;
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(trnpc.includes(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << INSIDE << ": " << endl;
	for(int i=0; i < lx.size(); i++){
		cout << " _";
	}
	cout << endl;
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(trnpc.inside(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << OVERLAPS << ": " << endl;
	for(int i=0; i < lx.size(); i++){
		cout << " _";
	}
	cout << endl;
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(trnpc.overlaps(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << TOUCHES << ": " << endl;
	for(int i=0; i < lx.size(); i++){
		cout << " _";
	}
	cout << endl;
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(trnpc.touches(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	return 0;

	cout << "WITHIN: " << endl;
	for(int i=0; i < lx.size(); i++){
		cout << " _";
	}
	cout << endl;
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(trnpc.within(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << "CONTAINS: " << endl;
	for(int i=0; i < lx.size(); i++){
		cout << " _";
	}
	cout << endl;
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(trnpc.contains(i,j)){
				cout << "X ";
			}else{
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << "INTERSECTS: " << endl;
	for(int i=0; i < lx.size(); i++){
		cout << " _";
	}
	cout << endl;
	for(int i=0; i < lx.size(); i++){
		cout << "|";
		for(int j=0; j < lx.size(); j++){
			if(trnpc.intersects(i,j)){
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