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
echo "Directorio de inicio: " >> ${OUTPUTFILE}
pwd >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

for filename in gst_tripsMadrid_0050k gst_tripsMadrid_3200k
do
	./time_allTopoRels_byQueriesFile ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.gst ${DATAFOLDER}${filename}.queries.2 ${REPETICIONES} >> ${OUTPUTFILE}
done

echo "Fecha FIN experimentos: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0
