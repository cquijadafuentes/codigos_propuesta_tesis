#include <iostream>
#include "TopoRel_GST.hpp"

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
	while(opc < 0 || opc > 18){
		cout << " ****************************** " << endl;
		cout << "1 - Información general." << endl;
		cout << "2 - Información hijos de la raíz." << endl;
		cout << "3 - Información hijos de un nodo." << endl;
		cout << "4 - Espacio de la estrucutra." << endl;
		cout << "5 - Información de una ruta." << endl;
		cout << "6 - Mostrar csa." << endl;
		cout << "7 - Mostrar marcas fin de secuencia." << endl;
		cout << "8 - Mostrar lcp." << endl;
		cout << "9 - Mostrar Marcas de Nodos y Ramas." << endl;
		cout << "10 - Arrays en columnas." << endl;
		cout << "11 - Operación allContained." << endl;
		cout << "12 - Operación allIntersectPP." << endl;
		cout << "13 - ." << endl;
		cout << " ***** Navegación en Nodos ****" << endl;
		cout << "14 - Weiner Link de un nodo" << endl;
		cout << "15 - Mostrar contenido de gstMapNodo2Ruta" << endl;
		cout << "16 - Mostrar todas las rutas." << endl;
		cout << "17 - ." << endl;
		cout << "18 - Mostrar gstMapNodo2Ruta de un nodo" << endl;
		cout << "19 - " << endl;
		cout << "20 - " << endl;
		cout << "21 - " << endl;
		cout << "22 - " << endl;
		cout << "23 - " << endl;
		cout << "24 - " << endl;
		cout << "25 - " << endl;
		cout << "26 - " << endl;
		cout << " ****************************** " << endl;
		cout << "0 - Salir." << endl;
		cout << " ****************************** " << endl;
		cout << "INGRESE OPCIÓN: ";
		cin >> opc;
	}
	return opc;
}

//--------------	CASE 1	------------//
void infoGralConjunto(TopoRelGST gst){
	cout << "--- Información del conjunto ---" << endl;
	cout << "Nº de rutas: " << gst.n_rutas << endl;
	cout << "Largo de la secuencia total: " << gst.n_concat << endl;
	cout << "Nº de stops: " << gst.n_stops << endl;
	cout << "Largo de secuencia más corta: " << gst.len_min << endl;
	cout << "Largo de secuencia más larga: " << gst.len_max << endl;
	cout << "Marca de fin de secuenica: " << gst.finSec << endl;
	cout << "Largo de gstMapRuta2Nodo: " << gst.gstMapRuta2Nodo.size();
	cout << "Largo de gstMapNodo2Ruta: " << gst.gstMapNodo2Ruta.size() << endl;
	cout << "Largo de gstMFSbv (Marcas fin de secuencia): " << gst.gstMFSbv.size() << endl;
	cout << "--- Información del CompressedSuffixTree ---" << endl;
    cout << "Cantidad de nodos del CST: " << gst.cst.nodes() << endl;
    cout << "Cantidad de hojas del CST: " << gst.cst.size() << endl;
    sd_vector<>::rank_1_type nodesRank(&gst.gstMNodos);
    cout << "Cantidad de 1s en markNodes: " << nodesRank.rank(gst.cst.nodes()) << endl;
    sd_vector<>::rank_1_type branchesRank(&gst.gstMRamas);
    cout << "Cantidad de 1s en markRamas: " << branchesRank.rank(gst.cst.nodes()) << endl;
    cout << "Largo del Suffix Array: " << gst.cst.csa.size() << endl;
    cout << "Largo del LCP Array: " << gst.cst.lcp.size() << endl;
}

//--------------	CASE 2	------------//
void infoRootChildren(TopoRelGST gst){
    auto root = gst.cst.root();
	cout << "--- Información de la raíz del CST ---" << endl;
	cout << "Hojas del árbol: " << gst.cst.size(root) << endl;
	cout << "Cantidad de hijos: " << gst.cst.degree(root) << endl;
	cout << "--- Hijos de la raíz ---" << endl;
    cout << "id\ted_1\tdeg\tdep\tndep\tsize\tlb\trb\tsun\tleaf\ttext" << endl;
    for (auto child: gst.cst.children(root)) {
        cout << gst.cst.id(child) << "\t";
        cout << "'" << gst.cst.edge(child, 1) << "'" << "\t";       // D-th char of the edge-label
        cout << gst.cst.degree(child) << "\t";      // Number of children
        cout << gst.cst.depth(child) << "\t";       // String depth
        cout << gst.cst.node_depth(child) << "\t";  // 
        cout << gst.cst.size(child) << "\t";        // Number of leaves in the subtree
        cout << gst.cst.lb(child) << "\t";          // Leftmost leaf
        cout << gst.cst.rb(child) << "\t";          // Rightmost leaf
        cout << gst.cst.sn(child) << "\t";          // Suffix number
        cout << gst.cst.is_leaf(child) << "\t";     // IsLeaf
        for(int i=1; i<=gst.cst.depth(child); i++){
            cout << gst.cst.edge(child, i);
        }
        cout << "\t" << endl;
    }
    cout << endl;
}

