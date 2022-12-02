#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "ERROR! Faltan argumentos. Ejemplo de uso: " << endl;
		cout << argv[0] << " <input_filename> <cantidad_tray_salida> <output_filename>" << endl;
		return -1;
	}

	cout << "Bien... procesando..." << endl;

	fstream entrada;
	entrada.open(argv[1], fstream::in);

	int nRutas, nStops, largoTray, aux;
	vector<vector<int>> rutas;
    entrada >> nRutas >> nStops;
	for(int j = 0; j < nRutas; j++){
		entrada >> largoTray;
        vector<int> vt(largoTray, 0);
        for(int i = 0; i < largoTray; i++){
            entrada >> vt[i];
        }
        rutas.push_back(vt);
	}
	cout << nRutas << " rutas cargadas exitosamente." << endl;

	int cantTray = (int) atoi(argv[2]);

	cout << "Generando " << cantTray << " trayectorias en " << argv[3] << endl;
	int generadas = 0;
	fstream salida;
	salida.open(argv[3], fstream::out);
	salida << cantTray << " " << nStops << endl;
	while(generadas++ < cantTray){
		int x = generadas % nRutas;
		salida << rutas[x].size();
		for(int i=0; i < rutas[x].size(); i++){
			salida << " " << rutas[x][i];
		}
		salida << endl;
	}



	return 0;
}