#include <iostream>
#include <vector>
#include "TopoRel_Naive.hpp"

using namespace std;

int main(){
	cout << "Ingrese datos de las dos listas indicando, por cada una, la cantidad de elementos y los elementos." << endl;
	cout << "|LA| LA_1 LA_2 ... LA_|LA| |LB| LB_1 LB_2 .. LB_|LB|" << endl;
	cout << "Ingresar cero para terminar (0)" << endl;

	int x, y;
	cin >> x;
	while(x != 0){
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
		cin >> x;
	}

	return 0;
}
