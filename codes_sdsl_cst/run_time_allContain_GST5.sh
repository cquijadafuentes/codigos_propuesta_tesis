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


echo "Ejecución para 10000 consultas..."
././time_TopoRel_allContain_GST5_xLote ${INPUTGST5} 10000 < ${INPUTTXT} >> ${OUTPUTFILE}
echo "Ejecución para 20000 consultas..."
./time_TopoRel_all_contain_GST5 ${INPUTGST5} 20000 < ${INPUTTXT} >> ${OUTPUTFILE}
echo "Ejecución para 30000 consultas..."
./time_TopoRel_all_contain_GST5 ${INPUTGST5} 30000 < ${INPUTTXT} >> ${OUTPUTFILE}
echo "Ejecución para 40000 consultas..."
./time_TopoRel_all_contain_GST5 ${INPUTGST5} 40000 < ${INPUTTXT} >> ${OUTPUTFILE}
echo "Ejecución para 50000 consultas..."
./time_TopoRel_all_contain_GST5 ${INPUTGST5} 50000 < ${INPUTTXT} >> ${OUTPUTFILE}
echo "Ejecución para 60000 consultas..."
./time_TopoRel_all_contain_GST5 ${INPUTGST5} 60000 < ${INPUTTXT} >> ${OUTPUTFILE}
echo "Ejecución para 70000 consultas..."
./time_TopoRel_all_contain_GST5 ${INPUTGST5} 70000 < ${INPUTTXT} >> ${OUTPUTFILE}
echo "Ejecución para 80000 consultas..."
./time_TopoRel_all_contain_GST5 ${INPUTGST5} 80000 < ${INPUTTXT} >> ${OUTPUTFILE}
echo "Ejecución para 90000 consultas..."
./time_TopoRel_all_contain_GST5 ${INPUTGST5} 90000 < ${INPUTTXT} >> ${OUTPUTFILE}
echo "Ejecución para 100000 consultas..."
./time_TopoRel_all_contain_GST5 ${INPUTGST5} 100000 < ${INPUTTXT} >> ${OUTPUTFILE}

gst_tripsMadrid_0100k.txt
gst_tripsMadrid_0200k.txt
gst_tripsMadrid_0400k.txt
gst_tripsMadrid_0800k.txt
gst_tripsMadrid_1600k.txt
gst_tripsMadrid_3200k.txt
