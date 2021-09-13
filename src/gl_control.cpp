#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>

#include "headers/gl_control.hpp"
#include "headers/gl_values.hpp"
#include "headers/gl_mesh_types.hpp"
#include "headers/image_edit.hpp"
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


void GLMeshCtrl::setMeshCoordSystem(GLMesh& mesh) {
    glUseProgram(mesh.shaderProgram);

    glEnable(GL_DEPTH_TEST);

    mesh.modelMatrix = glm::mat4(1.0f);
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    projectionMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

    static unsigned int viewLoc = glGetUniformLocation(mesh.shaderProgram, "view");
    static unsigned int projectionLoc = glGetUniformLocation(mesh.shaderProgram, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void GLMeshCtrl::setBasicMeshSpawnPos(GLMesh& mesh) {
    mesh.modelMatrix = glm::translate(mesh.modelMatrix, mesh.position);
}

void GLMeshCtrl::spinBasicMeshAnim(GLMesh& mesh) {
    mesh.modelMatrix = glm::rotate(mesh.modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));
}

void GLMeshCtrl::drawBasicMesh(GLMesh& mesh) {
    glBindVertexArray(mesh.vao);
    static unsigned int modelLoc = glGetUniformLocation(mesh.shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mesh.modelMatrix));
    if (mesh.isTextureEnabled()) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.getTexture0());
    } else {
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glUseProgram(mesh.shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, mesh.verticesCount);
}

