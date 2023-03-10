#include <iostream>
#include <map>

using namespace std;

int main(int argc, char const *argv[]){
	int nr, max;
	cin >> nr >> max;
	map<int,int> m;
	for(int i = 0; i < nr; i++){
		int n, x;
		cin >> n;
		m[n]++;
		for(int j = 0; j < n; j++){
			cin >> x;
		}
	}

	map<int,int>::iterator mit;
	int puntox = 1;
	for(mit = m.begin(); mit != m.end(); mit++){
		while(puntox != mit->first){
			cout << puntox << "\t" << 0 << endl;
			puntox++;
		}
		cout << mit->first << "\t" << mit->second << endl;
		puntox++;
	}

	return 0;
}

	