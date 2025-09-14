#pragma once
#include <unordered_map>
#include <memory>

#include "Texture2D.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "FontAsset.h"
#include "IDManager.h"
#include "EngineTypes.h"

namespace EngineCore {

    enum class AssetType {
        TEXTURE2D = 0,
        MESH,
        SHADER,
        MATERIAL,
        FONT
    };

    class ResourceManager {
    friend class Engine;
    public:
        ResourceManager();
        static int Init();
        static void Shutdown();

        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;

        static ResourceManager* GetInstance();

        // Creates opengl object
        void CreateGLTexture2D(Texture2DID id);
        // Creates opengl object
        void CreateGLMesh(MeshID id);
        // Creates opengl object
        void CreateGLShader(ShaderID id);
        
        // Deletes opengl object
        void DeleteGLTexture2D(Texture2DID id);
        // Deletes opengl object
        void DeleteGLMesh(MeshID id);
        // Deletes opengl object
        void DeleteGLShader(ShaderID id);

        Texture2D* GetTexture2D(Texture2DID id);
        Mesh* GetMesh(MeshID id);
        Shader* GetShader(ShaderID id);
        Material* GetMaterial(MaterialID id);
        FontAsset* GetFontAsset(FontID id);

        const FontAsset::Glyph& GetFontGlyph(FontID id, char c, int pixelSize);
        unsigned int GetFontAtlasTextureID(FontID id, int pixelSize);

        Texture2DID AddTexture2DFromFile(const std::string& path, bool useAbsolutDir = false);
        Texture2DID AddTexture2DFromMemory(const unsigned char* data, int width, int height, int channels);
        MeshID AddMeshFromFile(const std::string& path);
        MeshID AddMeshFromMemory(const Vertex* vertices, size_t verticesSize, const unsigned int* indices, size_t indicesSize);
        ShaderID AddShaderFromFile(const std::string& vertexPath, const std::string& fragmentPath);
        ShaderID AddShaderFromMemory(const std::string& vertexCode, const std::string& fragmentCode);
        MaterialID AddMaterial(ShaderID shaderID);
        FontID AddFontFromFile(const std::string& path, bool useAbsolutDir = false);
        /*
        * @brief IMPORTANT the data ptr has to be alive as long as this FontAsset exist
        */
        FontID AddFontFromMemory(const FT_Byte* data, FT_Long size);

    private:
        struct AssetIDCounter {
            IDManager m_texture2DIDCounter;
            IDManager m_meshIDCounter;
            IDManager m_shaderIDCounter;
            IDManager m_materialsIDCounter;
            IDManager m_fontAssetsIDCounter;
            IDManager m_default;

            AssetIDCounter() = default;
            unsigned int operator[](AssetType counter) {
                switch (counter) {
                case AssetType::TEXTURE2D: return m_texture2DIDCounter.GetNewUniqueIdentifier();
                case AssetType::MESH: return m_meshIDCounter.GetNewUniqueIdentifier();
                case AssetType::SHADER: return m_shaderIDCounter.GetNewUniqueIdentifier();
                case AssetType::MATERIAL: return m_materialsIDCounter.GetNewUniqueIdentifier();
                case AssetType::FONT: return m_fontAssetsIDCounter.GetNewUniqueIdentifier();
                default: return m_default.GetNewUniqueIdentifier();
                }
            }
        };

        AssetIDCounter m_assetIDCounter;
        std::unordered_map<Texture2DID, std::unique_ptr<Texture2D>> m_texture2Ds;
        std::unordered_map<MeshID, std::unique_ptr<Mesh>> m_meshes;
        std::unordered_map<ShaderID, std::unique_ptr<Shader>> m_shaders;
        std::unordered_map<MaterialID, std::unique_ptr<Material>> m_materials;
        std::unordered_map<FontID, std::unique_ptr<FontAsset>> m_fonts;

        unsigned int GetNewUniqueId(AssetType counter);
        void Cleanup();
    };

}