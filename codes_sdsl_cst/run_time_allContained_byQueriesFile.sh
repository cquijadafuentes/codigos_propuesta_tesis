#!/bin/bash

if [ $# -ne 4 ]; then
	echo "$(basename $0) <data_folder_1> <data_folder_2> <output_file> <repeticiones>"
	echo "Generará resultados para las 3 implementaciones de allContained <repeticiones> veces con los archivos de consulta correspondientes."
	exit 1
fi

DATAFOLDER1=$1
DATAFOLDER2=$2
OUTPUTFILE=$3
REPETICIONES=$4


echo "Fecha INICIO experimentos: $(date +'%Y/%m/%d %H:%M:%S') allContainedByQueriesFile_naive_gst" > ${OUTPUTFILE}

echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}
echo "**************** DATASET = ${DATAFOLDER1} ****************" >> ${OUTPUTFILE}
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}

for filename in gst_tripsMadrid_0005k gst_tripsMadrid_0010k gst_tripsMadrid_0015k gst_tripsMadrid_0020k gst_tripsMadrid_0025k
do
	./time_TopoRel_allContained_naive_gst_byQueriesFile ${DATAFOLDER1}${filename}.txt ${DATAFOLDER1}${filename}.naivepc ${DATAFOLDER1}${filename}.gst ${DATAFOLDER1}${filename}.queries.1 ${REPETICIONES} >> ${OUTPUTFILE}
done
echo " ----------------------------------------------------- " >> ${OUTPUTFILE}

for i in 2 3 
do
	for filename in gst_tripsMadrid_0005k gst_tripsMadrid_0010k gst_tripsMadrid_0015k gst_tripsMadrid_0020k gst_tripsMadrid_0025k
	do
		./time_TopoRel_allContained_naive_gst_byQueriesFile ${DATAFOLDER1}${filename}.txt ${DATAFOLDER1}${filename}.naivepc ${DATAFOLDER1}${filename}.gst ${DATAFOLDER1}${filename}.queries.${i} ${REPETICIONES} >> ${OUTPUTFILE}
	done
	echo " ----------------------------------------------------- " >> ${OUTPUTFILE}
done

echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}
echo "**************** DATASET = ${DATAFOLDER2} ****************" >> ${OUTPUTFILE}
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}

for filename in 20stops_min_05k  20stops_min_10k  20stops_min_15k  20stops_min_20k  20stops_min_25k
do
	./time_TopoRel_allContained_naive_gst_byQueriesFile ${DATAFOLDER2}${filename}.txt ${DATAFOLDER2}${filename}.naivepc ${DATAFOLDER2}${filename}.gst ${DATAFOLDER2}${filename}.queries.1 ${REPETICIONES} >> ${OUTPUTFILE}
done
echo " ----------------------------------------------------- " >> ${OUTPUTFILE}



for i in 2 3 
do
	for filename in 20stops_min_05k  20stops_min_10k  20stops_min_15k  20stops_min_20k  20stops_min_25k
	do
		./time_TopoRel_allContained_naive_gst_byQueriesFile ${DATAFOLDER2}${filename}.txt ${DATAFOLDER2}${filename}.naivepc ${DATAFOLDER2}${filename}.gst ${DATAFOLDER2}${filename}.queries.${i} ${REPETICIONES} >> ${OUTPUTFILE}
	done
	echo " ----------------------------------------------------- " >> ${OUTPUTFILE}
done

echo "Fecha FIN experimentos: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0
