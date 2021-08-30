#ifndef USE_3DFIRSTLIGHTINGSCENE
#define USE_3DFIRSTLIGHTINGSCENE
//#define FUNCTION_TIME_TESTING //Uncomment this if want to testing timing between function calls.
//#define PURE_TESTING //Uncomment this to disable current main execution and do pure testing.
//#define WIREFRAME_MODE
#endif // !USE_3DFIRSTLIGHTINGSCENE

#ifdef USE_3DFIRSTLIGHTINGSCENE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "headers/gl_setup.hpp"
#include "headers/gl_mesh_types.hpp"
#include "headers/gl_control.hpp"

#include <LTM_CPPOpenGLAPIConfig.h>
#ifdef SHOW_API_VERSION
    #include <iostream>
#endif

#ifdef FUNCTION_TIME_TESTING
    #include <chrono>
    #ifndef PURE_TESTING
    #endif // !PURE_TESTING

void function_time_testing();

void testTime0(GLMesh& mesh);
void testTime1(GLMesh& mesh);
#endif // FUNCTION_TIME_TESTING

#ifdef PURE_TESTING
#ifndef FUNCTION_TIME_TESTING
#endif // !FUNCTION_TIME_TESTING

void pure_testing();
#endif // PURE_TESTING



int main() {
#ifdef SHOW_API_VERSION
    std::cout << "API Version: " << LTM_CPPOPENGLAPICONFIG_MAJOR 
        << "." << LTM_CPPOPENGLAPICONFIG_MINOR << "\n";
#endif
    
#ifndef PURE_TESTING
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

#ifdef FUNCTION_TIME_TESTING
    function_time_testing();
#endif // FUNCTION_TIME_TESTING

    glDeleteVertexArrays(1, &basicLightingCube0.getVAO());
    glDeleteBuffers(1, &basicLightingCube0.getVBO());
    glDeleteProgram(shaderProgramHandle);
#else
    pure_testing();
#endif // !PURE_TESTING
}

#ifdef PURE_TESTING
void pure_testing() {
    /*std::cout << (GL_REPEAT == gl_wrap::GL_REPEAT_WRAP) << std::endl;
    std::cout << (GL_TEXTURE_2D == gl_wrap::GL_TEXTURE_2D_WRAP) << std::endl;*/

    configureOpenGL();
    unsigned int testShaderProgram, testVS, testFS;
    createShaderProgram(testShaderProgram, testVS, testFS);
    BasicLightingCube basicLightingCube0(0.5f, 0.0f, -1.0f, testShaderProgram);
    std::cout << testShaderProgram << std::endl << basicLightingCube0.getShaderProgram() << std::endl;
}
#endif // PURE_TESTING


#ifdef FUNCTION_TIME_TESTING
void function_time_testing() {
    auto t1 = std::chrono::high_resolution_clock::now();
    testTime0(basicLightingCube0);
    auto t2 = std::chrono::high_resolution_clock::now();

    auto t3 = std::chrono::high_resolution_clock::now();
    testTime1(basicLightingCube0);
    auto t4 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> firstCount = t2 - t1;
    std::chrono::duration<double, std::milli> secondCount = t4 - t3;
    std::cout << firstCount.count() << std::endl;
    std::cout << secondCount.count() << std::endl;
}

void testTime0(GLMesh& mesh) {
    for (int i = 0; i < 100; i++) {
        mesh.getPositionX();
        mesh.getPositionY();
        mesh.getPositionZ();
    }
}
void testTime1(GLMesh& mesh) {
    for (int i = 0; i < 100; i++) {
        mesh.position[0];
        mesh.position[1];
        mesh.position[2];
    }
}
#endif // FUNCTION_TIME_TESTING

#endif // !USE_3DFIRSTLIGHTINGSCENE
