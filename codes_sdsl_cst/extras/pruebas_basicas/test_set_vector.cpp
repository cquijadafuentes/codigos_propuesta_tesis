#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Faltan argumentos:" << endl;
		cout << argv[0] << " <cant_nums_to_generate> <max_num_to_generate>" << endl;
		return 0;
	}
	int size = atoi(argv[1]);
	int maxN = atoi(argv[2]);
	srand((unsigned) time(NULL));
	set<int> ms;
	while(ms.size() < size){
		ms.insert(rand() % maxN);
	}

	vector<int> mv(ms.begin(), ms.end());
	for(int i=0; i<mv.size(); i++){
		cout << mv[i] << " ";
	}
	cout << endl;

	return 0;
}