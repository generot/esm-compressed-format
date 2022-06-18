#include "../include/compress.h"
#include "../include/allocator.h"
#include "../include/read_write_ppm.h"

using namespace std;

void traverse_tree(quadtree_t *root) {
    if(!root) return;

    cout << "Average: " << root->avg << endl;

    for(int ix = 0; ix < BRANCHES_N; ix++) {
        traverse_tree(root->branches[ix]);
    }
}

int main(int argc, char **argv) {
    if(argc < 2) {
        return -1;
    }

    image_t img = read_ppm(string(argv[1]));
    quadtree_t *compressed_img = compress_img(img, 0, 0, img.width, img.height);

    traverse_tree(compressed_img);

    // for(int i = 0; i < img.height; i++) {
    //     for(int j = 0; j < img.width; j++) {
    //         RGB px = rgb_from_pixel(img.pixel_data[j + i * img.width]);

    //         std::cout << (int)px.r << " " << (int)px.g << " " << (int)px.b << std::endl;
    //     }
    // }

    return 0;
}