#pragma once
#include <glm/glm.hpp>
#include <string>

class GLMesh {
protected:
    bool enableTexture = false;
    std::string meshName;
    glm::vec3 position;
    float* vertices;
    unsigned int* texture0 = nullptr;
    glm::vec4 color;
    unsigned int vao, vbo;
    unsigned int verticesCount;
    unsigned int shaderProgram;
    void setTextureWrapFilter(unsigned int wrap_s, unsigned int wrap_t, unsigned int min_filter, unsigned int max_filter);
public:
    GLMesh(std::string meshName, glm::vec3 position, unsigned int& shaderProgram, unsigned int* vao = nullptr, unsigned int* vbo = nullptr);
    bool isTextureEnabled();
    void setTexture0(std::string texturePath);
    void setColor(glm::vec4 rgbaValue);
    std::string& getMeshName();
    unsigned int getTexture0();
    unsigned int getVAO();
    unsigned int getVBO();
    unsigned int getVerticesCount();
    glm::vec3& getPosition();
    unsigned int getShaderProgram();
    ~GLMesh();
};

class BasicCubeMesh : public GLMesh {
private:
    int cubeVerticesArraySize;
public:
    BasicCubeMesh(std::string meshName, glm::vec3 position, unsigned int& shaderProgram, unsigned int* vao = nullptr, unsigned int* vbo = nullptr);
};
