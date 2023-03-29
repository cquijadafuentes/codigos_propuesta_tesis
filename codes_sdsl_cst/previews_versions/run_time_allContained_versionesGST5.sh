#!/bin/bash

if [ $# -ne 5 ]; then
	echo "$(basename $0) <data_folder_1> <data_folder_2> <output_file> consultas repeticiones"
	echo "Generará resultados para las 3 operaciones de conjunto con <consultas> y <repeticiones>."
	exit 1
fi

DATAFOLDER1=$1
DATAFOLDER2=$2
OUTPUTFILE=$3
CONSULTAS=$4
REPETICIONES=$5


echo "Fecha INICIO experimentos: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTPUTFILE}

echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}
echo "**************** DATASET = ${DATAFOLDER1} ****************" >> ${OUTPUTFILE}
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}

for filename in gst_tripsMadrid_0005k gst_tripsMadrid_0010k gst_tripsMadrid_0015k gst_tripsMadrid_0020k gst_tripsMadrid_0025k
do
	./time_TopoRel_allContained_versionesGST5 ${DATAFOLDER1}${filename}.gst5 ${CONSULTAS} ${REPETICIONES} >> ${OUTPUTFILE}
done

echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}
echo "**************** DATASET = ${DATAFOLDER2} ****************" >> ${OUTPUTFILE}
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}

for filename in 20stops_min_05k  20stops_min_10k  20stops_min_15k  20stops_min_20k  20stops_min_25k
do
	./time_TopoRel_allContained_versionesGST5 ${DATAFOLDER2}${filename}.gst5 ${CONSULTAS} ${REPETICIONES} >> ${OUTPUTFILE}
done

echo "Fecha FIN experimentos: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0
