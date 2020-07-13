
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <mpi.h>

#include "libbmp.h"
#include "matrix_helper.h"

#define PI 3.141592653589793238462643
#define SOURCE 0
#define DEFAULT_TAM 1000

/***************************************************************************
 * MAIN
***************************************************************************/
int main(int argc, char *argv[]) {
  
  int mi_rank, p;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &mi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  /***************************************************************************
   * Variables globales
  ***************************************************************************/
  double *gaus_kernel;
  mh_transform transform;
  bmp_img in_image;

  /***************************************************************************
   * Estructuras para comunicación
  ***************************************************************************/
  //bmp_pixel
  MPI_Datatype mpi_pixel;
  MPI_Datatype pixel_type[3] = {MPI_UNSIGNED_CHAR, MPI_UNSIGNED_CHAR, MPI_UNSIGNED_CHAR};
  int pixel_blocklen[3] = {1, 1, 1};
  MPI_Aint pixel_disp[3];
  pixel_disp[0] = offsetof(bmp_pixel, blue);
  pixel_disp[1] = offsetof(bmp_pixel, green);
  pixel_disp[2] = offsetof(bmp_pixel, red);
  MPI_Type_create_struct(3, pixel_blocklen, pixel_disp, pixel_type, &mpi_pixel);
  MPI_Type_commit(&mpi_pixel);

  //mh_transform
  MPI_Datatype mpi_transform;
  MPI_Datatype transform_type[8];
  int transform_blocklen[8];
  for (unsigned i = 0; i < 8; i++){
    transform_type[i] = MPI_INT;
    transform_blocklen[i] = 1;
  }
  MPI_Aint transform_disp[8];
  transform_disp[0] = offsetof(mh_transform, block_n);
  transform_disp[1] = offsetof(mh_transform, block_width_size);
  transform_disp[2] = offsetof(mh_transform, block_height_size);
  transform_disp[3] = offsetof(mh_transform, height_size);
  transform_disp[4] = offsetof(mh_transform, width_size);
  transform_disp[5] = offsetof(mh_transform, k);
  transform_disp[6] = offsetof(mh_transform, transform_height_size);
  transform_disp[7] = offsetof(mh_transform, transform_width_size);
  MPI_Type_create_struct(8, transform_blocklen, transform_disp, transform_type, &mpi_transform);
  MPI_Type_commit(&mpi_transform);

  
  
  
  /***************************************************************************
   * Inicialización
  ***************************************************************************/
  if (mi_rank == SOURCE){
    //Comprobar argumentos
    if (argc != 4){
      perror("Error, uso canny_secuencial <n_bloques> <ruta_img.bmp> <nombre_dir_output>");
      exit(EXIT_FAILURE);
    }

    //Cargar imagen
    if (bmp_img_read(&in_image, argv[2]) != 0){
      perror("Error al leer la imagen");
      exit(EXIT_FAILURE);
    }
    printf("Imagen %s cargada correctamente\n", argv[1]);

    //Cargar número de bloques, debe ser raiz cuadrada
    unsigned n;
    if ((n = atoi(argv[1])) == 0){
      perror("Error al leer el número de bloques");
      exit(EXIT_FAILURE);
    }
    unsigned block_per_row = sqrt(n);

    //Inicializar datos para el control de la imagen real y los bloques
    transform = mh_init_transform(block_per_row, in_image.img_header.biHeight, in_image.img_header.biWidth, 2);

    /***************************************************************************
    * Gauss inicialización
    ***************************************************************************/
    //Gaussian blur 5x5, calcular kernel
    //k = 2, sigma = 1
    int sigma = 10;
    printf("<-- From master: Gauss (k=%d, sigma=%d) -->\n", transform.k, sigma);

    //Reservar memoria para el kernel
    if ((gaus_kernel = malloc((2*transform.k+1)*(2*transform.k+1)* sizeof(double))) == NULL){
      fprintf(stderr, "(%d): Error(%d) alocando memoria para el kernel: %s\n",
        mi_rank, errno, strerror(errno)
      );
      exit(EXIT_FAILURE);
    }

    double distance = 0;
    double euler = 1 / (2. * PI * sigma * sigma);
    double suma_total = 0;

    for (int i = -transform.k; i <= transform.k; i++){
      for (int j = -transform.k; j <= transform.k; j++){
        distance = ((i*i) + (j*j)) / 2.;
        gaus_kernel[(i+transform.k)*(2*transform.k+1)+j+transform.k] = euler*exp(-distance);
        suma_total += gaus_kernel[(i+transform.k)*(2*transform.k+1)+j+transform.k];
      }
    }

    for (int i = 0; i < 2*transform.k+1; i++)
      for (int j = 0; j < 2*transform.k+1; j++)
        gaus_kernel[i*(2*transform.k+1)+j] *= (1. / suma_total);

    //Control prints
    printf("[%d] Gaussian kernel:\n", mi_rank);
    for (int i = 0; i < 2*transform.k+1; i++){
      for (int j = 0; j < 2*transform.k+1; j++){
        printf("[%f]", gaus_kernel[i*(2*transform.k+1)+j]);
      }
      printf("\n");
    }
    printf("[%d] Transform:\n", mi_rank);
    mh_print_transform(transform);

    /***************************************************************************
     * Broadcast transform y kernel
    ***************************************************************************/
    //Broadcast transform
    MPI_Bcast(
      &transform,
      1,
      mpi_transform,
      SOURCE,
      MPI_COMM_WORLD
    );

    //Broadcast kernel
    MPI_Bcast(
      gaus_kernel,
      (2*transform.k+1)*(2*transform.k+1),
      MPI_DOUBLE,
      SOURCE,
      MPI_COMM_WORLD
    );

    
  }
  else{
    //Resto de programas
    //Broadcast transform
    MPI_Bcast(
      &transform,
      1,
      mpi_transform,
      SOURCE,
      MPI_COMM_WORLD
    );

    //Reservar memoria para el kernel
    if ((gaus_kernel = malloc((2*transform.k+1)*(2*transform.k+1)*sizeof(double))) == NULL){
      fprintf(stderr, "(%d): Error(%d) alocando memoria para el kernel: %s\n",
        mi_rank, errno, strerror(errno)
      );
      exit(EXIT_FAILURE);
    }

    //Broadcast kernel
    MPI_Bcast(
      gaus_kernel,
      (2*transform.k+1)*(2*transform.k+1),
      MPI_DOUBLE,
      SOURCE,
      MPI_COMM_WORLD
    );
  }

  /***************************************************************************
   * Scatter matrix inicialización
  ***************************************************************************/
  //Buffer entrada
  bmp_pixel img_block[transform.block_height_size][transform.block_width_size];

  if (mi_rank == SOURCE){
    //Memoria continua, buffer salida
    bmp_pixel scatter_matrix[transform.transform_height_size][transform.transform_width_size];

    //Rellenar matriz
    unsigned i_logic, j_logic;
    for (unsigned i = 0; i < transform.transform_height_size; i++){
      i_logic = mh_get_i(transform, i);
      for (unsigned j = 0; j < transform.transform_width_size; j++){
        j_logic = mh_get_j(transform, j);
        if (i_logic == NULL || j_logic == NULL){
          bmp_pixel_init(&scatter_matrix[i][j], 128, 128, 128);
        }else{
          bmp_pixel_init(&scatter_matrix,
            in_image.img_pixels[i_logic][j_logic].blue,
            in_image.img_pixels[i_logic][j_logic].green,
            in_image.img_pixels[i_logic][j_logic].red);
        }
      }
    }

    MPI_Scatter(
      scatter_matrix,
      transform.block_height_size * transform.block_width_size,
      mpi_pixel,
      img_block,
      transform.block_height_size * transform.block_width_size,
      mpi_pixel,
      SOURCE,
      MPI_COMM_WORLD
    );

    MPI_Gather(
      img_block,
      transform.block_height_size * transform.block_width_size,
      mpi_pixel,
      scatter_matrix,
      transform.block_height_size * transform.block_width_size,
      mpi_pixel,
      SOURCE,
      MPI_COMM_WORLD
    );

    bmp_img out_image;
    out_image.img_header = in_image.img_header;
    out_image.img_pixels = malloc(transform.height_size * sizeof(bmp_pixel*));
    for (unsigned i = 0; i < transform.height_size; i++){
      out_image.img_pixels[i] = malloc(transform.width_size * sizeof(bmp_pixel));
    }

    for (unsigned i = 0; i < transform.transform_height_size; i++){
      i_logic = mh_get_i(transform, i);
      for (unsigned j = 0; j < transform.transform_width_size; j++){
        j_logic = mh_get_j(transform, j);
        if (i != NULL && j != NULL){
          bmp_pixel_init(&out_image.img_pixels[i_logic][j_logic],
            scatter_matrix[i][j].blue,
            scatter_matrix[i][j].green,
            scatter_matrix[i][j].red
          );
        }
      }
    }

    //Guardar resultados
  struct stat st = {0};
  char out_dir[50];
  sprintf(out_dir, "out/%s", argv[3]);
  if (stat(out_dir, &st) == -1){
    mkdir(out_dir, 0700);
  }
  printf("Archivos de salida en %s\n", out_dir);

  //Imagen
  char out_img[100];
  sprintf(out_img, "%s/out_img.bmp", out_dir);
  if (bmp_img_write(&out_image, out_img) == 0){
    printf("Imagen de salida guardada en %s\n", out_img);
  }
  else{
    perror("No se ha podido guardar la imagen");
  }
  }else{
    MPI_Scatter(
      (void *)NULL,
      (void *)NULL,
      MPI_DATATYPE_NULL,
      img_block,
      transform.block_height_size * transform.block_width_size,
      mpi_pixel,
      SOURCE,
      MPI_COMM_WORLD
    );

    MPI_Gather(
      img_block,
      transform.block_height_size * transform.block_width_size,
      mpi_pixel,
      (void *)NULL,
      (void *)NULL,
      MPI_DATATYPE_NULL,
      SOURCE,
      MPI_COMM_WORLD
    );
  }

  /***************************************************************************
   * Computo
  ***************************************************************************/

  /***************************************************************************
   * Gather
  ***************************************************************************/
  //MPI_Gather()
  
  
    


   

 
  
  

  /***************************************************************************
   * Liberación memoria y finalización
  ***************************************************************************/
  if (gaus_kernel != NULL) free(gaus_kernel);
  MPI_Type_free(&mpi_transform);
  MPI_Type_free(&mpi_pixel);

  MPI_Finalize();
  return EXIT_SUCCESS;
}
