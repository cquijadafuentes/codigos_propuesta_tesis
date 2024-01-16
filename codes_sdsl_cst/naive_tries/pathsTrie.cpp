#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Las secuencias se definen con el largo al principio
    Por ejemplo, la secuencia [1,3,5] se define con el 
    arreglo: [3,1,3,5]
*/

#define N 11010
long long int sizeBytes = 0;

typedef struct TrieNode TrieNode;

struct TrieNode {
    // The Trie Node Structure
    // Each node has N children, starting from the root
    // and a flag to check if it's a leaf node
    int data; // Storing for printing purposes only
    TrieNode* children[N];
    bool is_leaf;
};

TrieNode* make_trienode(int data) {
    // Allocate memory for a TrieNode
    TrieNode* node = new TrieNode;
    sizeBytes = sizeBytes + (int)(sizeof(int) + sizeof(bool) + (N * sizeof(node)));
    for (int i=0; i<N; i++)
        node->children[i] = NULL;
    node->is_leaf = false;
    node->data = data;
    return node;
}

void free_trienode(TrieNode* node) {
    // Free the trienode sequence
    for(int i=0; i<N; i++) {
        if (node->children[i] != NULL) {
            free_trienode(node->children[i]);
        }
        else {
            continue;
        }
    }
    free(node);
}

TrieNode* insert_trie(TrieNode* root, int* seq) {
    // Inserts the sequence onto the Trie
    // ASSUMPTION: The seq only has lower case characters
    TrieNode* temp = root;

    for (int i=1; i <= seq[0]; i++) {
        // Get the relative position in the alphabet list
        int idx = seq[i]-1;
        if (temp->children[idx] == NULL) {
            // If the corresponding child doesn't exist,
            // simply create that child!
            temp->children[idx] = make_trienode(seq[i]);
        }
        // Go down a level, to the child referenced by idx
        // since we have a prefix match
        temp = temp->children[idx];
    }
    // At the end of the sequence, mark this node as the leaf node
    temp->is_leaf = true;
    return root;
}

int search_trie(TrieNode* root, int* seq)
{
    // Searches for sequence in the Trie
    TrieNode* temp = root;

    for(int i=1; i <= seq[0]; i++)
    {
        int position = seq[i] - 1;
        if (temp->children[position] == NULL)
            return 0;
        temp = temp->children[position];
    }
    if (temp != NULL && temp->is_leaf)
        return 1;
    return 0;
}

int check_divergence(TrieNode* root, int* seq) {
    // Checks if there is branching at the last character of seq
    // and returns the largest position in the seq where branching occurs
    TrieNode* temp = root;
    if (seq[0] == 0)
        return 0;
    // We will return the largest index where branching occurs
    int last_index = 0;
    for (int i=1; i <= seq[0]; i++) {
        int position = seq[i] - 1;
        if (temp->children[position]) {
            // If a child exists at that position
            // we will check if there exists any other child
            // so that branching occurs
            for (int j=0; j<N; j++) {
                if (j != position && temp->children[j]) {
                    // We've found another child! This is a branch.
                    // Update the branch position
                    last_index = i + 1;
                    break;
                }
            }
            // Go to the next child in the sequence
            temp = temp->children[position];
        }
    }
    return last_index;
}

int is_leaf_node(TrieNode* root, char* seq) {
    // Checks if the prefix match of seq and root
    // is a leaf node
    TrieNode* temp = root;
    for (int i=1; i <= seq[0]; i++) {
        int position = (int) seq[i] - 1;
        if (temp->children[position]) {
            temp = temp->children[position];
        }
    }
    return temp->is_leaf;
}

void print_trie(TrieNode* root) {
    // Prints the nodes of the trie
    if (!root)
        return;
    TrieNode* temp = root;
    printf(" | %d", temp->data);
    for (int i=0; i<N; i++) {
        print_trie(temp->children[i]); 
    }
    printf(" ^");
}

void print_search(TrieNode* root, int* seq) {
    printf("Searching for ");
    for(int i=1; i <= seq[0]; i++) printf("%d", seq[i]);
    printf("\n");
    if (search_trie(root, seq) == 0)
        printf("Not Found\n");
    else
        printf("Found!\n");
}

using namespace std;

int main(int argc, char const *argv[]){
    TrieNode* root = make_trienode(0);
    // Cargando datos
    int n, x, aux, max;
    int* path = (int*) malloc((N+1)*(sizeof(int)));
    scanf("%d %d", &n, &max);
    for(int i = 0; i < n; i++){
        scanf("%d", &x);
        path[0] = x;
        for(int j=1; j<=x; j++){
            scanf("%d", &aux);
            path[j] = aux;
        }
        root = insert_trie(root, path);
    }
    printf("rutas\tbytesTrie\n");
    printf("%d\t%lld\n", n, sizeBytes);
    return 0; 
}