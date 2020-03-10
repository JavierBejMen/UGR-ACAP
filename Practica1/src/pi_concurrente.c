#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "mpi.h"
#include <time.h>

#define PI 3.141592653589793238462643

int main(int argc, char **argv)
{
  int mi_rank, p, source, dest, tag=0;
  if(argc != 2){
    perror("Error: numero de parametros incorrectos\n");
    printf("Uso: %s <m_intervalos>\n", argv[0]);

    exit(EXIT_FAILURE);
  }

  struct timespec start_comp, end_comp, start_reduce, end_reduce,
                  start_crea, end_crea, start_dest, end_dest;

  double width, local_sum, x, error, global_sum;
  unsigned long intervals;
  long double time;

  /* get the number of intervals */
  intervals = atoi(argv[1]);
  width = 1.0 / intervals;
  time = 0;
  error = 0;
  unsigned long local_intervals, bot, top;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &mi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  local_intervals = intervals/p;
  bot = mi_rank * local_intervals;
  top = bot + local_intervals;

  local_sum = 0;
  for (unsigned long i=bot; i<top; ++i) {
    x = (i + 0.5) * width;
    local_sum += 4.0 / (1.0 + x * x);
  }

  MPI_Reduce(&local_sum,
             &global_sum,
             1,
             MPI_DOUBLE,
             MPI_SUM,
             0,
             MPI_COMM_WORLD);

  if(mi_rank == 0){
    global_sum *= width;
    error = fabs(global_sum/PI - 1);
    printf("%14lu %0.14f %0.14f\n", intervals, error, global_sum);
  }

  MPI_Finalize();
  exit(EXIT_SUCCESS);
}
