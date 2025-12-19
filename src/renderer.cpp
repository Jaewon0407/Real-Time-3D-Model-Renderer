#include "../header/renderer.h"
#include "../header/shader.h"
#include "../header/mesh.h"
#include <glad/glad.h>

Renderer::Renderer() 
    : m_lightPos(3.0f, 4.0f, 2.0f),
      m_lightColor(1.0f, 1.0f, 1.0f),
      m_objectColor(0.6f, 0.6f, 0.7f) {
    glEnable(GL_DEPTH_TEST);
}

void Renderer::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(const Mesh& mesh, const Shader& shader,
                     const glm::mat4& model, const glm::mat4& view,
                     const glm::mat4& projection, const glm::vec3& cameraPos) {
    shader.use();

    // Upload transformation matrices to GPU
    shader.setMat4("uModel", model);
    shader.setMat4("uView", view);
    shader.setMat4("uProjection", projection);

    // Upload lighting data to GPU
    shader.setVec3("uViewPos", cameraPos);
    shader.setVec3("uLightPos", m_lightPos);
    shader.setVec3("uLightColor", m_lightColor);
    shader.setVec3("uObjectColor", m_objectColor);
    
    mesh.draw();
}