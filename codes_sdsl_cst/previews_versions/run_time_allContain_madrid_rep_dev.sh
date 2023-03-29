#!/bin/bash

echo "***************** Madrid_50_rep *****************" > ~/resultados_2023-01-11_allContainMadrid_rep_der.txt
~/codigos_propuesta_tesis/codes_sdsl_cst/time_TopoRel_all_contain 50 < /home/carlos/GST/datasets/madrid_first50lines_repeateduntil2k.txt >> ~/resultados_2023-01-11_allContainMadrid_rep_der.txt
echo "***************** Madrid_100_rep *****************" >> ~/resultados_2023-01-11_allContainMadrid_rep_der.txt
~/codigos_propuesta_tesis/codes_sdsl_cst/time_TopoRel_all_contain 100 < /home/carlos/GST/datasets/madrid_first100lines_repeateduntil2k.txt >> ~/resultados_2023-01-11_allContainMadrid_rep_der.txt
echo "***************** Madrid_50_der *****************" >> ~/resultados_2023-01-11_allContainMadrid_rep_der.txt
~/codigos_propuesta_tesis/codes_sdsl_cst/time_TopoRel_all_contain 50 < /home/carlos/GST/datasets/madrid_first50lines_deriveduntil2k.txt >> ~/resultados_2023-01-11_allContainMadrid_rep_der.txt
echo "***************** Madrid_100_der *****************" >> ~/resultados_2023-01-11_allContainMadrid_rep_der.txt
~/codigos_propuesta_tesis/codes_sdsl_cst/time_TopoRel_all_contain 100 < /home/carlos/GST/datasets/madrid_first100lines_deriveduntil2k.txt >> ~/resultados_2023-01-11_allContainMadrid_rep_der.txt
