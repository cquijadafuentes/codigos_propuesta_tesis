#!/bin/bash

rm test_alfabeto_ext TopoRel_Naive.o
make TopoRel_Naive.o test_alfabeto_ext
./test_alfabeto_ext info_stops.txt
