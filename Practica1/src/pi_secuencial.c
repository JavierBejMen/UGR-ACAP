#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define PI 3.141592653589793238462643

int main(int argc, char **argv)
{
  // Comprobacion argumentos
  if(argc != 3){
    perror("Error: numero de parametros incorrectos\n");
    printf("Uso: %s <clock_modo> <m_intervalos>\n", argv[0]);

    exit(EXIT_FAILURE);
  }

  // clock
  double time;
  long seconds, ns;
  struct timespec start, end;
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

  // Variables computo
  double width, x;
  long double sum, total_sum, error;
  unsigned long intervals;

  /* get the number of intervals */
  intervals = strtoul(argv[2], NULL, 0);
  width = 1.0 / intervals;
  time = 0;
  error = 0;
  total_sum = 0;

  // Repetir 3 veces el experimento
  for(int j = 0; j < 3; ++j){
    sum = 0;
    /* do the computation */
    clock_gettime(clk_modo, &start);
    for (unsigned long i=0; i<intervals; ++i) {
      x = (i + 0.5) * width;
      sum += 4.0 / (1.0 + x * x);
    }

    sum *= width;

    clock_gettime(clk_modo, &end);
    seconds = end.tv_sec-start.tv_sec;
    ns = end.tv_nsec-start.tv_nsec;

    //underflow
    if(start.tv_nsec > end.tv_nsec){
      --seconds;
      ns += 1000000000;
    }

    time += seconds + (double)ns/(double)1000000000;
    total_sum += sum;
  }

  time /= (double)3;
  total_sum = total_sum/(long double)3;
  error = fabsl((PI-total_sum));

  // Output
  FILE *f;

  char f_name[50];
  switch (atoi(argv[1])) {
    case 0:
      strcat(f_name, "data/secuencial_wall.dat");
      break;
    case 1:
      strcat(f_name, "data/secuencial_cpu.dat");
      break;
    default:
      perror("Error: modo de reloj desconocido");
      printf("Modo reloj soportado: [0][1]");
      exit(EXIT_FAILURE);
  }

  if((f=fopen(f_name, "a+")) != NULL){
    fprintf(f, "%lu %f %0.25Lf %Le\n", intervals, time, total_sum, error);

    fclose(f);
  }

  exit(EXIT_SUCCESS);
}
