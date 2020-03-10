set terminal png size 500,500
set output 'data/error_estudio_plot.png'

set xlabel "n intervalos"
set ylabel "error"

set style line 1 \
    linecolor rgb '#0060ad' \
    linetype 1 linewidth 2


set style line 2 \
    linecolor rgb '#dd181f' \
    linetype 1 linewidth 2

set style line 3 \
    linecolor rgb '#32CD32' \
    linetype 1 linewidth 2

plot 'data/secuencial_estudio_0.dat' using 1:2 title 'Izq' w lines linestyle 1, \
     'data/secuencial_estudio_1.dat' using 1:2 title 'Med' w lines linestyle 2, \
     'data/secuencial_estudio_2.dat' using 1:2 title 'Der' w lines linestyle 3
