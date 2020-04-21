#include<stdio.h>
#include<stdlib.h>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"


int main() {
  int nDevice;
  cudaGetDeviceCount(&nDevice);
  for (int i = 0; i < nDevice; i++) {
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, i);
    printf("Dispositivo : %d\n", i);
    printf("  Nombre : %s\n", prop.name);
    printf("  Frecuencia Reloj : %d KHz\n", prop.memoryClockRate);
    printf("  Ancho del Bus de Memoria : %d bits\n", prop.memoryBusWidth);
    printf("  Ancho de Banda : %f GB/s\n\n",
           2.0*prop.memoryClockRate*(prop.memoryBusWidth/8)/1.0e6);
  }
}
