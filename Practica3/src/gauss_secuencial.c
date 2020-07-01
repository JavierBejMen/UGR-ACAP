
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>

#include "libbmp.h"

#define PI 3.141592653589793238462643

int main(int argc, char const *argv[]) {

  //Comprobar argumentos
  if (argc != 3){
    perror("Error, uso canny_secuencial <ruta_img.bmp> <nombre_dir_output>");
    return EXIT_FAILURE;
  }

  //Leer imagen
  bmp_img in_image;

  if (bmp_img_read(&in_image, argv[1]) != 0){
    perror("Error al leer la imagen");

    return EXIT_FAILURE;
  }

  printf("Imagen %s cargada correctamente\n", argv[1]);

/*******************************************************************************
* Gauss
*******************************************************************************/
  //Gaussian blur 5x5, calcular kernel
  //k = 2, sigma = 1
  int k = 5, sigma = 50;
  printf("<-- Canny (k=%d, sigma=%d) -->\n", k, sigma);
  double gaus_kernel[2*k+1][2*k+1];

  double distance = 0;
  double euler = 1 / (2. * PI * sigma * sigma);
  double suma_total = 0;

  for (int i = -k; i <= k; i++){
    for (int j = -k; j <= k; j++){
      distance = ((i*i) + (j*j)) / 2.;
      gaus_kernel[i+k][j+k] = euler*exp(-distance);
      suma_total += gaus_kernel[i+k][j+k];
    }
  }

  for (int i = 0; i < 2*k+1; i++)
    for (int j = 0; j < 2*k+1; j++)
      gaus_kernel[i][j] *= (1. / suma_total);

  printf("Gaussian kernel:\n");
  for (int i = 0; i < 2*k+1; i++){
    for (int j = 0; j < 2*k+1; j++){
      printf("[%f]", gaus_kernel[i][j]);
    }
    printf("\n");
  }

  //Reservar memoria para la solucion
  size_t width = in_image.img_header.biWidth;
  size_t height = in_image.img_header.biHeight;

  bmp_pixel **out_data;
  out_data = malloc(height * sizeof(bmp_pixel*));
  if (out_data == NULL){
    perror("Error alocando pixels columna de la solucion");
    return EXIT_FAILURE;
  }

  for (int i = 0; i < height; i++){
    if ((out_data[i] = malloc(width * sizeof(bmp_pixel))) == NULL){
      perror("Error alocando pixels fila de la solucion");
    }
  }

  //Recorrer imagen
  bmp_pixel pixel_gauss;
  size_t pixel_size = sizeof(bmp_pixel);
  printf("\nCalculando blur img_size[%d][%d]\n", in_image.img_header.biHeight-k,
    in_image.img_header.biWidth-k);

  for (int i = k; i < height-k; i++){
    for (int j = k; j < width-k; j++){
      //Gauss per pixel
      bmp_pixel_init(&pixel_gauss, 0,0,0);
      for (int l = -k; l <= k; l++){
        for (int m = -k; m <= k; m++){
          pixel_gauss.blue += in_image.img_pixels[i+l][j+m].blue
            * gaus_kernel[l+k][m+k];
          pixel_gauss.red += in_image.img_pixels[i+l][j+m].red
            * gaus_kernel[l+k][m+k];
          pixel_gauss.green += in_image.img_pixels[i+l][j+m].green
            * gaus_kernel[l+k][m+k];
        }
      }

      bmp_pixel_init(&pixel_gauss, pixel_gauss.red/((2*k+1)*(2*k+1)),
        pixel_gauss.green/((2*k+1)*(2*k+1)),
        pixel_gauss.blue/((2*k+1)*(2*k+1)));

      out_data[i][j] = pixel_gauss;
    }
  }

  //Crear imagen con el resultado
  bmp_img out_image;
  out_image.img_header = in_image.img_header;
  out_image.img_pixels = out_data;


  //Guardar resultados
  struct stat st = {0};
  char out_dir[50];
  sprintf(out_dir, "out/%s", argv[2]);
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

  //Liberar
  bmp_img_free(&in_image);
  bmp_img_free(&out_image);

  return EXIT_SUCCESS;
}
