#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]){
	int nr, max;
	cin >> nr >> max;
	vector<int> m(max,0);
	for(int i = 0; i < nr; i++){
		int n, x;
		cin >> n;
		for(int j = 0; j < n; j++){
			cin >> x;
			m[x]++;
		}
	}

	for(int i=0; i<m.size(); i++){
		cout << i << "\t" << m[i] << endl;
	}

	return 0;
}

	