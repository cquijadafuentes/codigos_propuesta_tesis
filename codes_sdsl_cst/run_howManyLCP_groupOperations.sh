#!/bin/bash

if [ $# -ne 4 ]; then
	echo "$(basename $0) <data_folder> <output_file> consultas repeticiones"
	echo "Generará resultados operación de allContained con 500 consultas y 1 repetición y promedio de celdas recorridas del LCP durante la operación."
	exit 1
fi

DATAFOLDER=$1
OUTPUTFILE=$2
CONSULTAS=$3
REPETICIONES=$4


echo "Fecha INICIO experimentos: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTPUTFILE}

for filename in gst_tripsMadrid_0005k gst_tripsMadrid_0010k gst_tripsMadrid_0015k gst_tripsMadrid_0020k gst_tripsMadrid_0025k gst_tripsMadrid_0030k gst_tripsMadrid_0035k gst_tripsMadrid_0040k gst_tripsMadrid_0045k gst_tripsMadrid_0050k 
do
	./test_TopoRel_all_contained_howManyLCP ${DATAFOLDER}${filename}.gst5 ${CONSULTAS} ${REPETICIONES} >> ${OUTPUTFILE}
done
echo "Fecha FIN experimentos: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0