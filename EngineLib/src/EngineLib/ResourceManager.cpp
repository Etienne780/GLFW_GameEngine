#include <CoreLib\Log.h>

#include "EngineLib\ResourceManager.h"

namespace EngineCore {

    ResourceManager& ResourceManager::GetInstance() {
        static ResourceManager instance;
        return instance;
    }

    void ResourceManager::CreateGLTexture2D(Asset_Texture2DID id) {
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

    void ResourceManager::CreateGLMesh(Asset_MeshID id) {
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

    void ResourceManager::CreateGLShader(Asset_ShaderID id) {
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

    void ResourceManager::DeleteGLTexture2D(Asset_Texture2DID id) {
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

    void ResourceManager::DeleteGLMesh(Asset_MeshID id) {
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

    void ResourceManager::DeleteGLShader(Asset_ShaderID id) {
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

    Texture2D* ResourceManager::GetTexture2D(Asset_Texture2DID id) {
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

    Mesh* ResourceManager::GetMesh(Asset_MeshID id) {
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

    Shader* ResourceManager::GetShader(Asset_ShaderID id) {
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

    Material* ResourceManager::GetMaterial(Asset_MaterialID id) {
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

    Asset_Texture2DID ResourceManager::AddTexture2DFromFile(const std::string& path, bool useAbsolutDir) {
        unsigned int id = GetNewUniqueId(IDCounter::TEXTURE2D);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Texture2D, there are no ids left");
            return Asset_Texture2DID(ENGINE_INVALID_ID);
        }
        #endif
        m_texture2Ds.emplace(id, std::make_unique<Texture2D>(path, useAbsolutDir));
        return Asset_Texture2DID(id);
    }

    Asset_Texture2DID ResourceManager::AddTexture2DFromMemory(const unsigned char* data, int width, int height, int channels) {
        unsigned int id = GetNewUniqueId(IDCounter::TEXTURE2D);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Texture2D, there are no ids left");
            return Asset_Texture2DID(ENGINE_INVALID_ID);
        }
        #endif
        m_texture2Ds.emplace(id, std::make_unique<Texture2D>(data, width, height, channels));
        return Asset_Texture2DID(id);
    }

    Asset_MeshID ResourceManager::AddMeshFromFile(const std::string& path) {
        unsigned int id = GetNewUniqueId(IDCounter::MESH);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Mesh, there are no ids left");
            return Asset_MeshID(ENGINE_INVALID_ID);
        }
        #endif
        m_meshes.emplace(id, std::make_unique<Mesh>(path));
        return Asset_MeshID(id);
    }

    Asset_MeshID ResourceManager::AddMeshFromMemory(const Vertex* vertices, size_t verticesSize, const unsigned int* indices, size_t indicesSize) {
        unsigned int id = GetNewUniqueId(IDCounter::MESH);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Mesh, there are no ids left");
            return Asset_MeshID(ENGINE_INVALID_ID);
        }
        #endif
        m_meshes.emplace(id, std::make_unique<Mesh>(vertices, verticesSize, indices, indicesSize));
        return Asset_MeshID(id);
    }

    Asset_ShaderID ResourceManager::AddShaderFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
        unsigned int id = GetNewUniqueId(IDCounter::SHADER);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Shader, there are no ids left");
            return Asset_ShaderID(ENGINE_INVALID_ID);
        }

        if (!vertexPath.ends_with(".vert")) {
            Log::Error("ResourceManager: Cant add Shader, Vertex shader doesnt end with .vert");
            Log::Print(Log::levelError, "         {}", vertexPath);
            return Asset_ShaderID(ENGINE_INVALID_ID);
        }

        if (!fragmentPath.ends_with(".frag")) {
            Log::Error("ResourceManager: Cant add Shader, Fragment shader doesnt end with .frag");
            Log::Print(Log::levelError, "         {}", fragmentPath);
            return Asset_ShaderID(ENGINE_INVALID_ID);
        }
        #endif
        m_shaders.emplace(id, std::make_unique<Shader>(vertexPath, fragmentPath));
        return Asset_ShaderID(id);
    }

    Asset_ShaderID ResourceManager::AddShaderFromMemory(const std::string& vertexCode, const std::string& fragmentCode) {
        unsigned int id = GetNewUniqueId(IDCounter::SHADER);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Shader, there are no ids left");
            return Asset_ShaderID(ENGINE_INVALID_ID);
        }
        #endif
        m_shaders.emplace(id, std::make_unique<Shader>(vertexCode, fragmentCode, SHADER_IsShaderCode));
        return Asset_ShaderID(id);
    }

    Asset_MaterialID ResourceManager::AddMaterial(Asset_ShaderID shaderID) {
        unsigned int id = GetNewUniqueId(IDCounter::MATERIAL);
        #ifndef NDEBUG
        if (id == ENGINE_INVALID_ID) {
            Log::Error("ResourceManager: Cant add Material, there are no ids left");
            return Asset_MaterialID(ENGINE_INVALID_ID);
        }
        #endif

        Shader* s = GetShader(shaderID);
        if (!s) {
            Log::Error("ResourceManager: Material was created without shader, shader ID was invalid '{}'", shaderID.value);
            m_materials.emplace(id, std::make_unique<Material>());
            return Asset_MaterialID(id);
        }

        m_materials.emplace(id, std::make_unique<Material>(shaderID));
        return Asset_MaterialID(id);
    }

    unsigned int ResourceManager::GetNewUniqueId(IDCounter counter) {
        return m_idCounters[counter]++;
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