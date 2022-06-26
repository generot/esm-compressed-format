#ifndef __READ_WRITE_PPM_H__
#define __READ_WRITE_PPM_H__

#include "commons.h"

#include <cstdio>
#include <sstream>

#define MODE_STRLEN 3
#define PX_DATA_STRLEN 20

image_t read_ppm(std::string filepath);
void write_ppm(std::string filepath, std::vector<pixel_t> pixel_data, int width, int height, int max_px);

pixel_t pixel_from_rgb(byte_t r, byte_t g, byte_t b);
pixel_t pixel_from_rgb(RGB rgb);

RGB rgb_from_pixel(pixel_t px);

#endif //__READ_WRITE_PPM_H__