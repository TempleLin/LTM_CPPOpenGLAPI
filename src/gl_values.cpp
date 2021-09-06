#include "headers/gl_values.hpp"

float windowWidth { 1200 };
float windowHeight { 960 };

float fovMax { 45.0f };
float fovMin { 1.0f };
float fov { 45.0f };

glm::mat4 viewMatrix{ glm::mat4(1.0f) };
glm::mat4 projectionMatrix{ glm::mat4(1.0f) };

glm::vec3 cameraPos{ glm::vec3(0.0f, 0.0f, 3.0f) };
glm::vec3 cameraFront{ glm::vec3(0.0f, 0.0f, -1.0f) };
glm::vec3 cameraUp{ glm::vec3(0.0f, 1.0f, 0.0f) };

float deltaTime { 0.0f };
float lastFrame { 0.0f };

float cameraMoveSpeed { 2.5f };