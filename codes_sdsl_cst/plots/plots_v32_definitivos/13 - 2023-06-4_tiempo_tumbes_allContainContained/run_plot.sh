#!/bin/bash

echo "Iniciando gráficas"

for folder in allContain allEqual
do
	cd ${folder}
	pwd
	gnuplot plot_xl
	gnuplot plot_5k50k
	cd ..
done

echo "FIN"
exit 0