//--------------	CASE 3	------------//
void infoNodo(TopoRelGST gst){
	cout << "Ingrese id del nodo a evaluar: ";
	int id;
	cin >> id;
	auto nodo = gst.cst.inv_id(id);
	if(gst.cst.id(nodo) != id){
		cout << "El nodo solicitado no existe." << endl;
		return;
	}
	cout << "Información del nodo " << id << endl;
	cout << "Profundidad del nodo: " << gst.cst.node_depth(nodo) << endl;
	cout << "Secuencia del nodo: " << extract(gst.cst, nodo) << endl;
	cout << "Profundidad : " << gst.cst.depth(nodo) << endl;
	cout << "Id del padre del nodo: " << gst.cst.id(gst.cst.parent(nodo)) << endl;
	cout << "Marca en gstMRamas: " << gst.gstMRamas[id] << endl;
	cout << "Marca en gstMNodos: " << gst.gstMNodos[id] << endl;
	cout << "Nodo hoja: " << print_bool(gst.cst.is_leaf(nodo)) << endl;
	cout << "Hojas del sub-árbol: " << gst.cst.size(nodo);
	int left = gst.cst.lb(nodo);
	int right = gst.cst.rb(nodo);
	cout << " - desde " << left << " hasta " << right << endl;
	cout << "Sibling: " << gst.cst.id(gst.cst.sibling(nodo)) << endl;
	cout << "SuffixLink: " << gst.cst.id(gst.cst.sl(nodo)) << endl;
	cout << "Cantidad de hijos: " << gst.cst.degree(nodo) << endl;
	if(!gst.cst.is_leaf(nodo)){
		cout << "--- Información de los hijos del nodo ---" << endl;
		cout << "id\ted_1\tdeg\tdep\tndep\tsize\tlb\trb\tsun\tleaf\ttext" << endl;
	    for (auto child: gst.cst.children(nodo)) {
	        cout << gst.cst.id(child) << "\t";
	        cout << "'" << gst.cst.edge(child, 1) << "'" << "\t";       // D-th char of the edge-label
	        cout << gst.cst.degree(child) << "\t";      // Number of children
	        cout << gst.cst.depth(child) << "\t";       // String depth
	        cout << gst.cst.node_depth(child) << "\t";  // 
	        cout << gst.cst.size(child) << "\t";        // Number of leaves in the subtree
	        cout << gst.cst.lb(child) << "\t";          // Leftmost leaf
	        cout << gst.cst.rb(child) << "\t";          // Rightmost leaf
	        cout << gst.cst.sn(child) << "\t";          // Suffix number
	        cout << gst.cst.is_leaf(child) << "\t";     // IsLeaf
	        cout << extract(gst.cst, child);
	        cout << "\t" << endl;
	    }
	}
}

