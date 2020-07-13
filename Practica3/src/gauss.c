#include "gauss.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

unsigned gauss_set_kernel (gauss_kernel *kernel, int k, int sigma){
    kernel->tam = 2 * k + 1;
    kernel->k = k;
    kernel->sigma = sigma;
    if ((kernel->kernel = malloc(kernel->tam * sizeof(double*))) == NULL){
        return 1;
    }else{
        for (unsigned i = 0; i < kernel->tam; i++){
            if ((kernel->kernel[i] = malloc(kernel->tam * sizeof(double))) == NULL){
                gauss_free_kernel(kernel, i);
                return 1;
            }
        }
    }

   

    double distance = 0;
    double euler = 1 / (2. * PI * sigma * sigma);
    double suma_total = 0;

    for (int i = -k; i <= k; i++){
        for (int j = -k; j <= k; j++){
            distance = ((i*i) + (j*j)) / 2.;
            kernel->kernel[i+k][j+k] = euler*exp(-distance);
            suma_total += kernel->kernel[i+k][j+k];
        }
    }

    for (unsigned i = 0; i < kernel->tam; i++)
        for (unsigned j = 0; j < kernel->tam; j++)
            kernel->kernel[i][j] *= (1. / suma_total);

    return kernel->tam;
}

void gauss_free_kernel (gauss_kernel *kernel, unsigned rows){
    if (rows == 0) rows = kernel->tam;
    if (kernel->kernel != NULL){
        for (unsigned i = 0; i < rows; i++)
            if (kernel->kernel[i] != NULL)
                free(kernel->kernel[i]);
        
        free(kernel->kernel);
    }    
}

void gauss_print_kernel (gauss_kernel *kernel){
    for (unsigned i = 0; i < kernel->tam; i++){
        for (unsigned j = 0; j < kernel->tam; j++)
            printf("[%f]", kernel->kernel[i][j]);

        printf("\n");
    }
}

void gauss_apply_kernel (gauss_kernel *kernel, bmp_pixel **in, bmp_pixel **out, unsigned height, unsigned width){
    unsigned pixel_color[3];
    for (unsigned i = 0; i < height; i++){
        for (unsigned j = 0; j < width; j++){
            if (i >= kernel->k && i < height - kernel->k && j < width - kernel->k && j >= kernel->k){
                pixel_color[0] = 0;
                pixel_color[1] = 0;
                pixel_color[2] = 0;
                for (int l = -kernel->k; l <= kernel->k; l++){
                    for (int m = -kernel->k; m <= kernel->k; m++){
                        pixel_color[0] += in[i+l][j+m].red * kernel->kernel[l+kernel->k][m+kernel->k];
                        pixel_color[1] += in[i+l][j+m].green * kernel->kernel[l+kernel->k][m+kernel->k];
                        pixel_color[2] += in[i+l][j+m].blue * kernel->kernel[l+kernel->k][m+kernel->k];
                    }
                }
                
            }else{
                pixel_color[0] = in[i][j].red;
                pixel_color[1] = in[i][j].green;
                pixel_color[2] = in[i][j].blue;
            }
            
            bmp_pixel_init(
                &out[i][j],
                pixel_color[0],
                pixel_color[1],
                pixel_color[2]);

        }
    }
}
