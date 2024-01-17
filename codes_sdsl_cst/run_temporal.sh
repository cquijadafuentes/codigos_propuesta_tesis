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

for filename in gst_800ktrips_02kstops gst_800ktrips_04kstops gst_800ktrips_06kstops gst_800ktrips_08kstops gst_800ktrips_10kstops
do
	echo "Procesando ${filename}..." >> ${OUTPUTFILE}
	./TopoRel_GST_build ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.gst
	./TopoRel_GST_size ${DATAFOLDER}${filename}.gst >> ${OUTPUTFILE}
	./TopoRel_Naive_size ${DATAFOLDER}${filename}.txt >> ${OUTPUTFILE}
	./time_allTopoRels_byQueriesFile ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.gst ${DATAFOLDER}${filename}.queries.1 10 >> ${OUTPUTFILE}
done

echo "Fecha FIN construcción: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0

