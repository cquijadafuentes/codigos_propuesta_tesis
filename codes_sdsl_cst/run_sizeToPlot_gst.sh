#!/bin/bash

if [ $# -ne 3 ]; then
	echo "$(basename $0) <folder_trips> <folder_trips20+> <output_file>"
	echo "Generará archivo con nombre <output_file> que contiene los tamaños de las estructuras que forman gst6."
	exit 1
fi

DATAFOLDER=$1
DATAFOLDER2=$2
OUTPUTFILE=$3


echo "Fecha INICIO: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTPUTFILE}

for filename in gst_tripsMadrid_0005k gst_tripsMadrid_0010k gst_tripsMadrid_0015k gst_tripsMadrid_0020k gst_tripsMadrid_0025k gst_tripsMadrid_0030k gst_tripsMadrid_0035k gst_tripsMadrid_0040k gst_tripsMadrid_0045k gst_tripsMadrid_0050k
do
	./TopoRel_GST_size ${DATAFOLDER}${filename}.gst >> ${OUTPUTFILE}
done


for filename in 20stops_min_05k  20stops_min_10k  20stops_min_15k  20stops_min_20k  20stops_min_25k
do
	./TopoRel_GST_size ${DATAFOLDER2}${filename}.gst >> ${OUTPUTFILE}
done

echo "Fecha FIN: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0
