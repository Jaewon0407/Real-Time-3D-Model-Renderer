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
        glm::mat4 model = glm::mat4(1.0f);
 
        renderer.render(mesh, shader, model, view, projection, cameraPos);
        window.swapBuffers();
        window.pollEvents();
    }
    
    return 0;
}