#include <iostream>

#include "headers/gl_mesh_types.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>

#include "headers/image_edit.hpp"
#include "headers/gl_values.hpp"
#include "headers/gl_control.hpp"
#include <LTM_CPPOpenGLAPIConfig.h>

GLMesh::GLMesh(std::string meshName, glm::vec3 position, unsigned int shaderProgram, unsigned int vao, unsigned int vbo) {
    this->meshName = meshName;
    this->position = position;

    this->vao = vao;
    glBindVertexArray(this->vao);
    this->vbo = vbo;
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    this->shaderProgram = shaderProgram;
    glUseProgram(this->shaderProgram);

    this->color = GLGC::getDefaultObjectColor();
    this->glUniEnableTexture = glGetUniformLocation(this->shaderProgram, "enableTexture");
    this->glUniObjectColor = glGetUniformLocation(this->shaderProgram, "objectColor");
    this->glUniAmbientColor = glGetUniformLocation(this->shaderProgram, "ambientColor");
    this->glUniAmbientStrength = glGetUniformLocation(this->shaderProgram, "ambientStrength");
    glUniform1ui(glUniEnableTexture, false);
    glUniform4fv(glUniObjectColor, 1, &GLGC::getDefaultObjectColor()[0]);
    glUniform4fv(glUniAmbientColor, 1, &GLGC::getDefaultAmbientColor()[0]);
    glUniform4fv(glUniAmbientStrength, 1, &GLGC::getDefaultAmbientStrength()[0]);
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

void GLMesh::setTexture0(std::string texturePath) {
    this->enableTexture = true;
    glUniform1ui(this->glUniEnableTexture, true);
    this->texture0 = new GLuint(0);
    glGenTextures(1, this->texture0);
    glBindTexture(GL_TEXTURE_2D, *texture0);

    setTextureWrapFilter(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = loadImage(PROJECT_SOURCE_RELATIVEPATH + texturePath, width, height, nrChannels);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        freeImage(*data);
    } else {
        std::cout << "Failed to load texture." << std::endl;
    }
    glUseProgram(this->shaderProgram);
    int ourTexture = glGetUniformLocation(this->shaderProgram, "ourTexture");
    glUniform1i(ourTexture, 0);
}

void GLMesh::setColor(glm::vec4 color, bool isNormalized) {
    if (isNormalized) {
        this->color = color;
    } else {
        this->color = color / 255.f;
    }
    glUseProgram(this->shaderProgram);
    glUniform4fv(this->glUniObjectColor, 1, &this->color[0]);
    this->hasDefaultColor = false;
}
void GLMesh::setAmbientColor(glm::vec4 ambientColor, bool isNormalized) {
    if (isNormalized) {
        this->ambientColor = ambientColor;
    } else {
        this->color = ambientColor / 255.f;
    }
    glUseProgram(this->shaderProgram);
    glUniform4fv(glUniAmbientColor, 1, &this->ambientColor[0]);
    this->hasDefaultAmbientColor = false;
}
void GLMesh::setAmbientStrength(glm::vec4 ambientStrength, bool isNormalized) {
    if (isNormalized) {
        this->ambientStrength = ambientStrength;
    } else {
        this->ambientStrength = ambientStrength / 255.f;
    }
    glUseProgram(this->shaderProgram);
    glUniform4fv(glUniAmbientStrength, 1, &this->ambientStrength[0]);
    this->hasDefaultAmbientStrength = false;
}
void GLMesh::setToDefaultColor() {
    setColor(GLGlobalControl::getDefaultObjectColor(), true);
    this->hasDefaultColor = true;
    std::cout << "Now color is: " << glm::to_string(this->color) << std::endl;
}
void GLMesh::setToDefaultAmbientColor() {
    setAmbientColor(GLGlobalControl::getDefaultAmbientColor(), true);
    this->hasDefaultAmbientColor = true;
}
void GLMesh::setToDefaultAmbientStrength() {
    setAmbientStrength(GLGlobalControl::getDefaultAmbientStrength(), true);
    this->hasDefaultAmbientStrength = true;
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

std::string& GLMesh::getMeshName() {
    return this->meshName;
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
    return verticesCount;
}
glm::vec3& GLMesh::getPosition() {
    return position;
}

GLMesh::~GLMesh() {
    delete[] vertices;
    delete texture0;
    vertices = nullptr;
    texture0 = nullptr;
}

BasicCubeMesh::BasicCubeMesh(std::string meshName, glm::vec3 position, unsigned int shaderProgram, unsigned int vao, unsigned int vbo) : GLMesh(meshName, position, shaderProgram, vao, vbo) {
    cubeVerticesArraySize = 180;
    verticesCount = 36;
    vertices = new float[cubeVerticesArraySize] {
             //Positions         //Textures
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
             0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
             0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

             0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
             0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
             0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, cubeVerticesArraySize * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
