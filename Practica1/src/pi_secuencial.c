#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define PI 3.141592653589793238462643

int main(int argc, char **argv)
{
  if(argc != 2){
    perror("Error: numero de parametros incorrectos\n");
    printf("Uso: %s <m_intervalos>\n", argv[0]);

    exit(EXIT_FAILURE);
  }

  struct timespec start, end;

  double width, sum, x, error;
  unsigned long intervals;
  long double time;

  /* get the number of intervals */
  intervals = atoi(argv[1]);
  width = 1.0 / intervals;
  time = 0;
  error = 0;

  for(int j = 0; j < 3; ++j){
    sum = 0;
    /* do the computation */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for (unsigned long i=0; i<intervals; ++i) {
      x = (i + 0.5) * width;
      sum += 4.0 / (1.0 + x * x);
    }

    sum *= width;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    time += (end.tv_nsec-start.tv_nsec);
    error += fabs(sum/PI - 1);
  }

  error /= 3;
  time /= 3000000000;

  FILE *f;

  if((f=fopen("data/secuencial_monotonic.dat", "a+")) != NULL){
    fprintf(f, "%14lu %0.14f %0.14Lf\n", intervals, error, time);

    fclose(f);
  }

  exit(EXIT_SUCCESS);
}
