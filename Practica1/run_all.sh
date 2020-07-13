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

# Calcular tiempo secuencial
printf "Calculando tiempo secuencial (cpu)\n"
for i in {12000000..108000000..12000000}
do
  printf "%lu -->" $i
  ./bin/pi_secuencial 1 $i
  printf " OK\n"
done

printf "Calculando tiempo secuencial (wall)\n"
for i in {12000000..108000000..12000000}
do
  printf "%lu -->" $i
  ./bin/pi_secuencial 0 $i
  printf " OK\n"
done

printf "Tiempo secuencial generado\n"

for j in {2..8}
do
  printf "Calculando tiempo concurrente (cpu)(p=%d)\n" $j
  for i in {12000000..108000000..12000000}
  do
    printf "%lu -->" $i
    mpirun --use-hwthread-cpus -np $j ./bin/pi_concurrente 1 $i
    printf " OK\n"
  done

  printf "Calculando tiempo concurrente (wall)(p=%d)\n" $j
  for i in {12000000..108000000..12000000}
  do
    printf "%lu -->" $i
    mpirun --use-hwthread-cpus -np $j ./bin/pi_concurrente 0 $i
    printf " OK\n"
  done
done

printf "Tiempo concurrente generado\n"

printf "Generando graficas:\n"

printf "Estudio error secuencial -->"
gnuplot -e 'load "error_secuencial_estudio.p"'
printf " OK\n"

printf "Estudio error secuencial -->"
gnuplot -e 'load "secuencial_concurrente_estudio.p"'
printf " OK\n"






