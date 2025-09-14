#include "EngineLib\FontManager.h"
#include "EngineLib\ResourceManager.h"

namespace EngineCore {

    static ResourceManager* g_instance;

    ResourceManager::ResourceManager() {
    }

    int ResourceManager::Init() {
        g_instance = new ResourceManager();
        Log::Info("Engine::ResourceManager: Initialized ResourceManager successfully");
        return FontManager::Init();
    }

    void ResourceManager::Shutdown() {
        g_instance->Cleanup();
        FontManager::Shutdown();
        delete g_instance;
        g_instance = nullptr;
    }

    ResourceManager* ResourceManager::GetInstance() {
        return g_instance;
    }

    #pragma region ACTION_GL_ASSET

    void ResourceManager::CreateGLTexture2D(Texture2DID id) {
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant create Texture2D GL object, id is invalid!");
            return;
        }

        auto it = m_texture2Ds.find(id);
        if (it == m_texture2Ds.end()) {
            Log::Error("ResourceManager: Cant create Texture2D GL object, id '{}' not found!", id.value);
            return; 
        }
        it->second->CreateGL();
    }

    void ResourceManager::CreateGLMesh(MeshID id) {
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant create Mesh GL object, id is invalid!");
            return;
        }

        auto it = m_meshes.find(id);
        if (it == m_meshes.end()) {
            Log::Error("ResourceManager: Cant create Mesh GL object, id '{}' not found!", id.value);
            return;
        }
        it->second->CreateGL();
    }

    void ResourceManager::CreateGLShader(ShaderID id) {
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant create GL ShaderProgramm, id is invalid!");
            return;
        }

        auto it = m_shaders.find(id);
        if (it == m_shaders.end()) {
            Log::Error("ResourceManager: Cant create GL ShaderProgramm, id '{}' not found!", id.value);
            return;
        }
        it->second->CreateGL();
    }

    void ResourceManager::DeleteGLTexture2D(Texture2DID id) {
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant delete Texture2D GL object, id is invalid!");
            return;
        }

        auto it = m_texture2Ds.find(id);
        if (it == m_texture2Ds.end()) {
            Log::Error("ResourceManager: Cant delete Texture2D GL object, id '{}' not found!", id.value);
            return;
        }
        it->second->DeleteGL();
    }

    void ResourceManager::DeleteGLMesh(MeshID id) {
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant delete Mesh GL object, id is invalid!");
            return;
        }

        auto it = m_meshes.find(id);
        if (it == m_meshes.end()) {
            Log::Error("ResourceManager: Cant delete Mesh GL object, id '{}' not found!", id.value);
            return;
        }
        it->second->DeleteGL();
    }

    void ResourceManager::DeleteGLShader(ShaderID id) {
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant delete GL ShaderProgramm, id is invalid!");
            return;
        }

        auto it = m_shaders.find(id);
        if (it == m_shaders.end()) {
            Log::Error("ResourceManager: Cant delete GL ShaderProgramm, id '{}' not found!", id.value);
            return;
        }
        it->second->DeleteGL();
    }

    #pragma endregion

    #pragma region GET_ASSET

    Texture2D* ResourceManager::GetTexture2D(Texture2DID id) {
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant get Texture2D, id is invalid!");
            return nullptr;
        }

        auto it = m_texture2Ds.find(id);
        if (it == m_texture2Ds.end()) {
            Log::Error("ResourceManager: Cant get Texture2D, id '{}' not found!", id.value);
            return nullptr;
        }
        return it->second.get();
    }

    Mesh* ResourceManager::GetMesh(MeshID id) {
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant get Mesh, id is invalid!");
            return nullptr;
        }

        auto it = m_meshes.find(id);
        if (it == m_meshes.end()) {
            Log::Error("ResourceManager: Cant get Mesh, id '{}' not found!", id.value);
            return nullptr;
        }

        return it->second.get();
    }

    Shader* ResourceManager::GetShader(ShaderID id) {
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant get Shader, id is invalid!");
            return nullptr;
        }

        auto it = m_shaders.find(id);
        if (it == m_shaders.end()) {
            Log::Error("ResourceManager: Cant get Shader, id '{}' not found!", id.value);
            return nullptr;
        }

        return it->second.get();
    }

    Material* ResourceManager::GetMaterial(MaterialID id) {
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant get Material, id is invalid!");
            return nullptr;
        }

        auto it = m_materials.find(id);
        if (it == m_materials.end()) {
            Log::Error("ResourceManager: Cant get Material, id '{}' not found!", id.value);
            return nullptr;
        }

        return it->second.get();
    }

    FontAsset* ResourceManager::GetFontAsset(FontID id) {
        if (id.value == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant get Font, id is invalid!");
            return nullptr;
        }

        auto it = m_fonts.find(id);
        if (it == m_fonts.end()) {
            Log::Error("ResourceManager: Cant get Font, id '{}' not found!", id.value);
            return nullptr;
        }

        return it->second.get();
    }

    #pragma endregion

    const FontAsset::Glyph& ResourceManager::GetFontGlyph(FontID id, char c, int pixelSize) {
        auto* fontAsset = GetFontAsset(id);
        if (fontAsset == nullptr) {
            Log::Error("ResourceManager: Failed to get Glyph, font asset with id {} not found", id.value);
            static FontAsset::Glyph dummy{};
            return dummy;
        }

        return fontAsset->GetGlyph(c, pixelSize);
    }

    unsigned int ResourceManager::GetFontAtlasTextureID(FontID id, int pixelSize) {
        auto* fontAsset = GetFontAsset(id);
        if (fontAsset == nullptr) {
            Log::Error("ResourceManager: Failed to get AtlasTextureID, font asset with id {} not found", id.value);
            return ENGINE_INVALID_ID;
        }

        return fontAsset->GetAtlasTextureID(pixelSize);
    }

    #pragma region ADD_ASSET_ACTION

    Texture2DID ResourceManager::AddTexture2DFromFile(const std::string& path, bool useAbsolutDir) {
        unsigned int id = GetNewUniqueId(AssetType::TEXTURE2D);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Texture2D, there are no ids left");
            return Texture2DID(ENGINE_INVALID_ID);
        }
        #endif
        m_texture2Ds.emplace(id, std::make_unique<Texture2D>(path, useAbsolutDir));
        return Texture2DID(id);
    }

    Texture2DID ResourceManager::AddTexture2DFromMemory(const unsigned char* data, int width, int height, int channels) {
        unsigned int id = GetNewUniqueId(AssetType::TEXTURE2D);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Texture2D, there are no ids left");
            return Texture2DID(ENGINE_INVALID_ID);
        }
        #endif
        m_texture2Ds.emplace(id, std::make_unique<Texture2D>(data, width, height, channels));
        return Texture2DID(id);
    }

    MeshID ResourceManager::AddMeshFromFile(const std::string& path) {
        unsigned int id = GetNewUniqueId(AssetType::MESH);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Mesh, there are no ids left");
            return MeshID(ENGINE_INVALID_ID);
        }
        #endif
        m_meshes.emplace(id, std::make_unique<Mesh>(path));
        return MeshID(id);
    }

    MeshID ResourceManager::AddMeshFromMemory(const Vertex* vertices, size_t verticesSize, const unsigned int* indices, size_t indicesSize) {
        unsigned int id = GetNewUniqueId(AssetType::MESH);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Mesh, there are no ids left");
            return MeshID(ENGINE_INVALID_ID);
        }
        #endif
        m_meshes.emplace(id, std::make_unique<Mesh>(vertices, verticesSize, indices, indicesSize));
        return MeshID(id);
    }

    ShaderID ResourceManager::AddShaderFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
        unsigned int id = GetNewUniqueId(AssetType::SHADER);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Shader, there are no ids left");
            return ShaderID(ENGINE_INVALID_ID);
        }

        if (!vertexPath.ends_with(".vert")) {
            Log::Error("ResourceManager: Cant add Shader, Vertex shader doesnt end with .vert");
            Log::Print(Log::levelError, "         {}", vertexPath);
            return ShaderID(ENGINE_INVALID_ID);
        }

        if (!fragmentPath.ends_with(".frag")) {
            Log::Error("ResourceManager: Cant add Shader, Fragment shader doesnt end with .frag");
            Log::Print(Log::levelError, "         {}", fragmentPath);
            return ShaderID(ENGINE_INVALID_ID);
        }
        #endif
        m_shaders.emplace(id, std::make_unique<Shader>(vertexPath, fragmentPath));
        return ShaderID(id);
    }

    ShaderID ResourceManager::AddShaderFromMemory(const std::string& vertexCode, const std::string& fragmentCode) {
        unsigned int id = GetNewUniqueId(AssetType::SHADER);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Shader, there are no ids left");
            return ShaderID(ENGINE_INVALID_ID);
        }
        #endif
        m_shaders.emplace(id, std::make_unique<Shader>(vertexCode, fragmentCode, SHADER_IsShaderCode));
        return ShaderID(id);
    }

    MaterialID ResourceManager::AddMaterial(ShaderID shaderID) {
        unsigned int id = GetNewUniqueId(AssetType::MATERIAL);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Material, there are no ids left");
            return MaterialID(ENGINE_INVALID_ID);
        }
        #endif

        Shader* s = GetShader(shaderID);
        if (!s) {
            Log::Error("ResourceManager: Material was created without shader, shader ID was invalid '{}'", shaderID.value);
            m_materials.emplace(id, std::make_unique<Material>());
            return MaterialID(id);
        }

        m_materials.emplace(id, std::make_unique<Material>(shaderID));
        return MaterialID(id);
    }

    FontID ResourceManager::AddFontFromFile(const std::string& path, bool useAbsolutDir) {
        unsigned int id = GetNewUniqueId(AssetType::FONT);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Font, there are no ids left");
            return FontID(ENGINE_INVALID_ID);
        }
        #endif
        m_fonts.emplace(id, std::make_unique<FontAsset>(FontManager::GetFTLib(), path, useAbsolutDir));
        return FontID(id);
    }

    FontID ResourceManager::AddFontFromMemory(const FT_Byte* data, FT_Long size) {
        unsigned int id = GetNewUniqueId(AssetType::FONT);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Font, there are no ids left");
            return FontID(ENGINE_INVALID_ID);
        }
        #endif
        m_fonts.emplace(id, std::make_unique<FontAsset>(FontManager::GetFTLib(), data, size));
        return FontID(id);
    }

    #pragma endregion

    #pragma region Delete_Asset

    void ResourceManager::DeleteAsset(Texture2DID id) {
        DeleteAssetInternal<Texture2DID, std::unique_ptr<Texture2D>>(m_texture2Ds, AssetType::TEXTURE2D, id);
    }

    void ResourceManager::DeleteAsset(MeshID id) {
        DeleteAssetInternal<MeshID, std::unique_ptr<Mesh>>(m_meshes, AssetType::MESH, id);
    }

    void ResourceManager::DeleteAsset(ShaderID id) {
        DeleteAssetInternal<ShaderID, std::unique_ptr<Shader>>(m_shaders, AssetType::SHADER, id);
    }

    void ResourceManager::DeleteAsset(MaterialID id) {
        DeleteAssetInternal<MaterialID, std::unique_ptr<Material>>(m_materials, AssetType::MATERIAL, id);
    }

    void ResourceManager::DeleteAsset(FontID id) {
        DeleteAssetInternal<FontID, std::unique_ptr<FontAsset>>(m_fonts, AssetType::FONT, id);
    }

    #pragma endregion

    unsigned int ResourceManager::GetNewUniqueId(AssetType counter) {
        return m_assetIDCounter.GetNewFreeID(counter);
    }

    void ResourceManager::Cleanup() {
        for (auto& [id, texture] : m_texture2Ds) { texture->DeleteGL(); }
        for (auto& [id, mesh] : m_meshes) { mesh->DeleteGL(); }
        for (auto& [id, shader] : m_shaders) { shader->DeleteGL(); }
        m_texture2Ds.clear();
        m_meshes.clear();
        m_shaders.clear();
        m_materials.clear();
        m_fonts.clear();
    }

}