//--------------	CASE 	------------//
void infoRuta(TopoRelGST gst){
	cout << "Ingrese id de la ruta: ";
	int id;
	cin >> id;
	cout << endl;
	if(id < 0 || id >= gst.n_rutas){
		cout << "La ruta debe ser un número entre 0 y " << gst.n_rutas << "." << endl;
		return;
	}

	auto nodo = gst.gstMapRuta2Nodo[id];
	cout << "Información de la ruta " << id << endl;
    cout << "getRuta: ";
    vector<int> r = gst.getRuta(id);
    for (int i = 0; i < r.size(); ++i){
    	cout << r[i] << " ";
    }
    cout << endl;
    cout << "Largo ruta: " << gst.getLargoRuta(id) << endl;
	cout << "Nodo id: " << gst.cst.id(nodo) << endl;
	if(gst.cst.depth(nodo) > 100){
		cout << "Largo de la secuencia del nodo muy extensa... omitida" << endl;
	}else{
		cout << "Secuencia del nodo: " << extract(gst.cst, nodo) << endl;
	}
	cout << "Profundidad secuencia: " << gst.cst.depth(nodo) << endl;
	cout << "Profundidad del nodo: " << gst.cst.node_depth(nodo) << endl;
	cout << "Id del padre del nodo: " << gst.cst.id(gst.cst.parent(nodo)) << endl;
	cout << "Nodo raíz: " << print_bool(gst.cst.parent(nodo) == gst.cst.root()) << endl;
	cout << "Nodo hoja: " << print_bool(gst.cst.is_leaf(nodo)) << endl;;
	cout << "Hojas del sub-árbol: " << gst.cst.size(nodo);
	cout << "Cantidad de hijos: " << gst.cst.degree(nodo);
	if(!gst.cst.is_leaf(nodo)){
		cout << "--- Información de los hijos del nodo ---" << endl;
		cout << "id\ted_1\tdeg\tdep\tndep\tsize\tlb\trb\tsun\tleaf\ttext" << endl;
	    for (auto child: gst.cst.children(nodo)) {
	        cout << gst.cst.id(child) << "\t";
	        cout << "'" << gst.cst.edge(child, 1) << "'" << "\t";       // D-th char of the edge-label
	        cout << gst.cst.degree(child) << "\t";      // Number of children
	        cout << gst.cst.depth(child) << "\t";       // String depth
	        cout << gst.cst.node_depth(child) << "\t";  // 
	        cout << gst.cst.size(child) << "\t";        // Number of leaves in the subtree
	        cout << gst.cst.lb(child) << "\t";          // Leftmost leaf
	        cout << gst.cst.rb(child) << "\t";          // Rightmost leaf
	        cout << gst.cst.sn(child) << "\t";          // Suffix number
	        cout << gst.cst.is_leaf(child) << "\t";     // IsLeaf
	        //cout << "\t" << extract(gst.cst, child) << endl;
	    }
	}
	cout << endl;
}

//--------------	CASE 	------------//
void infoCSA(TopoRelGST gst){
	int size = gst.cst.csa.size();
	cout << "Compressed Suffix Array de tamaño " << size << endl;
	for(int i=0; i<size; i++){
		cout << gst.cst.csa[i] << " ";
	}
	cout << endl;
}

//--------------	CASE 	------------//
void infoLCP(TopoRelGST gst){
	int size = gst.cst.lcp.size();
	cout << "Longest Common Prefix array de tamaño " << size << endl;
	for(int i=0; i<size; i++){
		cout << gst.cst.lcp[i] << " ";
	}
	cout << endl;
}

//--------------	CASE 	------------//
void infoMNodos(TopoRelGST gst){
	int size = gst.gstMNodos.size();
	cout << "Arreglo de Nodos Marcados: " << size << endl;
	for(int i=0; i<size; i++){
		cout << gst.gstMNodos[i] << " ";
	}
	cout << endl;
	for(int i=0; i<size; i++){
		cout << i%10 << " ";
	}
	cout << endl;
	
	size = gst.gstMRamas.size();
	cout << "Arreglo de Ramas con hijos Marcados: " << size << endl;
	for(int i=0; i<size; i++){
		cout << gst.gstMRamas[i] << " ";
	}
	cout << endl;
	for(int i=0; i<size; i++){
		cout << i%10 << " ";
	}
	cout << endl;
}

//--------------	CASE 	------------//
void infoArrays(TopoRelGST gst){
	cout << "Información de Arrays involucrados" << endl;
	cout << "i\tMFS\trank\tcsa\tlcp" << endl;
	for(int i=0; i<gst.cst.csa.size(); i++){
		cout << i << "\t";
		cout << gst.gstMFSbv[i] << "\t";
		cout << gst.gstMFSrank(i) << "\t";
		cout << gst.cst.csa[i] << "\t";
		cout << gst.cst.lcp[i] << endl;
	}
}

//--------------	CASE 	------------//
void infoMFS(TopoRelGST gst){
	int size = gst.n_concat;
	cout << "gstMFSbv de tamaño " << size << endl;	
	cout << endl;
	for(int i=0; i<size; i++){
		cout << (i%10);
	}
	cout << endl;
	for(int i=0; i<size; i++){
		if(gst.gstMFSbv[i] == 1){
			cout << "|";
		}else{
			cout << " ";
		}
	}
	cout << endl;
}

//--------------	CASE 11	------------//
void allContainedOperation(TopoRelGST gst){
	cout << "All Contained" << endl;
	cout << "Ingrese la id de la ruta (entre 0 y " << (gst.n_rutas-1) << "): ";
	int x;
	cin >> x;
	cout << endl;
	vector<int> res = gst.tr_allContained(x);
	cout << "El resultado contiene " << res.size() << " elementos: " << endl;
	for(int i=0; i<res.size(); i++){
		cout << res[i] << "  ";
	}
	cout << endl;
}

