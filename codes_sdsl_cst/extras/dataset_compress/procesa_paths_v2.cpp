#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>

/*
	Código diseñado para procesar los archivos con los paths de compress dataset.
*/

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <input_filename.txt> <files_folder> <output_filename>" << endl;
		cout << "<input_filename> corresponde a la lista de archivos de caminos" << endl;
		cout << "<files_folder> corresponde a la carpeta donde estan los archivos listados" << endl;
		cout << "<output_filename> es el nombre del archivo de sailda con formato naive" << endl;
		return 0;
	}

	ifstream listado(argv[1], ifstream::in);
	if(listado){
		cout << "listado de archivos cargado correctamente." << endl;
	}else{
		cout << "Error! durante la lectura del listado " << argv[1] << endl;
		return 0;
	}
	string f;
	int stop_min = 1000000;
	int stop_max = -stop_min;
	vector<vector<int>> paths;
	cout << "Procesando caminos del listado..." << endl;
	while(listado >> f){
		ifstream pathfile(argv[2]+f, ifstream::in);
		if(!pathfile){
			cout << "Error! durante la lectura del archivo " << f << endl;
			break;
		}
		int stop;
		vector<int> path;
		while(pathfile >> stop){
			path.push_back(stop);
			if(stop < stop_min) stop_min = stop;
			if(stop > stop_max) stop_max = stop;
		}
		if(path.size() > 1){
			paths.push_back(path);
		}
		pathfile.close();
	}
	listado.close();
	cout << "cantidad de caminos encontrados: " << paths.size() << endl;
	cout << "Extremos (etapa 1)" << endl;
	cout << "\tmin: " << stop_min << " - max: " << stop_max << endl;

	cout << "Limpiando trips... (" << paths.size() << ")" << endl;
	// Verificar que las lineas son equivalentes a segmentos de linea simples
	// No hay intersección consigo mismos, ni en interior ni en bordes
	int cantRedFlags = 0;
	for(int i = 0; i < paths.size(); i++){
		// Comprobar que si cumple con un segmento de línea simple
		bool redFlag = false;
		for(int j = 0; j < paths[i].size()-1; j++){
			for(int k = j + 1; k < paths[i].size(); k++){
				if(paths[i][j] == paths[i][k]){
					redFlag = true;
				}
			}
		}
		if(redFlag){
			cantRedFlags++;
			//cout << "TRIP con intersección consigo mismo" << endl;
			// Se elimina la línea
			paths.erase(paths.begin() + i);
			i--;
		}
	}
	cout << cantRedFlags << " caminos eliminados en el proceso." << endl;
	cout << paths.size() << " caminos restantes en el dataset." << endl;
	
	int intAux = stop_min;
	stop_min = stop_max;
	stop_max = intAux;
	cout << "Acotando identificadores de los stops..." << endl;
	set<int> ids;
	for (int i=0; i<paths.size(); i++){
		for(int j=0; j<paths[i].size(); j++){
			intAux = paths[i][j];
			ids.insert(intAux);
			if(intAux < stop_min) stop_min = intAux;
			if(intAux > stop_max) stop_max = intAux;
		}
	}
	cout << "Cantidad de ids diferentes: " << ids.size() << endl;
	cout << "Extremos (etapa 2)" << endl;
	cout << "\tmin: " << stop_min << " - max: " << stop_max << endl;
	cout << "Acotando ids de stops..." << endl;
	map<int,int> newIds;
	int newStop = 1;
	for(set<int>::iterator it=ids.begin(); it!=ids.end(); it++){
		newIds[*it] = newStop;
		newStop++;
	}
	cout << "Cantidad de elementos en el map: " << newIds.size() << endl;
	cout << "Extremos (etapa 3-final)" << endl;
	cout << "\tmin: 1 - max: " << ids.size() << endl;

	cout << "Generando archivo de salida..." << endl;
	fstream salida(argv[3], fstream::out);
	salida << paths.size() << " " << ids.size() << endl;
	for(int i=0; i<paths.size(); i++){
		salida << paths[i].size();
		for(int j=0; j<paths[i].size(); j++){
			salida << " " << newIds[paths[i][j]];
		}
		salida << endl;
	}
	salida.close();
	cout << "Fin." << endl;

	return 0;

}