void GLMeshCtrl::setTexture0(GLMesh& mesh, std::string texturePath) {
    mesh.enableTexture = true;
    glUniform1ui(mesh.glUniEnableTexture, true);
    mesh.texture0 = new GLuint(0);
    glGenTextures(1, mesh.texture0);
    glBindTexture(GL_TEXTURE_2D, *(mesh.texture0));

    mesh.setTextureWrapFilter(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = loadImage(PROJECT_SOURCE_RELATIVEPATH + texturePath, width, height, nrChannels);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        freeImage(*data);
    } else {
        std::cout << "Failed to load texture." << std::endl;
    }
    glUseProgram(mesh.shaderProgram);
    int ourTexture = glGetUniformLocation(mesh.shaderProgram, "ourTexture");
    glUniform1i(ourTexture, 0);
}
void GLMeshCtrl::setDiffuseColor(GLMesh& mesh, glm::vec3 color, bool isNormalized) {
    if (isNormalized) {
        mesh.color = color;
    } else {
        mesh.color = color / 255.f;
    }
    glUseProgram(mesh.shaderProgram);
    glUniform3fv(mesh.glUniDiffuseColor, 1, &(mesh.color[0]));
    mesh.hasDefaultColor = false;
}
void GLMeshCtrl::setShininess(GLMesh& mesh, float shininess) {
    glUseProgram(mesh.shaderProgram);
    glUniform1f(mesh.glUniShininess, shininess);
    mesh.hasDefaultShininess = false;
}
void GLMeshCtrl::setSpecularStrength(GLMesh& mesh, float specularStrength) {
    glUseProgram(mesh.shaderProgram);
    glUniform1f(mesh.glUniSpecularStrength, specularStrength);
    mesh.hasDefaultAmbientStrength = false;
}
void GLMeshCtrl::setOpacity(GLMesh& mesh, float opacity) {
    mesh.opacity = (opacity > 1) ? 1 : ((opacity < 0) ? 0 : opacity);
    glUseProgram(mesh.shaderProgram);
    glUniform1f(mesh.glUniObjectOpacity, mesh.opacity);
}
void GLMeshCtrl::setAmbientColor(GLMesh& mesh, glm::vec3 ambientColor, bool isNormalized) {
    if (isNormalized) {
        mesh.ambientColor = ambientColor;
    } else {
        mesh.color = ambientColor / 255.f;
    }
    glUseProgram(mesh.shaderProgram);
    glUniform3fv(mesh.glUniAmbientColor, 1, &(mesh.ambientColor[0]));
    mesh.hasDefaultAmbientColor = false;
}
void GLMeshCtrl::setAmbientStrength(GLMesh& mesh, float ambientStrength) {
    mesh.ambientStrength = ambientStrength;
    glUseProgram(mesh.shaderProgram);
    glUniform1f(mesh.glUniAmbientStrength, mesh.ambientStrength);
    mesh.hasDefaultAmbientStrength = false;
}
void GLMeshCtrl::setToDefaultColor(GLMesh& mesh) {
    setDiffuseColor(mesh, GLGlobalCtrl::getDefaultObjectColor(), true);
    mesh.hasDefaultColor = true;
}
void GLMeshCtrl::setToDefaultAmbientColor(GLMesh& mesh) {
    setAmbientColor(mesh, GLGlobalCtrl::getDefaultAmbientColor(), true);
    mesh.hasDefaultAmbientColor = true;
}
void GLMeshCtrl::setToDefaultAmbientStrength(GLMesh& mesh) {
    setAmbientStrength(mesh, GLGlobalCtrl::getDefaultAmbientStrength());
    mesh.hasDefaultAmbientStrength = true;
}
void GLMeshCtrl::setToDefaultSpecularStrength(GLMesh& mesh) {
    setSpecularStrength(mesh, GLGlobalCtrl::getDefaultSpecularStrength());
    mesh.hasDefaultSpecularStrength = true;
}
void GLMeshCtrl::setToDefaultShininess(GLMesh& mesh) {
    setShininess(mesh, GLGlobalCtrl::getDefaultShininess());
    mesh.hasDefaultShininess = true;
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


glm::vec3 GLGlobalCtrl::viewBackgroundColor = glm::vec3(0.2f, 0.3f, 0.3f);
glm::vec3 GLGlobalCtrl::defaultObjectColor = glm::vec3(0.f, 0.f, 0.f);
glm::vec3 GLGlobalCtrl::defaultAmbientColor = glm::vec3(1.f, 1.f, 1.f);
float GLGlobalCtrl::defaultAmbientStrength = .1f;
float GLGlobalCtrl::defaultSpecularStrength = .5f;
float GLGlobalCtrl::defaultShininess = 32.f;

void GLGlobalCtrl::setDeltaTime() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
void GLGlobalCtrl::updateGlobalLightSource() {
    if (globalLightSources.empty()) return;
    auto lightSourcesIT = globalLightSources.begin();
    GLLightSource& firstLightSource = **lightSourcesIT;
    for (std::vector<GLMesh*>::iterator meshIT = meshesCollector.begin(); meshIT != meshesCollector.end(); ++meshIT) {
        (*meshIT)->detectGlobalLightSource(*firstLightSource.position, glm::vec3(*firstLightSource.color), *firstLightSource.strength);
    }
}
void GLGlobalCtrl::updateCameraViewPosToMeshes() {
    for (std::vector<GLMesh*>::iterator meshIT = meshesCollector.begin(); meshIT != meshesCollector.end(); ++meshIT) {
        (*meshIT)->detectViewCameraPos(cameraPos);
    }
}
void GLGlobalCtrl::enableMeshesTransparency() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
glm::vec3 GLGlobalCtrl::getViewBackgroundColor() {
    return viewBackgroundColor;
}
glm::vec3 GLGlobalCtrl::getDefaultObjectColor() {
    return defaultObjectColor;
}
glm::vec3 GLGlobalCtrl::getDefaultAmbientColor() {
    return defaultAmbientColor;
}
float GLGlobalCtrl::getDefaultAmbientStrength() {
    return defaultAmbientStrength;
}
float GLGlobalCtrl::getDefaultSpecularStrength() {
    return defaultSpecularStrength;
}
float GLGlobalCtrl::getDefaultShininess() {
    return defaultShininess;
}
void GLGlobalCtrl::changeViewBackgroundColor(glm::vec3 color, bool normalized) {
    viewBackgroundColor = normalized ? color : color / 255.f;
}
void GLGlobalCtrl::changeDefaultColor(glm::vec3 color, bool normalized) {
    defaultObjectColor = normalized ? color : color / 255.f;
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        if ((*it)->isDefaultColor()) {
            GLMeshCtrl::setToDefaultColor(**it);
        }
    }
}
void GLGlobalCtrl::changeDefaultAmbientColor(glm::vec3 ambientColor, bool normalized) {
    defaultAmbientColor = normalized ? ambientColor : ambientColor / 255.f;
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        if ((*it)->isDefaultAmbientColor()) {
            GLMeshCtrl::setToDefaultAmbientColor(**it);
        }
    }
}
void GLGlobalCtrl::changeDefaultAmbientStrength(float ambientStrength) {
    defaultAmbientStrength = ambientStrength;
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        if ((*it)->isDefaultAmbientStrength()) {
            GLMeshCtrl::setToDefaultAmbientStrength(**it);
        }
    }
}
void GLGlobalCtrl::changeDefaultSpecularStrength(float specularStrength) {
    defaultSpecularStrength = specularStrength;
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        if ((*it)->isDefaultSpecularStrength()) {
            GLMeshCtrl::setToDefaultSpecularStrength(**it);
        }
    }
}
void GLGlobalCtrl::changeDefaultShiness(float shininess) {
    defaultShininess = shininess;
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        if ((*it)->isDefaultShininess()) {
            GLMeshCtrl::setToDefaultShininess(**it);
        }
    }
}
void GLGlobalCtrl::resetAllDefaultValues() {
    changeViewBackgroundColor(glm::vec3(0.2f, 0.3f, 0.3f), true);
    defaultObjectColor = glm::vec4(0.f, 0.f, 0.f, 1.f);
    changeDefaultColor(defaultObjectColor, true);
    defaultAmbientColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    changeDefaultAmbientColor(defaultAmbientColor, true);
    defaultAmbientStrength = .1f;
    changeDefaultAmbientStrength(defaultAmbientStrength);
    defaultSpecularStrength = .5f;
    changeDefaultSpecularStrength(defaultSpecularStrength);
    defaultShininess = 32.f;
    changeDefaultSpecularStrength(defaultShininess);
}
void GLGlobalCtrl::resetAllMeshesColor() {
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        GLMeshCtrl::setToDefaultColor(**it);
    }
}
void GLGlobalCtrl::resetAllMeshesAmbientColor() {
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        GLMeshCtrl::setToDefaultAmbientColor(**it);
    }
}
void GLGlobalCtrl::resetAllMeshesAmbientStrength() {
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        GLMeshCtrl::setToDefaultAmbientStrength(**it);
    }
}
void GLGlobalCtrl::resetAllMeshesSpecularStrength() {
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        GLMeshCtrl::setToDefaultSpecularStrength(**it);
    }
}
void GLGlobalCtrl::resetAllMeshesDefaultShininess() {
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        GLMeshCtrl::setToDefaultShininess(**it);
    }
}