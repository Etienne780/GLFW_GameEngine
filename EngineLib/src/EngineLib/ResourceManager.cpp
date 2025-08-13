#include <CoreLib\Log.h>

#include "EngineLib\EngineTypes.h"
#include "EngineLib\ResourceManager.h"

namespace EngineCore {

    ResourceManager& ResourceManager::GetInstance() {
        static ResourceManager instance;
        return instance;
    }

    void ResourceManager::CreateTexture2D(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Warn("ResourceManager: Cant create Texture2D, id is invalid!");
            return;
        }

        auto it = m_texture2Ds.find(id);
        if (it == m_texture2Ds.end()) {
            Log::Warn("ResourceManager: Cant create Texture2D, id '{}' not found!", id);
            return;
        }

        it->second->CreateGL();
    }

    void ResourceManager::CreateMesh(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Warn("ResourceManager: Cant create Mesh, id is invalid!");
            return;
        }

        auto it = m_meshes.find(id);
        if (it == m_meshes.end()) {
            Log::Warn("ResourceManager: Cant create Mesh, id '{}' not found!", id);
            return;
        }

        it->second->CreateGL();
    }

    void ResourceManager::DeleteTexture2D(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Warn("ResourceManager: Cant delete Texture2D, id is invalid!");
            return;
        }

        auto it = m_texture2Ds.find(id);
        if (it == m_texture2Ds.end()) {
            Log::Warn("ResourceManager: Cant delete Texture2D, id '{}' not found!", id);
            return;
        }

        it->second->DeleteGL();
    }

    void ResourceManager::DeleteMesh(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Warn("ResourceManager: Cant delete Mesh, id is invalid!");
            return;
        }

        auto it = m_meshes.find(id);
        if (it == m_meshes.end()) {
            Log::Warn("ResourceManager: Cant delete Mesh, id '{}' not found!", id);
            return;
        }

        it->second->DeleteGL();
    }

    Texture2D* ResourceManager::GetTexture2D(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Warn("ResourceManager: Cant get Texture2D, id is invalid!");
            return nullptr;
        }

        auto it = m_texture2Ds.find(id);
        if (it == m_texture2Ds.end()) {
            Log::Warn("ResourceManager: Cant get Texture2D, id '{}' not found!", id);
            return nullptr;
        }
        return it->second.get();
    }

    Mesh* ResourceManager::GetMesh(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Warn("ResourceManager: Cant get Mesh, id is invalid!");
            return nullptr;
        }

        auto it = m_meshes.find(id);
        if (it == m_meshes.end()) {
            Log::Warn("ResourceManager: Cant get Mesh, id '{}' not found!", id);
            return nullptr;
        }

        return it->second.get();
    }

    unsigned int ResourceManager::AddTexture2DFromFile(const std::string& path) {
        unsigned int id = GetNewUniqueId(ASSET_TEXTURE2D);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Warn("ResourceManager: Cant add Texture2D, there are no ids left");
            return ENGINE_INVALID_ID;
        }
        #endif
        m_texture2Ds.emplace(id, std::make_unique<Texture2D>(path.c_str()));
        return id;
    }

    unsigned int ResourceManager::AddTextureFromMemory(const unsigned char* data, int width, int height, int channels) {
        unsigned int id = GetNewUniqueId(ASSET_TEXTURE2D);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Warn("ResourceManager: Cant add Texture2D, there are no ids left");
            return ENGINE_INVALID_ID;
        }
        #endif
        m_texture2Ds.emplace(id, std::make_unique<Texture2D>(data, width, height, channels));
        return id;
    }

    unsigned int ResourceManager::AddMesh(Mesh& mesh) {
        unsigned int id = GetNewUniqueId(ASSET_Mesh);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Warn("ResourceManager: Cant add Mesh, there are no ids left");
            return ENGINE_INVALID_ID;
        }
        #endif
        // m_texture2Ds.emplace(id, std::make_unique<Mesh>(mesh));
        return id;
    }

    unsigned int ResourceManager::GetNewUniqueId(int assetIndex) {
        return m_idCounters[assetIndex];
    }

}