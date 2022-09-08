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
	cout << "Ingrese datos de las dos listas indicando, por cada una, la cantidad de elementos y los elementos." << endl;
	cout << "|LA| LA_1 LA_2 ... LA_|LA| |LB| LB_1 LB_2 .. LB_|LB|" << endl;

	int x, y;
	cin >> x;
	vector<int> lx(x);
	for(int i = 0; i < x; i++){
		cin >> lx[i];
	}
	cin >> y;
	vector<int> ly(y);
	for(int i = 0; i < y; i++){
		cin >> ly[i];
	}
	cout << "Toporel A,B: ";
	cout << toporel(lx, ly) << endl;
	cout << "Toporel B,A: ";
	cout << toporel(ly, lx) << endl;
	cout << endl;
	
	cout << COVEREDBY << ": ";
	print_bool(tr_coveredby(lx, ly));
	cout << endl;

	cout << COVERS << ": ";
	print_bool(tr_covers(lx, ly));
	cout << endl;

	cout << DISJOINT << ": ";
	print_bool(tr_disjoint(lx, ly));
	cout << endl;

	cout << EQUALS << ": ";
	print_bool(tr_equals(lx, ly));
	cout << endl;

	cout << INCLUDES << ": ";
	print_bool(tr_includes(lx, ly));
	cout << endl;

	cout << INSIDE << ": ";
	print_bool(tr_inside(lx, ly));
	cout << endl;

	cout << OVERLAPS << ": ";
	print_bool(tr_overlaps(lx, ly));
	cout << endl;

	cout << TOUCHES << ": ";
	print_bool(tr_touches(lx, ly));
	cout << endl;

	cout << "WITHIN: ";
	print_bool(tr_within(lx, ly));
	cout << endl;

	cout << "CONTAINS: ";
	print_bool(tr_contains(lx, ly));
	cout << endl;

	cout << "INTERSECTS: ";
	print_bool(tr_intersects(lx, ly));
	cout << endl;
	
	return 0;
}
