#!/bin/bash

if [ $# -ne 3 ]; then
	echo "$(basename $0) <data_folder> <repeticiones> <output_file>"
	echo "Ejecuta time_allTopoRels_byQueriesFile para los archivos de trips de 50k-100k-150k-200k."
	exit 1
fi

DATAFOLDER=$1
REPETICIONES=$2
OUTPUTFILE=$3


echo "Fecha INICIO experimentos: $(date +'%Y/%m/%d %H:%M:%S') allContainedByQueriesFile_GST" > ${OUTPUTFILE}
echo "Experimentos de todas las relaciones tipo all en dos datasets." >> ${OUTPUTFILE}
echo "Directorio de inicio: " >> ${OUTPUTFILE}
pwd >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

for i in 1 2 3 
do
	for filename in gst_tripsMadrid_0400k gst_tripsMadrid_0800k gst_tripsMadrid_1600k gst_tripsMadrid_3200k
	do
		./time_allTopoRels_byQueriesFile ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.gst ${DATAFOLDER}${filename}.queries.${i} ${REPETICIONES} >> ${OUTPUTFILE}
	done
	echo " ----------------------------------------------------- " >> ${OUTPUTFILE}
done

echo "Fecha FIN experimentos: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0
