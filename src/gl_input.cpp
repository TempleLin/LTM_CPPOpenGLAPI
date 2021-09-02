#include <iostream>
#include "headers/gl_input.hpp"
#include "headers/gl_values.hpp"
#include <GLFW/glfw3.h>

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX{ windowWidth / 2 }, lastY{ windowHeight / 2 };
    /*
    * @To make sure the first time mouse clicks inside the window won't cause huge movement in mouse.
    *  Instead will be set as first default value.
    */
    static bool firstMouse{ true };

    /*
    * @Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to
    *  the right so we initially rotate a bit to the left.
    */
    static float yaw = -90.0f;
    static float pitch = 0.0f;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    /*
    * @Ensures will not look above the sky range or over the feet range.
    * @89 instead of 90 degrees is bc 90 will cause the look rotation to flip.
    */
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    /*
    * @Set the front direction of the camera.
    * @The use of cos(glm::radians(pitch)) is to get the projection of the hypotenuse on the X-Z plane.
    * @After projection will then convert it to the X or Z axis through sin/cos.
    * @See image "CameraLookPitchRoll.png" from "ImageNotes" folder for clearer understanding.
    */
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    /*
    * @Setting the fov(Field of View) changes the look range of the view. 
    *  Since it must be projected to the same NDC(Normalized Device Coordinates), it will give the illusion of zooming.
    */
    fov -= (float)yoffset;
    if (fov < fovMin)
        fov = fovMin;
    if (fov > fovMax)
        fov = fovMax;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    /*
    * @Use up and down arrow keys to increase/decrease camera movement speed.
    */
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cameraMoveSpeed += 0.1f;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cameraMoveSpeed -= (cameraMoveSpeed >= 0.1f) ? 0.01f : 0;
    }

    /*
    * @Used W, A, S, D keys to move camera.
    */
    float cameraSpeedCurrentFrame = cameraMoveSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeedCurrentFrame * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeedCurrentFrame * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // @Use cross multiply matrix to multiply front and up to get the right direction of the camera.
        // @Normalize the value to 0 ~ 1 to assure constant value.
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeedCurrentFrame;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeedCurrentFrame;
    }
}