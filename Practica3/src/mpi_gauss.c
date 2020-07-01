
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include "mpi.h"

#include "libbmp.h"

#define PI 3.141592653589793238462643
#define SOURCE 0
#define DEFAULT_TAM 1000

int main(int argc, char const *argv[]) {

  //Comprobar argumentos
  if (argc != 3){
    perror("Error, uso canny_secuencial <ruta_img.bmp> <nombre_dir_output>");
    return EXIT_FAILURE;
  }

  //Buffer de comunicacion
  char *buffer;
  if ((buffer = malloc(DEFAULT_TAM*sizeof buffer)) == NULL){
    perror("Error alocando memoria del buffer");
    return EXIT_FAILURE;
  }
  int posicion;

  //Vaiables
  int k;
  double *gaus_kernel;

  //Mpi region
  int mi_rank, p;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &mi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if(mi_rank == SOURCE){


    /***************************************************************************
    * Gauss
    ***************************************************************************/
      //Gaussian blur 5x5, calcular kernel
      //k = 2, sigma = 1
      k = 2;
      int sigma = 10;
      printf("<-- From master: Gauss (k=%d, sigma=%d) -->\n", k, sigma);

      if ((gaus_kernel = malloc((2*k+1)*(2*k+1)* sizeof(double))) == NULL){
        fprintf(stderr, "(%d): Error(%d) alocando memoria para el kernel: %s\n",
          mi_rank, errno, strerror(errno)
        );
        return EXIT_FAILURE;
      }

      double distance = 0;
      double euler = 1 / (2. * PI * sigma * sigma);
      double suma_total = 0;

      for (int i = -k; i <= k; i++){
        for (int j = -k; j <= k; j++){
          distance = ((i*i) + (j*j)) / 2.;
          gaus_kernel[(i+k)*(2*k+1)+j+k] = euler*exp(-distance);
          suma_total += gaus_kernel[(i+k)*(2*k+1)+j+k];
        }
      }

      for (int i = 0; i < 2*k+1; i++)
        for (int j = 0; j < 2*k+1; j++)
          gaus_kernel[i*(2*k+1)+j] *= (1. / suma_total);

      printf("Gaussian kernel:\n");
      for (int i = 0; i < 2*k+1; i++){
        for (int j = 0; j < 2*k+1; j++){
          printf("[%f]", gaus_kernel[i*(2*k+1)+j]);
        }
        printf("\n");
      }

      //Envio de datos a los demas, no he realizado un solo bcast con pack porque no he
      //encontrado manera de hacerlo sin crear tipos y cosas complejas

      //Bcast con el kernel
      posicion = 0;
      MPI_Bcast(
        &k,
        1,
        MPI_INT,
        SOURCE,
        MPI_COMM_WORLD
      );

      MPI_Bcast(
        gaus_kernel,
        (2*k+1)*(2*k+1),
        MPI_DOUBLE,
        SOURCE,
        MPI_COMM_WORLD
      );
  }
  else{
    //Resto de prgramas
    MPI_Bcast(
      &k,
      1,
      MPI_INT,
      SOURCE,
      MPI_COMM_WORLD
    );

    if ((gaus_kernel = malloc((2*k+1)*(2*k+1)*sizeof(double))) == NULL){
      fprintf(stderr, "(%d): Error(%d) alocando memoria para el kernel: %s\n",
        mi_rank, errno, strerror(errno)
      );
      return EXIT_FAILURE;
    }

    MPI_Bcast(
      gaus_kernel,
      (2*k+1)*(2*k+1),
      MPI_DOUBLE,
      SOURCE,
      MPI_COMM_WORLD
    );
  }

  //Reparto de datos circular, la imagen
  if (mi_rank == 0){
    int n_p;
    MPI_Comm_size(MPI_COMM_WORLD, &n_p);


  }
}

  if (gaus_kernel != NULL) free(gaus_kernel);
  MPI_Finalize();
  return EXIT_SUCCESS;
}
