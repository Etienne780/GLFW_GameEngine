#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "CoreLib\Log.h"
#include "EngineLib\Vertex.h"

#include "EngineLib\Mesh.h"


namespace EngineCore {

    Mesh::Mesh(const std::string& path) {
        Log::Info("Mesh(std::string) is not implemented");
    }

    Mesh::Mesh(const Vertex* vertices, size_t verticesSize, const unsigned int* indices, size_t indicesSize) {
        if (vertices && verticesSize > 0) {
            m_vertices.assign(vertices, vertices + verticesSize);
        }
        else {
            m_vertices.clear();
        }

        if (indices && indicesSize > 0) {
            m_indices.assign(indices, indices + indicesSize);
        }
        else {
            m_indices.clear();
        }

        m_indexCount = static_cast<GLsizei>(m_indices.size());
        CreateGL();
    }

    Mesh::~Mesh() {
       DeleteGL();
    }

    void Mesh::Draw() {
        if (!m_exists) {
            CreateGL();
            m_exists = true;
        }

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    }

    void Mesh::CreateGL() {
        if (m_exists) return;

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

        // Attribute setzen (Position, UV, Normals etc.)
        GLsizei vertexSize = 8 * sizeof(float);
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);
        glEnableVertexAttribArray(0);
        // UV attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        m_exists = true;
    }

    void Mesh::DeleteGL() {
        if (!m_exists) return;

        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);

        m_exists = false;
    }

}