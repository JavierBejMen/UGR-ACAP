#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


int main(int argc, char **argv)
{
  if(argc<2)
  {
    printf("No se ha introducido el directorio de los vectores\n");
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
    printf("Error en lectura %s\n", name_0);
    exit(EXIT_FAILURE);
  }
  if ( (in_1 = fopen(name_1, "r")) == NULL ){
    printf("Error en lectura %s\n", name_1);
    exit(EXIT_FAILURE);
  }

  int numElementsA, numElementsB, numElements;
  fscanf(in_0, "%d", &numElementsA);
  fscanf(in_1, "%d", &numElementsB);
  if (numElementsA != numElementsB)
  {
    fprintf(stderr, "Distinto tamaño de vectores\n");
    exit(EXIT_FAILURE);
  }
  numElements = numElementsB;
  size_t size = numElements * sizeof(float);
  printf("Operacion sobre %d elementos\n", numElements);

  /** Asignacion memoria **/
  float *input0 = (float *)malloc(size);
  float *input1 = (float *)malloc(size);
  float *output = (float *)malloc(size);

  if (input0 == NULL || input1 == NULL || output == NULL)
  {
    fprintf(stderr, "Error allocando memoria\n");
    exit(EXIT_FAILURE);
  }

  /** Inicializa vectores **/
  float aux;
  for (int i = 0; i < numElements; ++i)
  {
    fscanf(in_0,"%f",&aux);
    input0[i] = aux;
    fscanf(in_1,"%f",&aux);
    input1[i] = aux;
  }

  /** Operacion **/
  clock_t start, end;
  double elapsed;
  start = clock();
  for (int i = 0; i < numElements; ++i)
  {
    for(int j = 0; j < 1; ++j)
		  output[i] = pow(pow(log(5*input0[i]*100*input1[i]+7*input0[i])/0.33, 3), 7);
  }
  end = clock();
  elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;

  printf("Operacion terminada: %f segundos\n", elapsed);

  /** Escritura resultados **/
  FILE * out;
  char name_out[50];
  strcpy(name_out, argv[1]);
  strcat(name_out, "output_cpu.raw");

  if( ( out = fopen( name_out, "w" ) ) == NULL )
  {
    printf( "No se pudo crear la salida %s\n", name_out);
  }

  for (int i = 0; i < numElements; i++)
  {
    fprintf(out, "%.5f\n", output[i]);
  }

  /** Liberar y cerrar **/
  free(input0);
  free(input1);
  free(output);

  fclose(in_0);
  fclose(in_1);
  fclose(out);

  /** Archivo para plot **/
  FILE * data;
  data = fopen("data_cpu.dat", "a");
  fprintf(data,"%d %f\n", numElements,elapsed);
  fclose(data);

  return EXIT_SUCCESS;
}
