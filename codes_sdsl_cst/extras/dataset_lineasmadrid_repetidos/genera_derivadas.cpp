#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

vector<int> derivarRuta(vector<int> ruta, int n_max, int cantPrev, int cantPost);

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
	for(int i=0; i<rutas.size(); i++){
		salida << rutas[i].size() << " ";
		for(int j=0; j<rutas[i].size(); j++){
			salida << rutas[i][j] << " ";
		}
		salida << endl;
		generadas++;
	}
	srand (time(NULL));
	int cantPrev, cantPost;
	while(generadas < cantTray){
		int x = generadas % nRutas;
		cantPrev = rand() % 20 + 1;
		cantPost = rand() % 20 + 1;
		vector<int> rDerivada = derivarRuta(rutas[x], nStops, cantPrev, cantPost);
		salida << rDerivada.size();
		for(int i=0; i < rDerivada.size(); i++){
			salida << " " << rDerivada[i];
		}
		salida << endl;
		generadas++;
	}



	return 0;
}

vector<int> derivarRuta(vector<int> ruta, int n_max, int cantPrev, int cantPost){
	vector<int> derivada;
	int aux;
	// Marcar para no repetir los datos en la ruta
	vector<bool> marcas(n_max+1, false);
	for(int i=0; i<ruta.size(); i++){
		marcas[ruta[i]] = true;
	}
	// Insertar elementos previos
	for(int i=0; i<cantPrev; i++){
		// Generar un elemento aleatorio que no esté en la ruta original
		aux = rand() % n_max + 1;
		while(marcas[aux]){
			aux = rand() % n_max + 1;
		}
		derivada.push_back(aux);
		marcas[aux] = true;
	}
	// Insertar ruta original
	derivada.insert(derivada.end(), ruta.begin(), ruta.end());
	// Insertar elementos post
	vector<int> post(cantPost);
	for(int i=0; i<cantPost; i++){
		// Generar un elemento aleatorio que no esté en la ruta original
		aux = rand() % n_max + 1;
		while(marcas[aux]){
			aux = rand() % n_max + 1;
		}
		derivada.push_back(aux);
		marcas[aux] = true;
	}
	return derivada;
}