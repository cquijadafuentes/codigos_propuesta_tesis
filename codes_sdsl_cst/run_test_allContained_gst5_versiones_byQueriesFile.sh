#!/bin/bash

if [ $# -ne 3 ]; then
	echo "$(basename $0) <data_folder_1> <data_folder_2> <output_file>"
	echo "Entrega la cantidad de errores que hay en las 3 versiones de la operación allContained."
	echo "En cada folder deben existir los archivos <file>.gst5, <file>.queries.<i> y el archivo <file>.results.<i>"
	exit 1
fi

DATAFOLDER1=$1
DATAFOLDER2=$2
OUTPUTFILE=$3


echo "Fecha INICIO Comparación de resultados: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTPUTFILE}

echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}
echo "**************** DATASET = ${DATAFOLDER1} ****************" >> ${OUTPUTFILE}
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}

for filename in gst_tripsMadrid_0005k gst_tripsMadrid_0010k gst_tripsMadrid_0015k gst_tripsMadrid_0020k gst_tripsMadrid_0025k
do
	for i in 1 2 3 
	do
		./TopoRel_Naive_test_allContained_byQueries ${DATAFOLDER1}${filename}.txt ${DATAFOLDER1}${filename}.queries.${i} > ${DATAFOLDER1}${filename}.results.${i}
		./TopoRel_GST_5_test_allContained_byQueries ${DATAFOLDER1}${filename}.gst5 ${DATAFOLDER1}${filename}.queries.${i} ${DATAFOLDER1}${filename}.results.${i} >> ${OUTPUTFILE}
	done
	echo " ----------------------------------------------------- " >> ${OUTPUTFILE}
done



echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}
echo "**************** DATASET = ${DATAFOLDER2} ****************" >> ${OUTPUTFILE}
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> ${OUTPUTFILE}

for filename in 20stops_min_05k  20stops_min_10k  20stops_min_15k  20stops_min_20k  20stops_min_25k
do
	for i in 1 2 3 
	do
		./TopoRel_Naive_test_allContained_byQueries ${DATAFOLDER2}${filename}.txt ${DATAFOLDER2}${filename}.queries.${i} > ${DATAFOLDER2}${filename}.results.${i}
		./TopoRel_GST_5_test_allContained_byQueries ${DATAFOLDER2}${filename}.gst5 ${DATAFOLDER2}${filename}.queries.${i}  ${DATAFOLDER2}${filename}.results.${i} >> ${OUTPUTFILE}
	done
	echo " ----------------------------------------------------- " >> ${OUTPUTFILE}
done



echo "Fecha FIN Comparación de resultados: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0
