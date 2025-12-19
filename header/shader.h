#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    
    void use() const;
    void setMat4(const char* name, const glm::mat4& mat) const;
    void setVec3(const char* name, const glm::vec3& vec) const;
    void setVec3(const char* name, float x, float y, float z) const;
    
    GLuint getID() const { return m_program; }
    
private:
    GLuint m_program;
    
    std::string loadTextFile(const char* path);
    GLuint compileShader(GLenum type, const std::string& source);
};