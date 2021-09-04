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
    glm::vec4 color, ambientColor, ambientStrength;
    unsigned int vao, vbo;
    unsigned int verticesCount;
    unsigned int shaderProgram;
    int glUniObjectColor, glUniAmbientColor, glUniAmbientStrength, glUniEnableTexture;
    bool hasDefaultColor{ true }, hasDefaultAmbientColor{ true }, hasDefaultAmbientStrength{ true };
    void setTextureWrapFilter(unsigned int wrap_s, unsigned int wrap_t, unsigned int min_filter, unsigned int max_filter);
public:
    GLMesh(std::string meshName, glm::vec3 position, unsigned int shaderProgram, unsigned int vao, unsigned int vbo);
    bool isTextureEnabled();

    void setTexture0(std::string texturePath);
    void setColor(glm::vec4 color, bool isNormalized);
    void setAmbientColor(glm::vec4 ambientColor, bool isNormalized);
    void setAmbientStrength(glm::vec4 ambientStrength, bool isNormalized);
    void setToDefaultColor(); // @Resets to world value defaultObjectColor
    void setToDefaultAmbientColor(); // @Resets to world value defaultAmbientColor
    void setToDefaultAmbientStrength(); // @Resets to world value defaultAmbientStrength
    bool isDefaultColor(); // @Check if the mesh's color is default world color.
    bool isDefaultAmbientColor();
    bool isDefaultAmbientStrength();

    std::string& getMeshName();
    unsigned int getTexture0();
    unsigned int& getVAO();
    unsigned int& getVBO();
    unsigned int getVerticesCount();
    glm::vec3& getPosition();
    unsigned int& getShaderProgram();
    ~GLMesh();
};

class BasicCubeMesh : public GLMesh {
private:
    int cubeVerticesArraySize;
public:
    BasicCubeMesh(std::string meshName, glm::vec3 position, unsigned int shaderProgram, unsigned int vao, unsigned int vbo);
};
