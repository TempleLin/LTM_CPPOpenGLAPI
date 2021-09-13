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
    glm::mat4 modelMatrix = glm::mat4{ 1.f };
    bool diffMapEnabled{ false }, specMapEnabled{ false };
    std::string name;
    glm::vec3 position;
    float* vertices;
    unsigned int* texture0{ nullptr }, *texture1{ nullptr };
    glm::vec3 color, ambientColor;
    float opacity;
    float ambientStrength;
    unsigned int vao, vbo;
    unsigned int verticesCount;
    unsigned int shaderProgram;
    int glUniViewCameraPos, glUniDiffuseColor, glUniDiffuseStrength, glUniObjectOpacity, glUniAmbientColor, glUniAmbientStrength, 
        glUniDiffuseMapEnabled, glUniSpecularMapEnabled, glUniAffectedLightPos, glUniAffectedLightColor, glUniAffectedLightStrength, glUniSpecularStrength, glUniShininess;
    bool hasDefaultColor{ true }, hasDefaultAmbientColor{ true }, hasDefaultAmbientStrength{ true }, hasDefaultSpecularStrength{ true },
        hasDefaultShininess{ true };

    // @Functions below are called by global control.
    void setTextureWrapFilter(unsigned int wrap_s, unsigned int wrap_t, unsigned int min_filter, unsigned int max_filter);
    void detectGlobalLightSource(const glm::vec3& lightPos, const glm::vec3& lightColor, float lightStrength);
    void detectViewCameraPos(glm::vec3& cameraPos);
public:
    GLMesh(std::string meshName, glm::vec3 position, unsigned int shaderProgram, unsigned int vao, unsigned int vbo);
    bool isDiffuseMapEnabled();
    bool isSpecularMapEnabled();

    bool isDefaultColor(); // @Check if the mesh's color is default world color.
    bool isDefaultAmbientColor();
    bool isDefaultAmbientStrength();
    bool isDefaultSpecularStrength();
    bool isDefaultShininess();

    std::string& getName();
    glm::vec3& getPosition();
    glm::vec3& getDiffuseColor();
    glm::vec3& getAmbientColor();
    float getOpacity();
    float getAmbientStrength();
    unsigned int getTexture0_diffuse();
    unsigned int getTexture1_specular();
    unsigned int& getVAO();
    unsigned int& getVBO();
    unsigned int& getShaderProgram();
    unsigned int getVerticesCount();
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