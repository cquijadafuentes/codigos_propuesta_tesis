#include <iostream>
#include <string>
#include <sstream>
#include <fstream>      // std::fstream
#include <set>
#include <map>
#include <vector>


/*
	Este código se crea con el fin de obtener un archivo con las líneas de xctrdata (madrid)
	Se tiene como entrada el archivo lineStops.txt que indica los stops de cada línea
	El archivo indica código de la línea, dos puntos (:) luego los stops separados por espacio
	Cada línea del archivo corresponde a la especificación de una línea del sistema de transporte
*/


using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Faltan argumentos. Indique nombre del archivo" << endl;
		cout << argv[0] << " <lineStops.txt> <outputfile_name>" << endl;
		return -1;
	}
	fstream streamLineas;
	streamLineas.open(argv[1], fstream::in);
	string stln;
	vector<vector<int>> stopsXlinea;
		// SET se utiliza para determinar cuántas son las paradas existentes
		// para luego hacer un map de la id_real con un id_generado que es correlativo
	set<int> set_stops;
	int maxIdStop = -1;
	while(getline(streamLineas, stln)){		
		cout << stln << endl;
		string idLinea;
		string stopsAux;
		stringstream ss_linea(stln);
		getline(ss_linea, idLinea, ' ');
		cout << "--> " << idLinea << endl;
		vector<int> stops;
		int x;
		while(getline(ss_linea, stopsAux, ',')){
			x = stoi(stopsAux);
			stops.push_back(x);
			set_stops.insert(x);
			maxIdStop = (x > maxIdStop) ? x : maxIdStop;
		}
		stopsXlinea.push_back(stops);
	}
	streamLineas.close();
	cout << stln << endl;
	int lp = stopsXlinea.size() - 1;
	for(int i = 0; i < stopsXlinea[lp].size(); i++){
		cout << " " << stopsXlinea[lp][i];
	}
	cout << endl;

	cout << set_stops.size() << " stops distintos." << endl;
	cout << maxIdStop << " maxIdStop." << endl;
	cout << stopsXlinea.size() << " líneas identificadas." << endl;
	// Crear mapa que relacione id_real con id_generada que es correlativa
	map<int,int> mapIdsStops;
	int idGen = 1;
	int idaux;
	for(set<int>::iterator it=set_stops.begin(); it!=set_stops.end(); it++){
		idaux = *it;
		mapIdsStops[idaux] = idGen;
		idGen++;
	}
	cout << ".. LIMPIEZA .." << endl;
	// Verificar que las lineas son equivalentes a segmentos de linea simples
	// No hay intersección consigo mismos, ni en interior ni en bordes
	int cantRedFlags = 0;
	int idStopAux;
	maxIdStop = 0;
	for(int i = 0; i < stopsXlinea.size(); i++){
		// Comprobar que si cumple con un segmento de línea simple
		bool redFlag = false;
		for(int j = 0; j < stopsXlinea[i].size()-1; j++){
			for(int k = j + 1; k < stopsXlinea[i].size(); k++){
				if(stopsXlinea[i][j] == stopsXlinea[i][k]){
					redFlag = true;
					cantRedFlags++;
				}
			}
		}
		if(redFlag){
			cout << "Esta linea tiene intersección consigo misma" << endl;
			// Se elimina la línea
			stopsXlinea.erase(stopsXlinea.begin() + i);
			i--;
		}else{
			// Se actualizan los id por los generados consecutivamente
			for(int j = 0; j < stopsXlinea[i].size(); j++){
				stopsXlinea[i][j] = mapIdsStops[stopsXlinea[i][j]];
				maxIdStop = (stopsXlinea[i][j] > maxIdStop) ? stopsXlinea[i][j] : maxIdStop;
			}
		}
	}
	cout << cantRedFlags << " líneas eliminadas en el proceso." << endl;
	cout << maxIdStop << " maxIdStop." << endl;
	cout << stopsXlinea.size() << " líneas identificadas." << endl;

	fstream outputfile(argv[2], fstream::out);
	outputfile << stopsXlinea.size() << " " << mapIdsStops.size() << endl;
	for(int i = 0; i < stopsXlinea.size(); i++){
		outputfile << stopsXlinea[i].size();
		for(int j = 0; j < stopsXlinea[i].size(); j++){
			outputfile << " " << stopsXlinea[i][j];
		}
		outputfile << endl;
	}

	return 0;
}