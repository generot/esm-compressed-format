#include "../include/read_write_ppm.h"

using namespace std;

pixel_t pixel_from_rgb(byte_t r, byte_t g, byte_t b) {
    pixel_t r_field = (pixel_t)r;
    pixel_t g_field = (pixel_t)g;
    pixel_t b_field = (pixel_t)b;

    return (r_field << 16) | (g_field << 8) | b_field;
}

pixel_t pixel_from_rgb(RGB rgb) {
    pixel_t r_field = (pixel_t)rgb.r;
    pixel_t g_field = (pixel_t)rgb.g;
    pixel_t b_field = (pixel_t)rgb.b;

    return (r_field << 16) | (g_field << 8) | b_field;
}

RGB rgb_from_pixel(pixel_t px) {
    pixel_t r_field = (px >> 16) & 0xff;
    pixel_t g_field = (px >> 8)  & 0xff;
    pixel_t b_field = (px >> 0)  & 0xff;

    return RGB{ (byte_t)r_field, (byte_t)g_field, (byte_t)b_field };
}

image_t read_ppm(string filepath) {
    FILE *file = fopen(filepath.c_str(), "rb");
    vector<pixel_t> pixel_data;

    int r, g, b;

    char mode[MODE_STRLEN] = {0};

    int width = 0; 
    int height = 0;
    int max_pxval = 0;

    fscanf(file, "%s\n%d %d\n%d\n", mode, &width, &height, &max_pxval);

    string modestr(mode);

    if(modestr == "P3") {
        char buff[PX_DATA_STRLEN] = {0};

        while(fgets(buff, PX_DATA_STRLEN - 1, file)) {
            stringstream rgb(buff);
            cout << rgb.str() << endl;

            rgb >> r >> g >> b;

            pixel_data.push_back(pixel_from_rgb(r, g, b));
        }
    } else if(modestr == "P6") {
        size_t sz = width * height * 3;
        byte_t *data = new byte_t[sz]{0};

        fread(data, sz, sizeof(byte_t), file);

        for(size_t i = 0; i < sz - 3; i += 3) {
            r = data[i + 0];
            g = data[i + 1];
            b = data[i + 2];

            pixel_data.push_back(pixel_from_rgb(r, g, b));
        }
    }

    fclose(file);

    return image_t{ width, height, pixel_data };
}