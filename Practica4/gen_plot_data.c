#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {

  FILE *dCpu, *dGpu, *dataBar, *dataSpeed;

  if( (dCpu=fopen("data_cpu.dat", "r")) == NULL ){
    perror("Error al abrir datos de cpu");
    exit(EXIT_FAILURE);
  }

  if( (dGpu=fopen("data_gpu.dat", "r")) == NULL ){
    perror("Error al abrir datos de gpu");
    exit(EXIT_FAILURE);
  }

  if( (dataSpeed=fopen("data_speedup.dat", "w")) == NULL ){
    perror("Error al abrir datos de salida speedup");
    exit(EXIT_FAILURE);
  }

  if( (dataBar=fopen("data_bar.dat", "w")) == NULL ){
    perror("Error al abrir datos de salida bar");
    exit(EXIT_FAILURE);
  }

  float auxCpu, auxGpu;
  int nEle;

  for(int i = 1; i < 9; ++i){
    fscanf(dCpu, "%d", &nEle);
    fscanf(dGpu, "%d", &nEle);
    fscanf(dCpu, "%f", &auxCpu);
    fscanf(dGpu, "%f", &auxGpu);

    fprintf(dataBar, "%f %d %f\n%f %d %f\n", 2*i-0.25, nEle, auxCpu,
    2*i+0.25, nEle, auxGpu);
    fprintf(dataSpeed, "%d %f\n", nEle, auxCpu/auxGpu);
  }

  fclose(dCpu);
  fclose(dGpu);
  fclose(dataBar);
  fclose(dataSpeed);

  exit(EXIT_SUCCESS);
}
