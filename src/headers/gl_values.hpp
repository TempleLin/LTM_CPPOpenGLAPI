#pragma once
#include <glm/glm.hpp>

#define WINDOWWIDTH 800
#define WINDOWHEIGHT 600

#define FOV_MAX 45.0f // @Maximum field of view.
#define FOV_MIN 1.0f //@Minimum field of view.
inline float fov{ 45.0f }; // @Starting field of view.

inline glm::mat4 modelMatrix{ glm::mat4(1.0f) };
inline glm::mat4 viewMatrix{ glm::mat4(1.0f) };
inline glm::mat4 projectionMatrix{ glm::mat4(1.0f) };

inline glm::vec3 cameraPos{ glm::vec3(0.0f, 0.0f, 3.0f) };
inline glm::vec3 cameraFront{ glm::vec3(0.0f, 0.0f, -1.0f) };
inline glm::vec3 cameraUp{ glm::vec3(0.0f, 1.0f, 0.0f) };

inline float deltaTime { 0.0f };	// Time between current frame and last frame
inline float lastFrame { 0.0f }; // Time of last frame

inline float cameraMoveSpeed { 2.5f };