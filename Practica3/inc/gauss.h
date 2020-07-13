#ifndef __GAUSS_H__
#define __GAUSS_H__

#include <stdio.h>
#include "libbmp.h"

#define PI 3.141592653589793238462643

typedef struct _gauss_kernel{
    int k;
    double sigma;
    unsigned tam;
    double **kernel;
} gauss_kernel;

unsigned gauss_set_kernel   (gauss_kernel *kernel, int k, int sigma);
void     gauss_free_kernel  (gauss_kernel *kernel, unsigned rows);
void     gauss_print_kernel (gauss_kernel *kernel);
void     gauss_apply_kernel (gauss_kernel *kernel,
                             bmp_pixel **in,
                             bmp_pixel **out,
                             unsigned heigth,
                             unsigned width);

#endif