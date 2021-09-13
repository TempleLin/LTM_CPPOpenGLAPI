#include "headers/gl_control.hpp"
#include "headers/gl_values.hpp"
#include "headers/image_edit.hpp"
#include <LTM_CPPOpenGLAPIConfig.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

void GLMeshCtrl::setMeshCoordSystem(GLMesh& mesh) {
    glUseProgram(mesh.shaderProgram);

    glEnable(GL_DEPTH_TEST);

    mesh.modelMatrix = glm::mat4(1.0f);
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    projectionMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

    static unsigned int viewLoc = glGetUniformLocation(mesh.shaderProgram, "view");
    static unsigned int projectionLoc = glGetUniformLocation(mesh.shaderProgram, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void GLMeshCtrl::setBasicMeshSpawnPos(GLMesh& mesh) {
    mesh.modelMatrix = glm::translate(mesh.modelMatrix, mesh.position);
}

void GLMeshCtrl::spinBasicMeshAnim(GLMesh& mesh) {
    mesh.modelMatrix = glm::rotate(mesh.modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));
}

void GLMeshCtrl::drawBasicMesh(GLMesh& mesh) {
    glBindVertexArray(mesh.vao);
    static unsigned int modelLoc = glGetUniformLocation(mesh.shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mesh.modelMatrix));
    if (mesh.isTextureEnabled()) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.getTexture0());
    } else {
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glUseProgram(mesh.shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, mesh.verticesCount);
}

void GLMeshCtrl::setTexture0(GLMesh& mesh, std::string texturePath) {
    mesh.enableTexture = true;
    glUniform1ui(mesh.glUniEnableTexture, true);
    mesh.texture0 = new GLuint(0);
    glGenTextures(1, mesh.texture0);
    glBindTexture(GL_TEXTURE_2D, *(mesh.texture0));

    mesh.setTextureWrapFilter(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = loadImage(PROJECT_SOURCE_RELATIVEPATH + texturePath, width, height, nrChannels);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        freeImage(*data);
    } else {
        std::cout << "Failed to load texture." << std::endl;
    }
    glUseProgram(mesh.shaderProgram);
    int ourTexture = glGetUniformLocation(mesh.shaderProgram, "ourTexture");
    glUniform1i(ourTexture, 0);
}
void GLMeshCtrl::overrideDiffuseColor(GLMesh& mesh, glm::vec3 color, bool isNormalized) {
    if (isNormalized) {
        mesh.color = color;
    } else {
        mesh.color = color / 255.f;
    }
    glUseProgram(mesh.shaderProgram);
    glUniform3fv(mesh.glUniDiffuseColor, 1, &(mesh.color[0]));
    mesh.hasDefaultColor = false;
}
void GLMeshCtrl::overrideShininess(GLMesh& mesh, float shininess) {
    glUseProgram(mesh.shaderProgram);
    glUniform1f(mesh.glUniShininess, shininess);
    mesh.hasDefaultShininess = false;
}
void GLMeshCtrl::overrideSpecularStrength(GLMesh& mesh, float specularStrength) {
    glUseProgram(mesh.shaderProgram);
    glUniform1f(mesh.glUniSpecularStrength, specularStrength);
    mesh.hasDefaultAmbientStrength = false;
}
void GLMeshCtrl::setOpacity(GLMesh& mesh, float opacity) {
    mesh.opacity = (opacity > 1) ? 1 : ((opacity < 0) ? 0 : opacity);
    glUseProgram(mesh.shaderProgram);
    glUniform1f(mesh.glUniObjectOpacity, mesh.opacity);
}
void GLMeshCtrl::overrideAmbientColor(GLMesh& mesh, glm::vec3 ambientColor, bool isNormalized) {
    if (isNormalized) {
        mesh.ambientColor = ambientColor;
    } else {
        mesh.color = ambientColor / 255.f;
    }
    glUseProgram(mesh.shaderProgram);
    glUniform3fv(mesh.glUniAmbientColor, 1, &(mesh.ambientColor[0]));
    mesh.hasDefaultAmbientColor = false;
}
void GLMeshCtrl::overrideAmbientStrength(GLMesh& mesh, float ambientStrength) {
    mesh.ambientStrength = ambientStrength;
    glUseProgram(mesh.shaderProgram);
    glUniform1f(mesh.glUniAmbientStrength, mesh.ambientStrength);
    mesh.hasDefaultAmbientStrength = false;
}
void GLMeshCtrl::resetToDefaultColor(GLMesh& mesh) {
    overrideDiffuseColor(mesh, GLGlobalCtrl::getDefaultObjectColor(), true);
    mesh.hasDefaultColor = true;
}
void GLMeshCtrl::resetToDefaultAmbientColor(GLMesh& mesh) {
    overrideAmbientColor(mesh, GLGlobalCtrl::getDefaultAmbientColor(), true);
    mesh.hasDefaultAmbientColor = true;
}
void GLMeshCtrl::resetToDefaultAmbientStrength(GLMesh& mesh) {
    overrideAmbientStrength(mesh, GLGlobalCtrl::getDefaultAmbientStrength());
    mesh.hasDefaultAmbientStrength = true;
}
void GLMeshCtrl::resetToDefaultSpecularStrength(GLMesh& mesh) {
    overrideSpecularStrength(mesh, GLGlobalCtrl::getDefaultSpecularStrength());
    mesh.hasDefaultSpecularStrength = true;
}
void GLMeshCtrl::resetToDefaultShininess(GLMesh& mesh) {
    overrideShininess(mesh, GLGlobalCtrl::getDefaultShininess());
    mesh.hasDefaultShininess = true;
}