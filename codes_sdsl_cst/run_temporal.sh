#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: $(basename $0) <folder>"
	echo "Generará archivos en formato gst"
	exit 1
fi

DATAFOLDER=$1

echo "Fecha INICIO construcción: $(date +'%Y/%m/%d %H:%M:%S')"
echo "Directorio de inicio: "
pwd
echo ""

for filename in gst_tripsMadrid_1600k gst_tripsMadrid_3200k
do
	./TopoRel_GST_build ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.gst
done

echo "Fecha FIN construcción: $(date +'%Y/%m/%d %H:%M:%S')"
exit 0

