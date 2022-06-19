#include "../include/treeops.h"

using namespace std;

void traverse_tree(quadtree_t *root) {
    if(!root) {
        cout << "END" << endl;
        return;
    }

    cout << "Average: " << root->avg << endl;

    for(int ix = 0; ix < BRANCHES_N; ix++) {
        traverse_tree(root->branches[ix]);
    }
}

void breadth_first_traversal(quadtree_t *root, bft_callback clb) {
    queue<quadtree_t*> q;

    q.push(root);

    while(!q.empty()) {
        quadtree_t *curr = q.front();
        q.pop();

        clb(curr);

        for(int i = 0; i < BRANCHES_N; i++) {
            if(curr->branches[i]) {
                q.push(curr->branches[i]);
            }
        }
    }
}