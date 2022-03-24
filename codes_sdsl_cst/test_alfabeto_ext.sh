#!/bin/bash

rm test_alfabeto_ext
make test_alfabeto_ext
./test_alfabeto_ext info_stops.txt
