#include "matrix_helper.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

mh_transform mh_init_transform(unsigned block_n,
    unsigned height_s, unsigned width_s, unsigned k
){
    mh_transform transform;
    transform.block_n = block_n;
    transform.height_size = height_s;
    transform.width_size = width_s;
    transform.block_width_size = ceil(width_s / block_n);
    transform.block_height_size = ceil(height_s / block_n);
    transform.transform_height_size = transform.block_height_size * block_n + 2 * k * block_n;
    transform.transform_width_size = transform.block_width_size * block_n + 2 * k * block_n;
    transform.k = k;

    return transform;
}

unsigned mh_get_i(mh_transform transform, unsigned transform_i){
    unsigned block_index = transform_i / transform.block_height_size * (transform.block_height_size + 2 * transform.k);
    if (transform_i < block_index + transform.k
        || transform_i >= block_index + transform.block_height_size + transform.k
    ){
        return NULL;
    }else{
        unsigned i = transform_i - (block_index * 2 * transform.k) + transform.k;
        return (i < transform.height_size) ? i : NULL;
    }
}

unsigned mh_get_j(mh_transform transform, unsigned transform_j){
    unsigned block_index = transform_j / transform.block_width_size * (transform.block_width_size + 2 * transform.k);
    if (transform_j < block_index + transform.k
        || transform_j >= block_index + transform.block_width_size + transform.k
    ){
        return NULL;
    }else{
        unsigned j = transform_j - (block_index * 2 * transform.k) + transform.k;
        return (j < transform.width_size) ? j : NULL;
    }
}

void mh_print_transform(mh_transform transform){
    printf("Blocks_per_row: %d\n", transform.block_n);
    printf("Blocks_height_size: %d\n", transform.block_height_size);
    printf("Blocks_width_size: %d\n", transform.block_width_size);
    printf("K: %d\n", transform.k);
    printf("Height_size: %d\n", transform.height_size);
    printf("Width_size: %d\n", transform.width_size);
    printf("Transform_height_size: %d\n", transform.transform_height_size);
    printf("Transform_width_size: %d\n", transform.transform_width_size);
}
