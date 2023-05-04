#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: $(basename $0) <folder_trips> <folder_trips20+>"
	echo "Generará archivos en formato gst para trips y trips20+"
	exit 1
fi

DATAFOLDER=$1
DATAFOLDER2=$2

echo "Fecha INICIO construcción: $(date +'%Y/%m/%d %H:%M:%S')"
echo "Directorio de inicio: "
pwd
echo ""

for filename in gst_tripsMadrid_0005k  gst_tripsMadrid_0015k  gst_tripsMadrid_0025k  gst_tripsMadrid_0035k  gst_tripsMadrid_0045k gst_tripsMadrid_0010k  gst_tripsMadrid_0020k  gst_tripsMadrid_0030k  gst_tripsMadrid_0040k  gst_tripsMadrid_0050k
do
	./TopoRel_GST_build ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.gst
done


for filename in 20stops_min_05k  20stops_min_10k  20stops_min_15k  20stops_min_20k  20stops_min_25k
do
	./TopoRel_GST_build ${DATAFOLDER2}${filename}.txt ${DATAFOLDER2}${filename}.gst
done

echo "Fecha FIN construcción: $(date +'%Y/%m/%d %H:%M:%S')"
exit 0
