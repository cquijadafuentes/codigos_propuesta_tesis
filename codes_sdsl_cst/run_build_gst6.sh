#!/bin/bash

if [ $# -ne 5 ]; then
	echo "$(basename $0) <data_folder>"
	echo "Generará arcvivos en formato gst6"
	exit 1
fi

DATAFOLDER=$1

for filename in gst_tripsMadrid_0005k gst_tripsMadrid_0010k gst_tripsMadrid_0015k gst_tripsMadrid_0020k gst_tripsMadrid_0025k gst_tripsMadrid_0030k gst_tripsMadrid_0035k gst_tripsMadrid_0040k gst_tripsMadrid_0045k gst_tripsMadrid_0050k
do
	./TopoRel_GST_6_build ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.gst6
done