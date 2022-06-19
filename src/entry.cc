#include "../include/compress.h"
#include "../include/allocator.h"
#include "../include/read_write_ppm.h"
#include "../include/treeops.h"

using namespace std;

void write_esm_file(quadtree_t *compressed_img, pixel64_t width, pixel64_t height, string filepath) {
    FILE *f = fopen(filepath.c_str(), "wb+");

    pixel64_t dims = (width << 32) | height;
    fwrite(&dims, sizeof(pixel64_t), 1, f);

    bft_callback func = [=](quadtree_t *node) -> void {
        fwrite(&node->avg, sizeof(pixel_t), 1, f);
    };

    breadth_first_traversal(compressed_img, func);

    fclose(f);
}

string change_ext(string path, string new_ext) {
    size_t ix = path.find('.');
    string new_path = path.substr(0, ix) + "." + new_ext;

    return new_path;
}

int main(int argc, char **argv) {
    if(argc < 2) {
        cerr << "Usage: ./esm <image_to_compress.ppm>" << endl;
        return -1;
    }

    string filepath = string(argv[1]);
    image_t img = read_ppm(filepath);

    if(img.width >= MAX_X || img.height >= MAX_Y) {
        cerr << "Input image exceeds dimensional limit (6000 x 6000)." << endl;
        return -1;
    }

    cout << "Image size: " << img.width << " x " << img.height << endl;

    string outpath = change_ext(filepath, "esm");
    quadtree_t *compressed_img = compress_img(img, 0, 0, img.width, img.height);

    write_esm_file(compressed_img, img.width, img.height, outpath);

    return 0;
}