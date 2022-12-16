#include <iostream>
#include <string>
#include <sstream>
#include <fstream>      // std::fstream
#include <set>
#include <map>
#include <vector>


/*
	Este código se crea con el fin de obtener un archivo con los trips de xctrdata (madrid)
	Se tiene como entrada el archivo lineStops.txt que indica los stops de cada línea
	y el archivo madrid_trips.txt que corresponde a los datos generados
	El archivo indica código de la línea, dos puntos (:) luego los stops separados por espacio
	Cada línea del archivo corresponde a la especificación de una línea del sistema de transporte
*/


using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Faltan argumentos. Indique nombre del archivo" << endl;
		cout << argv[0] << " <lineStops.txt> <madrid_trips.txt> [<can_max_trips>]" << endl;
		return -1;
	}
	int tope = 100;
	if(argc >= 4){
		tope = (int) atoi(argv[3]);

	}
	int respaldoTope = tope;

	fstream streamLineas;
	streamLineas.open(argv[1], fstream::in);
	string stln;
	vector<vector<int>> stopsXlinea;
	map<string,int> idsXlinea;
	int cantLineas = 0;
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
		idsXlinea[idLinea.substr(0,idLinea.length()-1)] = cantLineas++;
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

	for(map<string,int>::iterator it = idsXlinea.begin(); it != idsXlinea.end(); it++){
		cout << it->first << " >> " << it->second << endl;
	}

	cout << ".. Procesando Trips .." << endl;
	streamLineas.open(argv[2], fstream::in);
	vector<vector<int>> trips;
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
		if(pStopI < pStopF){
			for(int i=pStopI; i<=pStopF; i++){
				stopstrip.push_back(stopsXlinea[pL][i]);
			}
		}else{
			for(int i=pStopF; i<=pStopI; i++){
				stopstrip.push_back(stopsXlinea[pL][i]);
			}
		}
		trips.push_back(stopstrip);
	}

	fstream outfileTrips("gst_tripsMadrid_sinLimpiar.txt", fstream::out);
	outfileTrips << trips.size() << " " << mapIdsStops.size() << endl;
	for(int i = 0; i < trips.size(); i++){
		outfileTrips << trips[i].size();
		for(int j = 0; j < trips[i].size(); j++){
			outfileTrips << " " << trips[i][j];
		}
		outfileTrips << endl;
	}

	return 0;
}