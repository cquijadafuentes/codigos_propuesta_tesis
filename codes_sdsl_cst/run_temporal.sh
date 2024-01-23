#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: $(basename $0) <data_folder> <data_basefilename> <output_file>"
	exit 1
fi

DATAFOLDER=$1
FILENAME=$2
OUTPUTFILE=$3

echo "Fecha INICIO construcción: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTPUTFILE}
echo "Directorio de inicio: " >> ${OUTPUTFILE}
pwd >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

./time_allTopoRels_byQueriesFile ${DATAFOLDER}${FILENAME}.txt ${DATAFOLDER}${FILENAME}.gst ${DATAFOLDER}${FILENAME}.queries.1 1 >> ${OUTPUTFILE}
./time_allIntersectPPk_byQueriesFile ${DATAFOLDER}${FILENAME}.txt ${DATAFOLDER}${FILENAME}.gst ${DATAFOLDER}${FILENAME}.queries.intersect 1 >> ${OUTPUTFILE}

echo "Fecha FIN construcción: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}


exit 0