#ifndef __MATRIX_HELPER_H__
#define __MATRIX_HELPER_H__

typedef struct _mh_transform{
    int block_n;
    int block_width_size;
    //int block_width_module;
    int block_height_size;
    //int block_height_module;
    int height_size;
    int width_size;
    int k;
    int transform_height_size;
    int transform_width_size;
} mh_transform;

mh_transform    mh_init_transform   (unsigned block_n,
                                     unsigned height_s, 
                                     unsigned width_s,
                                     unsigned k);

unsigned        mh_get_i            (mh_transform transform,
                                     unsigned transform_i);

unsigned        mh_get_j            (mh_transform transform, 
                                     unsigned transform_j);

void            mh_print_transform  (mh_transform transform);

#endif