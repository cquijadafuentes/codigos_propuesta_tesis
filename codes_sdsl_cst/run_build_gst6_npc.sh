#!/bin/bash

if [ $# -ne 2 ]; then
	echo "$(basename $0) <data_folder_trips> <data_folder_20+>"
	echo "Generará arcvivos en formato gst6"
	exit 1
fi

DATAFOLDER=$1
DATAFOLDER2=$2

for filename in gst_tripsMadrid_0005k gst_tripsMadrid_0010k gst_tripsMadrid_0015k gst_tripsMadrid_0020k gst_tripsMadrid_0025k
do
	./TopoRel_GST_6_build ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.gst6
done

for filename in 20stops_min_05k 20stops_min_10k 20stops_min_15k 20stops_min_20k 20stops_min_25k 
do
	./TopoRel_GST_6_build ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.gst6
done
