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
	while(opc < 0 || opc > 9){
		cout << " ****************************** " << endl;
		cout << "1 - Información general del conjunto." << endl;
		cout << "2 - Información hijos de la raíz." << endl;
		cout << "3 - Información hijos de un nodo." << endl;
		cout << "4 - Espacio de la estrucutra." << endl;
		cout << "5 - Información de una ruta." << endl;
		cout << "6 - Mostrar csa." << endl;
		cout << "7 - Mostrar marcas fin de secuencia." << endl;
		cout << "8 - Mostrar lcp." << endl;
		cout << "9 - Arrays en columnas." << endl;
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
	cout << "Largo de secuencia más corta: " << gst5.len_min << endl;
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
	cout << "Id del padre del nodo: " << gst5.cst.id(gst5.cst.parent(nodo)) << endl;
	cout << "Nodo hoja: " << print_bool(gst5.cst.is_leaf(nodo)) << endl;
	cout << "Hojas del sub-árbol: " << gst5.cst.size(nodo) << endl;
	cout << "Cantidad de hijos: " << gst5.cst.degree(nodo) << endl;
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
	        //cout << extract(gst5.cst, child);
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
	cout << "Información de la ruta " << id << endl;
	cout << "Nodo id: " << gst5.cst.id(nodo) << endl;
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

void infoCSA(TopoRelGST_5 gst5){
	int size = gst5.cst.csa.size();
	cout << "Compressed Suffix Array de tamaño " << size << endl;
	for(int i=0; i<size; i++){
		cout << gst5.cst.csa[i] << " ";
	}
	cout << endl;
}

void infoLCP(TopoRelGST_5 gst5){
	int size = gst5.cst.lcp.size();
	cout << "Longest Common Prefix array de tamaño " << size << endl;
	for(int i=0; i<size; i++){
		cout << gst5.cst.lcp[i] << " ";
	}
	cout << endl;
}

void infoArrays(TopoRelGST_5 gst5){
	cout << "Información de Arrays involucrados" << endl;
	cout << "i\tMFS\trank\tcsa\tlcp" << endl;
	for(int i=0; i<gst5.cst.csa.size(); i++){
		cout << i << "\t";
		cout << gst5.gstMFSbv[i] << "\t";
		cout << gst5.gstMFSrank(i) << "\t";
		cout << gst5.cst.csa[i] << "\t";
		cout << gst5.cst.lcp[i] << endl;
	}
}

void infoMFS(TopoRelGST_5 gst5){
	int size = gst5.n_concat;
	cout << "gstMFSbv de tamaño " << size << endl;	
	for(int i=0; i<size; i++){
		cout << (i%10);
	}
	cout << endl;
	for(int i=0; i<size; i++){
		if(gst5.gstMFSbv[i] == 1){
			cout << "|";
		}else{
			cout << " ";
		}
	}
	cout << endl;
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
	    	case 6:
	    		infoCSA(gst5);
	    		break;
	    	case 7:
	    		infoMFS(gst5);
	    		break;
	    	case 8:
	    		infoLCP(gst5);
	    		break;
	    	case 9:
	    		infoArrays(gst5);
	    		break;
	    	default:
	    		cout << "Ocurrió algún error..." << endl;
    	}

    	opc = desplegarMenu();
    }
    return 0;
}