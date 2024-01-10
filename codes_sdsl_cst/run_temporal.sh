#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: $(basename $0) <folder> <output_file>"
	exit 1
fi

DATAFOLDER=$1
OUTPUTFILE=$2

echo "Fecha INICIO construcción: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTPUTFILE}
echo "Directorio de inicio: " >> ${OUTPUTFILE}
pwd >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

for filename in gst_tripsMadrid_1350stops_1600ktrips gst_tripsMadrid_2700stops_1600ktrips gst_tripsMadrid_5500stops_1600ktrips gst_tripsMadrid_11000stops_1600ktrips
do
	./TopoRel_GST_size ${DATAFOLDER}${filename}.gst >> ${OUTPUTFILE}
	./TopoRel_Naive_size ${DATAFOLDER}${filename}.txt >> ${OUTPUTFILE}
	./time_allTopoRels_byQueriesFile ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.gst ${DATAFOLDER}${filename}.queries.1 1 >> ${OUTPUTFILE}
done

echo "Fecha FIN construcción: $(date +'%Y/%m/%d %H:%M:%S')"
exit 0

