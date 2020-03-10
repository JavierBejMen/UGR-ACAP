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
printf "Calculando tiempo secuencial\n"
printf "1000000 --> "
./bin/pi_secuencial 1000000
printf "done\n"
printf "10000000 --> "
./bin/pi_secuencial 10000000
printf "done\n"
printf "100000000 --> "
./bin/pi_secuencial 100000000
printf "done\n"
printf "1000000000 --> "
./bin/pi_secuencial 1000000000
printf "done\n"
printf "10000000000 --> "
./bin/pi_secuencial 10000000000
printf "done\n"
printf "100000000000 --> "
./bin/pi_secuencial 100000000000
printf "done\n"

printf "Tiempo secuencial generado\n"