//--------------	CASE 11	------------//
void allIntersectionPPOperation(TopoRelGST gst){
	cout << "All Intersection PlusPlus" << endl;
	cout << "Ingrese la id de la ruta (entre 0 y " << (gst.n_rutas-1) << "): ";
	int x;
	cin >> x;
	cout << endl;
	unordered_map<int,int> res = gst.tr_allIntersectPP(x);
	cout << "El resultado contiene " << res.size() << " elementos: " << endl;
	for (const auto& pair:res) {
        cout << pair.first << ": " << pair.second << endl;
    }
	cout << endl;
}

//--------------	CASE 	------------//
void weinerLink(TopoRelGST gst){
	cout << "Weiner Link de un nodo" << endl;
	cout << "Ingrese el id del nodo que desea consultar: ";
	int x;
	cin >> x;
	auto nodo = gst.cst.inv_id(x);
	cout << "Ingrese el elemento para la consulta de wl: ";
	int c;
	cin >> c;
	cout << endl;
	auto nodoWL = gst.cst.wl(nodo, c);
	cout << "Nodo resultante id: " << gst.cst.id(nodoWL) << endl;
	cout << "Secuencia original: " << extract(gst.cst, nodo) << endl;
	cout << "Secuencia obtenida: " << extract(gst.cst, nodoWL) << endl;
	cout << endl;
}

//--------------	CASE 15	------------//
void nodo2Rutas(TopoRelGST gst){
	cout << "gstMapNodo2Ruta contiene " << gst.gstMapNodo2Ruta.size() << " elementos." << endl;
	for(auto it=gst.gstMapNodo2Ruta.begin(); it != gst.gstMapNodo2Ruta.end(); it++){
		cout << "Nodo " << it->first << " contiene la ruta " << it->second << endl;
	}
	cout << endl;
}

//--------------	CASE 15	------------//
void mostrarRutas(TopoRelGST gst){
	cout << "Mostrando las " << gst.n_rutas << " rutas de la estructura:" << endl;
	for(int i=0; i<gst.n_rutas; i++){
		cout << "Ruta " << i << ": ";
		gst.printRuta(i);
		cout << endl;
	}
	cout << endl;
}


//--------------	CASE 18	------------//
void nodo2RutasEnNodo(TopoRelGST gst){
	cout << "Nodo2Ruta de un nodo" << endl;
	cout << "Ingrese el id del nodo que desea consultar: ";
	int x;
	cin >> x;
	auto nodo = gst.cst.inv_id(x);
	x = gst.cst.id(nodo);
	int count = gst.gstMapNodo2Ruta.count(x);
	cout << "El nodo con id " << x << " tiene " << count << " rutas asociadas." << endl;
	auto rango = gst.gstMapNodo2Ruta.equal_range(x);
    for(auto j=rango.first; j!=rango.second; j++){
		cout << j->second << " ";
    }
    cout << endl;
}

int main(int argc, char const *argv[]){

	if(argc < 2){
		cout << "Error! faltan argumentos." << endl;
		cout << argv[0] << " <input_filename>" << endl;
		return 0;
	}
	
	TopoRelGST gst(argv[1]);
	

    int opc = desplegarMenu();
    while(opc != 0){    	
		cout << " ****************************** " << endl;
    	switch(opc){
		    case 0:
		    	cout << "FIN" << endl;
		    	return 0;
	    	case 1:
	    		infoGralConjunto(gst);
	    		break;
	    	case 2:
	    		infoRootChildren(gst);
	    		break;
	    	case 3:
	    		infoNodo(gst);
	    		break;
	    	case 4:
	    		gst.sizeEstructura();
	    		break;
	    	case 5:
	    		infoRuta(gst);
	    		break;
	    	case 6:
	    		infoCSA(gst);
	    		break;
	    	case 7:
	    		infoMFS(gst);
	    		break;
	    	case 8:
	    		infoLCP(gst);
	    		break;
	    	case 9:
	    		infoMNodos(gst);
	    		break;
	    	case 10:
	    		infoArrays(gst);
	    		break;
	    	case 11:
	    		allContainedOperation(gst);
	    		break;
	    	case 12:
	    		allIntersectionPPOperation(gst);
	    		break;
	    	case 13:
	    		cout << "No implementada ... " << endl;
	    		break;
	    	case 14:
	    		weinerLink(gst);
	    		break;
	    	case 15:
	    		nodo2Rutas(gst);
	    		break;
	    	case 16:
	    		mostrarRutas(gst);
	    		break;
	    	case 17:
	    		cout << "No implementada ... " << endl;
	    		break;
	    	case 18:
	    		nodo2RutasEnNodo(gst);
	    		break;
	    	default:
	    		cout << "Ocurrió algún error..." << endl;
    	}

    	opc = desplegarMenu();
    }
    return 0;
}