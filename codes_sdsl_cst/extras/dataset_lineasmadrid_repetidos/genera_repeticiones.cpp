#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "ERROR! Faltan argumentos. Ejemplo de uso: " << endl;
		cout << argv[0] << " <input_filename> <cantidad_tray_salida> <output_filename>" << endl;
		return -1;
	}

	cout << "Bien... procesando..." << endl;
	int nRutas, n_stops, largoTray, aux;
	vector<vector<int>> routes;
    cin >> nRutas >> n_stops;
	for(int j = 0; j < nRutas; j++){
		cin >> largoTray;
        vector<int> vt(largoTray, 0);
        for(int i = 0; i < largoTray; i++){
            cin >> vt[i];
        }
        routes.push_back(vt);
	}

	int cantTray = (int) atoi(argv[2]);

	cout << nRutas << " rutas cargadas exitosamente." << endl;
	cout << "Generando " << cantTray << " trayectorias en " << argv[3] << endl;

	return 0;
}