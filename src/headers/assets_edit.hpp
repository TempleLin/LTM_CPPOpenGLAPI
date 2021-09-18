#pragma once
#include <string>

#define LTM_VSDEBUG 0
#define LTM_RUNEXE 1

class AssetsRelativePath {
private:
	static bool modeSet;
	static std::string path;
public:
	static std::string& getPath();
	// @The folders will be in source path during VS dubug mode; 
	// otherwise will be alongside exe.
	friend void setAppRunMode(int mode);
	friend bool runModeNotSet();
	friend unsigned char* loadImage
		(std::string texturePath, int& width, int& height, int& nrChannels);
};
void freeImage(unsigned char& data);