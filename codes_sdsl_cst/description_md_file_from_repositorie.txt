# TGRST_paper_pub
Respository with clean codes of TGRST paper.
Este repositorio tiene por objetivo difundir la implementación funcional del Topological Relation Generalized Suffix-Tree (TRGST).
Esta implementación representa un conjunto de caminos como secuencias en un Generalized Suffix-Tree y permite el realizar consultas de relaciones entre los caminos.
La clase ToporRel_GST (.cpp y .hpp) corresponde a la implementación de la estructura basada en elementos de la librería sdsl de Simón Gog.
La implementación ingenua que se utiliza como baseline de comparación en el artículo esta en la clase TopoRel_Naive (.cpp y .hpp) y también utiliza elementos de la librería sdsl.

Para la instalación de la librería se deben seguir las instrucciones estándar del repositorio: https://github.com/simongog/sdsl-lite

El código publicado es funcional y se proporcionan dos archivos para realizar pruebas de las funciones implementadas.

# Archivos de Prueba

En la carpeta ./dataset/ se pueden encontrar tres archivos:

- gst_tripsMadrid_0005k.txt
	Corresponde al archivo con la definición del conjunto de caminos que se utiliza como entrada para la representación ingenua (naive). El formato es un archivo de texto plano donde la primera línea contiene dos enteros separados por un espacio, el primero indica la cantidad de caminos que contiene el archivo y el segundo la cantidad de stops para la red sobre la que pasan estos caminos. Luego siguen tantas líneas como caminos del conjunto, donde cada linea define primero un entero con la cantidad de elementos que componen la secuencia y luego, separados por espacio, todos los identificadores que componen el camino correspondiente. Cada identificador corresponde a un número entero que puede estar entre [1,s], donde s es la cantidad de stops de la red sobre la que se definen los caminos.

- gst_tripsMadrid_0005k.gst
	Corresponde a la representación en formato TRGST para el mismo conjunto anterior. Esta construcción se puede hacer mediante la ejecución del ./TopoRel_GST_build entregando como parámetro el archivo naive (.txt) y el nombre del archivo de salida (.gst), tal como muestra el siguiente ejemplo:
	./TopoRel_GST_build dataset/gst_tripsMadrid_0005k.txt dataset/gst_tripsMadrid_0005k.gst

- gst_tripsMadrid_0005k.queries
	Este archivo contiene las consultas que se utilizan para ejecutar pruebas en las que se realizan varias consultas de una sola ejecución. Este archivo contiene dos líneas, en la primera se indica la cantidad de consultas que contiene el archivo y la segunda línea contiene los elementos a consultar separados por espacios. Cada elemento corresponde a un identificador del conjunto de caminos para el que corresponde el archivo de consultas.


# Archivos posibles a ejecutar

Al compilar el proyecto (recordar que se requiere la instalación previa de la librería sdsl) se generan los archivos ejecutables que se listan a continuación:

time_allTopoRels_byQueriesFile
	Entrada: 
		input_file.txt: Nombre del archivo de entrada en formato naive (.txt).
		input_file.gst: Nombre del archivo de entrada en formato TRGST (.gst).
		queries_file: Nombre del archivo de consultas.
		repeticiones: Cantidad de repeticiones a utilizar para determinar el tiempo promedio de responder las consultas.
		min_intersetion (opcional): Cantidad de intersecciones requeridas para calcular resultado de all_intersect.
	Salida: Por cada una de las operaciones implementadas entrega nombre de la operación, cantidad de rutas del conjunto, cantidad de consultas de la ejecución, tiempo de la implementación Naive, tiempo de la implementación TRGST, cantidad de elementos en el resultado de la implementación Naive, y cantidad de elementos en el resultado de la implementación TRGST.

TopoRel_GST_build
	Entrada: nombre del archivo de entrada (formato naive) y nombre del archivo de salida (formato .gst)
	Salida: al finalizar su ejecución se construyó en base al archivo de entrada la representación en formato TRGST en el archivo de salida.

TopoRel_GST_size
	Entrada: nombre del archivo de entrada (Debe estar en formato .gst)
	Salida: tamaño en bytes de cada sub-estructura que compone el TRGST.

TopoRel_GST_test: La entrada corresponde al nombre del archivo en formato trgst. Despliega un menú de opciones que permite probar algunas de las funcionalidades implementadas con salida en detalle y permitiendo ejecutar consultas por identificadores ingresados manualmente.

TopoRel_GST_time: 
	Entrada: nombre del archivo de entrada en formato gst, nombre del archivo de consultas, identificador de la operación que se desea ejecutar.
	El identificador puede ser: (1) allContain, (2) allContained, (3) allEqual, ó (4) allIntersect.
	Salida: tiempo que tarda en calcularse la operación indicada por el id de la operación.

Los archivos TopoRel_Naive_size, TopoRel_Naive_test, y TopoRel_Naive_time son equivalentes a los antes descritos pero con la entrada correspondiente a archivos en formato naive.

