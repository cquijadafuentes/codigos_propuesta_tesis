#include <iostream>
#include "TopoRel_GST_5.hpp"

using namespace std;
using namespace sdsl;

string print_bool(bool x){
	if(x){
		return "true";
	}
	return "false";
}

int desplegarMenu(){
	int opc = -1;
	while(opc < 0 || opc > 5){
		cout << " ****************************** " << endl;
		cout << "1 - Información general del conjunto." << endl;
		cout << "2 - Información hijos de la raíz." << endl;
		cout << "3 - Información hijos de un nodo." << endl;
		cout << "4 - Espacio de la estrucutra." << endl;
		cout << "5 - Información de una ruta." << endl;
		cout << "" << endl;
		cout << "" << endl;
		cout << "" << endl;
		cout << "0 - Salir." << endl;
		cout << " ****************************** " << endl;
		cout << "INGRESE OPCIÓN: ";
		cin >> opc;
	}
	return opc;
}

void infoGralConjunto(TopoRelGST_5 gst5){
	cout << "--- Información del conjunto ---" << endl;
	cout << "Nº de rutas: " << gst5.n_rutas << endl;
	cout << "Largo de la secuencia total: " << gst5.n_concat << endl;
	cout << "Nº de stops: " << gst5.n_stops << endl;
	cout << "--- Información del CompressedSuffixTree ---" << endl;
    cout << "Cantidad de nodos: " << gst5.cst.nodes() << endl;
    cout << "Cantidad de hojas: " << gst5.cst.size() << endl;
    cout << "Cantidad de nodos del CST: " << gst5.cst.nodes() << endl;
}

void infoRootChildren(TopoRelGST_5 gst5){
    auto root = gst5.cst.root();
	cout << "--- Información de la raíz del CST ---" << endl;
	cout << "Hojas del árbol: " << gst5.cst.size(root) << endl;
	cout << "Cantidad de hijos: " << gst5.cst.degree(root) << endl;
	cout << "--- Hijos de la raíz ---" << endl;
    cout << "id\ted_1\tdeg\tdep\tndep\tsize\tlb\trb\tsun\tleaf\ttext" << endl;
    for (auto child: gst5.cst.children(root)) {
        cout << gst5.cst.id(child) << "\t";
        cout << "'" << gst5.cst.edge(child, 1) << "'" << "\t";       // D-th char of the edge-label
        cout << gst5.cst.degree(child) << "\t";      // Number of children
        cout << gst5.cst.depth(child) << "\t";       // String depth
        cout << gst5.cst.node_depth(child) << "\t";  // 
        cout << gst5.cst.size(child) << "\t";        // Number of leaves in the subtree
        cout << gst5.cst.lb(child) << "\t";          // Leftmost leaf
        cout << gst5.cst.rb(child) << "\t";          // Rightmost leaf
        cout << gst5.cst.sn(child) << "\t";          // Suffix number
        cout << gst5.cst.is_leaf(child) << "\t";     // IsLeaf
        for(int i=1; i<=gst5.cst.depth(child); i++){
            cout << gst5.cst.edge(child, i);
        }
        cout << "\t" << endl;
    }
    cout << endl;
}

