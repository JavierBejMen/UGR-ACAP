set terminal png size 700,500

set xlabel "n elementos"
set ylabel "time"

set yrange [0:]

set style line 1 \
    linecolor rgb '#0060ad' \
    linetype 1 linewidth 2


set style line 2 \
    linecolor rgb '#dd181f' \
    linetype 1 linewidth 2

set output 'cpu_lines.png'
plot 'data_gpu.dat' using 1:2 title 'cpu' w lines linestyle 1
set output 'gpu_lines.png'
plot 'data_gpu.dat' using 1:2 title 'gpu' w lines linestyle 2

set output 'gpu_cpu_lines.png'
plot 'data_cpu.dat' using 1:2 title 'cpu' w lines linestyle 1, \
     'data_gpu.dat' using 1:2 title 'gpu' w lines linestyle 2

set output 'cpu_bars.png'
set style fill solid
set boxwidth 0.5
plot '< sort -nk2 data_cpu.dat' using 2:xtic(1) title 'cpu' with boxes ls 1

set output 'gpu_bars.png'
plot '< sort -nk2 data_gpu.dat' using 2:xtic(1) title 'gpu' with boxes ls 2
