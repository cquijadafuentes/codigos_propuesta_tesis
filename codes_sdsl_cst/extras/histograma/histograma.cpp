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
	for(mit = m.begin(); mit != m.end(); mit++){
		cout << mit->first << "\t" << mit->second << endl;
	}

	return 0;
}

	