#pragma once
#include <string>

typedef unsigned int GLuint;
struct GLFWwindow;

void glfw_error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void checkCompileErrors(GLuint shader, char* type);
GLFWwindow* configureOpenGL();
void createShaderProgram(unsigned int& vertexShader, std::string vsPath, unsigned int& fragmentShader, std::string fsPath, unsigned int& shaderProgram);
