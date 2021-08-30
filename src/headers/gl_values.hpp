#pragma once
#include <glm/glm.hpp>

#define CURRENT_RELATIVE_FOLDERPATH "src/"

#define WINDOWWIDTH 800
#define WINDOWHEIGHT 600

#define FOV_MAX 45.0f // @Maximum field of view.
#define FOV_MIN 1.0f //@Minimum field of view.

inline glm::mat4 modelMatrix{ glm::mat4(1.0f) };
inline glm::mat4 viewMatrix{ glm::mat4(1.0f) };
inline glm::mat4 projectionMatrix{ glm::mat4(1.0f) };

inline glm::vec3 cameraPos{ glm::vec3(0.0f, 0.0f, 3.0f) };
inline glm::vec3 cameraFront{ glm::vec3(0.0f, 0.0f, -1.0f) };
inline glm::vec3 cameraUp{ glm::vec3(0.0f, 1.0f, 0.0f) };