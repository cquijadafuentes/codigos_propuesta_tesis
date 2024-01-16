// C++ program to demonstrate Ternary Search Tree (TST) 
// insert, traverse and search operations 
#include <bits/stdc++.h> 
using namespace std; 
#define MAX 11010 

// A node of ternary search tree 
struct Node { 
	int data; 

	// True if this character is last character of one of 
	// the words 
	bool isEndOfString = true; 
	Node *left, *eq, *right; 
}; 

long long int sizeBytes = 0;

// A utility function to create a new ternary search tree 
// node 
Node* newNode(int data) 
{ 
	Node* temp = new Node(); 
	sizeBytes += sizeof(int) + sizeof(bool) + sizeof(NULL)*3;
	temp->data = data; 
	temp->isEndOfString = false; 
	temp->left = temp->eq = temp->right = NULL; 
	return temp; 
} 

// Function to insert a new word in a Ternary Search Tree 
void insert(Node** root, int* word) 
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
bool searchTST(Node* root, int* word) 
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
	int caminos[100];	// En gstTripsMadrid_6400 la ruta más larga es de 71 stops
	for(int i = 0; i < n; i++){
		entrada >> x;
		caminos[x] = 0;
		for(int j=0; j<x; j++){
			entrada >> aux;
			caminos[j] = aux;
		}
		insert(&root, caminos);
	}
	entrada.close();

	cout << "Rutas: " << n << endl;
	cout << "sizeBytes: " << sizeBytes << endl;
	searchTST(root, caminos) ? cout << "Found\n"
						: cout << "Not Found\n"; 
	return 0; 
} 

// This code is contributed by tapeshdua420.
