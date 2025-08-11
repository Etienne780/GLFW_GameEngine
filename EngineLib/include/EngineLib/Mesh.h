#pragma once
#include <vector>

namespace EngineCore {

    class Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        ~Mesh();

    private:
        bool m_exists = false;

        GLuint m_vao, m_vbo, m_ebo;
        GLsizei m_indexCount;

        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;

        void Draw() const;

        void Create();
        void Delete();
    };

}
