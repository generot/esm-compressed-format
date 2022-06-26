/**
 * region_data_t prec_img_inner_proc(baked_image_t& out, image_t img_data, int x, int y, int width, int height) {
    region_data_t curr_region = {0};
    pixel64_t key = combine_into_64(x, y, width, height);

    if(width <= 1 && height <= 1) {
        pixel_t px = img_data.index_2d(x, y);

        //cout << "(X: " << x << "; Y: " << y  << ";)"  << " -> " << px << endl;

        curr_region.sum = curr_region.sum + rgb_from_pixel(px);
        curr_region.avg = rgb_from_pixel(px);

        out[key] = curr_region;

        return curr_region;
    }

    int half_w = width / 2;
    int half_h = height / 2;

    region_data_t top_left = prec_img_inner_proc(out, img_data, x, y, half_w, half_h);
    region_data_t top_right = prec_img_inner_proc(out, img_data, x + half_w, y, half_w, half_h);
    region_data_t bottom_left = prec_img_inner_proc(out, img_data, x, y + half_h, half_w, half_h);
    region_data_t bottom_right = prec_img_inner_proc(out, img_data, x + half_w, y + half_h, half_w, half_h);

    curr_region.sum = top_left.sum + top_right.sum + bottom_left.sum + bottom_right.sum;
    curr_region.avg = curr_region.sum.from_64_to_8() / 4;

    out[key] = curr_region;

    return curr_region;
}

baked_image_t precompute_img(image_t img_data, int x, int y, int width, int height) {
    baked_image_t data;

    prec_img_inner_proc(data, img_data, x, y, width, height);

    return data;
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

    if(width <= 1 && height <= 1) {
        pixel_t px = img_data.index_2d(x, y);

        cout << "(X: " << x << "; Y: " << y  << ";)"  << " -> " << px << endl;

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
        //cout << "In compression..." << endl;
        root->avg = pixel_from_rgb(mode);

        for(int i = 0; i < BRANCHES_N; i++) {
            delete root->branches[i];

            root->branches[i] = NULL;
        }
    }

    return root;
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

void decompress_img(vector<pixel_t> qtree, size_t ix, int x, int y, int width, int height, pixel_t *out) {
    int half_w = width / 2;
    int half_h = height / 2;

    if(ix >= qtree.size()) {
        return;
    }

    if(qtree[ix]) {
        cout << "(X: " << x << ", Y: " << y << ")" << "->" << width << " x " << height << " -> ";
        print_rgb(rgb_from_pixel(qtree[ix]));

        for(int i = y; i < y + height; i++) {
            for(int j = x; j < x + width; j++) {
                out[j + i * width] = qtree[ix];
            }
        }
    }

    decompress_img(qtree, 4 * ix + 1, x, y, half_w, half_h, out);
    decompress_img(qtree, 4 * ix + 2, x + half_w, y, half_w, half_h, out);
    decompress_img(qtree, 4 * ix + 3, x, y + half_h, half_w, half_h, out);
    decompress_img(qtree, 4 * ix + 4, x + half_w, y + half_h, half_w, half_h, out);
}

quadtree_t *compress_img2(image_t img_data, int x, int y, int width, int height, int threshold) {
    int half_w = width / 2;
    int half_h = height / 2;

    quadtree_t *new_node = new quadtree_t{0};
    // RGB curr_region = parent_region_avg;
    RGB curr_region = average_for_region(img_data.pixel_data, x, y, width, height);
    pixel_t mean_sq_err = mean_squared_error(img_data.pixel_data, x, y, width, height, curr_region);

    // if(!parent_region_avg.r) {
    //     curr_region = average_for_region(img_data.pixel_data, x, y, width, height);
    // }

    // if(width <= 1 && height <= 1) {
    //     new_node->avg = img_data.pixel_data[x + y * (x + width)];
    //     new_node->region = combine_into_64(x, y, width, height);

    //     return new_node;
    // }

    //cout << "(X: " << x << ", Y: " << y << ")" << "->" << width << " x " << height << endl;
    // print_rgb(curr_region);

    // RGB top_left = average_for_region(img_data.pixel_data, x, y, half_w, half_h);
    // RGB top_right = average_for_region(img_data.pixel_data, x + half_w, y, half_w, half_h);
    // RGB bottom_left = average_for_region(img_data.pixel_data, x, y + half_h, half_w, half_h);
    // RGB bottom_right = average_for_region(img_data.pixel_data, x + half_w, y + half_h, half_w, half_h);

    // RGB subregions[] = {top_left, top_right, bottom_left, bottom_right};
    // bool is_divisible = is_region_divisible(subregions);

    if(mean_sq_err < threshold) {
        //cout << "Region is not divisible" << endl;
        new_node->avg = pixel_from_rgb(curr_region);
        new_node->region = combine_into_64(x, y, width, height);
    } else {
        new_node->branches[0] = compress_img2(img_data, x, y, half_w, half_h, threshold);
        new_node->branches[1] = compress_img2(img_data, x + half_w, y, half_w, half_h, threshold);
        new_node->branches[2] = compress_img2(img_data, x, y + half_h, half_w, half_h, threshold);
        new_node->branches[3] = compress_img2(img_data, x + half_w, y + half_h, half_w, half_h, threshold);
    }

    return new_node;
}

*/