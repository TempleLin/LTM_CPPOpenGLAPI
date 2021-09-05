#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>

#include "headers/gl_control.hpp"
#include "headers/gl_values.hpp"
#include "headers/gl_mesh_types.hpp"
#include <LTM_CPPOpenGLAPIConfig.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

std::vector<unsigned int> vaoGarbageCollector, vboGarbageCollector, programGarbageCollector;
std::vector<GLMesh*> meshesCollector;
std::list<GLLightSource*> globalLightSources;


void setMeshCoordSystem(unsigned int& shaderProgram) {
    glUseProgram(shaderProgram);

    glEnable(GL_DEPTH_TEST);

    modelMatrix = glm::mat4(1.0f);
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    projectionMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

    static unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    static unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void setBasicMeshSpawnPos(GLMesh& mesh) {
    modelMatrix = glm::translate(modelMatrix, mesh.getPosition());
}

void spinBasicMeshAnim(GLMesh& mesh) {
    modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));
}

void drawBasicMesh(GLMesh& mesh) {
    glBindVertexArray(mesh.getVAO());
    static unsigned int modelLoc = glGetUniformLocation(mesh.getShaderProgram(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    if (mesh.isTextureEnabled()) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.getTexture0());
    } else {
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glUseProgram(mesh.getShaderProgram());
    glDrawArrays(GL_TRIANGLES, 0, mesh.getVerticesCount());
}

void checkCompileErrors(unsigned int shader, char* type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void createShaderProgram(unsigned int& vertexShader, std::string vsPath, unsigned int& fragmentShader, std::string fsPath, unsigned int& shaderProgram) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexPath = std::string(PROJECT_SOURCE_RELATIVEPATH).append(vsPath);
    std::string fragmentPath = std::string(PROJECT_SOURCE_RELATIVEPATH).append(fsPath);
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl
            << "Check if CURRENT_RELATIVE_FOLDERPATH is correct." << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    // vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, (char*)"VERTEX");
    // fragment Shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, (char*)"FRAGMENT");

    // shader Program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    checkCompileErrors(shaderProgram, (char*)"PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    programGarbageCollector.push_back(shaderProgram);
}

void createVAOs(unsigned int count, unsigned int* vaos) {
    glGenVertexArrays(count, vaos);
    for (int i = 0; i < count; i++) {
        vaoGarbageCollector.push_back(vaos[i]);
    }
}
void createVBOs(unsigned int count, unsigned int* vbos) {
    glGenBuffers(count, vbos);
    for (int i = 0; i < count; i++) {
        vboGarbageCollector.push_back(vbos[i]);
    }
}
void cleanProgramsGarbage() {
    try {
        for (int i = 0; i < programGarbageCollector.size(); i++) {
            glDeleteProgram(programGarbageCollector.at(i));
        }
    } catch(...){
        std::cout << "WARNING: PROGRAMS GARBAGE COLLECTOR ERROR" << "\n";
    }
}
void cleanVAOsGarbage() {
    try {
        glDeleteVertexArrays(vaoGarbageCollector.size(), &vaoGarbageCollector[0]);
    } catch (...) {
        std::cout << "WARNING: VAO GARBAGE COLLECTOR ERROR" << "\n";
    }
}
void cleanVBOsGarbage() {
    try {
        glDeleteBuffers(vboGarbageCollector.size(), &vboGarbageCollector[0]);
    } catch (...) {
        std::cout << "WARNING: VBO GARBAGE COLLECTOR ERROR" << "\n";
    }
}
void cleanGLObjectsGarbage() {
    cleanProgramsGarbage();
    cleanVAOsGarbage();
    cleanVBOsGarbage();
}


std::unique_ptr<glm::vec4> GLGlobalControl::viewBackgroundColor = std::make_unique<glm::vec4>(0.2f, 0.3f, 0.3f, 1.0f);
std::unique_ptr<glm::vec4> GLGlobalControl::defaultObjectColor = std::make_unique<glm::vec4>(0.f, 0.f, 0.f, 1.f);
std::unique_ptr<glm::vec4> GLGlobalControl::defaultAmbientColor = std::make_unique<glm::vec4>(1.f, 1.f, 1.f, 1.f);
std::unique_ptr<glm::vec4> GLGlobalControl::defaultAmbientStrength = std::make_unique<glm::vec4>(.1f, .1f, .1f, 1.f);

void GLGlobalControl::setDeltaTime() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
void GLGlobalControl::updateGlobalLightSource() {
    if (globalLightSources.empty()) return;
    auto lightSourcesIT = globalLightSources.begin();
    GLLightSource& firstLightSource = **lightSourcesIT;
    for (std::vector<GLMesh*>::iterator meshIT = meshesCollector.begin(); meshIT != meshesCollector.end(); ++meshIT) {
        (*meshIT)->detectGlobalLightSource(firstLightSource.getPosition(), glm::vec3(firstLightSource.getColor()));
    }
}
void GLGlobalControl::enableMeshesTransparency() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
glm::vec4 GLGlobalControl::getViewBackgroundColor() {
    return *viewBackgroundColor;
}
glm::vec4 GLGlobalControl::getDefaultObjectColor() {
    return *defaultObjectColor;
}
glm::vec4 GLGlobalControl::getDefaultAmbientColor() {
    return *defaultAmbientColor;
}
glm::vec4 GLGlobalControl::getDefaultAmbientStrength() {
    return *defaultAmbientStrength;
}
void GLGlobalControl::changeDefaultColor(glm::vec4 color, bool normalized) {
    *defaultObjectColor = normalized ? color : color / 255.f;
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        if ((*it)->isDefaultColor()) {
            (*it)->setToDefaultColor();
        }
    }
}
void GLGlobalControl::changeDefaultAmbientColor(glm::vec4 ambientColor, bool normalized) {
    *defaultAmbientColor = normalized ? ambientColor : ambientColor / 255.f;
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        if ((*it)->isDefaultAmbientColor()) {
            (*it)->setToDefaultAmbientColor();
        }
    }
}
void GLGlobalControl::changeDefaultAmbientStrength(glm::vec4 ambientStrength, bool normalized) {
    *defaultAmbientStrength = normalized ? ambientStrength : ambientStrength / 255.f;
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        if ((*it)->isDefaultAmbientStrength()) {
            (*it)->setToDefaultAmbientStrength();
        }
    }
}
void GLGlobalControl::resetAllDefaultValues() {
    *viewBackgroundColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
    *defaultObjectColor = glm::vec4(0.f, 0.f, 0.f, 1.f);
    *defaultAmbientColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    *defaultAmbientStrength = glm::vec4(1.f, 1.f, 1.f, 1.f);
}
void GLGlobalControl::resetAllMeshesColor() {
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        (*it)->setToDefaultColor();
    }
}
void GLGlobalControl::resetAllMeshesAmbientColor() {
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        (*it)->setToDefaultAmbientColor();
    }
}
void GLGlobalControl::resetAllMeshesAmbientStrength() {
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        (*it)->setToDefaultAmbientStrength();
    }
}
void GLGlobalControl::destructAllPointersValue() {
    viewBackgroundColor.reset();
    defaultObjectColor.reset();
    defaultAmbientColor.reset();
    defaultAmbientStrength.reset();
}