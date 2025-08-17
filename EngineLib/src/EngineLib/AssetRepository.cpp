#include <vector>
#include <glad/glad.h>
#include <CoreLib\Log.h>

#include "EngineLib\ResourceManager.h"
#include "EngineLib\Vertex.h"
#include "EngineLib\EngineTypes.h"
#include "EngineLib\AssetRepository.h"

namespace {
    unsigned int g_textureEngineMissingId = EngineCore::ENGINE_INVALID_ID;
	unsigned int g_meshEngineCubeId = EngineCore::ENGINE_INVALID_ID;
    unsigned int g_shaderEngineDefaultId = EngineCore::ENGINE_INVALID_ID;
    unsigned int g_materialEngineDefaultId = EngineCore::ENGINE_INVALID_ID;
}

namespace EngineCore::ID::TEXTURE::ENGINE {
    unsigned int Missing() { return g_textureEngineMissingId; }
}

namespace EngineCore::ID::MESH::ENGINE {
	unsigned int Cube() { return g_meshEngineCubeId; }
}

namespace EngineCore::ID::SHADER::ENGINE {
    unsigned int Default() { return g_shaderEngineDefaultId; }
}

namespace EngineCore::ID::MATERIAL::ENGINE {
    unsigned int Default() { return g_materialEngineDefaultId; }
}

namespace EngineCore {
	
	void LoadBaseAsset() {
		auto& rm = ResourceManager::GetInstance();

        #pragma region TEXTURE::ENGINE::Missing
        {
            g_textureEngineMissingId = rm.AddTexture2DFromMemory(nullptr, 0, 0, 0);
        }
        #pragma endregion

        #pragma region MESH::ENGINE::Cube
		{
            Vertex vertices[] = {
                // Back
                { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f }, // 0
                {  0.5f, -0.5f, -0.5f, 1.0f, 0.0f }, // 1
                {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f }, // 2
                { -0.5f,  0.5f, -0.5f, 0.0f, 1.0f }, // 3

                // Front
                { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f }, // 4
                {  0.5f, -0.5f,  0.5f, 1.0f, 0.0f }, // 5
                {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f }, // 6
                { -0.5f,  0.5f,  0.5f, 0.0f, 1.0f }, // 7

                // Left
                { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f }, // 8
                { -0.5f, -0.5f,  0.5f, 1.0f, 0.0f }, // 9
                { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f }, // 10
                { -0.5f,  0.5f, -0.5f, 0.0f, 1.0f }, // 11

                // Right
                { 0.5f, -0.5f, -0.5f, 0.0f, 0.0f }, // 12
                { 0.5f, -0.5f,  0.5f, 1.0f, 0.0f }, // 13
                { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f }, // 14
                { 0.5f,  0.5f, -0.5f, 0.0f, 1.0f }, // 15

                // Down
                { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f }, // 16
                {  0.5f, -0.5f, -0.5f, 1.0f, 0.0f }, // 17
                {  0.5f, -0.5f,  0.5f, 1.0f, 1.0f }, // 18
                { -0.5f, -0.5f,  0.5f, 0.0f, 1.0f }, // 19

                // Up
                { -0.5f,  0.5f, -0.5f, 0.0f, 0.0f }, // 20
                {  0.5f,  0.5f, -0.5f, 1.0f, 0.0f }, // 21
                {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f }, // 22
                { -0.5f,  0.5f,  0.5f, 0.0f, 1.0f }  // 23
            };

            unsigned int indices[] = {
                // Back (Z-)
                0, 1, 2, 0, 2, 3,

                // Front (Z+)
                4, 6, 5, 4, 7, 6,

                // Left (X-)
                8, 10, 9, 8, 11, 10,

                // Right (X+)
                12, 13, 14, 12, 14, 15,

                // Down (Y-)
                16, 18, 17, 16, 19, 18,

                // Up (Y+)
                20, 21, 22, 20, 22, 23
            };

            size_t verticesSize = sizeof(vertices) / sizeof(vertices[0]);
            size_t indicesSize = sizeof(indices) / sizeof(indices[0]);

            g_meshEngineCubeId = rm.AddMeshFromMemory(vertices, verticesSize, indices, indicesSize);
		}
        #pragma endregion

        #pragma region SHADER::ENGINE::Default
        {
            std::string vert = R"(
                #version 330 core
                layout(location = 0) in vec3 aPos;
                layout(location = 1) in vec2 aTexCoord;
                layout(location = 2) in vec2 aNormal;
                layout(location = 3) in mat4 instanceModel;
                
                out vec2 TexCoord;
                uniform mat4 view;
                uniform mat4 projection;
                
                void main() {
                    gl_Position = projection * view * instanceModel * vec4(aPos, 1.0);
                    TexCoord = aTexCoord;
                }
            )";
            std::string frag = R"(
                #version 330 core
                out vec4 FragColor;
                in vec2 TexCoord;

                uniform sampler2D utexture;

                void main()
                {
                	FragColor = texture(utexture, TexCoord);
                }
            )";
            g_shaderEngineDefaultId = rm.AddShaderFromMemory(vert, frag);
        }
        #pragma endregion

        #pragma region MATERIAL::ENGINE::Default
        {
            g_materialEngineDefaultId = rm.AddMaterial(g_shaderEngineDefaultId);
            Material* mat = rm.GetMaterial(g_materialEngineDefaultId);
            mat->SetParam("texture", g_textureEngineMissingId);
        }
        #pragma endregion

	}

}