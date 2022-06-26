#ifndef __COMMONS_H__
#define __COMMONS_H__

#include <iostream>
#include <vector>
#include <string>

#define MAX_X 6000
#define MAX_Y MAX_X

#define HIGH32_64(x) ((x) >> 32)
#define LOW32_64(x) ((x) & 0xffffffff)

using pixel_t = unsigned int;
using pixel64_t = unsigned long long int;
using byte_t = unsigned char;

struct esm_t {
    std::vector<pixel_t> px_data;

    int width;
    int height;
};

struct image_t {
    int width;
    int height;

    pixel_t *pixel_data;

    pixel_t index_2d(int x, int y) {
        return pixel_data[x + y * width];
    }
};

struct RGB {
    byte_t r, g, b;

    RGB operator-(const RGB& other) {
        return RGB{ (byte_t)(r - other.r), (byte_t)(g - other.g), (byte_t)(b - other.b) };
    }

    RGB operator+(const RGB& other) {
        return RGB{ (byte_t)(r + other.r), (byte_t)(g + other.g), (byte_t)(b + other.b) };
    }

    RGB operator*(const RGB& other) {
        return RGB{ (byte_t)(r * other.r), (byte_t)(g * other.g), (byte_t)(b * other.b) };
    }

    RGB operator/(byte_t div) {
        return RGB{ (byte_t)(r / div), (byte_t)(g / div), (byte_t)(b / div) };
    }

    bool operator<(int rhs) {
        return r < rhs && g < rhs && b < rhs;
    }

    bool operator>(int rhs) {
        return r > rhs && g > rhs && b > rhs;
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