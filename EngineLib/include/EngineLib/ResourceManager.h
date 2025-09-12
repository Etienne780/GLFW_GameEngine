#pragma once
#include <unordered_map>
#include <memory>

#include "Texture2D.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "EngineTypes.h"

namespace EngineCore {

    class ResourceManager {
    friend class Engine;
    public:
        static ResourceManager& GetInstance();

        ResourceManager(const ResourceManager&) = delete;
        void operator=(const ResourceManager&) = delete;

        // Creates opengl object
        void CreateGLTexture2D(Asset_Texture2DID id);
        // Creates opengl object
        void CreateGLMesh(Asset_MeshID id);
        // Creates opengl object
        void CreateGLShader(Asset_ShaderID id);
        
        // Deletes opengl object
        void DeleteGLTexture2D(Asset_Texture2DID id);
        // Deletes opengl object
        void DeleteGLMesh(Asset_MeshID id);
        // Deletes opengl object
        void DeleteGLShader(Asset_ShaderID id);

        Texture2D* GetTexture2D(Asset_Texture2DID id);
        Mesh* GetMesh(Asset_MeshID id);
        Shader* GetShader(Asset_ShaderID id);
        Material* GetMaterial(Asset_MaterialID id);

        Asset_Texture2DID AddTexture2DFromFile(const std::string& path, bool useAbsolutDir = false);
        Asset_Texture2DID AddTexture2DFromMemory(const unsigned char* data, int width, int height, int channels);
        Asset_MeshID AddMeshFromFile(const std::string& path);
        Asset_MeshID AddMeshFromMemory(const Vertex* vertices, size_t verticesSize, const unsigned int* indices, size_t indicesSize);
        Asset_ShaderID AddShaderFromFile(const std::string& vertexPath, const std::string& fragmentPath);
        Asset_ShaderID AddShaderFromMemory(const std::string& vertexCode, const std::string& fragmentCode);
        Asset_MaterialID AddMaterial(Asset_ShaderID shaderID);

    private:
        enum class IDCounter{
            TEXTURE2D = 0,
            MESH,
            SHADER,
            MATERIAL,
        };

        struct IDCounters {
            unsigned int m_texture2DIDCounter = 0;
            unsigned int m_meshIDCounter = 0;
            unsigned int m_shaderIDCounter = 0;
            unsigned int m_materialsIDCounter = 0;
            unsigned int m_default = 0;

            IDCounters() = default;
            unsigned int& operator[](IDCounter counter) {
                switch (counter) {
                case IDCounter::TEXTURE2D: return m_texture2DIDCounter;
                case IDCounter::MESH: return m_meshIDCounter;
                case IDCounter::SHADER: return m_shaderIDCounter;
                case IDCounter::MATERIAL: return m_materialsIDCounter;
                default: return m_default;
                }
            }
        };

        ResourceManager() = default;

        unsigned int GetNewUniqueId(IDCounter counter);

        IDCounters m_idCounters;
        std::unordered_map<Asset_Texture2DID, std::unique_ptr<Texture2D>> m_texture2Ds;
        std::unordered_map<Asset_MeshID, std::unique_ptr<Mesh>> m_meshes;
        std::unordered_map<Asset_ShaderID, std::unique_ptr<Shader>> m_shaders;
        std::unordered_map<Asset_MaterialID, std::unique_ptr<Material>> m_materials;

        void Cleanup();
    };

}