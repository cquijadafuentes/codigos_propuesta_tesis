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

for filename in gst_800ktrips_02kstops.txt gst_800ktrips_04kstops.txt gst_800ktrips_06kstops.txt gst_800ktrips_08kstops.txt gst_800ktrips_10kstops.txt
do
	./TopoRel_GST_build ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.gst
	./TopoRel_GST_size ${DATAFOLDER}${filename}.gst >> ${OUTPUTFILE}
	./TopoRel_Naive_size ${DATAFOLDER}${filename}.txt >> ${OUTPUTFILE}
	./time_allTopoRels_byQueriesFile ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.gst ${DATAFOLDER}${filename}.queries.1 1 >> ${OUTPUTFILE}
done

echo "Fecha FIN construcción: $(date +'%Y/%m/%d %H:%M:%S')"
exit 0

