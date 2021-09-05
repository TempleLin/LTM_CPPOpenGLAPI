#pragma once
#include <glm/glm.hpp>
#include <string>

class GLLightSource {
private:
    glm::vec4* color;
    glm::vec3* position;
    float* strength;
public:
    GLLightSource(glm::vec4& color, glm::vec3& position, float& strength);
    glm::vec4& getColor();
    glm::vec3& getPosition();
    float getStrength();
};

class GLMesh {
protected:
    bool enableTexture = false;
    std::string name;
    glm::vec3 position;
    float* vertices;
    unsigned int* texture0 = nullptr;
    glm::vec4 color, ambientColor, ambientStrength;
    unsigned int vao, vbo;
    unsigned int verticesCount;
    unsigned int shaderProgram;
    int glUniObjectColor, glUniAmbientColor, glUniAmbientStrength, glUniEnableTexture, glUniAffectedLightPos, glUniAffectedLightColor;
    bool hasDefaultColor{ true }, hasDefaultAmbientColor{ true }, hasDefaultAmbientStrength{ true };
    void setTextureWrapFilter(unsigned int wrap_s, unsigned int wrap_t, unsigned int min_filter, unsigned int max_filter);
public:
    GLMesh(std::string meshName, glm::vec3 position, unsigned int shaderProgram, unsigned int vao, unsigned int vbo);
    void setTexture0(std::string texturePath);
    void setColor(glm::vec4 color, bool isNormalized);
    void setAmbientColor(glm::vec4 ambientColor, bool isNormalized);
    void setAmbientStrength(glm::vec4 ambientStrength, bool isNormalized);
    void setToDefaultColor(); // @Resets to world value defaultObjectColor
    void setToDefaultAmbientColor(); // @Resets to world value defaultAmbientColor
    void setToDefaultAmbientStrength(); // @Resets to world value defaultAmbientStrength
    bool isTextureEnabled();
    bool isDefaultColor(); // @Check if the mesh's color is default world color.
    bool isDefaultAmbientColor();
    bool isDefaultAmbientStrength();

    void detectGlobalLightSource(glm::vec3& lightPos, glm::vec3& lightColor);

    std::string& getName();
    unsigned int getTexture0();
    unsigned int& getVAO();
    unsigned int& getVBO();
    unsigned int getVerticesCount();
    glm::vec3& getPosition();
    unsigned int& getShaderProgram();
    ~GLMesh();
};

class GLEmitterble {
protected:
    float lightStrength;
    GLLightSource* lightSource;
public:
    GLEmitterble(glm::vec4& color, glm::vec3& position, float& lightStrength);
    void enableLightEmit(float strength);
    void changeLightStrength(float strength);
    void disableLightEmit();
    ~GLEmitterble();
};

class GLBasicCubeMesh : public GLMesh {
protected:
    int cubeVerticesArraySize;
public:
    GLBasicCubeMesh(std::string meshName, glm::vec3 position, unsigned int shaderProgram, unsigned int vao, unsigned int vbo);
};

class GLEmmiterbleCubeMesh : public GLBasicCubeMesh, public GLEmitterble {
public:
    GLEmmiterbleCubeMesh(std::string meshName, glm::vec3 position, unsigned int shaderProgram, unsigned int vao, unsigned int vbo, float lightStrength);
};