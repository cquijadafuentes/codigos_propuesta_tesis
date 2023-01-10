#!/bin/bash

echo "***************** Madrid *****************\n" > ~/resultados_2023-01-09_allContainMadrid.txt
~/codigos_propuesta_tesis/codes_sdsl_cst/time_TopoRel_all_contain ~/datasets/lineas_madrid.txt >> ~/resultados_2023-01-09_allContainMadrid.txt
echo "***************** Madrid_x2 *****************\n" >> ~/resultados_2023-01-09_allContainMadrid.txt
~/codigos_propuesta_tesis/codes_sdsl_cst/time_TopoRel_all_contain ~/datasets/madrid_x2 >> ~/resultados_2023-01-09_allContainMadrid.txt
echo "***************** Madrid_x4 *****************\n" >> ~/resultados_2023-01-09_allContainMadrid.txt
~/codigos_propuesta_tesis/codes_sdsl_cst/time_TopoRel_all_contain ~/datasets/madrid_x4 >> ~/resultados_2023-01-09_allContainMadrid.txt
echo "***************** Madrid_x8 *****************\n" >> ~/resultados_2023-01-09_allContainMadrid.txt
~/codigos_propuesta_tesis/codes_sdsl_cst/time_TopoRel_all_contain ~/datasets/madrid_x8 >> ~/resultados_2023-01-09_allContainMadrid.txt
