#!/bin/bash

if [ $# -ne 1 ]; then
	echo "$(basename $0) <data_folder>
	echo ""
	exit 1
fi

CODEFOLDER=$1

echo "Procesando el archivo de 0100k"
./TopoRel_GST_5_save ${DATAFOLDER}tripMadrid_0100k.gst5 < ${DATAFOLDER}gst_tripsMadrid_0100k.txt
echo "Procesando el archivo de 0200k"
./TopoRel_GST_5_save ${DATAFOLDER}tripMadrid_0200k.gst5 < ${DATAFOLDER}gst_tripsMadrid_0200k.txt
echo "Procesando el archivo de 0400k"
./TopoRel_GST_5_save ${DATAFOLDER}tripMadrid_0400k.gst5 < ${DATAFOLDER}gst_tripsMadrid_0400k.txt
echo "Procesando el archivo de 0800k"
./TopoRel_GST_5_save ${DATAFOLDER}tripMadrid_0800k.gst5 < ${DATAFOLDER}gst_tripsMadrid_0800k.txt
echo "Procesando el archivo de 1600k"
./TopoRel_GST_5_save ${DATAFOLDER}tripMadrid_1600k.gst5 < ${DATAFOLDER}gst_tripsMadrid_1600k.txt
echo "Procesando el archivo de 3200k"
./TopoRel_GST_5_save ${DATAFOLDER}tripMadrid_3200k.gst5 < ${DATAFOLDER}gst_tripsMadrid_3200k.txt
echo "FIN"