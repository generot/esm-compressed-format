#include "../include/util.h"

using namespace std;

template <typename T>
vector<T> filter(vector<T> cont, function<bool(T)> callback) {
    vector<T> filtered;

    for(T elem : cont) {
        if(callback(elem)) {
            filtered.push_back(elem);
        }
    }

    return filtered;
}

pixel64_t combine_into_64(short w, short x, short y, short z) {
    return (pixel64_t)(((pixel64_t)w << 48) | ((pixel64_t)x << 32) | ((pixel64_t)y << 16) | (pixel64_t)z);
}

quadrant_t partition_64(pixel64_t p) {
    quadrant_t disasm = {0};

    disasm.x      = (p >> 48) & 0xffff;
    disasm.y      = (p >> 32) & 0xffff;
    disasm.width  = (p >> 16) & 0xffff;
    disasm.height = (p >> 0)  & 0xffff;

    return disasm;
}

void traverse_tree(quadtree_t *root, int spacing) {
    if(!root) {
        cout << "END" << endl;
        return;
    }

    for(int i = 0; i < spacing; i++) {
        cout << " ";
    }

    cout << "Avg(" << root->avg << ")";

    for(int i = 0; i < spacing; i++) {
        cout << " ";
    }

    cout << endl;

    for(int ix = 0; ix < BRANCHES_N; ix++) {
        traverse_tree(root->branches[ix], spacing / 4);
    }
}

void print_rgb(RGB px) {
    cout << "(" << (int)px.r << ", " << (int)px.g << ", " << (int)px.b << ")" << endl;
}

vector<quadtree_node_t> breadth_first_traversal(quadtree_t *root) {
    vector<quadtree_node_t> nodes;
    queue<quadtree_t*> q;

    q.push(root);

    while(!q.empty()) {
        quadtree_t *curr = q.front();
        q.pop();

        if(curr->avg) {
            nodes.push_back({ curr->avg, curr->region });
        }

        for(int i = 0; i < BRANCHES_N; i++) {
            if(curr->branches[i]) {
                q.push(curr->branches[i]);
            }
        }
    }

    return nodes;
}