#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <CoreLib\Log.h>
#include <CoreLib\Math\Matrix4x4.h>

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

    void Mesh::DrawInstanced(int instanceCount, const std::vector<Matrix4x4>& matrices) {
        if (!m_exists) {
            CreateGL();
        }

        std::vector<float> glMatrices;
        glMatrices.reserve(instanceCount * 16);
        for (const auto& m : matrices) {
            const float* data = m.ToOpenGLData();
            glMatrices.insert(glMatrices.end(), data, data + 16);
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, glMatrices.size() * sizeof(float), glMatrices.data(), GL_DYNAMIC_DRAW);

        glBindVertexArray(m_vao);
        glDrawElementsInstanced(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0, instanceCount);
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
        // Normal attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Instance buffer fort modelMatrix (4x4)
        glGenBuffers(1, &m_instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        // reserve data (fill later)
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

        // Attribut-Location 3..6 für die Matrix-Spalten
        std::size_t vec4Size = sizeof(float) * 4;
        std::size_t mat4Size = sizeof(float) * 16;

        for (int i = 0; i < 4; i++) {
            glEnableVertexAttribArray(3 + i);
            glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, mat4Size, (void*)(i * vec4Size));
            glVertexAttribDivisor(3 + i, 1); // per instance
        }

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