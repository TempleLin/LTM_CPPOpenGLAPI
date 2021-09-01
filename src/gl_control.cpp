#include "headers/gl_control.hpp"
#include "headers/gl_values.hpp"
#include "headers/gl_mesh_types.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


void setMeshCoordSystem(unsigned int& shaderProgram) {
    glUseProgram(shaderProgram);
    //std::cout << glGetError() << std::endl;

    glEnable(GL_DEPTH_TEST);

    modelMatrix = glm::mat4(1.0f);
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

    projectionMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    static unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    static unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void setBasicMeshSpawnPos(GLMesh& mesh) {
    modelMatrix = glm::translate(modelMatrix, mesh.getPosition());
}

void spinBasicMeshAnim(GLMesh& mesh) {
    modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));
}

void drawBasicMesh(GLMesh& mesh) {
    glBindVertexArray(mesh.getVAO());
    static unsigned int modelLoc = glGetUniformLocation(mesh.getShaderProgram(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    if (mesh.isTextureEnabled()) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.getTexture0());
    }
    glUseProgram(mesh.getShaderProgram());
    glDrawArrays(GL_TRIANGLES, 0, mesh.getVerticesCount());
}