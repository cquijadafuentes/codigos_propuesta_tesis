#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: $(basename $0) <folder> <output_file>"
	exit 1
fi

DATAFOLDER=$1
OUTPUTFILE=$2

echo "Fecha INICIO construcción: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTPUTFILE}
echo "Directorio de inicio: " >> ${OUTPUTFILE}
pwd >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

for filename in gst_tripsMadrid_0010k.txt gst_tripsMadrid_0020k.txt gst_tripsMadrid_0030k.txt gst_tripsMadrid_0040k.txt gst_tripsMadrid_0050k.txt

do
	echo "Procesando ${filename}..." >> ${OUTPUTFILE}
	./time_allTopoRels_byQueriesFile ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.gst ${DATAFOLDER}${filename}.queries.1 10 >> ${OUTPUTFILE}
done

echo "Fecha FIN construcción: $(date +'%Y/%m/%d %H:%M:%S')"
exit 0

