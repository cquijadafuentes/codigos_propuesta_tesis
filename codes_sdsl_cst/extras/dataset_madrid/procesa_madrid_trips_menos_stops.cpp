#include <iostream>
#include <string>
#include <sstream>
#include <fstream>      // std::fstream
#include <set>
#include <map>
#include <vector>


/*
	Este código se crea con el fin de obtener un archivo con los trips de xctrdata (madrid)
	reduciendo la cantidad de stops utilizados por el dataset.
	Se tiene como entrada el archivo lineStops.txt que indica los stops de cada línea
	y el archivo madrid_trips.txt que corresponde a los datos generados
	El archivo indica código de la línea, dos puntos (:) luego los stops separados por espacio
	Cada línea del archivo corresponde a la especificación de una línea del sistema de transporte
*/


using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "Faltan argumentos. Indique nombre del archivo" << endl;
		cout << argv[0] << " <lineStops.txt> <madrid_trips.txt> <outputfile> [<cant_max_trips> [<cant_stops>]]" << endl;
		cout << "<cant_max_trips> indica la cantidad máxima de trips que se entregan como salida" << endl;
		cout << "<cant_stops> indica la cantidad de stops para la salida" << endl;
		return -1;
	}
	int tope = 100;
	if(argc >= 5){
		tope = (int) atoi(argv[4]);

	}
	int newCantStops = 0;
	if(argc >= 6){
		newCantStops = (int) atoi(argv[5]);
		if(newCantStops < 1){
			cout << "Error en el argumento <cant_stops>: " << newCantStops << endl;
			return -1;
		}
	}

	int respaldoTope = tope;

	fstream streamLineas;
	string stln;
	vector<vector<int>> stopsXlinea;
	map<string,int> idsXlinea;
	int cantLineas = 0;
		// SET se utiliza para determinar cuántas son las paradas existentes
		// para luego hacer un map de la id_real con un id_generado que es correlativo
	map<int,int> mapIdsStops;
	int maxIdStop = -1;
	streamLineas.open(argv[1], fstream::in);
	while(getline(streamLineas, stln)){		
//		cout << stln << endl;
		string idLinea;
		string stopsAux;
		stringstream ss_linea(stln);
		getline(ss_linea, idLinea, ' ');
//		cout << "--> " << idLinea << endl;
		vector<int> stops;
		int x;
		while(getline(ss_linea, stopsAux, ',')){
			x = stoi(stopsAux);
			stops.push_back(x);
			if(mapIdsStops[x] == 0){
				mapIdsStops[x] = mapIdsStops.size();
			}
			maxIdStop = (x > maxIdStop) ? x : maxIdStop;
		}
		stopsXlinea.push_back(stops);
		idsXlinea[idLinea.substr(0,idLinea.length()-1)] = cantLineas++;
	}
	streamLineas.close();
//	cout << stln << endl;
	int lp = stopsXlinea.size() - 1;
/*
	for(int i = 0; i < stopsXlinea[lp].size(); i++){
		cout << " " << stopsXlinea[lp][i];
	}
	cout << endl;
*/
	cout << mapIdsStops.size() << " stops distintos." << endl;
	cout << maxIdStop << " maxIdStop." << endl;
	cout << stopsXlinea.size() << " líneas identificadas." << endl;
/*
	for(map<string,int>::iterator it = idsXlinea.begin(); it != idsXlinea.end(); it++){
		cout << it->first << " >> " << it->second << endl;
	}
*/
	cout << ".. Procesando Trips .." << endl;
	vector<vector<int>> trips;
	streamLineas.open(argv[2], fstream::in);
	while(getline(streamLineas, stln) && 0 < tope--){		
		//cout << stln << endl;
		string idLinea;
		string stopI;
		string stopF;
		stringstream ss_linea(stln);
		getline(ss_linea, idLinea, ':');
		int pL = idsXlinea[idLinea];
		//cout << "idLinea: " << idLinea << " en posicisión " << pL << endl;
		getline(ss_linea, stopI, ':');
		getline(ss_linea, stopF, ':');
		getline(ss_linea, stopF, ':');
		int pStopI = 0;
		int pStopF = 0;

		for(int i=0; i < stopsXlinea[pL].size(); i++){
			if(stopsXlinea[pL][i] == stoi(stopI)){
				//cout << "stopInicial: " << stopI << " en posición: " << i << endl;
				pStopI = i;
			}
			if(stopsXlinea[pL][i] == stoi(stopF)){
				//cout << "stopFinal: " << stopF << " en posición: " << i << endl;
				pStopF = i;
			}
		}
		vector<int> stopstrip;
		int idCorregido;
		if(pStopI < pStopF){
			for(int i=pStopI; i<=pStopF; i++){
				idCorregido = (mapIdsStops[stopsXlinea[pL][i]] % newCantStops) + 1;
				stopstrip.push_back(idCorregido);
			}
		}else{
			for(int i=pStopF; i<=pStopI; i++){
				idCorregido = (mapIdsStops[stopsXlinea[pL][i]] % newCantStops) + 1;
				stopstrip.push_back(idCorregido);
			}
		}
		trips.push_back(stopstrip);
	}
	streamLineas.close();

	cout << ".. LIMPIEZA TRIPS.. (" << trips.size() << ")" << endl;
	// Verificar que las lineas son equivalentes a segmentos de linea simples
	// No hay intersección consigo mismos, ni en interior ni en bordes
	int cantRedFlags = 0;
	for(int i = 0; i < trips.size(); i++){
		// Comprobar que si cumple con un segmento de línea simple
		bool redFlag = false;
		for(int j = 0; j < trips[i].size()-1; j++){
			for(int k = j + 1; k < trips[i].size(); k++){
				if(trips[i][j] == trips[i][k]){
					redFlag = true;
				}
			}
		}
		if(redFlag){
			cantRedFlags++;
			cout << "TRIP con intersección consigo mismo [" << cantRedFlags << "]" << endl;
			// Se elimina la línea
			trips.erase(trips.begin() + i);
			i--;
		}
	}
	cout << cantRedFlags << " trips eliminados en el proceso." << endl;
	cout << trips.size() << " trips identificados." << endl;

	fstream outfileTrips(argv[3], fstream::out);
	outfileTrips << trips.size() << " " << newCantStops << endl;
	for(int i = 0; i < trips.size(); i++){
		outfileTrips << trips[i].size();
		for(int j = 0; j < trips[i].size(); j++){
			outfileTrips << " " << trips[i][j];
		}
		outfileTrips << endl;
	}

	return 0;
}