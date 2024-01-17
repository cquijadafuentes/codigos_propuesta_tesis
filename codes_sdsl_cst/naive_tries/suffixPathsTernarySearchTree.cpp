// C++ program to demonstrate Ternary Search Tree (TST) 
// insert, traverse and search operations 
#include <bits/stdc++.h> 
using namespace std; 
#define MAX 11010 

// A node of ternary search tree 
struct Node { 
	short data; 

	// True if this character is last character of one of 
	// the words 
	bool isEndOfString = true; 
	Node *left, *eq, *right; 
}; 

// A utility function to create a new ternary search tree 
// node 
Node* newNode(short data) 
{ 
	Node* temp = new Node(); 
	temp->data = data; 
	temp->isEndOfString = false; 
	temp->left = temp->eq = temp->right = NULL; 
	return temp; 
} 

// Function to insert a new word in a Ternary Search Tree 
void insert(Node** root, short* word) 
{ 
	// Base Case: Tree is empty 
	if (!(*root)) 
		*root = newNode(*word); 

	// If current character of word is smaller than root's 
	// character, then insert this word in left subtree of 
	// root 
	if ((*word) < (*root)->data) 
		insert(&((*root)->left), word); 

	// If current character of word is greater than root's 
	// character, then insert this word in right subtree of 
	// root 
	else if ((*word) > (*root)->data) 
		insert(&((*root)->right), word); 

	// If current character of word is same as root's 
	// character, 
	else { 
		if (*(word + 1) != 0)
			insert(&((*root)->eq), word + 1); 

		// the last character of the word 
		else
			(*root)->isEndOfString = true; 
	} 
} 

// Function to search a given word in TST 
bool searchTST(Node* root, short* word) 
{ 
	if (!root) 
		return 0; 

	if (*word < (root)->data) 
		return searchTST(root->left, word); 

	else if (*word > (root)->data) 
		return searchTST(root->right, word); 

	else { 
		if (*(word + 1) == 0)
			return root->isEndOfString; 

		return searchTST(root->eq, word + 1); 
	} 
}

long long int dfsSizeInBytes(Node* node){
	long long int size = (int)sizeof(node);
	if(node != NULL){
		size += (int)(sizeof(node->data)+sizeof(node->isEndOfString));
		size += dfsSizeInBytes(node->left);
		size += dfsSizeInBytes(node->right);
		size += dfsSizeInBytes(node->eq);
	}
	return size;
}

void print(short* x){
	int p = 0;
	while(x[p] != 0){
		cout << x[p++] << " ";
	}
	cout << endl;
}

// Driver program to test above functions 
int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <input_file>" << endl;
		return 0;
	}
	Node* root = NULL; 
	// Cargando datos
	ifstream entrada(argv[1], ifstream::in);
	int n, x, aux, max;
	entrada >> n >> max;
	short caminos[100];	// En gstTripsMadrid_6400 la ruta más larga es de 71 stops
	short ruta105[100];
	for(int i = 0; i < n; i++){
		entrada >> x;
		caminos[x] = 0;
		for(int j=0; j<x; j++){
			entrada >> aux;
			caminos[j] = aux;
		}
		// Se insertan el camino y todos sus sufijos
		for(int j=0; j<x; j++){
			insert(&root, caminos+j);
		}
		
	}
	entrada.close();

	cout << "rutas\tbytesSuffixTST" << endl;
	cout << n << "\t" << dfsSizeInBytes(root) << endl;

	searchTST(root, ruta105) ? cout << "Found\n"
						: cout << "Not Found\n";
	for(int i=0; i<15; i++){
		ruta105[i] = i+3;
	}
	ruta105[15] = 0;
	searchTST(root, ruta105) ? cout << "Found\n"
						: cout << "Not Found\n";

	return 0; 
} 

// This code is contributed by tapeshdua420.