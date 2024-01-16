#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: $(basename $0) <folder> <output_file>"
	exit 1
fi

DATAFOLDER=$1
OUTPUTFILE=$2
RUN=suffixPathsTernarySearchTree

rm ${RUN}
g++ -o ${RUN} ${RUN}.cpp

echo "Fecha INICIO: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTPUTFILE}
echo "Directorio de inicio: " >> ${OUTPUTFILE}
pwd >> ${OUTPUTFILE}
echo "Directorio de datos: ${DATAFOLDER}" >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

for filename in gst_tripsMadrid_0005k.txt gst_tripsMadrid_0010k.txt gst_tripsMadrid_0015k.txt gst_tripsMadrid_0020k.txt gst_tripsMadrid_0025k.txt gst_tripsMadrid_0030k.txt gst_tripsMadrid_0035k.txt gst_tripsMadrid_0040k.txt gst_tripsMadrid_0045k.txt gst_tripsMadrid_0050k.txt gst_tripsMadrid_0100k.txt gst_tripsMadrid_0150k.txt gst_tripsMadrid_0200k.txt gst_tripsMadrid_0400k.txt gst_tripsMadrid_0800k.txt gst_tripsMadrid_1600k.txt gst_tripsMadrid_3200k.txt gst_tripsMadrid_6400k.txt
do
	echo "Procesando ${filename}..." >> ${OUTPUTFILE}
	echo ./${RUN} ${DATAFOLDER}${filename} >> ${OUTPUTFILE}
done

echo "Fecha FIN: $(date +'%Y/%m/%d %H:%M:%S')"
exit 0

