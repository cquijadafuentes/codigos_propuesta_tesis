#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: $(basename $0) <data_folder> <output_file>"
	exit 1
fi

DATAFOLDER=$1
OUTPUTFILE=$2
FILENAME=gst_tripsMadrid_10M

echo "Fecha INICIO construcción: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTPUTFILE}
echo "Directorio de inicio: " >> ${OUTPUTFILE}
pwd >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

./extras/dataset_madrid/procesa_madrid_trips ${DATAFOLDER}lineStops.txt ${DATAFOLDER}madrid_trips.txt 10000000
mv gst_tripsMadrid.txt ${DATAFOLDER}${FILENAME}.txt
./TopoRel_GST_build ${DATAFOLDER}${FILENAME}.txt ${DATAFOLDER}${FILENAME}.gst
./extras/generador_consultas/generador_consultas ${DATAFOLDER}${FILENAME}.txt ${DATAFOLDER}${FILENAME}.queries.1 50 1
./extras/generador_consultas/generador_consultas ${DATAFOLDER}${FILENAME}.txt ${DATAFOLDER}${FILENAME}.queries.intersect 50 2

./time_allTopoRels_byQueriesFile ${DATAFOLDER}${FILENAME}.txt ${DATAFOLDER}${FILENAME}.gst ${DATAFOLDER}${FILENAME}.queries.1 1 >> ${OUTPUTFILE}
./time_allIntersectPPk_byQueriesFile ${DATAFOLDER}${FILENAME}.txt ${DATAFOLDER}${FILENAME}.gst ${DATAFOLDER}${FILENAME}.queries.intersect 1 >> ${OUTPUTFILE}

echo "Fecha FIN construcción: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}


exit 0