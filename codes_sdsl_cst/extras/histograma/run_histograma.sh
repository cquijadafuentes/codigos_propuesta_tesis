#!/bin/bash

if [ $# -ne 1 ]; then
	echo "$(basename $0) <data_folder>"
	echo "Generará resultados de cada archivo en la lista."
	exit 1
fi

DATAFOLDER=$1

g++ -o histograma histograma.cpp

for filename in 20stops_min_05k.txt  20stops_min_10k.txt  20stops_min_15k.txt  20stops_min_20k.txt  20stops_min_25k.txt
do
	./histograma ${DATAFOLDER}${filename} > histo_${filename}
done

exit 0