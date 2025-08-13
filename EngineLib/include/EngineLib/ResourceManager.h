#pragma once
#include <unordered_map>
#include <memory>

#include "Texture2D.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

namespace EngineCore {

    class ResourceManager {
    public:
        static ResourceManager& GetInstance();

        ResourceManager(const ResourceManager&) = delete;
        void operator=(const ResourceManager&) = delete;

        // Creates opengl object
        void CreateTexture2D(unsigned int id);
        void CreateMesh(unsigned int id);
        
        // Deletes opengl object
        void DeleteTexture2D(unsigned int id);
        void DeleteMesh(unsigned int id);

        Texture2D* GetTexture2D(unsigned int id);
        Mesh* GetMesh(unsigned int id);

        unsigned int AddTexture2DFromFile(const std::string& path);
        unsigned int AddTextureFromMemory(const unsigned char* data, int width, int height, int channels);
        unsigned int AddMesh(Mesh& mesh);

    private:
        int ASSET_TEXTURE2D = 0;
        int ASSET_Mesh = 1;
        struct IDCounters {
            unsigned int m_texture2DIDCounter = 0;
            unsigned int m_MeshIDCounter = 0;

            IDCounters() = default;

            unsigned int& operator[](int index) {
                switch (index) {
                case 0: return m_texture2DIDCounter;
                case 1: return m_MeshIDCounter;
                }
            }
        };

        ResourceManager() = default;

        unsigned int GetNewUniqueId(int assetIndex);

        IDCounters m_idCounters;
        std::unordered_map<unsigned int, std::unique_ptr<Texture2D>> m_texture2Ds;
        std::unordered_map<unsigned int, std::unique_ptr<Mesh>> m_meshes;
        std::unordered_map<unsigned int, std::unique_ptr<Shader>> m_shaders;
        std::unordered_map<unsigned int, std::unique_ptr<Material>> m_materials;
    };

}