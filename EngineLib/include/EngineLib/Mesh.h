#pragma once
#include <vector>
#include "EngineLib\Vertex.h"
// #include "Components\MeshRenderer_C.h"

typedef unsigned int GLuint;
typedef int GLsizei;

namespace EngineCore {

    namespace Component {
        class MeshRenderer;
    }

    class Mesh {
    friend class Component::MeshRenderer;
    public:
        Mesh(const std::string& path);
        Mesh(const Vertex* vertices, size_t verticesSize, const unsigned int* indices, size_t indicesSize);
        ~Mesh();

        void CreateGL();
        void DeleteGL();

    private:
        bool m_exists = false;

        GLuint m_vao, m_vbo, m_ebo;
        GLsizei m_indexCount;

        std::string m_path;
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;

        void Draw();
    };

}
