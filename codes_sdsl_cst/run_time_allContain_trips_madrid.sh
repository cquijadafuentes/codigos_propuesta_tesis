#!/bin/bash

echo "**************** 100001 trips ****************" > ~/GST/resultados/resultados_2023-01-18_allContainTrips.txt
~/GST/codigos_propuesta_tesis/codes_sdsl_cst/time_TopoRel_all_contain < ~/GST/datasets/gst_tripsMadrid_0100k.txt >> ~/GST/resultados/resultados_2023-01-18_allContainTrips.txt
echo "**************** 200008 trips ****************" >> ~/GST/resultados/resultados_2023-01-18_allContainTrips.txt
~/GST/codigos_propuesta_tesis/codes_sdsl_cst/time_TopoRel_all_contain < ~/GST/datasets/gst_tripsMadrid_0200k.txt >> ~/GST/resultados/resultados_2023-01-18_allContainTrips.txt
echo "**************** 400825 trips ****************" >> ~/GST/resultados/resultados_2023-01-18_allContainTrips.txt
~/GST/codigos_propuesta_tesis/codes_sdsl_cst/time_TopoRel_all_contain < ~/GST/datasets/gst_tripsMadrid_0400k.txt >> ~/GST/resultados/resultados_2023-01-18_allContainTrips.txt
echo "**************** 800651 trips ****************" >> ~/GST/resultados/resultados_2023-01-18_allContainTrips.txt
~/GST/codigos_propuesta_tesis/codes_sdsl_cst/time_TopoRel_all_contain < ~/GST/datasets/gst_tripsMadrid_0800k.txt >> ~/GST/resultados/resultados_2023-01-18_allContainTrips.txt
echo "**************** 1601316 trips ****************" >> ~/GST/resultados/resultados_2023-01-18_allContainTrips.txt
~/GST/codigos_propuesta_tesis/codes_sdsl_cst/time_TopoRel_all_contain < ~/GST/datasets/gst_tripsMadrid_1600k.txt >> ~/GST/resultados/resultados_2023-01-18_allContainTrips.txt
echo "**************** 3204780 trips ****************" >> ~/GST/resultados/resultados_2023-01-18_allContainTrips.txt
~/GST/codigos_propuesta_tesis/codes_sdsl_cst/time_TopoRel_all_contain < ~/GST/datasets/gst_tripsMadrid_3200k.txt >> ~/GST/resultados/resultados_2023-01-18_allContainTrips.txt
echo "**************** 6409931 trips ****************" >> ~/GST/resultados/resultados_2023-01-18_allContainTrips.txt
~/GST/codigos_propuesta_tesis/codes_sdsl_cst/time_TopoRel_all_contain < ~/GST/datasets/gst_tripsMadrid.txt >> ~/GST/resultados/resultados_2023-01-18_allContainTrips.txt

