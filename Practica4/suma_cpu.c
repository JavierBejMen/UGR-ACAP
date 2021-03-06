#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


int main(int argc, char **argv)
{
  if(argc<2){
    perror("No se ha introducido el directorio de los vectores\n");
    exit(EXIT_FAILURE);
  }

	/** Lectura **/
  FILE *in_0, *in_1;
  char name_0[50], name_1[50];

  strcpy(name_0, argv[1]);
  strcat(name_0, "input0.raw");
  strcpy(name_1, argv[1]);
  strcat(name_1, "input0.raw");

  if ( (in_0 = fopen(name_0,"r")) == NULL ) {
    fprintf(stderr, "Error en lectura %s\n", name_0);
    exit(EXIT_FAILURE);
  }
  if ( (in_1 = fopen(name_1, "r")) == NULL ){
    fprintf(stderr, "Error en lectura %s\n", name_1);
    exit(EXIT_FAILURE);
  }

  int nEle0, nEle1, nElementos;
  fscanf(in_0, "%d", &nEle0);
  fscanf(in_1, "%d", &nEle1);
  if (nEle0 != nEle1){
    perror("Distinto tamaño de vectores\n");
    exit(EXIT_FAILURE);
  }
  nElementos = nEle1;
  size_t size = nElementos * sizeof(float);
  printf("Operacion sobre %d elementos\n", nElementos);

  /** Asignacion memoria **/
  float *input0 = (float *)malloc(size);
  float *input1 = (float *)malloc(size);
  float *output = (float *)malloc(size);

  if (input0 == NULL || input1 == NULL || output == NULL){
    perror("Error allocando memoria\n");
    exit(EXIT_FAILURE);
  }

  /** Inicializa vectores **/
  float aux;
  for (int i = 0; i < nElementos; ++i){
    fscanf(in_0,"%f",&aux);
    input0[i] = aux;
    fscanf(in_1,"%f",&aux);
    input1[i] = aux;
  }

  /** Operacion **/
  clock_t start, end;
  double elapsed;
  start = clock();
  for (int i = 0; i < nElementos; ++i){
    //for(int j = 0; j < 1; ++j)
		  output[i] = pow(pow(log(5*input0[i]*100*input1[i]+7*input0[i])/0.33, 3), 7);
  }
  end = clock();
  elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;

  printf("Operacion terminada: %f segundos\n", elapsed);

  /** Escritura resultados **/
  FILE *out;
  char name_out[50];
  strcpy(name_out, argv[1]);
  strcat(name_out, "output_cpu.raw");

  if( ( out = fopen( name_out, "w" ) ) == NULL ) {
    printf( "No se pudo crear el archivo de salida %s\n", name_out);
  }
  else{
    for (int i = 0; i < nElementos; i++)
      fprintf(out, "%.5f\n", output[i]);

    fclose(out);
  }

  /** Liberar y cerrar **/
  free(input0);
  free(input1);
  free(output);

  fclose(in_0);
  fclose(in_1);

  /** Archivo para plot **/
  FILE *data;
  if( (data = fopen("data_cpu.dat", "a")) == NULL){
    printf("No se pudo crear archivo para plot");
  }
  else{
    fprintf(data,"%d %f\n", nElementos,elapsed);
    fclose(data);
  }

  exit(EXIT_SUCCESS);
}
