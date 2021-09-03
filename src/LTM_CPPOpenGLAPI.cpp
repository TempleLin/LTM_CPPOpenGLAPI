#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "headers/gl_setup.hpp"
#include "headers/gl_mesh_types.hpp"
#include "headers/gl_control.hpp"
#include "headers/gl_input.hpp"
#include "headers/gl_values.hpp"

#define CMAKE_CHECK_CONFIGS
#include <LTM_CPPOpenGLAPIConfig.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

int main() {
    CHECK_CONFIGS_FROM_CMAKE();
    
    GLFWwindow* window = configureOpenGL();
    unsigned int vertexShaderHandle{ 0 }, fragmentShaderHandle{ 0 }, shaderProgramHandle{ 0 };
    createShaderProgram(vertexShaderHandle, "shaders/shaderVS.glsl", fragmentShaderHandle, "shaders/shaderFS.glsl", shaderProgramHandle);
    glUseProgram(shaderProgramHandle);

    unsigned int vaos[2], vbos[2];
    createVAOs(2, vaos);
    createVBOs(2, vbos);
    BasicCubeMesh basicLightingCube0("Basic Lighting Cube 0", glm::vec3(0.5f, 0.8f, -1.0f), shaderProgramHandle, vaos[0], vbos[0]);
    basicLightingCube0.setTexture0("assets/imgs/container.jpg");
    basicLightingCube0.setColor(glm::vec4(0, 0, 1, 1), true);

    createShaderProgram(vertexShaderHandle, "shaders/shaderVS.glsl", fragmentShaderHandle, "shaders/shaderFS.glsl", shaderProgramHandle);
    BasicCubeMesh basicLightingCube1("Basic Lighting Cube 1", glm::vec3(-0.5f, -0.3f, -0.3f), shaderProgramHandle, vaos[1], vbos[1]);
    basicLightingCube1.setTexture0("assets/imgs/container.jpg");
    basicLightingCube1.setColor(glm::vec4(1, 0, 0, 1), true);

    // Our state (Dear ImGUI)
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    enableMeshesTransparency();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! 
        //  You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        // 3. Show another simple window.
        if (show_another_window) {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        
        glClearColor(viewBackGroundColor.x, viewBackGroundColor.y, viewBackGroundColor.z, viewBackGroundColor.w);
        // @Clear both color buffer and Z-depth buffer before each frame render.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setDeltaTime();

        setMeshCoordSystem(basicLightingCube0.getShaderProgram());
        setBasicMeshSpawnPos(basicLightingCube0);
        spinBasicMeshAnim(basicLightingCube0);
        drawBasicMesh(basicLightingCube0);

        setMeshCoordSystem(basicLightingCube1.getShaderProgram());
        setBasicMeshSpawnPos(basicLightingCube1);
        spinBasicMeshAnim(basicLightingCube1);
        drawBasicMesh(basicLightingCube1);

        processInput(window);

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glBindVertexArray(0);
    }

    // Dear ImGUI Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    cleanGLObjectsGarbage();
}