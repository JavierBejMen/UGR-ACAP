#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <cuda_runtime.h>
#include <helper_cuda.h>

/** Operacion con los vectores en la GPU **/
__global__ void operacion(const float *A, const float *B, float *C, int nElementos);

int main(int argc, char **argv) {
  if(argc<2) {
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
  if (nEle0 != nEle1) {
    perror("Distinto tamaño de vectores\n");
    exit(EXIT_FAILURE);
  }
  nElementos = nEle0;
  size_t size = nElementos * sizeof(float);
  printf("Operacion sobre %d elementos\n", nElementos);

  /** Asignacion memoria **/
  float *input0 = (float *)malloc(size);
  float *input1 = (float *)malloc(size);
  float *output = (float *)malloc(size);

  if (input0 == NULL || input1 == NULL || output == NULL) {
    perror("Error allocando memoria\n");
    exit(EXIT_FAILURE);
  }

  /** Inicializa vectores **/
  float aux;
  for (int i = 0; i < nElementos; ++i) {
    fscanf(in_0,"%f",&aux);
    input0[i] = aux;
    fscanf(in_1,"%f",&aux);
    input1[i] = aux;
  }

  /** Asignacion memoria cuda **/
  float *device0 = NULL;
  if (cudaMalloc((void **)&device0, size) != cudaSuccess) {
    perror("Error allocando device 0\n");
    exit(EXIT_FAILURE);
  }

  float *device1 = NULL;
  if (cudaMalloc((void **)&device1, size) != cudaSuccess) {
    perror("Error alocando device 1\n");
    exit(EXIT_FAILURE);
  }

  float *deviceOut = NULL;
  if (cudaMalloc((void **)&deviceOut, size) != cudaSuccess) {
    perror("Error alocando device out\n");
    exit(EXIT_FAILURE);
  }

  clock_t start, end;
  double elapsed;
  start = clock();

  /** Enviar datos a dispositivo **/
  if (cudaMemcpy(device0, input0, size, cudaMemcpyHostToDevice) != cudaSuccess) {
    perror("Error copiando input0 a device0\n");
    exit(EXIT_FAILURE);
  }

  if (cudaMemcpy(device1, input1, size, cudaMemcpyHostToDevice) != cudaSuccess) {
    perror("Error copiando input1 a device1\n");
    exit(EXIT_FAILURE);
  }

  /** Lanzar kernel **/
  int threadsPerBlock = 512;
  int blocksPerGrid =(nElementos + threadsPerBlock - 1) / threadsPerBlock;
  printf("CUDA kernel: %d bloques de %d hebras\n", blocksPerGrid, threadsPerBlock);
  operacion<<<blocksPerGrid, threadsPerBlock>>>(device0, device1, deviceOut, nElementos);

  cudaError_t err = cudaGetLastError();

  if (err != cudaSuccess) {
    fprintf(stderr, "Error en la ejecucion del kernel CUDA\n", cudaGetErrorString(err));
    exit(EXIT_FAILURE);
  }

  /** Traer resultado de dispositivo **/
  printf("Traer resultado a memoria\n");
  if (cudaMemcpy(output, deviceOut, size, cudaMemcpyDeviceToHost) != cudaSuccess) {
    perror("Error al traer resultado a memoria\n");
    exit(EXIT_FAILURE);
  }

  end = clock();
  elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;

  printf("Operacion terminada: %f segundos\n", elapsed);

  /** Liberar memoria de dispositivo **/
  if (cudaFree(device0) != cudaSuccess) {
    perror("Error al liberar device0\n");
    exit(EXIT_FAILURE);
  }

  if (cudaFree(device1) != cudaSuccess) {
    perror("Error al liberar device1\n");
    exit(EXIT_FAILURE);
  }

  if (cudaFree(deviceOut) != cudaSuccess) {
    perror("Error al liberar deviceOut\n");
    exit(EXIT_FAILURE);
  }

  /** Escritura resultados **/
  FILE *out;
  char name_out[50];
  strcpy(name_out, argv[1]);
  strcat(name_out, "output_gpu.raw");

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
  if( (data = fopen("data_gpu.dat", "a")) == NULL){
    printf("No se pudo crear archivo para plot");
  }
  else{
    fprintf(data,"%d %f\n", nElementos,elapsed);
    fclose(data);
  }

  exit(EXIT_SUCCESS);
}

__global__ void operacion(const float *A, const float *B, float *C, int nElementos) {
  int i = blockDim.x * blockIdx.x + threadIdx.x;

  if (i < nElementos) {
    //for(int j = 0; j < 1; ++j)
      C[i] = pow(pow(log(5*A[i]*100*B[i]+7*A[i])/0.33, 3), 7);
  }
}
