#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "headers/gl_setup.hpp"
#include "headers/gl_mesh_types.hpp"
#include "headers/gl_control.hpp"

#include <LTM_CPPOpenGLAPIConfig.h>
#ifdef SHOW_API_VERSION
    #include <iostream>
#endif


int main() {
#ifdef SHOW_API_VERSION
    #ifndef API_VERSION
        std::cout << "Set to show API version but version not defined in config." << "\n";
    #else
        std::cout << SHOW_API_VERSION << API_VERSION << "\n";
    #endif
#endif
#ifdef SHOW_TESTING_VERSION
    #ifndef TESTING_VERSION
        std::cout << "Set to show Unit Testing version but version not defined in config." << "\n";
    #else
        std::cout << SHOW_TESTING_VERSION << TESTING_VERSION << "\n";
    #endif
#endif
#ifdef API_TESTING_VER_COMPARE_MESSAGE
    std::cout << API_TESTING_VER_COMPARE_MESSAGE << "\n";
#endif
    
    GLFWwindow* window = configureOpenGL();
    unsigned int vertexShaderHandle{ 0 }, fragmentShaderHandle{ 0 }, shaderProgramHandle{ 0 };
    createShaderProgram(vertexShaderHandle, "shaders/shaderVS.glsl", fragmentShaderHandle, "shaders/shaderFS.glsl", shaderProgramHandle);
    glUseProgram(shaderProgramHandle);

    BasicLightingCube basicLightingCube0("Basic Lighting Cube 0", glm::vec3(0.5f, 0.8f, -1.0f), shaderProgramHandle);
    basicLightingCube0.setTexture0("assets/imgs/container.jpg");

    BasicLightingCube basicLightingCube1("Basic Lighting Cube 1", glm::vec3(-0.5f, -0.3f, -0.3f), shaderProgramHandle);
    basicLightingCube1.setTexture0("assets/imgs/container.jpg");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // @Clear both color buffer and Z-depth buffer before each frame render.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setMeshCoordSystem(basicLightingCube0.getShaderProgram());
        setBasicMeshSpawnPos(basicLightingCube0);
        spinBasicMeshAnim(basicLightingCube0);
        drawBasicMesh(basicLightingCube0);

        setMeshCoordSystem(basicLightingCube1.getShaderProgram());
        setBasicMeshSpawnPos(basicLightingCube1);
        spinBasicMeshAnim(basicLightingCube1);
        drawBasicMesh(basicLightingCube1);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glBindVertexArray(0);
    }

    glDeleteVertexArrays(1, &basicLightingCube0.getVAO());
    glDeleteBuffers(1, &basicLightingCube0.getVBO());
    glDeleteProgram(shaderProgramHandle);
}