#!/usr/bin/env bash

for i in {0..7}
do
  arg=$(echo "data/${i}/")
  ./suma_cpu $(echo "data/${i}/")
  ./suma_gpu $(echo "data/${i}/")
done

gnuplot -e 'load "plots.p"'
