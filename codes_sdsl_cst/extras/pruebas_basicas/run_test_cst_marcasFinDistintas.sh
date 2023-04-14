#!/bin/bash

if [ $# -ne 1 ]; then
	echo "$(basename $0) <data_folder>"
	echo "Generará arcvivos en formato gst6"
	exit 1
fi

DATAFOLDER=$1

for filename in gst_tripsMadrid_0005k gst_tripsMadrid_0010k gst_tripsMadrid_0015k gst_tripsMadrid_0020k gst_tripsMadrid_0025k

do
	./test_cst_marcasFinDistintas ${DATAFOLDER}${filename}.txt

done
