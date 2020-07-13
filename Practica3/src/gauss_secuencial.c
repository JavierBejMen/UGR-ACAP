
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <float.h>

#include "libbmp.h"
#include "gauss.h"

#define PI 3.141592653589793238462643

int main(int argc, char *argv[]) {
  //Comprobar argumentos
  if (argc != 5){
    perror("Error, uso gauss_secuencial <k> <sigma> <ruta_img.bmp> <nombre_dir_output>");
    exit(EXIT_FAILURE);
  }

  int k;
  int sigma;
  
  if ((k = atoi(argv[1])) == 0){
    perror("Error atoi returned 0 in k");
    exit(EXIT_FAILURE);
  }
  if ((sigma = atoi(argv[2])) == 0){
    perror("Error atof returned 0 in sigma");
    exit(EXIT_FAILURE);
  }
  bmp_img in_image;
  if (bmp_img_read(&in_image, argv[3]) != 0){
    perror("Error al leer la imagen");
    exit(EXIT_FAILURE);
  }



  printf("Imagen %s cargada correctamente\n", argv[3]);

/*******************************************************************************
* Gauss
*******************************************************************************/
  //Inicializar kernel
  gauss_kernel kernel;
  gauss_set_kernel(&kernel, k, sigma);
  
  printf("<-- Gauss (k=%d, sigma=%d) -->\n", k, sigma);
  printf("Gaussian kernel:\n");
  gauss_print_kernel(&kernel);
  
  //Reservar memoria para la solucion
  //bmp_pixel **out_data;
  bmp_img out_img;
  bmp_img_init_df(&out_img, in_image.img_header.biWidth, in_image.img_header.biHeight);

  //Aplicar kernel gaussiano
  printf("\nCalculando blur img_size[%d][%d]\n", in_image.img_header.biHeight-k,
    in_image.img_header.biWidth-k);

  
  //Wall time best of 3
  long double elapsed = LDBL_MAX;
  for (unsigned i = 0; i < 3; i++){
    long double start, end;
    struct timeval timecheck; 
    gettimeofday(&timecheck, NULL);
    start = (long double)timecheck.tv_sec + (long double)timecheck.tv_usec / 1000000;

    //Computo
    gauss_apply_kernel(
      &kernel,
      in_image.img_pixels,
      out_img.img_pixels,
      out_img.img_header.biHeight,
      out_img.img_header.biWidth);

    //Elapsed time
    gettimeofday(&timecheck, NULL);
    end = (long double)timecheck.tv_sec + (long double)timecheck.tv_usec / 1000000;
    elapsed = (end - start) < elapsed ? end - start : elapsed;
    printf("%Lf seconds elapsed (iteración %d)\n", (end - start), i);
  }

  printf("%Lf seconds elapsed (best of 3)\n", elapsed);

  //Guardar resultados
  struct stat st = {0};
  char out_dir[50];
  sprintf(out_dir, "out/%s", argv[4]);
  if (stat(out_dir, &st) == -1){
    mkdir(out_dir, 0700);
  }
  printf("Archivos de salida en %s\n", out_dir);

  //Imagen
  char out_img_name[100];
  sprintf(
    out_img_name,
    "%s/%ds%dk.bmp",
    out_dir,
    out_img.img_header.biHeight * out_img.img_header.biWidth,
    kernel.k);
  if (bmp_img_write(&out_img, out_img_name) == 0){
    printf("Imagen de salida guardada en %s\n", out_img_name);
  }
  else{
    perror("No se ha podido guardar la imagen");
  }

  //Data
  char out_data_name[100];
  sprintf(out_data_name, "%s/data.dat", out_dir);
  FILE *f;
  if ((f = fopen(out_data_name, "a+")) == NULL){
    perror("No se guardaron las mediciones");
  }else{
    fprintf(
      f,
      "%d %d %Lf",
      out_img.img_header.biHeight*out_img.img_header.biWidth,
      kernel.k,
      elapsed);
  }

  //Liberar
  bmp_img_free(&in_image);
  bmp_img_free(&out_img);
  gauss_free_kernel(&kernel, 0);
  if (f != NULL) fclose(f);

  return EXIT_SUCCESS;
}
