#include "../include/read_write_ppm.h"
#include "../include/compress.h"

#include <unordered_map>

using namespace std;

template <typename T>
T get_max(unordered_map<T, int> lut) {
    int max = 0;
    T max_val = T();

    for(pair<T, int> i : lut) {
        if(i.second > max) {
            max_val = i.first;
        }
    }

    return max_val;
}

RGB get_mode(quadtree_t **arr) {
    unordered_map<pixel_t, int> lut;

    int n = BRANCHES_N;

    for(int i = 0; i < n; i++) {
        lut[arr[i]->avg]++;
    }

    pixel_t most_common_px = get_max<pixel_t>(lut);

    return rgb_from_pixel(most_common_px);
}

RGB get_average(quadtree_t **arr) {
    int n = BRANCHES_N;

    int avg_r = 0;
    int avg_b = 0;
    int avg_g = 0;

    for(int i = 0; i < n; i++) {
        RGB px = rgb_from_pixel(arr[i]->avg);

        avg_r += px.r;
        avg_g += px.g;
        avg_b += px.b;
    }

    return RGB{ (byte_t)(avg_r / n), (byte_t)(avg_g / n), (byte_t)(avg_b / n) };
}

quadtree_t *compress_img(image_t img_data, int x, int y, int width, int height) {
    quadtree_t *root = new quadtree_t{0};

    if(width == 1 && height == 1) {
        pixel_t px = img_data.index_2d(x, y);

        root->mode = px;
        root->avg = px;

        return root;
    }

    root->branches[0] = compress_img(img_data, x, y, width / 2, height / 2);
    root->branches[1] = compress_img(img_data, x + width / 2, y, width / 2, height / 2);
    root->branches[2] = compress_img(img_data, x, y + height / 2, width / 2, height / 2);
    root->branches[3] = compress_img(img_data, x + width / 2, y + height / 2, width / 2, height / 2);

    RGB avg = get_average(root->branches);
    RGB mode = get_mode(root->branches);

    if(mode - avg < BASE_CMPR_FACTOR) {
        root->avg = pixel_from_rgb(mode);

        for(int i = 0; i < BRANCHES_N; i++) {
            delete root->branches[i];

            root->branches[i] = NULL;
        }
    }

    return root;
}