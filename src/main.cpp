#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE


#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

#include "../header/window.h"
#include "../header/shader.h"
#include "../header/mesh.h"
#include "../header/renderer.h"

#include "../extern/imgui/imgui.h"
#include "../extern/imgui/backends/imgui_impl_glfw.h"
#include "../extern/imgui/backends/imgui_impl_opengl3.h"

float g_cameraRadius = 5.0f;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    g_cameraRadius -= (float)yoffset * 0.5f;
}

int main() {
    Window window(800, 800, "Jaewon's Window");

    glfwSetScrollCallback(window.getHandle(), scroll_callback);

    Renderer renderer;
    Shader shader("shaders/cube.vert", "shaders/cube.frag");
    Mesh mesh("models/bird.obj");

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    double horizontalAngle = 0.0f;
    double verticleAngle = 0.0f;

    // ImGui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.getHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Model transform state
    glm::vec3 modelTranslation(0.0f, 0.0f, 0.0f);
    glm::vec3 modelRotation(0.0f, 0.0f, 0.0f); // Euler angles in degrees


    while (!window.shouldClose()) {
        // zoom in and out
        double cursorX, cursorY;
        glfwGetCursorPos(window.getHandle(), &cursorX, &cursorY);

        if (glfwGetKey(window.getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window.getHandle(), true);
        }

        if (glfwGetKey(window.getHandle(), GLFW_KEY_D) == GLFW_PRESS) {
            horizontalAngle += 0.005f;
        }

        if (glfwGetKey(window.getHandle(), GLFW_KEY_A) == GLFW_PRESS) {
            horizontalAngle -= 0.005f;
        }

        if (glfwGetKey(window.getHandle(), GLFW_KEY_W) == GLFW_PRESS) {
            verticleAngle += 0.005f;
        }

        if (glfwGetKey(window.getHandle(), GLFW_KEY_S) == GLFW_PRESS) {
            verticleAngle -= 0.005f;
        }

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Model Controls");
        ImGui::SliderFloat3("Translate", (float*)&modelTranslation, -10.0f, 10.0f);
        ImGui::SliderFloat3("Rotate (deg)", (float*)&modelRotation, -180.0f, 180.0f);
        ImGui::End();

        int width, height;
        window.getFramebufferSize(width, height);
        float aspect = (float)width / (float)height;
        glViewport(0, 0, width, height);

        renderer.clear(0.02f, 0.02f, 0.08f, 1.0f);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        glm::vec3 cameraPos(
            g_cameraRadius * cos(verticleAngle) * sin(horizontalAngle),
            g_cameraRadius * sin(verticleAngle),
            g_cameraRadius * cos(verticleAngle) * cos(horizontalAngle)
        );
        glm::vec3 target(0.0f, 0.0f, 0.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        glm::mat4 view = glm::lookAt(cameraPos, target, up);

        // Compose model matrix from ImGui controls
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, modelTranslation);
        model = glm::rotate(model, glm::radians(modelRotation.x), glm::vec3(1,0,0));
        model = glm::rotate(model, glm::radians(modelRotation.y), glm::vec3(0,1,0));
        model = glm::rotate(model, glm::radians(modelRotation.z), glm::vec3(0,0,1));

        renderer.render(mesh, shader, model, view, projection, cameraPos);

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.swapBuffers();
        window.pollEvents();
    }

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}