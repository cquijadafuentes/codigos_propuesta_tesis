#include <sdsl/suffix_trees.hpp>
#include <fstream>
#include <vector>

using namespace sdsl;
using namespace std;

int main(int argc, char const *argv[]){
	string texto = "umulmundumulmum";
	char letra = 'u';
	if(argc >= 2){
		texto = argv[1];
	}
	if(argc >=3){
		letra = argv[2][0];
	}
	cout << "Texto: " << texto << endl;
	cout << "Letra: " << letra << endl;
	cst_sct3<> cst;
	construct_im(cst, "umulmundumulmum", 1);
	
	cout << "Información del CompressedSuffixTree:" << endl;
	cout << "Cantidad de nodos: " << cst.nodes() << endl;
	cout << "Cantidad de hojas: " << cst.size() << endl;

	cout << "Recorrido de hijos de root del CompressedSuffixTree:" << endl;
	auto root = cst.root();
	cout << "id\ted_1\tdeg\tdep\tndep\tsize\tlb\trb\tsun\tleaf\ttext" << endl;
	for (auto& child: cst.children(root)) {
        cout << cst.id(child) << "\t";
        cout << "'" << cst.edge(child, 1) << "'" << "\t";		// D-th char of the edge-label
        cout << cst.degree(child) << "\t";		// Number of children
        cout << cst.depth(child) << "\t";		// String depth
        cout << cst.node_depth(child) << "\t";	// 
        cout << cst.size(child) << "\t";		// Number of leaves in the subtree
        cout << cst.lb(child) << "\t";			// Leftmost leaf
        cout << cst.rb(child) << "\t";			// Rightmost leaf
        cout << cst.sn(child) << "\t";			// Suffix number
        cout << cst.is_leaf(child) << "\t";		// IsLeaf
        for(int i=1; i<=cst.depth(child); i++){
        	cout << cst.edge(child, i);
        }
        cout << "\t" << endl;
    }

    cout << endl;
    cout << "BFS del CompressedSuffixTree:" << endl;
	cout << "id\ted_1\tdeg\tdep\tndep\tsize\tlb\trb\tsun\tleaf\ttext" << endl;
	typedef cst_bfs_iterator<cst_sct3<>> iterator;
    iterator begin = iterator(&cst, cst.root());
    iterator end   = iterator(&cst, cst.root(), true, true);
    int count = 0;
    for (iterator it = begin; it != end; ++it) {
        cout << cst.id(*it) << "\t";
        cout << "'" << cst.edge(*it, 1) << "'" << "\t";		// D-th char of the edge-label
        cout << cst.degree(*it) << "\t";		// Number of children
        cout << cst.depth(*it) << "\t";			// String depth
        cout << cst.node_depth(*it) << "\t";	// 
        cout << cst.size(*it) << "\t";			// Number of leaves in the subtree
        cout << cst.lb(*it) << "\t";			// Leftmost leaf
        cout << cst.rb(*it) << "\t";			// Rightmost leaf
        cout << cst.sn(*it) << "\t";			// Suffix number
        cout << cst.is_leaf(*it) << "\t";		// IsLeaf
        for(int i=1; i<=cst.depth(*it); i++){
        	cout << cst.edge(*it, i);
        }
        cout << "\t" << endl;

        if(++count % 5 == 0){
        	cout << endl;
        }
    }

    cout << endl;
    cout << "Pruebas de Navegación en CompressedSuffixTree: " << endl;
    
    auto nodeAux = cst.root();
    cout << "Root: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
    	cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.select_leaf(3);
    cout << "Leaf 3: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
    	cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.leftmost_leaf(cst.root());
    cout << "Leftmost: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
    	cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.rightmost_leaf(cst.root());
    cout << "Rightmost: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
    	cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.parent(cst.rightmost_leaf(cst.root()));
    cout << "Parent: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
    	cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.select_child(cst.child(cst.root(), 'l'),1);
    cout << "Sibling de 'lmu': " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
    	cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.select_child(cst.root(),6);
    cout << "Select child 6 de root: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
    	cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.child(cst.root(),'u');
    cout << "child starts 'u' desde root: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
    	cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.inv_id(7);
    cout << "inv_id de 7: " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
    	cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.sl(cst.inv_id(7));
    cout << "suffix link nodo 'mum': " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
    	cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;

    nodeAux = cst.wl(cst.inv_id(7), 'l');
    cout << "Weiner link nodo 'mum' y 'l': " << cst.id(nodeAux) << "\t'";
    for(int i=1; i<=cst.depth(nodeAux); i++){
    	cout << cst.edge(nodeAux, i);
    }
    cout << "'" << endl;


	return 0;
}