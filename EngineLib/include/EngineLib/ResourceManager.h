#pragma once
#include <unordered_map>
#include <memory>
#include <CoreLib\Log.h>

#include "Texture2D.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "FontAsset.h"
#include "IDManager.h"
#include "EngineTypes.h"

namespace EngineCore {
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

        void DeleteAsset(Texture2DID id);
        void DeleteAsset(MeshID id);
        void DeleteAsset(ShaderID id);
        void DeleteAsset(MaterialID id);
        void DeleteAsset(FontID id);

    private:
        enum class AssetType {
            TEXTURE2D = 0,
            MESH,
            SHADER,
            MATERIAL,
            FONT
        };

        struct AssetIDCounter {
            IDManager m_texture2DIDCounter;
            IDManager m_meshIDCounter;
            IDManager m_shaderIDCounter;
            IDManager m_materialsIDCounter;
            IDManager m_fontAssetsIDCounter;
            IDManager m_default;

            AssetIDCounter() = default;
            unsigned int GetNewFreeID(AssetType type) {
                switch (type) {
                case AssetType::TEXTURE2D: return m_texture2DIDCounter.GetNewUniqueIdentifier();
                case AssetType::MESH: return m_meshIDCounter.GetNewUniqueIdentifier();
                case AssetType::SHADER: return m_shaderIDCounter.GetNewUniqueIdentifier();
                case AssetType::MATERIAL: return m_materialsIDCounter.GetNewUniqueIdentifier();
                case AssetType::FONT: return m_fontAssetsIDCounter.GetNewUniqueIdentifier();
                default: return m_default.GetNewUniqueIdentifier();
                }
            }

            void FreeID(AssetType type, unsigned int id) {
                switch (type) {
                case AssetType::TEXTURE2D: m_texture2DIDCounter.FreeUniqueIdentifier(id); break;
                case AssetType::MESH: m_meshIDCounter.FreeUniqueIdentifier(id); break;
                case AssetType::SHADER: m_shaderIDCounter.FreeUniqueIdentifier(id); break;
                case AssetType::MATERIAL: m_materialsIDCounter.FreeUniqueIdentifier(id); break;
                case AssetType::FONT: m_fontAssetsIDCounter.FreeUniqueIdentifier(id); break;
                default: m_default.FreeUniqueIdentifier(id); break;
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

        inline const char* AssetTypeToString(AssetType type) {
            switch (type) {
            case AssetType::TEXTURE2D: return "Texture2D";
            case AssetType::MESH:      return "Mesh";
            case AssetType::SHADER:    return "Shader";
            case AssetType::MATERIAL:  return "Material";
            case AssetType::FONT:      return "Font";
            default:                   return "Unknown";
            }
        }

        template<typename IDType, typename T>
        void DeleteAssetInternal(std::unordered_map<IDType, T>& container, AssetType type, IDType id) {
            auto it = container.find(id);
            if (it != container.end()) {
                container.erase(it);
                m_assetIDCounter.FreeID(type, id.value);
            }
            else {
                Log::Error("ResourceManager: Could not delete {} with id {}", AssetTypeToString(type), id.value);
            }
        }

    };

}