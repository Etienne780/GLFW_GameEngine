#pragma once
#include <unordered_map>
#include <memory>

#include "Texture2D.h"
#include "Mesh.h"

namespace EngineCore {

    class ResourceManager {
    public:
        static ResourceManager& Instance();

        ResourceManager(const ResourceManager&) = delete;
        void operator=(const ResourceManager&) = delete;

        void LoadTexture2D(unsigned int id);
        void UnloadTexture2D(unsigned int id);
        void GetTexture2D(unsigned int id);
        
        void LoadMesh(unsigned int id);
        void UnloadMesh(unsigned int id);
        void GetMesh(unsigned int id);

    private:
        struct Asset;
        struct Texture2DAsset;
        struct MeshAsset;
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

        void AddTexture2D(Texture2D& texture);
        void AddMesh(Mesh& texture);

        unsigned int GetNewUniqueId(int assetIndex);

        IDCounters m_idCounters;
        std::unordered_map<unsigned int, std::unique_ptr<Texture2DAsset>> m_texture2Ds;
        std::unordered_map<unsigned int, std::unique_ptr<MeshAsset>> m_meshes;

        struct Asset {
            bool m_isBacked = false;
            std::string m_path;
            unsigned int m_id = ENGINE_INVALID_ID;

            Asset(const std::string& path) : m_isBacked(false), m_path(path) {};
            Asset(unsigned int id) : m_isBacked(true), m_id(id) {};
        };

        struct Texture2DAsset : Asset {
            Texture2D m_texture;

            Texture2DAsset(Texture2D& texture, const std::string& path) : Asset(path), m_texture(texture) {};
            Texture2DAsset(Texture2D& texture, unsigned int id) : Asset(id), m_texture(texture) {};
        };

        struct MeshAsset : Asset {
            Mesh m_mesh;

            MeshAsset(Mesh& mesh, std::string& path) : Asset(path), m_mesh(mesh) {};
            MeshAsset(Mesh& mesh, unsigned int id) : Asset(id), m_mesh(mesh) {};
        };
    };

}