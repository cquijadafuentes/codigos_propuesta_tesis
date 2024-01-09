#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: $(basename $0) <folder>"
	echo "Generará archivos en formato gst"
	exit 1
fi

DATAFOLDER=$1

echo "Fecha INICIO construcción: $(date +'%Y/%m/%d %H:%M:%S')"
echo "Directorio de inicio: "
pwd
echo ""

for filename in gst_tripsMadrid_1350stops_1600ktrips gst_tripsMadrid_2700stops_1600ktrips gst_tripsMadrid_5500stops_1600ktrips gst_tripsMadrid_1100stops_1600ktrips
do
	./time_allTopoRels_byQueriesFile ${DATAFOLDER}${filename}.txt ${DATAFOLDER}${filename}.gst ${DATAFOLDER}${filename}.queries.1 1 > ~/resultados_2024-01-09_allTopoRels_stops_variables_1rep.txt
done

echo "Fecha FIN construcción: $(date +'%Y/%m/%d %H:%M:%S')"
exit 0

