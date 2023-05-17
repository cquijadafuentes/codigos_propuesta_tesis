#!/bin/bash

if [ $# -ne 1 ]; then
	echo "$(basename $0) <data_folder>"
	echo "Generará resultados de cada archivo en la lista."
	exit 1
fi

DATAFOLDER=$1

g++ -o histograma histograma.cpp

for filename in gst_tripsMadrid_0005k.txt  gst_tripsMadrid_0010k.txt  gst_tripsMadrid_0015k.txt  gst_tripsMadrid_0020k.txt  gst_tripsMadrid_0025k.txt
do
	./histograma < ${DATAFOLDER}${filename} > histo_${filename}
done

exit 0
