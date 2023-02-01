#!/bin/bash

if [ $# -ne 3 ]; then
	echo "$(basename $0) <output_file> <code_folder> <data_folder>"
	echo ""
	exit 1
fi

OUTPUTFILE=$1
CODEFOLDER=$2
DATAFOLDER=$3

echo "***************** Madrid *****************" >> ${OUTPUTFILE}

${CODEFOLDER}test_comparaGST4constructores < ${DATAFOLDER}madrid_x1 >> ${OUTPUTFILE}
echo "***************** Madrid_x2 *****************" >> ${OUTPUTFILE}
${CODEFOLDER}test_comparaGST4constructores < ${DATAFOLDER}madrid_x2 >> ${OUTPUTFILE}
echo "***************** Madrid_x4 *****************" >> ${OUTPUTFILE}
${CODEFOLDER}test_comparaGST4constructores < ${DATAFOLDER}madrid_x4 >> ${OUTPUTFILE}
echo "***************** Madrid_x8 *****************" >> ${OUTPUTFILE}
${CODEFOLDER}test_comparaGST4constructores < ${DATAFOLDER}madrid_x8 >> ${OUTPUTFILE}
