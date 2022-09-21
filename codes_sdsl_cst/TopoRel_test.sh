#!/bin/bash

echo "*********** GST ***********\n"
./TopoRel_GST_test < ./TopoRel_test_input.txt
echo "********** Naive **********\n"
./TopoRel_Naive_test < ./TopoRel_test_input.txt