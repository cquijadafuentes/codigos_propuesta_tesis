#!/bin/bash


echo "Stops Madrid" >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
echo "madrid_x1" >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
./size_TopoRel < /home/carlos/GST/datasets/madrid_x1 >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
echo "madrid_x2" >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
./size_TopoRel < /home/carlos/GST/datasets/madrid_x2 >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
echo "madrid_x4" >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
./size_TopoRel < /home/carlos/GST/datasets/madrid_x4 >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
echo "madrid_x8" >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
./size_TopoRel < /home/carlos/GST/datasets/madrid_x8 >> /home/carlos/GST/resultados/sizes_2022-11-24.txt


echo "Stops Transantiago" >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
echo "stops_x1" >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
./size_TopoRel < /home/carlos/GST/datasets/stops_x1 >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
echo "stops_x2" >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
./size_TopoRel < /home/carlos/GST/datasets/stops_x2 >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
echo "stops_x4" >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
./size_TopoRel < /home/carlos/GST/datasets/stops_x4 >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
echo "stops_x8" >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
./size_TopoRel < /home/carlos/GST/datasets/stops_x8 >> /home/carlos/GST/resultados/sizes_2022-11-24.txt


echo "Trips Madrid" >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
echo "trips_100k" >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
./size_TopoRel < /home/carlos/GST/datasets/gst_tripsMadrid_0100k.txt >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
echo "trips_200k" >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
./size_TopoRel < /home/carlos/GST/datasets/gst_tripsMadrid_0200k.txt >> /home/carlos/GST/resultados/sizes_2022-11-24.txt
