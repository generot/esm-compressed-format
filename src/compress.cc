#include "../include/read_write_ppm.h"
#include "../include/compress.h"

template <typename A>
size_t get_max(A *arr, size_t size) {
    size_t max_ix = 0;

    for(size_t i = 0; i < size; i++) {
        if(arr[i] > arr[max_ix]) {
            max_ix = i;
        }
    }

    return max_ix;
}

RGB get_mode(quadtree_t **arr) {
    pixel_t lut[BRANCHES_N];
    int n = BRANCHES_N;

    for(int i = 0; i < n; i++) {
        lut[arr[i]->avg]++;
    }

    size_t max_ix = get_max<pixel_t>(lut, BRANCHES_N);

    return rgb_from_pixel(lut[max_ix]);
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