#include "../include/compress.h"
#include "../include/read_write_ppm.h"
#include "../include/util.h"

#include <cstdlib>

using namespace std;

esm_t read_esm_file(string filepath) {
    FILE *f = fopen(filepath.c_str(), "rb");

    pixel64_t dims = 0;
    size_t nodes_n = 0;

    fread(&dims, sizeof(pixel64_t), 1, f);
    fread(&nodes_n, sizeof(size_t), 1, f);

    quadtree_node_t *img_data = new quadtree_node_t[nodes_n]{{0}};

    pixel64_t width = HIGH32_64(dims);
    pixel64_t height = LOW32_64(dims);

    fread(img_data, sizeof(quadtree_node_t), nodes_n, f);

    vector<pixel_t> img_pixel_data = decompress_img2(img_data, nodes_n, (int)width, (int)height);

    return esm_t{ img_pixel_data, (int)width, (int)height };
}

void write_esm_file(quadtree_t *compressed_img, pixel64_t width, pixel64_t height, string filepath) {
    FILE *f = fopen(filepath.c_str(), "wb+");

    pixel64_t dims = (width << 32) | height;
    fwrite(&dims, sizeof(pixel64_t), 1, f);

    vector<quadtree_node_t> nodes = breadth_first_traversal(compressed_img);
    size_t nodes_n = nodes.size();
    
    fwrite(&nodes_n, sizeof(size_t), 1, f);

    for(quadtree_node_t i : nodes) {
        fwrite(&i, sizeof(quadtree_node_t), 1, f);
    }

    fclose(f);
}

string change_ext(string path, string new_ext, string tail = "") {
    size_t ix = path.find('.');
    string new_path = path.substr(0, ix) + tail + "." + new_ext;

    return new_path;
}

int main(int argc, char **argv) {
    if(argc < 3) {
        cerr << "Usage: ./esm <mode> <image_to_compress.ppm>" << endl;
        return -1;
    }

    string mode = string(argv[1]);
    string filepath = string(argv[2]);

    if(mode == "-c") {
        image_t img = read_ppm(filepath);

        int compression_threshold = atoi(argv[3]);

        if(img.width >= MAX_X || img.height >= MAX_Y) {
            cerr << "Input image exceeds dimensional limit (6000 x 6000)." << endl;
            return -1;
        }

        cout << "Image dimensions: " << img.width << " x " << img.height << endl;

        string outpath = change_ext(filepath, "esm");
        quadtree_t *compressed_img = compress_img2(img, 0, 0, img.width, img.height, compression_threshold);

        cout << "Done..." << endl;

        write_esm_file(compressed_img, img.width, img.height, outpath);
    } else if(mode == "-uc") {
        esm_t img = read_esm_file(filepath);
        string outpath = change_ext(filepath, "ppm", "-esmout");

        write_ppm(outpath, img.px_data, img.width, img.height, 255);
    }

    return 0;
}