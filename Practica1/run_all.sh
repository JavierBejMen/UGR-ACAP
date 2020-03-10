#!/usr/bin/env bash

# Calcular mejor punto
printf "Calculando mejor punto\n"
for i in {1..20}
do
  ./bin/pi_secuencial_estudio 0 $i
done

for i in {1..20}
do
  ./bin/pi_secuencial_estudio 1 $i
done

for i in {1..20}
do
  ./bin/pi_secuencial_estudio 2 $i
done

printf "Datos generados\n"

gnuplot -e 'load "error_secuencial_estudio.p"'

printf "Plot generado\n"

# Calcular tiempo secuencial
printf "Calculando tiempo secuencial (cpu)\n"
for i in {100000000..1000000000..100000000}
do
  printf "%lu -->" $i
  ./bin/pi_secuencial 1 $i
  printf "done\n"
done

printf "Calculando tiempo secuencial (wall)\n"
for i in {100000000..1000000000..100000000}
do
  printf "%lu -->" $i
  ./bin/pi_secuencial 0 $i
  printf "done\n"
done

printf "Tiempo secuencial generado\n"
