#!/bin/bash

echo "Iniciando gráficas"

for folder in allContain allEqual allContained allIntersect
do
	cd ${folder}
	pwd
	gnuplot plot
	cd ..
done

echo "FIN"
exit 0