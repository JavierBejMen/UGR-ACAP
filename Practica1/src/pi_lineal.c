#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979

int main(int argc, char **argv)
{
  if(argc != 2){
    perror("Error: numero de parametros incorrectos\n");
    printf("Uso: %s <m_intervalos>\n", argv[0]);

    exit(EXIT_FAILURE);
  }

  double width, sum, x, error;
  int intervals, i;

  /* get the number of intervals */
  intervals = atoi(argv[1]);
  width = 1.0 / intervals;

  /* do the computation */
  sum = 0;

  for (i=0; i<intervals; ++i) {
    x = (i + 0.5) * width;
    sum += 4.0 / (1.0 + x * x);
  }

  sum *= width;
  error = fabs(sum/PI - 1);

  printf("PI_est %0.14f\n", sum);
  printf("Error %0.14f\n", error);

  exit(EXIT_SUCCESS);
}
