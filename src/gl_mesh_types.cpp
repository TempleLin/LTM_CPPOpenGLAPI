#include <iostream>

#include "headers/gl_mesh_types.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>

#include "headers/image_edit.hpp"
#include "headers/gl_values.hpp"
#include "headers/gl_control.hpp"
#include <LTM_CPPOpenGLAPIConfig.h>

GLLightSource::GLLightSource(glm::vec3& color, glm::vec3& position, float& strength) {
    this->color = &color;
    this->position = &position;
    this->strength = &strength;
}
glm::vec3& GLLightSource::getColor() {
    return *(this->color);
}
glm::vec3& GLLightSource::getPosition() {
    return *(this->position);
}
float GLLightSource::getStrength() {
    return *(this->strength);
}

GLMesh::GLMesh(std::string meshName, glm::vec3 position, unsigned int shaderProgram, unsigned int vao, unsigned int vbo) {
    this->name = meshName;
    this->position = position;

    this->vao = vao;
    glBindVertexArray(this->vao);
    this->vbo = vbo;
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    this->shaderProgram = shaderProgram;
    glUseProgram(this->shaderProgram);

    this->color = GLGlobalCtrl::getDefaultObjectColor();
    this->ambientColor = GLGlobalCtrl::getDefaultAmbientColor();
    this->ambientStrength = GLGlobalCtrl::getDefaultAmbientStrength();
    this->opacity = 1.f;
    this->glUniViewCameraPos = glGetUniformLocation(this->shaderProgram, "cameraViewPos");
    this->glUniEnableTexture = glGetUniformLocation(this->shaderProgram, "enableTexture");
    this->glUniObjectColor = glGetUniformLocation(this->shaderProgram, "objectColor");
    this->glUniObjectOpacity = glGetUniformLocation(this->shaderProgram, "objectOpacity");
    this->glUniAmbientColor = glGetUniformLocation(this->shaderProgram, "ambientColor");
    this->glUniAmbientStrength = glGetUniformLocation(this->shaderProgram, "ambientStrength");
    this->glUniAffectedLightPos = glGetUniformLocation(this->shaderProgram, "lightPos");
    this->glUniAffectedLightColor = glGetUniformLocation(this->shaderProgram, "lightColor");
    this->glUniAffectedLightStrength = glGetUniformLocation(this->shaderProgram, "lightStrength");
    glUniform3fv(glUniViewCameraPos, 1, &cameraPos[0]);
    glUniform1ui(glUniEnableTexture, false);
    glUniform3fv(glUniObjectColor, 1, &GLGlobalCtrl::getDefaultObjectColor()[0]);
    glUniform1f(glUniObjectOpacity, opacity);
    glUniform3fv(glUniAmbientColor, 1, &GLGlobalCtrl::getDefaultAmbientColor()[0]);
    glUniform1f(glUniAmbientStrength, GLGlobalCtrl::getDefaultAmbientStrength());
    glUniform4f(glUniAffectedLightColor, 1, 1, 1, 1);
    glUniform1f(glUniAffectedLightStrength, 1.f);
    std::cout << "GLMesh: " << meshName << " Constructed" << std::endl;
    meshesCollector.push_back(this);
}

void GLMesh::setTextureWrapFilter(unsigned int wrap_s, unsigned int wrap_t, unsigned int min_filter, unsigned int max_filter) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_filter);
}

bool GLMesh::isTextureEnabled() {
    return this->enableTexture;
}
bool GLMesh::isDefaultColor() {
    return this->hasDefaultColor;
}
bool GLMesh::isDefaultAmbientColor() {
    return this->hasDefaultAmbientColor;
}
bool GLMesh::isDefaultAmbientStrength() {
    return this->hasDefaultAmbientStrength;
}

void GLMesh::detectGlobalLightSource(const glm::vec3& lightPos, const glm::vec3& lightColor, float lightStrength) {
    glUseProgram(this->shaderProgram);
    glUniform3fv(glUniAffectedLightPos, 1, &lightPos[0]);
    glUniform3fv(glUniAffectedLightColor, 1, &lightColor[0]);
    glUniform1f(glUniAffectedLightStrength, lightStrength);
}
void GLMesh::detectViewCameraPos(glm::vec3& cameraPos) {
    glUniform3fv(this->glUniViewCameraPos, 1, &cameraPos[0]);
}

std::string& GLMesh::getName() {
    return this->name;
}
glm::vec3& GLMesh::getPosition() {
    return this->position;
}
glm::vec3& GLMesh::getColor() {
    return this->color;
}
glm::vec3& GLMesh::getAmbientColor() {
    return this->ambientColor;
}
float GLMesh::getOpacity() {
    return this->opacity;
}
float GLMesh::getAmbientStrength() {
    return this->ambientStrength;
}
unsigned int GLMesh::getTexture0() {
    return texture0 ? *texture0 : NULL;
}
unsigned int& GLMesh::getVAO() {
    return this->vao;
}
unsigned int& GLMesh::getVBO() {
    return this->vbo;
}
unsigned int& GLMesh::getShaderProgram() {
    return this->shaderProgram;
}
unsigned int GLMesh::getVerticesCount() {
    return this->verticesCount;
}

GLMesh::~GLMesh() {
    delete[] vertices;
    delete texture0;
    vertices = nullptr;
    texture0 = nullptr;
}

GLBasicCubeMesh::GLBasicCubeMesh(std::string meshName, glm::vec3 position, unsigned int shaderProgram, unsigned int vao, unsigned int vbo) : GLMesh(meshName, position, shaderProgram, vao, vbo) {
    cubeVerticesArraySize = 288;
    verticesCount = 36;
    vertices = new float[cubeVerticesArraySize] {
             //Positions(3).....  //Normals(3)....... //Texture0(2)
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, cubeVerticesArraySize * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // @Positions attributes.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // @Normals attributes.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // @Texture0 attributes. 
    glEnableVertexAttribArray(2);
}

GLEmitterble::GLEmitterble(glm::vec3& color, glm::vec3& position, float& lightStrength) {
    this->lightStrength = lightStrength;
    lightSource = new GLLightSource(color, position, this->lightStrength);
}
void GLEmitterble::enableLightEmit() {
    // TODO: Put the lightsource into a vector of pointer lightsources if doesn't exist.
    for (std::list<GLLightSource*>::iterator it = globalLightSources.begin(); it != globalLightSources.end(); ++it) {
        if (*it == this->lightSource) {
            return;
        }
    }
    globalLightSources.push_front(this->lightSource);
}
void GLEmitterble::changeLightStrength(float strength) {
    this->lightStrength = strength;
}
void GLEmitterble::disableLightEmit() {
    // TODO: Remove the lightsource from a vector of pointer lightsources if exists.
    for (std::list<GLLightSource*>::iterator it = globalLightSources.begin(); it != globalLightSources.end(); ++it) {
        if (*it == this->lightSource) {
            globalLightSources.erase(it);
            return;
        }
    }
}
GLEmitterble::~GLEmitterble() {
    disableLightEmit();
    delete lightSource;
    // TODO: Remove the lightsource from a vector of pointer lightsources if still exists.
}

GLEmmiterbleCubeMesh::GLEmmiterbleCubeMesh(std::string meshName, glm::vec3 position, unsigned int shaderProgram, unsigned int vao, unsigned int vbo, float lightStrength)
    : GLBasicCubeMesh(meshName, position, shaderProgram, vao, vbo), GLEmitterble(this->color, position, lightStrength){

}