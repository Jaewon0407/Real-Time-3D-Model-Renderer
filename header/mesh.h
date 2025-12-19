#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Mesh {
public:
    Mesh(const std::string& filepath);
    ~Mesh();

    void draw() const;
    
    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<unsigned int>& getIndices() const { return indices; }

private:
    void loadModel(const std::string& filepath);
    void setupMesh();
    void normalizeModel(std::vector<Vertex>& vertices);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    unsigned int VAO, VBO, EBO;
};

#endif