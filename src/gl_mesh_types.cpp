#include <iostream>

#include "headers/gl_mesh_types.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>

#include "headers/image_edit.hpp"
#include "headers/gl_values.hpp"
#include "headers/gl_control.hpp"
#include <LTM_CPPOpenGLAPIConfig.h>

GLLightSource::GLLightSource(glm::vec4& color, glm::vec3& position, float& strength) {
    this->color = &color;
    this->position = &position;
    this->strength = &strength;
}
glm::vec4& GLLightSource::getColor() {
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

    this->color = GLGC::getDefaultObjectColor();
    this->glUniEnableTexture = glGetUniformLocation(this->shaderProgram, "enableTexture");
    this->glUniObjectColor = glGetUniformLocation(this->shaderProgram, "objectColor");
    this->glUniAmbientColor = glGetUniformLocation(this->shaderProgram, "ambientColor");
    this->glUniAmbientStrength = glGetUniformLocation(this->shaderProgram, "ambientStrength");
    this->glUniAffectedLightPos = glGetUniformLocation(this->shaderProgram, "lightPos");
    this->glUniAffectedLightColor = glGetUniformLocation(this->shaderProgram, "lightColor");
    this->glUniAffectedLightStrength = glGetUniformLocation(this->shaderProgram, "lightStrength");
    glUniform1ui(glUniEnableTexture, false);
    glUniform4fv(glUniObjectColor, 1, &GLGC::getDefaultObjectColor()[0]);
    glUniform4fv(glUniAmbientColor, 1, &GLGC::getDefaultAmbientColor()[0]);
    glUniform4fv(glUniAmbientStrength, 1, &GLGC::getDefaultAmbientStrength()[0]);
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
    setColor(GLGC::getDefaultObjectColor(), true);
    this->hasDefaultColor = true;
}
void GLMesh::setToDefaultAmbientColor() {
    setAmbientColor(GLGC::getDefaultAmbientColor(), true);
    this->hasDefaultAmbientColor = true;
}
void GLMesh::setToDefaultAmbientStrength() {
    setAmbientStrength(GLGC::getDefaultAmbientStrength(), true);
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

void GLMesh::detectGlobalLightSource(glm::vec3& lightPos, glm::vec3& lightColor, float lightStrength) {
    std::cout << "Light position: " << lightPos.x << "," << lightPos.y << "," << lightPos.z << "\n";
    std::cout << "Light color: " << lightColor.r << "," << lightColor.g << "," << lightColor.b << "\n";
    glUseProgram(this->shaderProgram);
    glUniform3fv(glUniAffectedLightPos, 1, &lightPos[0]);
    glUniform3fv(glUniAffectedLightColor, 1, &lightColor[0]);
    glUniform1f(glUniAffectedLightStrength, lightStrength);
}

std::string& GLMesh::getName() {
    return this->name;
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

GLEmitterble::GLEmitterble(glm::vec4& color, glm::vec3& position, float& lightStrength) {
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