void infoNodo(TopoRelGST_5 gst5){
	cout << "Ingrese id del nodo a evaluar: ";
	int id;
	cin >> id;
	auto nodo = gst5.cst.inv_id(id);
	if(gst5.cst.id(nodo) != id){
		cout << "El nodo solicitado no existe." << endl;
		return;
	}
	cout << "Información del nodo " << id << endl;
	cout << "Profundidad del nodo: " << gst5.cst.node_depth(nodo) << endl;
	cout << "Secuencia del nodo: " << extract(gst5.cst, nodo) << endl;
	cout << "Profundidad : " << gst5.cst.depth(nodo) << endl;
	cout << "Id del padre del nodo: " << gst5.cst.id(gst5.cst.parent(nodo));
	cout << "Nodo hoja: " << print_bool(gst5.cst.is_leaf(nodo)) << endl;;
	cout << "Hojas del sub-árbol: " << gst5.cst.size(nodo);
	cout << "Cantidad de hijos: " << gst5.cst.degree(nodo);
	if(!gst5.cst.is_leaf(nodo)){
		cout << "--- Información de los hijos del nodo ---" << endl;
		cout << "id\ted_1\tdeg\tdep\tndep\tsize\tlb\trb\tsun\tleaf\ttext" << endl;
	    for (auto child: gst5.cst.children(nodo)) {
	        cout << gst5.cst.id(child) << "\t";
	        cout << "'" << gst5.cst.edge(child, 1) << "'" << "\t";       // D-th char of the edge-label
	        cout << gst5.cst.degree(child) << "\t";      // Number of children
	        cout << gst5.cst.depth(child) << "\t";       // String depth
	        cout << gst5.cst.node_depth(child) << "\t";  // 
	        cout << gst5.cst.size(child) << "\t";        // Number of leaves in the subtree
	        cout << gst5.cst.lb(child) << "\t";          // Leftmost leaf
	        cout << gst5.cst.rb(child) << "\t";          // Rightmost leaf
	        cout << gst5.cst.sn(child) << "\t";          // Suffix number
	        cout << gst5.cst.is_leaf(child) << "\t";     // IsLeaf
	        for(int i=1; i<=gst5.cst.depth(child); i++){
	            cout << gst5.cst.edge(child, i);
	        }
	        cout << "\t" << endl;
	    }
	}
}

void infoRuta(TopoRelGST_5 gst5){
	cout << "Ingrese id de la ruta: ";
	int id;
	cin >> id;
	if(id < 0 || id >= gst5.n_rutas){
		cout << "La ruta debe ser un número entre 0 y " << gst5.n_rutas << "." << endl;
		return;
	}

	auto nodo = gst5.gstMapa[id];
	cout << "Información del nodo " << id << endl;
	cout << "Profundidad del nodo: " << gst5.cst.node_depth(nodo) << endl;
	cout << "Secuencia del nodo: " << extract(gst5.cst, nodo) << endl;
	cout << "Profundidad : " << gst5.cst.depth(nodo) << endl;
	cout << "Id del padre del nodo: " << gst5.cst.id(gst5.cst.parent(nodo));
	cout << "Nodo hoja: " << print_bool(gst5.cst.is_leaf(nodo)) << endl;;
	cout << "Hojas del sub-árbol: " << gst5.cst.size(nodo);
	cout << "Cantidad de hijos: " << gst5.cst.degree(nodo);
	if(!gst5.cst.is_leaf(nodo)){
		cout << "--- Información de los hijos del nodo ---" << endl;
		cout << "id\ted_1\tdeg\tdep\tndep\tsize\tlb\trb\tsun\tleaf" << endl;
	    for (auto child: gst5.cst.children(nodo)) {
	        cout << gst5.cst.id(child) << "\t";
	        cout << "'" << gst5.cst.edge(child, 1) << "'" << "\t";       // D-th char of the edge-label
	        cout << gst5.cst.degree(child) << "\t";      // Number of children
	        cout << gst5.cst.depth(child) << "\t";       // String depth
	        cout << gst5.cst.node_depth(child) << "\t";  // 
	        cout << gst5.cst.size(child) << "\t";        // Number of leaves in the subtree
	        cout << gst5.cst.lb(child) << "\t";          // Leftmost leaf
	        cout << gst5.cst.rb(child) << "\t";          // Rightmost leaf
	        cout << gst5.cst.sn(child) << "\t";          // Suffix number
	        cout << gst5.cst.is_leaf(child) << "\t";     // IsLeaf
	        cout << "\t" << endl;
	    }
	}
}

int main(int argc, char const *argv[]){

	if(argc < 2){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <input_filename>" << endl;
		return 0;
	}
	
	TopoRelGST_5 gst5(argv[1]);
	

    int opc = desplegarMenu();
    while(opc != 0){    	
		cout << " ****************************** " << endl;
    	switch(opc){
		    case 0:
		    	cout << "FIN" << endl;
		    	return 0;
	    	case 1:
	    		infoGralConjunto(gst5);
	    		break;
	    	case 2:
	    		infoRootChildren(gst5);
	    		break;
	    	case 3:
	    		infoNodo(gst5);
	    		break;
	    	case 4:
	    		gst5.sizeEstructura();
	    		break;
	    	case 5:
	    		infoRuta(gst5);
	    		break;
	    	default:
	    		cout << "Ocurrió algún error..." << endl;
    	}

    	opc = desplegarMenu();
    }
    return 0;
}