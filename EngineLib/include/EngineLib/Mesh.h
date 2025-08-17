#pragma once
#include <vector>
#include "EngineLib\Vertex.h"
// #include "Components\MeshRenderer_C.h"

typedef unsigned int GLuint;
typedef int GLsizei;
class Matrix4x4;

namespace EngineCore {

    class Mesh {
    friend class Renderer;
    public:
        Mesh(const std::string& path);
        Mesh(const Vertex* vertices, size_t verticesSize, const unsigned int* indices, size_t indicesSize);
        ~Mesh();

        void CreateGL();
        void DeleteGL();

    private:
        bool m_exists = false;

        GLuint m_vao = 0, m_vbo = 0, m_ebo = 0;
        GLuint m_instanceVBO = 0;
        GLsizei m_indexCount = 0;

        std::string m_path;
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;

        void DrawInstanced(int instanceCount, const std::vector<Matrix4x4>& matrices);
    };

}
