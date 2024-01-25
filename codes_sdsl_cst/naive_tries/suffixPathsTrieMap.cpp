#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

/*
    Las secuencias se definen con el largo al principio
    Por ejemplo, la secuencia [1,3,5] se define con el 
    arreglo: [3,1,3,5]
*/

#define N 11010

typedef struct TrieNode TrieNode;

struct TrieNode {
    int data;
    std::map<int, TrieNode*> children;
    bool is_leaf;
};

TrieNode* make_trienode(int data) {
    TrieNode* node = new TrieNode;
    node->is_leaf = false;
    node->data = data;
    return node;
}

void free_trienode(TrieNode* node) {
    for (auto& child : node->children) {
        free_trienode(child.second);
    }
    delete node;
}

TrieNode* insert_trie(TrieNode* root, int* seq) {
    TrieNode* temp = root;

    for (int i = 1; i <= seq[0]; i++) {
        int idx = seq[i] - 1;
        if (temp->children.find(idx) == temp->children.end()) {
            temp->children[idx] = make_trienode(seq[i]);
        }
        temp = temp->children[idx];
    }
    temp->is_leaf = true;
    return root;
}

int search_trie(TrieNode* root, int* seq) {
    TrieNode* temp = root;

    for (int i = 1; i <= seq[0]; i++) {
        int position = seq[i] - 1;
        if (temp->children.find(position) == temp->children.end())
            return 0;
        temp = temp->children[position];
    }
    return (temp != nullptr && temp->is_leaf) ? 1 : 0;
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
    for (auto& child : temp->children) {
        print_trie(child.second); 
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

long long int size_in_bytes(TrieNode* node) {
    long long int sizeBytes = 0;

    if (node != nullptr) {
        sizeBytes += sizeof(int) + sizeof(bool) + sizeof(std::map<int, TrieNode*>) 
            + node->children.size()*(sizeof(decltype(node->children)::key_type) 
            + sizeof(decltype(node->children)::mapped_type));

        for (auto& child : node->children) {
            sizeBytes += size_in_bytes(child.second);
        }
    }

    return sizeBytes;
}

int main(int argc, char const *argv[]){
    if(argc < 2){
        printf("Error! Faltan argumentos.");
        printf("%s <input_file>", argv[0]);
        return 0;
    }
    FILE* file = fopen(argv[1], "r");
    if(file == NULL){
        printf("Error! En la lectura del archivo\n");
        return 0;
    }
    TrieNode* root = make_trienode(0);
    // Cargando datos
    int n, x, aux, max;
    int* path = (int*) malloc((N+1)*(sizeof(int)));
    fscanf(file, "%d %d", &n, &max);
    for(int i = 0; i < n; i++){
        fscanf(file, "%d", &x);
        path[0] = x;
        for(int j=1; j<=x; j++){
            fscanf(file, "%d", &aux);
            path[j] = aux;
        }
        // Se insertan el camino y todos sus sufijos
        for(int j=0; j<x; j++){
            root = insert_trie(root, path+j);
            path[j+1] = path[j]-1;
        }
    }
    printf("rutas\tbytesSuffTrieMap\n");
    printf("%d\t%lld\n", n, size_in_bytes(root));
    //print_trie(root);
    return 0; 
}
