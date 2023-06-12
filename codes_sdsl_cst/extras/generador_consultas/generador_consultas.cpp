#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <ctime>

using namespace std;

int myrandom(int i){
	return rand()%i;
}

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "Error! faltan argumentos:" << endl;
		cout << argv[0] << " <inputfile_txt> <outputfile> <num_queries> [<op_queries>]" << endl;
		cout << "op_queries:" << endl;
		cout << "\t1 - random (default)" << endl;
		cout << "\t2 - las <num_queries> más largas" << endl;
		cout << "\t3 - las <num_queries> más cortas" << endl;
		return 0;
	}

	ifstream ifsNaive(argv[1], ifstream::in);
	ofstream outfile(argv[2]);

	int num_queries = atoi(argv[3]);
	int op_queries = 1;
	if(argc > 4){
		op_queries = atoi(argv[4]);
		if(op_queries > 3 || op_queries < 1){
			cout << "Error! <op_queries> no corresponde" << endl;
		}
	}


	int nr, max;
	ifsNaive >> nr >> max;
	vector<pair<int,int>> rutas(nr);
	for(int i = 0; i < nr; i++){
		int n, x;
		ifsNaive >> n;
		rutas[i] = make_pair(n,i);
		for(int j = 0; j < n; j++){
			ifsNaive >> x;
		}
	}
	ifsNaive.close();

	if(op_queries == 1){
		// Ordenar aleatoriamente el vector de rutas
		srand(unsigned(time(0)));
		random_shuffle(rutas.begin(), rutas.end(), myrandom);
		// Generando resultados
		outfile << num_queries << endl;
		for(int i=0; i<=num_queries; i++){
			outfile << rutas[i].second << "\t";
		}
		outfile << endl;
		return 0;
	}

	sort(rutas.begin(), rutas.end());

	if(op_queries == 2){
		// Se generan <num_queries> desde las más largas
		outfile << num_queries << endl;
		for(int i=1; i<=num_queries; i++){
			outfile << rutas[nr-i].second << "\t";
		}
		outfile << endl;
		return 0;
	}

	// Se generan <num_queries> desde las más cortas
	outfile << num_queries << endl;
	for(int i=0; i<=num_queries; i++){
		outfile << rutas[i].second << "\t";
	}
	outfile << endl;
	outfile.close();
	return 0;
}
