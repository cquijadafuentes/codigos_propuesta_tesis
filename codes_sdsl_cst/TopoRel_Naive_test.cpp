#include <iostream>
#include <vector>
#include "TopoRel_Naive.hpp"
#include "TopoRel_GST.hpp"

using namespace std;

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
	cout << toporel(lx, ly) << endl;
	cout << toporel(ly, lx) << endl;
	cout << endl;
	cout << COVEREDBY << ": ";
	if(tr_coveredby(lx, ly)){
		cout << "true" << endl;
	}else{
		cout << "false" << endl;
	}

	cout << COVERS << ": ";
	if(tr_covers(lx, ly)){
		cout << "true" << endl;
	}else{
		cout << "false" << endl;
	}

	cout << DISJOINT << ": ";
	if(tr_disjoint(lx, ly)){
		cout << "true" << endl;
	}else{
		cout << "false" << endl;
	}

	cout << EQUALS << ": ";
	if(tr_equals(lx, ly)){
		cout << "true" << endl;
	}else{
		cout << "false" << endl;
	}

	cout << INCLUDES << ": ";
	if(tr_includes(lx, ly)){
		cout << "true" << endl;
	}else{
		cout << "false" << endl;
	}

	cout << INSIDE << ": ";
	if(tr_inside(lx, ly)){
		cout << "true" << endl;
	}else{
		cout << "false" << endl;
	}

	cout << OVERLAPS << ": ";
	if(tr_overlaps(lx, ly)){
		cout << "true" << endl;
	}else{
		cout << "false" << endl;
	}

	cout << TOUCHES << ": ";
	if(tr_touches(lx, ly)){
		cout << "true" << endl;
	}else{
		cout << "false" << endl;
	}
	
	return 0;
}
