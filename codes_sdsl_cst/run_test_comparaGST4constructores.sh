#!/bin/bash

echo "***************** Madrid *****************" > ~/GST/resultados_2023-01-18_constructores.txt
~/GST/codigos_propuesta_tesis/codes_sdsl_cst/test_comparaGST4constructores < ~/GST/datasets/madrid.txt >> ~/GST/resultados_2023-01-18_constructores.txt
echo "***************** Madrid_x2 *****************" >> ~/GST/resultados_2023-01-18_constructores.txt
~/GST/codigos_propuesta_tesis/codes_sdsl_cst/test_comparaGST4constructores < ~/GST/datasets/madrid_x2 >> ~/GST/resultados_2023-01-18_constructores.txt
echo "***************** Madrid_x4 *****************" >> ~/GST/resultados_2023-01-18_constructores.txt
~/GST/codigos_propuesta_tesis/codes_sdsl_cst/test_comparaGST4constructores < ~/GST/datasets/madrid_x4 >> ~/GST/resultados_2023-01-18_constructores.txt
echo "***************** Madrid_x8 *****************" >> ~/GST/resultados_2023-01-18_constructores.txt
~/GST/codigos_propuesta_tesis/codes_sdsl_cst/test_comparaGST4constructores < ~/GST/datasets/madrid_x8 >> ~/GST/resultados_2023-01-18_constructores.txt
