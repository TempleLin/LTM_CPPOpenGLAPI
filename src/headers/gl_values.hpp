#pragma once
#include <glm/glm.hpp>

extern float windowWidth;
extern float windowHeight;

extern float fovMax; // @Maximum field of view.
extern float fovMin; //@Minimum field of view.
extern float fov; // @Current field of view.

extern glm::mat4 viewMatrix;
extern glm::mat4 projectionMatrix;

extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;

extern float deltaTime;	// Time between current frame and last frame
extern float lastFrame; // Time of last frame

extern float cameraMoveSpeed;