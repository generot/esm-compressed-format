#ifndef __COMPRESS_H__
#define __COMPRESS_H__

#include "commons.h"

#define BASE_CMPR_FACTOR 100
#define BRANCHES_N 4

struct quadtree_t {
    pixel_t avg;
    pixel64_t region;

    quadtree_t *branches[BRANCHES_N];
};

struct quadtree_node_t {
    pixel_t avg;
    pixel64_t region;
};

quadtree_t *compress_img(image_t img_data, int x, int y, int width, int height);
quadtree_t *compress_img2(image_t img_data, int x, int y, int width, int height, pixel_t threshold);

void decompress_img(std::vector<pixel_t> qtree, size_t ix, int x, int y, int width, int height, pixel_t *out);
std::vector<pixel_t> decompress_img2(quadtree_node_t *regions, size_t sz, int width, int height);

#include "util.h"

#endif //__COMPRESS_H__