#include <iostream>
#include <vector>

using namespace std;

void print_bool(bool x){
	if(x){
		cout << "true";
	}else{
		cout << "false";
	}
}


int main(){
	int n, x, aux, max;
	cin >> n >> max;
	vector<vector<int>> lx(n);
	for(int i = 0; i < n; i++){
		cin >> x;
		for(int j=0; j<x; j++){
			cin >> aux;
			lx[i].push_back(aux);
		}
	}

	cout << "Ingrese mínimo de rutas para mostrar: ";
	cin >> aux;
	for(int i=0; i<n; i++){
		if(lx[i].size() >= aux){
			cout << lx[i].size() << " ";
			for(int j=0; j<lx[i].size(); j++){
				cout << lx[i][j] << " ";
			}
			cout << endl;
		}
	}
}