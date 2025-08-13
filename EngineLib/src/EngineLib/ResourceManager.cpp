#include "EngineLib\ResourceManager.h"

namespace EngineCore {

    ResourceManager& ResourceManager::Instance() {
        static ResourceManager instance;
        return instance;
    }

    void ResourceManager::AddTexture2D(Texture2D& texture) {
        unsigned int id = GetNewUniqueId(ASSET_TEXTURE2D);
        m_texture2Ds.emplace(id, std::make_unique<Texture2DAsset>(texture, texture.GetPath()));
    }

    void ResourceManager::AddMesh(Mesh& texture) {
        unsigned int id = GetNewUniqueId(ASSET_TEXTURE2D);
        //m_texture2Ds.emplace(id, std::make_unique<Texture2DAsset>(texture, texture.GetPath()));
    }

    unsigned int ResourceManager::GetNewUniqueId(int assetIndex) {
        return m_idCounters[assetIndex];
    }

}