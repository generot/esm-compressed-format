#ifndef __COMMONS_H__
#define __COMMONS_H__

#include <iostream>
#include <vector>
#include <string>

#define MAX_X 6000
#define MAX_Y MAX_X

using pixel_t = unsigned int;
using byte_t = unsigned char;

struct image_t {
    int width;
    int height;

    std::vector<pixel_t> pixel_data;
};

struct RGB {
    byte_t r;
    byte_t g;
    byte_t b;
};

#endif //__COMMONS_H__