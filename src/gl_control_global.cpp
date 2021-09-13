#include <fstream>
#include <sstream>
#include <iostream>

#include "headers/gl_control.hpp"
#include "headers/gl_values.hpp"
#include "headers/gl_mesh_types.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
            GLMeshCtrl::resetToDefaultColor(**it);
        }
    }
}
void GLGlobalCtrl::changeDefaultAmbientColor(glm::vec3 ambientColor, bool normalized) {
    defaultAmbientColor = normalized ? ambientColor : ambientColor / 255.f;
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        if ((*it)->isDefaultAmbientColor()) {
            GLMeshCtrl::resetToDefaultAmbientColor(**it);
        }
    }
}
void GLGlobalCtrl::changeDefaultAmbientStrength(float ambientStrength) {
    defaultAmbientStrength = ambientStrength;
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        if ((*it)->isDefaultAmbientStrength()) {
            GLMeshCtrl::resetToDefaultAmbientStrength(**it);
        }
    }
}
void GLGlobalCtrl::changeDefaultSpecularStrength(float specularStrength) {
    defaultSpecularStrength = specularStrength;
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        if ((*it)->isDefaultSpecularStrength()) {
            GLMeshCtrl::resetToDefaultSpecularStrength(**it);
        }
    }
}
void GLGlobalCtrl::changeDefaultShiness(float shininess) {
    defaultShininess = shininess;
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        if ((*it)->isDefaultShininess()) {
            GLMeshCtrl::resetToDefaultShininess(**it);
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
        GLMeshCtrl::resetToDefaultColor(**it);
    }
}
void GLGlobalCtrl::resetAllMeshesAmbientColor() {
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        GLMeshCtrl::resetToDefaultAmbientColor(**it);
    }
}
void GLGlobalCtrl::resetAllMeshesAmbientStrength() {
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        GLMeshCtrl::resetToDefaultAmbientStrength(**it);
    }
}
void GLGlobalCtrl::resetAllMeshesSpecularStrength() {
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        GLMeshCtrl::resetToDefaultSpecularStrength(**it);
    }
}
void GLGlobalCtrl::resetAllMeshesDefaultShininess() {
    for (std::vector<GLMesh*>::iterator it = meshesCollector.begin(); it != meshesCollector.end(); ++it) {
        GLMeshCtrl::resetToDefaultShininess(**it);
    }
}