#pragma once
#include <glm/glm.hpp>
#include <string>

class GLLightSource {
    friend class GLGlobalCtrl;
private:
    glm::vec3* color;
    glm::vec3* position;
    float* strength;
public:
    GLLightSource(glm::vec3& color, glm::vec3& position, float& strength);
    glm::vec3& getColor();
    glm::vec3& getPosition();
    float getStrength();
};

class GLMesh {
    friend class GLMeshCtrl;
    friend class GLGlobalCtrl;
protected:
    bool enableTexture = false;
    std::string name;
    glm::vec3 position;
    float* vertices;
    unsigned int* texture0 = nullptr;
    glm::vec3 color, ambientColor;
    float opacity;
    float ambientStrength;
    unsigned int vao, vbo;
    unsigned int verticesCount;
    unsigned int shaderProgram;
    int glUniViewCameraPos, glUniObjectColor, glUniObjectOpacity, glUniAmbientColor, glUniAmbientStrength, 
        glUniEnableTexture, glUniAffectedLightPos, glUniAffectedLightColor, glUniAffectedLightStrength;
    bool hasDefaultColor{ true }, hasDefaultAmbientColor{ true }, hasDefaultAmbientStrength{ true };

    // @Functions below are called by global control.
    void setTextureWrapFilter(unsigned int wrap_s, unsigned int wrap_t, unsigned int min_filter, unsigned int max_filter);
    void detectGlobalLightSource(glm::vec3& lightPos, glm::vec3& lightColor, float lightStrength);
    void detectViewCameraPos(glm::vec3& cameraPos);
public:
    GLMesh(std::string meshName, glm::vec3 position, unsigned int shaderProgram, unsigned int vao, unsigned int vbo);
    bool isTextureEnabled();
    bool isDefaultColor(); // @Check if the mesh's color is default world color.
    bool isDefaultAmbientColor();
    bool isDefaultAmbientStrength();

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
    GLEmitterble(glm::vec3& color, glm::vec3& position, float& lightStrength);
    void enableLightEmit();
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