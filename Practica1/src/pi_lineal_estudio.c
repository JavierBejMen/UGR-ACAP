#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979

int main(int argc, char **argv)
{
  if(argc != 3){
    perror("Error: numero de parametros incorrectos\n");
    printf("Uso: %s <modo> <m_intervalos>\n", argv[0]);

    exit(EXIT_FAILURE);
  }

  double width, sum, x, error;
  int intervals, i, modo;

  /* modo */
  modo = atoi(argv[1]);

  /* get the number of intervals */
  intervals = atoi(argv[2]);
  width = 1.0 / intervals;

  /* do the computation */
  sum = 0;

  switch (modo) {
    case 0:
      for (i=0; i<intervals; ++i) {
        x = (double)(i) * width;
        sum += 4.0 / (1.0 + x * x);
      }
      break;

    case 1:
      for (i=0; i<intervals; ++i) {
        x = (i + 0.5) * width;
        sum += 4.0 / (1.0 + x * x);
      }
      break;

    case 2:
      for (i=0; i<intervals; ++i) {
        x = (i + 1) * width;
        sum += 4.0 / (1.0 + x * x);
      }
      break;

    default:
      perror("Modo no reconozido\n");
      printf("Modos: \n0 -> p.izquierdo\n1 -> p.medio\n2 -> p.derecho\n");
      exit(EXIT_FAILURE);
  }

  sum *= width;
  error = fabs(sum/PI - 1);

  printf("PI_est %0.14f\n", sum);
  printf("Error %0.14f\n", error);

  exit(EXIT_SUCCESS);
}
