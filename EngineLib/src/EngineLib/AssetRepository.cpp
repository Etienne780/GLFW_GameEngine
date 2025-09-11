#include <vector>
#include <glad/glad.h>
#include <CoreLib/Log.h>

#include "EngineLib/ResourceManager.h"
#include "EngineLib/Vertex.h"
#include "EngineLib/NotoSans_Regular_ttf.h"
#include "EngineLib/FontManager.h"
#include "EngineLib/AssetRepository.h"

EngineCore::Asset_Texture2DID g_engineTextureMissingID = EngineCore::Asset_Texture2DID(EngineCore::ENGINE_INVALID_ID);
EngineCore::Asset_Texture2DID g_engineTextureCursedmod3ID = EngineCore::Asset_Texture2DID(EngineCore::ENGINE_INVALID_ID);
EngineCore::Asset_MeshID g_engineMeshCubeID = EngineCore::Asset_MeshID(EngineCore::ENGINE_INVALID_ID);
EngineCore::Asset_ShaderID g_engineShaderDefaultID = EngineCore::Asset_ShaderID(EngineCore::ENGINE_INVALID_ID);
EngineCore::Asset_ShaderID g_engineShaderDefaultTextID = EngineCore::Asset_ShaderID(EngineCore::ENGINE_INVALID_ID);
EngineCore::Asset_MaterialID g_engineMaterialDefaultID = EngineCore::Asset_MaterialID(EngineCore::ENGINE_INVALID_ID);
EngineCore::Asset_MaterialID g_engineMaterialDefaultTextID = EngineCore::Asset_MaterialID(EngineCore::ENGINE_INVALID_ID);
EngineCore::FontID g_engineFontDefaultID = EngineCore::FontID(EngineCore::ENGINE_INVALID_ID);

namespace EngineCore::ASSETS::ENGINE::TEXTURE {
    Asset_Texture2DID Missing() { return g_engineTextureMissingID; }
    Asset_Texture2DID Cursedmod3() { return g_engineTextureCursedmod3ID; }
}

namespace EngineCore::ASSETS::ENGINE::MESH {
	Asset_MeshID Cube() { return g_engineMeshCubeID; }
}

namespace EngineCore::ASSETS::ENGINE::SHADER {
    Asset_ShaderID Default() { return g_engineShaderDefaultID; }
    Asset_ShaderID DefaultText() { return g_engineShaderDefaultTextID; }
}

namespace EngineCore::ASSETS::ENGINE::MATERIAL {
    Asset_MaterialID Default() { return g_engineMaterialDefaultID; }
}

namespace EngineCore::ASSETS::ENGINE::MATERIAL {
    Asset_MaterialID DefaultText() { return g_engineMaterialDefaultTextID; }
}

namespace EngineCore::ASSETS::ENGINE::FONT {
    FontID Default() { return g_engineFontDefaultID; }
}

namespace EngineCore {
	
	void LoadBaseAsset() {
		auto& rm = ResourceManager::GetInstance();
    
        #pragma region TEXTURE::Missing
        {
            g_engineTextureMissingID = rm.AddTexture2DFromMemory(nullptr, 0, 0, 0);
        }
        #pragma endregion

        #pragma region TEXTURE::Missing
        {
            g_engineTextureCursedmod3ID = rm.AddTexture2DFromFile("assets\\images.jpg");
        }
        #pragma endregion

        #pragma region MESH::Cube
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

            g_engineMeshCubeID = rm.AddMeshFromMemory(vertices, verticesSize, indices, indicesSize);
		}
        #pragma endregion

        #pragma region SHADER::Default
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
            g_engineShaderDefaultID = rm.AddShaderFromMemory(vert, frag);
        }
        #pragma endregion

        #pragma region SHADER::DefaultText
        {
            std::string vert = R"(
                #version 330 core
                layout(location = 0) in vec3 aPos;
                layout(location = 1) in vec2 aTexCoord;
                
                out vec2 TexCoord;
                uniform mat4 model;
                uniform mat4 view;
                uniform mat4 projection;
                
                void main() {
                    gl_Position = projection * view * model * vec4(aPos, 1.0);
                    TexCoord = aTexCoord;
                }
            )";
            std::string frag = R"(
                #version 330 core
                out vec4 FragColor;
                in vec2 TexCoord;

                uniform sampler2D utexture;
                uniform vec3 textColor;

                void main()
                {
                    float alpha = texture(utexture, TexCoord).r;
                    FragColor = vec4(textColor, alpha);
                } 
            )";
            g_engineShaderDefaultTextID = rm.AddShaderFromMemory(vert, frag);
        }
        #pragma endregion

        #pragma region MATERIAL::Default
        {
            g_engineMaterialDefaultID = rm.AddMaterial(g_engineShaderDefaultID);
            Material* mat = rm.GetMaterial(g_engineMaterialDefaultID);
            mat->SetParam("texture", g_engineTextureMissingID);
        }
        #pragma endregion

        #pragma region MATERIAL::DefaultText
        {
            g_engineMaterialDefaultTextID = rm.AddMaterial(g_engineShaderDefaultTextID);
            Material* mat = rm.GetMaterial(g_engineMaterialDefaultTextID);
        }
        #pragma endregion

        #pragma region FONT::Default
        {
            g_engineFontDefaultID = FontManager::LoadFontMemory(StaticFont::NotoSans_Regular_ttf, StaticFont::NotoSans_Regular_ttf_len);
        }
        #pragma endregion

	}

}