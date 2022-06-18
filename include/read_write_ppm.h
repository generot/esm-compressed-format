#ifndef __READ_WRITE_PPM_H__
#define __READ_WRITE_PPM_H__

#include "commons.h"

#include <cstdio>
#include <sstream>

#define MODE_STRLEN 3
#define PX_DATA_STRLEN 20

image_t read_ppm(std::string filepath);
pixel_t pixel_from_rgb(byte_t r, byte_t g, byte_t b);
RGB rgb_from_pixel(pixel_t px);

#endif //__READ_WRITE_PPM_H__