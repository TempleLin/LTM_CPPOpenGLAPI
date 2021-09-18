#include "headers/assets_edit.hpp"
#include <LTM_CPPOpenGLAPIConfig.h>

#include <stb_image.h>

bool AssetsRelativePath::modeSet{ false };
std::string AssetsRelativePath::path{ "" };
std::string& AssetsRelativePath::getPath() {
    return path;
}

void setAppRunMode(int mode) {
    switch (mode) {
    case LTM_RUNEXE:
        AssetsRelativePath::path = "./assets/";
        break;
    case LTM_VSDEBUG:
        AssetsRelativePath::path = PROJECT_SOURCE_RELATIVEPATH + std::string("assets/");
        break;
    }
    AssetsRelativePath::modeSet = true;
}

bool runModeNotSet() {
    return AssetsRelativePath::modeSet;
}

unsigned char* loadImage(std::string texturePath, int& width, int& height, int& nrChannels) {
    stbi_set_flip_vertically_on_load(true);
    return stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
}

void freeImage(unsigned char& data) {
    stbi_image_free(&data);
}