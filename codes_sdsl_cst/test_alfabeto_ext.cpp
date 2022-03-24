#include <iostream>
#include <string>
#include <sstream>
#include <fstream>      // std::fstream
#include <map>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! falta nombre archivo a procesar como entrada." << endl;
		return -1;
	}

	fstream entrada;
	entrada.open(argv[1], fstream::in);
	int n_stops;
	entrada >> n_stops;
	cout << n_stops << endl;

	map<string,int> mapa_stops;
	for(int i = 1; i <= n_stops; i++){
		string id_stop;
		entrada >> id_stop;
		mapa_stops[id_stop] = i;
		cout << id_stop << " - " << i << endl;
	}

	int n_routes, n_t;
	string l, t;
	entrada >> n_routes;
	cout << n_routes << endl;
	vector<vector<int>> routes;

	for(int j = 0; j < n_routes; j++){
		cout << "-----------------" << endl;
		entrada >> l;
		cout << l << endl;
		n_t = 1;
		for(int i = 0; i < l.length(); i++){
			if(l[i] == ','){
				l[i] = ' ';
				n_t++;
			}
		}

		vector<int> vr(n_t, 0);
		cout << "-----------------" << endl;
		cout << l << endl;
		stringstream ss(l);
		cout << "-----------------" << endl;
		for(int i = 0; i < n_t; i++){
			ss >> t;
			cout << t << " ";
			vr[i] = mapa_stops[t];
		}
		cout << endl << "-----------------" << endl;
		for(int i = 0; i < n_t; i++){
			cout << vr[i] << " ";
		}
		cout << endl << "-----------------" << endl;
		routes.push_back(vr);
	}
	cout << "Total rutas: " << routes.size() << endl;
	return 0;
}