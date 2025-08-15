#include <CoreLib\Log.h>

#include "EngineLib\EngineTypes.h"
#include "EngineLib\ResourceManager.h"

namespace EngineCore {

    ResourceManager& ResourceManager::GetInstance() {
        static ResourceManager instance;
        return instance;
    }

    void ResourceManager::CreateGLTexture2D(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant create Texture2D GL object, id is invalid!");
            return;
        }

        auto it = m_texture2Ds.find(id);
        if (it == m_texture2Ds.end()) {
            Log::Error("ResourceManager: Cant create Texture2D GL object, id '{}' not found!", id);
            return; 
        }

        it->second->CreateGL();
    }

    void ResourceManager::CreateGLMesh(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant create Mesh GL object, id is invalid!");
            return;
        }

        auto it = m_meshes.find(id);
        if (it == m_meshes.end()) {
            Log::Error("ResourceManager: Cant create Mesh GL object, id '{}' not found!", id);
            return;
        }

        it->second->CreateGL();
    }

    void ResourceManager::CreateGLShader(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant create GL ShaderProgramm, id is invalid!");
            return;
        }

        auto it = m_shaders.find(id);
        if (it == m_shaders.end()) {
            Log::Error("ResourceManager: Cant create GL ShaderProgramm, id '{}' not found!", id);
            return;
        }

        it->second->CreateGL();
    }

    void ResourceManager::DeleteGLTexture2D(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant delete Texture2D GL object, id is invalid!");
            return;
        }

        auto it = m_texture2Ds.find(id);
        if (it == m_texture2Ds.end()) {
            Log::Error("ResourceManager: Cant delete Texture2D GL object, id '{}' not found!", id);
            return;
        }

        it->second->DeleteGL();
    }

    void ResourceManager::DeleteGLMesh(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant delete Mesh GL object, id is invalid!");
            return;
        }

        auto it = m_meshes.find(id);
        if (it == m_meshes.end()) {
            Log::Error("ResourceManager: Cant delete Mesh GL object, id '{}' not found!", id);
            return;
        }

        it->second->DeleteGL();
    }

    void ResourceManager::DeleteGLShader(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant delete GL ShaderProgramm, id is invalid!");
            return;
        }

        auto it = m_shaders.find(id);
        if (it == m_shaders.end()) {
            Log::Error("ResourceManager: Cant delete GL ShaderProgramm, id '{}' not found!", id);
            return;
        }

        it->second->DeleteGL();
    }

    Texture2D* ResourceManager::GetTexture2D(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant get Texture2D, id is invalid!");
            return nullptr;
        }

        auto it = m_texture2Ds.find(id);
        if (it == m_texture2Ds.end()) {
            Log::Error("ResourceManager: Cant get Texture2D, id '{}' not found!", id);
            return nullptr;
        }
        return it->second.get();
    }

    Mesh* ResourceManager::GetMesh(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant get Mesh, id is invalid!");
            return nullptr;
        }

        auto it = m_meshes.find(id);
        if (it == m_meshes.end()) {
            Log::Error("ResourceManager: Cant get Mesh, id '{}' not found!", id);
            return nullptr;
        }

        return it->second.get();
    }

    Shader* ResourceManager::GetShader(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant get Shader, id is invalid!");
            return nullptr;
        }

        auto it = m_shaders.find(id);
        if (it == m_shaders.end()) {
            Log::Error("ResourceManager: Cant get Shader, id '{}' not found!", id);
            return nullptr;
        }

        return it->second.get();
    }

    Material* ResourceManager::GetMaterial(unsigned int id) {
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant get Material, id is invalid!");
            return nullptr;
        }

        auto it = m_materials.find(id);
        if (it == m_materials.end()) {
            Log::Error("ResourceManager: Cant get Material, id '{}' not found!", id);
            return nullptr;
        }

        return it->second.get();
    }

    unsigned int ResourceManager::AddTexture2DFromFile(const std::string& path) {
        unsigned int id = GetNewUniqueId(ASSET_TEXTURE2D);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Texture2D, there are no ids left");
            return ENGINE_INVALID_ID;
        }
        #endif
        m_texture2Ds.emplace(id, std::make_unique<Texture2D>(path.c_str()));
        return id;
    }

    unsigned int ResourceManager::AddTexture2DFromMemory(const unsigned char* data, int width, int height, int channels) {
        unsigned int id = GetNewUniqueId(ASSET_TEXTURE2D);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Texture2D, there are no ids left");
            return ENGINE_INVALID_ID;
        }
        #endif
        m_texture2Ds.emplace(id, std::make_unique<Texture2D>(data, width, height, channels));
        return id;
    }

    unsigned int ResourceManager::AddMeshFromFile(const std::string& path) {
        unsigned int id = GetNewUniqueId(ASSET_MESH);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Mesh, there are no ids left");
            return ENGINE_INVALID_ID;
        }
        #endif
        m_meshes.emplace(id, std::make_unique<Mesh>(path));
        return id;
    }

    unsigned int ResourceManager::AddMeshFromMemory(const Vertex* vertices, size_t verticesSize, const unsigned int* indices, size_t indicesSize) {
        unsigned int id = GetNewUniqueId(ASSET_MESH);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Mesh, there are no ids left");
            return ENGINE_INVALID_ID;
        }
        #endif
        m_meshes.emplace(id, std::make_unique<Mesh>(vertices, verticesSize, indices, indicesSize));
        return id;
    }

    unsigned int ResourceManager::AddShaderFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
        unsigned int id = GetNewUniqueId(ASSET_SHADER);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Shader, there are no ids left");
            return ENGINE_INVALID_ID;
        }

        if (!vertexPath.ends_with(".vert")) {
            Log::Error("ResourceManager: Cant add Shader, Vertex shader doesnt end with .vert");
            Log::Print(Log::levelError, "         {}", vertexPath);
            return ENGINE_INVALID_ID;
        }

        if (!fragmentPath.ends_with(".frag")) { // hier fragmentPath prüfen
            Log::Error("ResourceManager: Cant add Shader, Fragment shader doesnt end with .frag");
            Log::Print(Log::levelError, "         {}", fragmentPath);
            return ENGINE_INVALID_ID;
        }
        #endif
        m_shaders.emplace(id, std::make_unique<Shader>(vertexPath, fragmentPath));
        return id;
    }

    unsigned int ResourceManager::AddShaderFromMemory(const std::string& vertexCode, const std::string& fragmentCode) {
        unsigned int id = GetNewUniqueId(ASSET_SHADER);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Shader, there are no ids left");
            return ENGINE_INVALID_ID;
        }
        #endif
        m_shaders.emplace(id, std::make_unique<Shader>(vertexCode, fragmentCode, SHADER_IsShaderCode));
        return id;
    }

    unsigned int ResourceManager::AddMaterial(unsigned int shaderID) {
        unsigned int id = GetNewUniqueId(ASSET_MATERIAL);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Material, there are no ids left");
            return ENGINE_INVALID_ID;
        }
        #endif

        Shader* s = GetShader(shaderID);
        if (!s) {
            Log::Error("ResourceManager: Material was created without shader, shader ID was invalid '{}'", shaderID);
            m_materials.emplace(id, std::make_unique<Material>());
            return id;
        }

        m_materials.emplace(id, std::make_unique<Material>(shaderID));
        return id;
    }

    unsigned int ResourceManager::GetNewUniqueId(int assetIndex) {
        return m_idCounters[assetIndex]++;
    }

    void ResourceManager::Cleanup() {
        m_materials.clear();
        for (auto& [id, texture] : m_texture2Ds) { texture->DeleteGL(); }
        for (auto& [id, mesh] : m_meshes) { mesh->DeleteGL(); }
        for (auto& [id, shader] : m_shaders) { shader->DeleteGL(); }
        m_texture2Ds.clear();
        m_meshes.clear();
        m_shaders.clear();
    }

}