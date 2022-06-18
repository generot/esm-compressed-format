#ifndef __COMPRESS_H__
#define __COMPRESS_H__

#include "commons.h"

#define BASE_CMPR_FACTOR 10
#define BRANCHES_N 4

struct quadtree_t {
    pixel_t mode;
    pixel_t avg;

    quadtree_t *branches[BRANCHES_N];
};

#endif //__COMPRESS_H__