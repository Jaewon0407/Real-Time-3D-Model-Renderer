#include "../header/mesh.h"
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

Mesh::Mesh(const std::string& filepath) : VAO(0), VBO(0), EBO(0) {
    loadModel(filepath);
    normalizeModel(vertices); // Keep all the model sizes consistent
    setupMesh();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::loadModel(const std::string& filepath) {
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(filepath, 
        aiProcess_Triangulate | 
        aiProcess_GenNormals | 
        aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];

        unsigned int vertexOffset = vertices.size();
        
        // Process vertices
        for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
            Vertex vertex;
            
            // Position
            vertex.position.x = mesh->mVertices[j].x;
            vertex.position.y = mesh->mVertices[j].y;
            vertex.position.z = mesh->mVertices[j].z;
            
            // Normals
            if (mesh->HasNormals()) {
                vertex.normal.x = mesh->mNormals[j].x;
                vertex.normal.y = mesh->mNormals[j].y;
                vertex.normal.z = mesh->mNormals[j].z;
            } else {
                vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
            }
            
            // Texture coordinates
            if (mesh->mTextureCoords[0]) {
                vertex.texCoord.x = mesh->mTextureCoords[0][j].x;
                vertex.texCoord.y = mesh->mTextureCoords[0][j].y;
            } else {
                vertex.texCoord = glm::vec2(0.0f, 0.0f);
            }
            
            vertices.push_back(vertex);
        }
        
        // Process indices
        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            aiFace face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++) {
                indices.push_back(face.mIndices[k] + vertexOffset);
            }
        }
    }
}

void Mesh::normalizeModel(std::vector<Vertex>& vertices) {
    if (vertices.empty()) return;
    
    // Find bounding box
    glm::vec3 min(FLT_MAX), max(-FLT_MAX);
    for (const auto& vertex : vertices) {
        min = glm::min(min, vertex.position);
        max = glm::max(max, vertex.position);
    }
    
    // Calculate center and scale
    glm::vec3 center = (min + max) * 0.5f;
    glm::vec3 size = max - min;
    float maxSize = glm::max(glm::max(size.x, size.y), size.z);
    float scale = 2.0f / maxSize; // Normalize to fit in [-1, 1]
    
    // Apply transformation
    for (auto& vertex : vertices) {
        vertex.position = (vertex.position - center) * scale;
    }
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), 
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 indices.data(), GL_STATIC_DRAW);

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                         (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // TexCoord attribute (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                         (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}