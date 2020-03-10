#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "mpi.h"
#include <time.h>

#define PI 3.141592653589793238462643

int main(int argc, char **argv)
{
  // Comprobacion parametros
  if(argc != 3){
    perror("Error: numero de parametros incorrectos\n");
    printf("Uso: %s <clock_modo> <m_intervalos>\n", argv[0]);

    exit(EXIT_FAILURE);
  }

  //clock
  double time_crea, time_comp, time_reduce, time_dets;
  long seconds, ns;
  int clk_modo;
  switch (atoi(argv[1])) {
    case 0:
      clk_modo = CLOCK_REALTIME;
      break;
    case 1:
      clk_modo = CLOCK_PROCESS_CPUTIME_ID;
      break;
    default:
      perror("Error: modo de reloj desconocido");
      printf("Modo reloj soportado: [0][1]");
      exit(EXIT_FAILURE);
  }

  struct timespec start_comp, end_comp, start_reduce, end_reduce,
                  start_crea, end_crea, start_dest, end_dest;

  // Variables de computo
  double width, x;
  long double local_sum, global_sum, error;
  unsigned long intervals, local_intervals, bot, top;

  intervals = atoi(argv[2]);
  width = 1.0 / intervals;
  error = 0;

  // Zona MPI
  time_crea = 0;
  int mi_rank, p, source, dest;

  clock_gettime(clk_modo, &start_crea);

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &mi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  local_intervals = intervals/p;
  bot = mi_rank * local_intervals;
  top = bot + local_intervals;

  local_sum = 0;
  global_sum = 0;

  if(mi_rank == 0){

    clock_gettime(clk_modo, &end_crea);
    seconds = end_crea.tv_sec-start_crea.tv_sec;
    ns = end_crea.tv_nsec-start_crea.tv_nsec;

    //underflow
    if(start_crea.tv_nsec > end_crea.tv_nsec){
      --seconds;
      ns += 1000000000;
    }

    time_crea += seconds + (double)ns/(double)1000000000;
  }

  for (unsigned long i=bot; i<top; ++i) {
    x = (i + 0.5) * width;
    local_sum += 4.0 / (1.0 + x * x);
  }

  MPI_Reduce(&local_sum,
             &global_sum,
             1,
             MPI_LONG_DOUBLE,
             MPI_SUM,
             0,
             MPI_COMM_WORLD);

  if(mi_rank == 0){
    global_sum *= width;
    error = fabsl(PI-global_sum);
    printf("%lu %f %0.25Lf %Le\n", intervals, time_crea, global_sum, error);
  }

  MPI_Finalize();
  exit(EXIT_SUCCESS);
}
