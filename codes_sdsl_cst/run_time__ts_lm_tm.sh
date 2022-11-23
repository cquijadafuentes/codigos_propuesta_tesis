#!/bin/bash


./time_TopoRel_boolean < TopoRel_input_clean.txt > ~/resultados_2022-11-23_tsantiago.txt

./time_TopoRel_boolean < ~/datasets/lineas_madrid.txt > ~/resultados_2022-11-23_lmadrid.txt

./time_TopoRel_boolean < ~/datasets/gst_tripsMadrid_0100k.txt > ~/resultados_2022-11-23_tripsmadrid.txt


