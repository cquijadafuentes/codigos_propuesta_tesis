#!/bin/bash

make clean TopoRel_Naive.o TopoRel_GST.o test_alfabeto_ext
./test_alfabeto_ext info_stops.txt
