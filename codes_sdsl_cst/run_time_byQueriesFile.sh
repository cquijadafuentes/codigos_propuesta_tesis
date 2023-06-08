#!/bin/bash

if [ $# -ne 3 ]; then
	echo "$(basename $0) <data_folder_1> <repeticiones> <output_file>"
	echo "Generará resultados allContained <repeticiones> veces con los archivos de consulta correspondientes."
	exit 1
fi

DATAFOLDER1=$1
REPETICIONES=$2
OUTPUTFILE=$3


echo "Fecha INICIO experimentos: $(date +'%Y/%m/%d %H:%M:%S') allContainedByQueriesFile_naive_gst" > ${OUTPUTFILE}

echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}
echo "**************** DATASET = ${DATAFOLDER1} ****************" >> ${OUTPUTFILE}
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}

for i in 1 2 3 
do
	for filename in gst_tripsMadrid_0050k gst_tripsMadrid_0100k gst_tripsMadrid_0150k gst_tripsMadrid_0200k gst_tripsMadrid_0400k gst_tripsMadrid_0800k gst_tripsMadrid_1600k gst_tripsMadrid_3200k
	do
		./time_allTopoRels_byQueriesFile ${DATAFOLDER1}${filename}.txt ${DATAFOLDER1}${filename}.gst ${DATAFOLDER1}${filename}.queries.${i} ${REPETICIONES} >> ${OUTPUTFILE}
	done
	echo " ----------------------------------------------------- " >> ${OUTPUTFILE}
done

echo "Fecha FIN experimentos: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0