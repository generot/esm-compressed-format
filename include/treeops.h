#ifndef __TREEOPS_H__
#define __TREEOPS_H__

#include "compress.h"
#include <queue>
#include <functional>

using bft_callback = std::function<void(quadtree_t*)>;

void traverse_tree(quadtree_t *root);
void breadth_first_traversal(quadtree_t *root, bft_callback clb);

#endif //__TREEOPS_H__