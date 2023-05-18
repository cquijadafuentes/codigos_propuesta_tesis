#!/bin/bash

if [ $# -ne 1 ]; then
	echo "$(basename $0) <data_folder>"
	echo "Generará resultados de cada archivo en la lista."
	exit 1
fi

DATAFOLDER=$1

g++ -o histograma histograma.cpp

for filename in gst_tripsMadrid_0050k.txt gst_tripsMadrid_0100k.txt gst_tripsMadrid_0150k.txt gst_tripsMadrid_0200k.txt gst_tripsMadrid_0400k.txt gst_tripsMadrid_0800k.txt gst_tripsMadrid_1600k.txt gst_tripsMadrid_3200k.txt
do
	./histograma < ${DATAFOLDER}${filename} > histo_${filename}
done

exit 0
