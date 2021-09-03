#include "headers/gl_mesh_types.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "headers/image_edit.hpp"
#include "headers/gl_values.hpp"
#include <iostream>
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

    this->color = glm::vec4(0, 0, 0, 1);
    glUniform1ui(glGetUniformLocation(this->shaderProgram, "enableTexture"), false);
    glUniform4f(glGetUniformLocation(this->shaderProgram, "objectColor"), 0, 0, 0, 1);
    glUniform4f(glGetUniformLocation(this->shaderProgram, "ambientColor"), 1, 1, 1, 1);
    glUniform4f(glGetUniformLocation(this->shaderProgram, "ambientStrength"), 1.f, 1.f, 1.f, 1.f);
    std::cout << "GLMesh: " << meshName << " Constructed" << std::endl;
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
    glUniform1ui(glGetUniformLocation(this->shaderProgram, "enableTexture"), true);
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

void GLMesh::setColor(float R, float G, float B, float A) {
    if (R > 1.0f || G > 1.0f || B > 1.0f || A > 1.0f) {
        this->color[0] = R / 255.f;
        this->color[1] = G / 255.f;
        this->color[2] = B / 255.f;
        this->color[3] = A / 255.f;
    } else {
        this->color[0] = R;
        this->color[1] = G;
        this->color[2] = B;
        this->color[3] = A;
    }
    glUseProgram(this->shaderProgram);
    int ourColor = glGetUniformLocation(this->shaderProgram, "objectColor");
    glUniform4f(ourColor, R, G, B, A);
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
