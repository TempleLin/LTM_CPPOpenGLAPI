#include "headers/image_edit.hpp"

#include <stb_image.h>

unsigned char* loadImage(std::string texturePath, int& width, int& height, int& nrChannels) {
    stbi_set_flip_vertically_on_load(true);
    return stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
}

void freeImage(unsigned char& data) {
    stbi_image_free(&data);
}