#include <LTM_CPPOpenGLAPI.h>
#include <GLFW/glfw3.h>
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
        return 1;
    }

    glfwMakeContextCurrent(window);
    
    while (!glfwWindowShouldClose(window)){
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}