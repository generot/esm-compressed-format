#ifndef __UTIL_H__
#define __UTIL_H__

#include "compress.h"
#include <queue>
#include <functional>

struct quadrant_t {
    short x, y;
    short width, height;
};

template <typename T>
std::vector<T> filter(std::vector<T> cont, std::function<bool(T)> callback);

void traverse_tree(quadtree_t *root, int spacing);
std::vector<quadtree_node_t> breadth_first_traversal(quadtree_t *root);

void print_rgb(RGB px);

pixel64_t combine_into_64(short w, short x, short y, short z);
quadrant_t partition_64(pixel64_t p);

#endif //__UTIL_H__