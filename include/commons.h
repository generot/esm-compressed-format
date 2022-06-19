#ifndef __COMMONS_H__
#define __COMMONS_H__

#include <iostream>
#include <vector>
#include <string>

#define MAX_X 6000
#define MAX_Y MAX_X

using pixel_t = unsigned int;
using pixel64_t = unsigned long long int;
using byte_t = unsigned char;

struct image_t {
    int width;
    int height;

    std::vector<pixel_t> pixel_data;

    pixel_t index_2d(int x, int y) {
        return pixel_data[x + y * width];
    }
};

struct RGB {
    byte_t r, g, b;

    RGB operator-(const RGB& other) {
        return RGB{ (byte_t)(r - other.r), (byte_t)(g - other.g), (byte_t)(b - other.b) };
    }

    bool operator<(int rhs) {
        return r < rhs && g < rhs && b < rhs;
    }
};

struct RGB64 {
    pixel64_t r, g, b;

    RGB64 operator+(const RGB64& other) {
        return RGB64{ r + other.r, g + other.g, b + other.b };
    }

    RGB64 operator+(const RGB& other) {
        return RGB64{ r + (pixel64_t)other.r, g + (pixel64_t)other.g, b + (pixel64_t)other.b };
    }

    RGB64 operator/(pixel64_t div) {
        return RGB64{ r / div, g / div, b / div };
    }

    RGB from_64_to_8() {
        return RGB{ (byte_t)r, (byte_t)g, (byte_t)b };
    }
};

#endif //__COMMONS_H__