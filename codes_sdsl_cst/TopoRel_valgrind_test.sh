#!/bin/bash

make
valgrind --leak-check=yes ./TopoRel_Naive_test < TopoRel_Naive_test_input.txt
valgrind --leak-check=yes ./TopoRel_GST_test < TopoRel_GST_test_input.txt