#pragma once
#include <glm/glm.hpp>
#include "../header/mesh.h"
#include "../header/shader.h"

class Renderer {
public:
    Renderer();
    
    void clear(float r, float g, float b, float a);
    void render(const Mesh& mesh, const Shader& shader, 
                const glm::mat4& model, const glm::mat4& view, 
                const glm::mat4& projection, const glm::vec3& cameraPos);
    
    void setLightPosition(const glm::vec3& pos) { m_lightPos = pos; }
    void setLightColor(const glm::vec3& color) { m_lightColor = color; }
    void setObjectColor(const glm::vec3& color) { m_objectColor = color; }
    
private:
    glm::vec3 m_lightPos;
    glm::vec3 m_lightColor;
    glm::vec3 m_objectColor;
};