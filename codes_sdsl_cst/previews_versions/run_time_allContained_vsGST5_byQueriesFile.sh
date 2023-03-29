#!/bin/bash

if [ $# -ne 5 ]; then
	echo "$(basename $0) <data_folder_1> <data_folder_2> <output_file> <repeticiones1> <repeticiones2>"
	echo "Generará resultados para las 3 operaciones de conjunto <repeticiones> veces con los archivos de consulta correspondientes."
	echo "<repeticiones1> es para los experimentos de queries.1 y <repeticiones2> es para queries.2 y queries.3"
	exit 1
fi

DATAFOLDER1=$1
DATAFOLDER2=$2
OUTPUTFILE=$3
REPETICIONES1=$4
REPETICIONES2=$5


echo "Fecha INICIO experimentos: $(date +'%Y/%m/%d %H:%M:%S') allContainedByQueriesFile_GST5" > ${OUTPUTFILE}

echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}
echo "**************** DATASET = ${DATAFOLDER1} ****************" >> ${OUTPUTFILE}
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}

for filename in gst_tripsMadrid_0005k gst_tripsMadrid_0010k gst_tripsMadrid_0015k gst_tripsMadrid_0020k gst_tripsMadrid_0025k
do
	./time_TopoRel_allContained_vsGST5_byQueriesFile ${DATAFOLDER1}${filename}.gst5 ${DATAFOLDER1}${filename}.queries.1 ${REPETICIONES1} >> ${OUTPUTFILE}
done
echo " ----------------------------------------------------- " >> ${OUTPUTFILE}

for i in 2 3 
do
	for filename in gst_tripsMadrid_0005k gst_tripsMadrid_0010k gst_tripsMadrid_0015k gst_tripsMadrid_0020k gst_tripsMadrid_0025k
	do
		./time_TopoRel_allContained_vsGST5_byQueriesFile ${DATAFOLDER1}${filename}.gst5 ${DATAFOLDER1}${filename}.queries.${i} ${REPETICIONES2} >> ${OUTPUTFILE}
	done
	echo " ----------------------------------------------------- " >> ${OUTPUTFILE}
done

echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}
echo "**************** DATASET = ${DATAFOLDER2} ****************" >> ${OUTPUTFILE}
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}

for filename in 20stops_min_05k  20stops_min_10k  20stops_min_15k  20stops_min_20k  20stops_min_25k
do
	./time_TopoRel_allContained_vsGST5_byQueriesFile ${DATAFOLDER2}${filename}.gst5 ${DATAFOLDER2}${filename}.queries.1 ${REPETICIONES1} >> ${OUTPUTFILE}
done
echo " ----------------------------------------------------- " >> ${OUTPUTFILE}



for i in 2 3 
do
	for filename in 20stops_min_05k  20stops_min_10k  20stops_min_15k  20stops_min_20k  20stops_min_25k
	do
		./time_TopoRel_allContained_vsGST5_byQueriesFile ${DATAFOLDER2}${filename}.gst5 ${DATAFOLDER2}${filename}.queries.${i} ${REPETICIONES2} >> ${OUTPUTFILE}
	done
	echo " ----------------------------------------------------- " >> ${OUTPUTFILE}
done

echo "Fecha FIN experimentos: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0
