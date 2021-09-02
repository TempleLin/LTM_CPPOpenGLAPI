#pragma once

typedef unsigned int GLuint;
struct GLFWwindow;

void glfw_error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* configureOpenGL();
