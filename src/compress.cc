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

//DEPRECATED
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
//DEPRECATED

quadtree_t *compress_img(image_t img_data, int x, int y, int width, int height) {
    quadtree_t *root = new quadtree_t{0};

    if(width <= 1 && height <= 1) {
        pixel_t px = img_data.index_2d(x, y);

        //cout << "(X: " << x << "; Y: " << y  << ";)"  << " -> " << px << endl;

        root->mode = px;
        root->avg = px;

        root->sum = root->sum + rgb_from_pixel(px);

        return root;
    }

    int half_w = width / 2;
    int half_h = height / 2;

    root->branches[0] = compress_img(img_data, x, y, half_w, half_h);
    root->branches[1] = compress_img(img_data, x + half_w, y, half_w, half_h);
    root->branches[2] = compress_img(img_data, x, y + half_h, half_w, half_h);
    root->branches[3] = compress_img(img_data, x + half_w, y + half_h, half_w, half_h);

    for(int i = 0; i < BRANCHES_N; i++) {
        root->sum = root->sum + root->branches[i]->sum;
    }

    RGB64 avg = root->sum / (pixel64_t)(width * height);

    RGB avg8 = avg.from_64_to_8();
    RGB mode = get_mode(root->branches);

    // cout << "Average: " << (int)avg8.r << " " << (int)avg8.g << " " << (int)avg8.b << endl;
    // cout << "Mode: " << (int)mode.r << " " << (int)mode.g << " " << (int)mode.b << endl;

    if(mode - avg8 < BASE_CMPR_FACTOR) {
        cout << "In compression..." << endl;
        root->avg = pixel_from_rgb(mode);

        for(int i = 0; i < BRANCHES_N; i++) {
            delete root->branches[i];

            root->branches[i] = NULL;
        }
    }

    return root;
}