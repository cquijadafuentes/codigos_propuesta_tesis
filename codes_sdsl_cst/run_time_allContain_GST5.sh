#!/bin/bash

if [ $# -ne 3 ]; then
	echo "$(basename $0) <input_file_GST5> <input_file_txt> <OUTPUTFILE>"
	echo "Generará resultados para consultas de 10k, 20k, ..., 100k"
	exit 1
fi

INPUTGST5=$1
INPUTTXT=$2
OUTPUTFILE=$3

echo "Date: $now" > ${OUTPUTFILE}
echo "Ejecución para 10000 consultas..."
./time_TopoRel_all_contain_GST5 ${INPUTGST5} 10000 < ${INPUTTXT} >> ${OUTPUTFILE}
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

