#include <iostream>
#include <string>
#include <sstream>
#include <fstream>      // std::fstream
#include <set>
#include <vector>

/*
	Este código toma los archivos trips.txt y stop_times.txt del dataset publicado en https://datos.gob.cl/dataset/33245
	Antes de ser procesados se deben ordenar alfabéticamente las líneas posteriores a los encabezados.
	De esta manera se identifican las paradas del primer trip de cada ruta que se registran en estos ficheros.
*/

using namespace std;

int main(int argc, char const *argv[]){

	// fstream de archivos trips.txt y stops_times.txt
	fstream streamtrips;
	streamtrips.open("trips.txt", fstream::in);
	fstream streamstops;
	streamstops.open("stop_times.txt", fstream::in);
	// encabezados de los archivos
	string l_trips, l_stops;
	getline(streamtrips, l_trips);
	cout << l_trips << endl;
	getline(streamstops, l_stops);
	cout << l_stops << endl;

	// Strings para guardar los tokens ('t_*') desde trips.txt
	string t_trip_idroute_prev = "";
	string t_trip_idroute_act = "";
	string t_trip_idtrip;

	int totalroutes = 0;
	int totalstopsrecorridas = 0;

	getline(streamstops, l_stops);
	stringstream ss_stops;
	ss_stops.str(l_stops);
	string t_stops_idtrip;
	string t_stops_idstop;
	getline(ss_stops, t_stops_idtrip, ',');

	set<string> set_stops;
	vector<string> vector_rutas;
	string aux_rutas = "";

	while(getline(streamtrips, l_trips)){
		stringstream ss_trips(l_trips);
		getline(ss_trips, t_trip_idroute_act, ',');
		if(t_trip_idroute_act != t_trip_idroute_prev){
			t_trip_idroute_prev = t_trip_idroute_act;
			totalroutes++;

			// Identificando trip_id (3era columna)
			getline(ss_trips, t_trip_idtrip, ',');
			getline(ss_trips, t_trip_idtrip, ',');

			cout << totalroutes << "\tRoute: " << t_trip_idroute_act << " - Trip: " << t_trip_idtrip << " ----> ";

			// Encontrar los stops del par "route-trip" en archivo stops.
			while(t_trip_idtrip != t_stops_idtrip && getline(streamstops, l_stops)){
				ss_stops.str(l_stops);
				getline(ss_stops, t_stops_idtrip, ',');
			}

			// Recorriendo id_stops en archivo stops.
			do{
				// id_stop es la 4a columna, ya se ha leído una
				getline(ss_stops, t_stops_idstop, ',');
				getline(ss_stops, t_stops_idstop, ',');
				getline(ss_stops, t_stops_idstop, ',');
				cout << " " << t_stops_idstop;

				aux_rutas += t_stops_idstop + ",";

				set_stops.insert(t_stops_idstop);
				totalstopsrecorridas++;

				getline(streamstops, l_stops);
				ss_stops.str(l_stops);
				getline(ss_stops, t_stops_idtrip, ',');
			}while(t_trip_idtrip == t_stops_idtrip);
			cout << endl;
			aux_rutas.resize(aux_rutas.size()-1);
			cout << aux_rutas << endl;
			vector_rutas.push_back(aux_rutas);
			aux_rutas = "";
		}
	}
	cout << endl;

	streamtrips.close();
	streamstops.close();

	cout << "Total stops visitadas: " << totalstopsrecorridas << endl;
	cout << "Total stops en set: " << set_stops.size() << endl;
	cout << "Total rutas en vector: " << vector_rutas.size() << endl;

	fstream streamout;
	streamout.open("info_stops.txt", fstream::out);	

	streamout << set_stops.size() << endl;
	set<string>::iterator it;
	for(it = set_stops.begin(); it != set_stops.end(); it++){
		streamout << *it << endl;
	}
	streamout << vector_rutas.size() << endl;
	for(int i = 0; i < vector_rutas.size(); i++){
		streamout << vector_rutas[i] << endl;
	}
	
	return 0;
}