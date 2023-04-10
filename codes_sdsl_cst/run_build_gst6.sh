#!/bin/bash

if [ $# -ne 1 ]; then
	echo "$(basename $0) <data_folder>"
	echo "Generará arcvivos en formato gst6"
	exit 1
fi

DATAFOLDER=$1

for filename in 20stops_min_05k  20stops_min_10k  20stops_min_15k  20stops_min_20k  20stops_min_25k
do
	./TopoRel_GST_6_build ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.gst6
	./TopoRel_Naive_PreComp_build ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.naivepc

done