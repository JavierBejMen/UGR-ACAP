set terminal png size 500,500

set xlabel "n intervalos"
set ylabel "cpu time (s)"

set style line 1 \
    linecolor rgb '#0060ad' \
    linetype 1 linewidth 2


set style line 2 \
    linecolor rgb '#dd181f' \
    linetype 1 linewidth 2

set style line 3 \
    linecolor rgb '#32CD32' \
    linetype 1 linewidth 2

set style line 4 \
    linecolor rgb '#ad6000' \
    linetype 1 linewidth 2

set style line 5 \
    linecolor rgb '#00ffff' \
    linetype 1 linewidth 2

set output 'data/secuencial_estudio_cpu.png'
plot 'data/secuencial_cpu.dat' using 1:2 title 'Secuencial' w lines linestyle 1

set output 'data/secuencial_estudio_wall.png'
plot 'data/secuencial_wall.dat' using 1:2 title 'Secuencial' w lines linestyle 1

set output 'data/concurrente_estudio_cpu_2.png'
plot 'data/concurrente_cpu_2.dat' using 1:2 title 'Creación' w lines linestyle 1, \
     'data/concurrente_cpu_2.dat' using 1:3 title 'Computo' w lines linestyle 2, \
     'data/concurrente_cpu_2.dat' using 1:4 title 'Reduce' w lines linestyle 3

set output 'data/concurrente_estudio_wall_2.png'
plot 'data/concurrente_wall_2.dat' using 1:2 title 'Creación' w lines linestyle 1, \
     'data/concurrente_wall_2.dat' using 1:3 title 'Computo' w lines linestyle 2, \
     'data/concurrente_wall_2.dat' using 1:4 title 'Reduce' w lines linestyle 3

set output 'data/concurrente_estudio_cpu_4.png'
plot 'data/concurrente_cpu_4.dat' using 1:2 title 'Creación' w lines linestyle 1, \
     'data/concurrente_cpu_4.dat' using 1:3 title 'Computo' w lines linestyle 2, \
     'data/concurrente_cpu_4.dat' using 1:4 title 'Reduce' w lines linestyle 3

set output 'data/concurrente_estudio_wall_4.png'
plot 'data/concurrente_wall_4.dat' using 1:2 title 'Creación' w lines linestyle 1, \
     'data/concurrente_wall_4.dat' using 1:3 title 'Computo' w lines linestyle 2, \
     'data/concurrente_wall_4.dat' using 1:4 title 'Reduce' w lines linestyle 3

set output 'data/concurrente_estudio_cpu_6.png'
plot 'data/concurrente_cpu_6.dat' using 1:2 title 'Creación' w lines linestyle 1, \
     'data/concurrente_cpu_6.dat' using 1:3 title 'Computo' w lines linestyle 2, \
     'data/concurrente_cpu_6.dat' using 1:4 title 'Reduce' w lines linestyle 3

set output 'data/concurrente_estudio_wall_6.png'
plot 'data/concurrente_wall_6.dat' using 1:2 title 'Creación' w lines linestyle 1, \
     'data/concurrente_wall_6.dat' using 1:3 title 'Computo' w lines linestyle 2, \
     'data/concurrente_wall_6.dat' using 1:4 title 'Reduce' w lines linestyle 3

set output 'data/concurrente_estudio_cpu_8.png'
plot 'data/concurrente_cpu_8.dat' using 1:2 title 'Creación' w lines linestyle 1, \
     'data/concurrente_cpu_8.dat' using 1:3 title 'Computo' w lines linestyle 2, \
     'data/concurrente_cpu_8.dat' using 1:4 title 'Reduce' w lines linestyle 3

set output 'data/concurrente_estudio_wall_8.png'
plot 'data/concurrente_wall_8.dat' using 1:2 title 'Creación' w lines linestyle 1, \
     'data/concurrente_wall_8.dat' using 1:3 title 'Computo' w lines linestyle 2, \
     'data/concurrente_wall_8.dat' using 1:4 title 'Reduce' w lines linestyle 3

set output 'data/concurrente_estudio_wall_p.png'
plot 'data/concurrente_wall_2.dat' using 1:($2+$3+$4) title 'p=2' w lines linestyle 1, \
     'data/concurrente_wall_4.dat' using 1:($2+$3+$4) title 'p=4' w lines linestyle 2, \
     'data/concurrente_wall_6.dat' using 1:($2+$3+$4) title 'p=6' w lines linestyle 3, \
     'data/concurrente_wall_8.dat' using 1:($2+$3+$4) title 'p=8' w lines linestyle 4

set output 'data/concurrente_secuencial_estudio.png'
plot 'data/concurrente_wall_2.dat' using 1:($2+$3+$4) title 'p=2' w lines linestyle 1, \
     'data/concurrente_wall_4.dat' using 1:($2+$3+$4) title 'p=4' w lines linestyle 2, \
     'data/concurrente_wall_6.dat' using 1:($2+$3+$4) title 'p=6' w lines linestyle 3, \
     'data/concurrente_wall_8.dat' using 1:($2+$3+$4) title 'p=8' w lines linestyle 4, \
     'data/secuencial_wall.dat' using 1:2 title 'secuencial' w lines linestyle 5



