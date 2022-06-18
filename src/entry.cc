#include "../include/allocator.h"
#include "../include/read_write_ppm.h"

int main(int argc, char **argv) {
    if(argc < 2) {
        return -1;
    }

    image_t img = read_ppm(std::string(argv[1]));

    for(int i = 0; i < img.height; i++) {
        for(int j = 0; j < img.width; j++) {
            RGB px = rgb_from_pixel(img.pixel_data[j + i * img.width]);

            std::cout << (int)px.r << " " << (int)px.g << " " << (int)px.b << std::endl;
        }
    }

    return 0;
}