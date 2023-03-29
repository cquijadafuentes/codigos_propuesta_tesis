#!/bin/bash

if [ $# -ne 1 ]; then
	echo "$(basename $0) <data_folder>"
	echo "Guardará los archivos tripMadrid_0100k .. 3200k y gst_tripsMadrid_0005k 50k"
	exit 1
fi

DATAFOLDER=$1
for filename in tripMadrid_0100k tripMadrid_0200k tripMadrid_0400k tripMadrid_0800k tripMadrid_1600k tripMadrid_3200k
do
	./TopoRel_GST_5_save ${DATAFOLDER}${filename}.gst5 < ${DATAFOLDER}${filename}.txt
done

for filename in gst_tripsMadrid_0005k gst_tripsMadrid_0010k gst_tripsMadrid_0015k gst_tripsMadrid_0020k gst_tripsMadrid_0025k gst_tripsMadrid_0030k gst_tripsMadrid_0035k gst_tripsMadrid_0040k gst_tripsMadrid_0045k gst_tripsMadrid_0050k
do
	./TopoRel_GST_5_save ${DATAFOLDER}${filename}.gst5 < ${DATAFOLDER}${filename}.txt
done