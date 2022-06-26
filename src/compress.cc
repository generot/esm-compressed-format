#include "../include/read_write_ppm.h"
#include "../include/compress.h"

using namespace std;

RGB rgb_abs(RGB px) {
    return RGB{ (byte_t)abs(px.r), (byte_t)abs(px.g), (byte_t)abs(px.b) };
}

RGB average_for_region(pixel_t *data, int x, int y, int width, int height, int org_w) {
    RGB64 sum = {0};

    for(int i = y; i < y + height; i++) {
        for(int j = x; j < x + width; j++) {
            sum = sum + rgb_from_pixel(data[j + i * org_w]);
        }
    }

    RGB64 avg = sum / (pixel64_t)(width * height);

    return avg.from_64_to_8();
}

pixel_t mean_squared_error(pixel_t *data, int x, int y, int width, int height, int org_w, RGB forecast) {
    pixel_t mean = 0;

    for(int i = y; i < y + height; i++) {
        for(int j = x; j < x + width; j++) {
            RGB curr_px = rgb_from_pixel(data[j + i * org_w]);

            pixel_t r_sq = (forecast.r - curr_px.r) * (forecast.r - curr_px.r);
            pixel_t g_sq = (forecast.g - curr_px.g) * (forecast.g - curr_px.g);
            pixel_t b_sq = (forecast.b - curr_px.b) * (forecast.b - curr_px.b);

            mean = mean + r_sq + g_sq + b_sq;
        }
    }

    return mean / (3 * width * height);
}

vector<pixel_t> decompress_img2(quadtree_node_t *regions, size_t sz, int width, int height) {
    pixel_t *arr = new pixel_t[width * height]{0};

    for(size_t i = 0; i < sz; i++) {
        quadtree_node_t curr_region = regions[i];
        quadrant_t coords = partition_64(curr_region.region);

        // cout << "------ITER------" << endl;
        // printf("(%hu, %hu, %hu, %hu)\n", coords.x, coords.y, coords.width, coords.height);

        //TVA TRJABVA DA E RELATIVNO NA CJALATA MATRICA, NE SAMO NA EDIN REGION!!!!

        for(short i = coords.y; i < coords.y + coords.height; i++) {
            for(short j = coords.x; j < coords.x + coords.width; j++) {
                int ix = (int)j + (int)i * width;

                arr[ix] = curr_region.avg;
                //printf("(%hu, %hu) -> %hu\n", j, i, ix);

            }    
        }
    }

    vector<pixel_t> managed_mem = vector<pixel_t>(arr, arr + width * height);
    delete[] arr;

    return managed_mem;
}

//TUKA TRJABVA DA PAZIM I ORIGINALNITE RAZMERI!!!
quadtree_t *compress_img2(image_t img_data, int x, int y, int width, int height, int threshold) {
    int half_w = width / 2;
    int half_h = height / 2;

    quadtree_t *new_node = new quadtree_t{0};
    // RGB curr_region = parent_region_avg;
    RGB curr_region = average_for_region(img_data.pixel_data, x, y, width, height, img_data.width);
    pixel_t mean_sq_err = mean_squared_error(img_data.pixel_data, x, y, width, height, img_data.width, curr_region);

    new_node->avg = pixel_from_rgb(curr_region);
    new_node->region = combine_into_64(x, y, width, height);
    
    if(mean_sq_err > threshold) {
        //cout << "Region is not divisible" << endl;
        new_node->branches[0] = compress_img2(img_data, x, y, half_w, half_h, threshold);
        new_node->branches[1] = compress_img2(img_data, x + half_w, y, half_w, half_h, threshold);
        new_node->branches[2] = compress_img2(img_data, x, y + half_h, half_w, half_h, threshold);
        new_node->branches[3] = compress_img2(img_data, x + half_w, y + half_h, half_w, half_h, threshold);
    } else {
        new_node->avg = 0;
    }

    return new_node;
}