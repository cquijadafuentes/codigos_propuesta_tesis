#!/bin/bash

if [ $# -ne 2 ]; then
	echo "$(basename $0) <data_folder> <output_file>"
	echo "Generará archivo con nombre <output_file> que contiene los tamaños de las estructuras que forman gst6."
	exit 1
fi

DATAFOLDER=$1
OUTPUTFILE=$2


echo "Fecha INICIO experimentos: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTPUTFILE}

for filename in gst_tripsMadrid_0005k gst_tripsMadrid_0010k gst_tripsMadrid_0015k gst_tripsMadrid_0020k gst_tripsMadrid_0025k gst_tripsMadrid_0030k gst_tripsMadrid_0035k gst_tripsMadrid_0040k gst_tripsMadrid_0045k gst_tripsMadrid_0050k
do
	./TopoRel_GST_6_size ${DATAFOLDER}${filename}.gst6 >> ${OUTPUTFILE}
done
echo "Fecha FIN experimentos: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0