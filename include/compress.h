#ifndef __COMPRESS_H__
#define __COMPRESS_H__

#include "commons.h"

#define BASE_CMPR_FACTOR 100
#define BRANCHES_N 4

struct quadtree_t {
    pixel_t mode;
    pixel_t avg;

    RGB64 sum;

    quadtree_t *branches[BRANCHES_N];
};

quadtree_t *compress_img(image_t img_data, int x, int y, int width, int height);

#endif //__COMPRESS_H__