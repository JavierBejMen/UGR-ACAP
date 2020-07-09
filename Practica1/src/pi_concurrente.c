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
  double time_crea, time_comp, global_time_comp, time_reduce;
  double total_time_comp=0, total_time_reduce=0;
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
                  start_crea, end_crea;

  // Variables de computo
  double width, x;
  long double local_sum, global_sum, error;
  long double total_sum=0, total_error=0;
  unsigned long intervals, local_intervals, bot, top;

  intervals = atoi(argv[2]);
  width = 1.0 / intervals;
  error = 0;

  // Zona MPI
  time_crea = 0;
  int mi_rank, p;

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

    time_crea = seconds + (double)ns/(double)1000000000;
  }
  //3 times experiment
  for (int i = 0; i < 3; i++){
    local_sum = 0;
    //Computo
    clock_gettime(clk_modo, &start_comp);

    for (unsigned long i=bot; i<top; ++i) {
      x = (i + 0.5) * width;
      local_sum += 4.0 / (1.0 + x * x);
    }

    clock_gettime(clk_modo, &end_comp);
    seconds = end_comp.tv_sec-start_comp.tv_sec;
    ns = end_comp.tv_nsec-start_comp.tv_nsec;

    //underflow
    if(start_comp.tv_nsec > end_comp.tv_nsec){
      --seconds;
      ns += 1000000000;
    }

    time_comp = seconds + (double)ns/(double)1000000000;

    //Get max comp time
    MPI_Reduce(&time_comp,
              &global_time_comp,
              1,
              MPI_DOUBLE,
              MPI_MAX,
              0,
              MPI_COMM_WORLD);

    //Reduce
    if(mi_rank == 0)
      clock_gettime(clk_modo, &start_reduce);

    MPI_Reduce(&local_sum,
              &global_sum,
              1,
              MPI_LONG_DOUBLE,
              MPI_SUM,
              0,
              MPI_COMM_WORLD);

    if(mi_rank == 0){
      global_sum *= width;

      clock_gettime(clk_modo, &end_reduce);
      seconds = end_reduce.tv_sec-start_reduce.tv_sec;
      ns = end_reduce.tv_nsec-start_reduce.tv_nsec;

      //underflow
      if(start_reduce.tv_nsec > end_reduce.tv_nsec){
        --seconds;
        ns += 1000000000;
      }

      time_reduce = seconds + (double)ns/(double)1000000000;
      printf("AJA %f\n", time_reduce);

      error = fabsl(PI-global_sum);
      total_error += error;
      total_sum += global_sum;
      total_time_comp += global_time_comp;
      total_time_reduce += time_reduce;
    }

    MPI_Barrier(MPI_COMM_WORLD);
  }

  if (mi_rank == 0){
    total_error /= 3;
    total_sum /= 3;
    total_time_comp /= 3;
    total_time_reduce /= 3;

    printf("%lu %f %f %f %0.25Lf %Le\n", intervals, time_crea, total_time_comp, total_time_reduce, total_sum, total_error);
  }

  MPI_Finalize();
  exit(EXIT_SUCCESS);
}
