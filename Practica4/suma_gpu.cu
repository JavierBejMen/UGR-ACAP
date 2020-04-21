#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <cuda_runtime.h>
#include <helper_cuda.h>


__global__ void operacion(const float *A, const float *B, float *C, int nElementos)
{
  int i = blockDim.x * blockIdx.x + threadIdx.x;

  if (i < nElementos)
  {
    for(int j = 0; j < 1; ++j)
      C[i] = pow(pow(log(5*A[i]*100*B[i]+7*A[i])/0.33, 3), 7);
  }
}

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

  int nEle0, nEle1, nElementos;
  fscanf(in_0, "%d", &nEle0);
  fscanf(in_1, "%d", &nEle1);
  if (nEle0 != nEle1)
  {
    fprintf(stderr, "Distinto tamaño de vectores\n");
    exit(EXIT_FAILURE);
  }
  nElementos = nEle1;
  size_t size = nElementos * sizeof(float);
  printf("Operacion sobre %d elementos\n", nElementos);

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
  for (int i = 0; i < nElementos; ++i)
  {
    fscanf(in_0,"%f",&aux);
    input0[i] = aux;
    fscanf(in_1,"%f",&aux);
    input1[i] = aux;
  }

  /** Asignacion memoria cuda **/
  float *device0 = NULL;
  if (cudaMalloc((void **)&device0, size) != cudaSuccess)
  {
    fprintf(stderr, "Error allocando device 0\n");
    exit(EXIT_FAILURE);
  }

  float *device1 = NULL;
  if (cudaMalloc((void **)&device1, size) != cudaSuccess)
  {
    fprintf(stderr, "Error alocando device 1\n");
    exit(EXIT_FAILURE);
  }

  float *deviceOut = NULL;
  if (cudaMalloc((void **)&deviceOut, size) != cudaSuccess)
  {
    fprintf(stderr, "Error alocando device out\n");
    exit(EXIT_FAILURE);
  }

  clock_t start, end;
  double elapsed;
  start = clock();

  /** Enviar datos a dispositivo **/
  if (cudaMemcpy(device0, input0, size, cudaMemcpyHostToDevice) != cudaSuccess)
  {
    fprintf(stderr, "Error copiando input0 a device0\n");
    exit(EXIT_FAILURE);
  }

  if (cudaMemcpy(device1, input1, size, cudaMemcpyHostToDevice) != cudaSuccess)
  {
    fprintf(stderr, "Error copiando input1 a device1\n");
    exit(EXIT_FAILURE);
  }

  /** Lanzar kernel **/
  int threadsPerBlock = 256;
  int blocksPerGrid =(nElementos + threadsPerBlock - 1) / threadsPerBlock;
  printf("CUDA kernel: %d bloques de %d hebras\n", blocksPerGrid, threadsPerBlock);
  operacion<<<blocksPerGrid, threadsPerBlock>>>(device0, device1, deviceOut, nElementos);

  cudaError_t err = cudaGetLastError();

  if (err != cudaSuccess)
  {
    fprintf(stderr, "Error en la ejecucion del kernel CUDA\n", cudaGetErrorString(err));
    exit(EXIT_FAILURE);
  }

  /** Traer resultado de dispositivo **/
  printf("Copy output data from the CUDA device to the host memory\n");
  if (cudaMemcpy(output, deviceOut, size, cudaMemcpyDeviceToHost) != cudaSuccess)
  {
    fprintf(stderr, "Failed to copy vector C from device to host!\n");
    exit(EXIT_FAILURE);
  }

  end = clock();
  elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;

  printf("Operacion terminada: %f segundos\n", elapsed);

  /** Liberar memoria de dispositivo **/
  if (cudaFree(device0) != cudaSuccess)
  {
    fprintf(stderr, "Failed to free device vector A!\n");
    exit(EXIT_FAILURE);
  }

  if (cudaFree(device1) != cudaSuccess)
  {
    fprintf(stderr, "Failed to free device vector B!\n");
    exit(EXIT_FAILURE);
  }

  if (cudaFree(deviceOut) != cudaSuccess)
  {
    fprintf(stderr, "Failed to free device vector C!\n");
    exit(EXIT_FAILURE);
  }

  /** Escritura resultados **/
  FILE * out;
  char name_out[50];
  strcpy(name_out, argv[1]);
  strcat(name_out, "output_gpu.raw");

  if( ( out = fopen( name_out, "w" ) ) == NULL )
  {
    printf( "No se pudo crear la salida %s\n", name_out);
  }

  for (int i = 0; i < nElementos; i++)
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
  data = fopen("data_gpu.dat", "a");
  fprintf(data,"%d %f\n", nElementos,elapsed);
  fclose(data);

  return EXIT_SUCCESS;
}
