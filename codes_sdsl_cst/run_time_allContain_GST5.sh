#!/bin/bash

if [ $# -ne 2 ]; then
	echo "$(basename $0) <input_file_GST5> <input_file_txt>"
	echo "Generará resultados para consultas de 10k, 20k, 30k, 40k, 50k para cada archivo de trips"
	exit 1
fi

DATAFOLDER=$1
OUTPUTFILE=$2


echo "Fecha inicio experimentos: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTPUTFILE}

for filename in gst_tripsMadrid_0100k gst_tripsMadrid_0200k gst_tripsMadrid_0400k gst_tripsMadrid_0800k gst_tripsMadrid_1600k gst_tripsMadrid_3200k
do
	echo "./TopoRel_GST_5_save ${DATAFOLDER}${filename}.gst5 < ${DATAFOLDER}${filename}.txt >> ${OUTPUTFILE}"
	echo "./time_TopoRel_allContain_GST5_xLote ${DATAFOLDER}${filename}.gst5 10000 5 < ${DATAFOLDER}${filename}.txt >> ${OUTPUTFILE}"
done
echo "Fecha fin experimentos: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0