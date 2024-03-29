#include <iostream>
#include "TopoRel_GST_5.hpp"

using namespace std;
using namespace sdsl;

void print_bool(bool x){
	if(x){
		cout << "true";
	}else{
		cout << "false";
	}
}

int main(int argc, char const *argv[]){

	if(argc < 2){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <output_filename>" << endl;
		return 0;
	}

	cout << "Input:" << endl;
	cout << "cant_rutas max_stop" << endl;
	cout << "cant_stops_r1 r1_st1 r1_st2 ···" << endl;
	cout << "cant_stops_r2 r2_st1 r2_st2 ···" << endl;
	cout << "···" << endl << endl;
	
	int nr, max;
	cin >> nr >> max;
	vector<vector<int>> vi;
	for(int i = 0; i < nr; i++){
		int n, x;
		cin >> n;
		vector<int> v;
		for(int j = 0; j < n; j++){
			cin >> x;
			v.push_back(x);
		}
		vi.push_back(v);
	}
	
	cout << vi.size() << endl;
	for(int i = 0; i < vi.size(); i++){
		cout << vi[i].size() << " -> ";
		for(int j = 0; j < vi[i].size(); j++){
			cout << vi[i][j] << " " ;
		}
		cout << endl;
	}
	cout << endl;
	
	TopoRelGST_5 gst_0(vi, max);

	string filename(argv[1]);
	if(gst_0.save(filename)){
		cout << "Se ha guardado exitosamente." << endl;
	}else{
		cout << "Falla en el guardado." << endl;
	}	

	return 0;
}