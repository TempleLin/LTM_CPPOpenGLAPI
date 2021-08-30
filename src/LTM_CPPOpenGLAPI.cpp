#include <LTM_CPPOpenGLAPI.h>
#include <glad/glad.h>
#include <glad.c>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

int main(int argc, char *argv[]){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = nullptr;

    window = glfwCreateWindow(800, 600, "LTM_CPPOpenGLAPI", NULL, NULL);
    if(!window){
        std::cout << "Failed to create GLFW window" << "\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -2;
    }

    try {
        glm::mat4 testMatrix = glm::mat4(1.0f);
        glm::mat4 testMatrix2 = glm::translate(testMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
        std::cout << glm::to_string(testMatrix2);
    } catch (...){
        std::cout << "Something wrong with GLM." << "\n";
    }
    
    while (!glfwWindowShouldClose(window)){
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glViewport(0, 0, 800, 600);

    return 0;
}