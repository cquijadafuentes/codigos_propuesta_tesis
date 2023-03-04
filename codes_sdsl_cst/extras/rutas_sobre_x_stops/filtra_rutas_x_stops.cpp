#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Faltan argumentos!" << endl;
		cout << argv[0] << " <min_rutas>" << endl;
		return 0;
	}
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

	int tope = atoi(argv[1]);
	int cant = 0;
	cout << "Ingrese mínimo de rutas para mostrar: ";
	cin >> tope;
	for(int i=0; i<n; i++){
		if(lx[i].size() >= tope){
			cant++;
			cout << lx[i].size() << " ";
			for(int j=0; j<lx[i].size(); j++){
				cout << lx[i][j] << " ";
			}
			cout << endl;
		}
	}

	cout << cant << " rutas encontradas con largo mayor a " << tope << endl;
	cout << max << " id stop max." << endl;
	return 0;
}

