// C++ program to demonstrate Ternary Search Tree (TST) 
// insert, traverse and search operations 
#include <bits/stdc++.h> 
using namespace std; 
#define MAX 50 

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
int main(){ 
	Node* root = NULL;
	int up[] = {4,5,6,0}; 
	int cat[] = {1,3,5,0}; 
	int cats[] = {1,2,3,0}; 
	cout << "sizeBytes: " << sizeBytes << endl;
	insert(&root, up); 
	cout << "sizeBytes: " << sizeBytes << endl;
	insert(&root, cat); 
	cout << "sizeBytes: " << sizeBytes << endl;
	insert(&root, cats); 
	cout << "sizeBytes: " << sizeBytes << endl;
	int pu[] = {2,4,6,0};
	cout << "\nFollowing are search results for cats, bu "
			"and cat respectively\n"; 
	searchTST(root, cats) ? cout << "Found\n"
						: cout << "Not Found\n"; 
	searchTST(root, up) ? cout << "Found\n"
						: cout << "Not Found\n"; 
	searchTST(root, cat) ? cout << "Found\n"
						: cout << "Not Found\n"; 
	searchTST(root, pu) ? cout << "Found\n"
						: cout << "Not Found\n"; 

	cout << "sizeBytes: " << sizeBytes << endl;

	return 0; 
} 

// This code is contributed by tapeshdua420.
