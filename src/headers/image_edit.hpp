#pragma once
#include <string>

unsigned char* loadImage(std::string texturePath, int& width, int& height, int& nrChannels);
void freeImage(unsigned char& data);