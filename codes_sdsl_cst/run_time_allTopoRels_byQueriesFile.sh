#!/bin/bash

if [ $# -ne 4 ]; then
	echo "$(basename $0) <data_folder_1> <data_folder_2> <output_file> <repeticiones1>"
	echo "Ejecuta time_allTopoRels_byQueriesFile para los archivos de trips (<data_folder_1>) y trips20+ (<data_folder_2>)."
	exit 1
fi

DATAFOLDER1=$1
DATAFOLDER2=$2
OUTPUTFILE=$3
REPETICIONES=$4


echo "Fecha INICIO experimentos: $(date +'%Y/%m/%d %H:%M:%S') allContainedByQueriesFile_GST6" > ${OUTPUTFILE}

for i in 1 2 3 
do
	for filename in gst_tripsMadrid_0005k gst_tripsMadrid_0010k gst_tripsMadrid_0015k gst_tripsMadrid_0020k gst_tripsMadrid_0025k
	do
		./time_allTopoRels_byQueriesFile ${DATAFOLDER1}${filename}.txt ${DATAFOLDER1}${filename}.naivepc ${DATAFOLDER1}${filename}.gst6 ${DATAFOLDER1}${filename}.queries.${i} ${REPETICIONES} >> ${OUTPUTFILE}
	done
	echo " ----------------------------------------------------- " >> ${OUTPUTFILE}
done

echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}
echo "**************** DATASET = ${DATAFOLDER2} ****************" >> ${OUTPUTFILE}
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}



for i in 1 2 3 
do
	for filename in 20stops_min_05k  20stops_min_10k  20stops_min_15k  20stops_min_20k  20stops_min_25k
	do
		./time_allTopoRels_byQueriesFile ${DATAFOLDER2}${filename}.txt ${DATAFOLDER2}${filename}.naivepc ${DATAFOLDER2}${filename}.gst6 ${DATAFOLDER2}${filename}.queries.${i} ${REPETICIONES} >> ${OUTPUTFILE}
	done
	echo " ----------------------------------------------------- " >> ${OUTPUTFILE}
done

echo "Fecha FIN experimentos: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0
