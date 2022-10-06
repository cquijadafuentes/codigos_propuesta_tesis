#!/bin/bash

make
valgrind --leak-check=yes ./TopoRel_Naive_test < TopoRel_test_input.txt
valgrind --leak-check=yes ./TopoRel_GST_test < TopoRel_test_input.txt