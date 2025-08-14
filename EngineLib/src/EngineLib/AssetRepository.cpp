#include <vector>

#include "EngineLib\ResourceManager.h"
#include "EngineLib\Vertex.h"
#include "EngineLib\AssetRepository.h"

namespace {
    unsigned int g_textureNoId = 0;
	unsigned int g_meshCubeId = 0;
    unsigned int g_shaderDefaultId = 0;
    unsigned int g_materialDefaultId = 0;
}

namespace EngineCore::ID::TEXTURE {
    unsigned int No() { return g_textureNoId; }
}

namespace EngineCore::ID::MESH {
	unsigned int Cube() { return g_meshCubeId; }
}

namespace EngineCore::ID::SHADER {
    unsigned int Default() { return g_shaderDefaultId; }
}

namespace EngineCore::ID::MATERIAL {
    unsigned int Default() { return g_materialDefaultId; }
}

namespace EngineCore {
	
	void LoadBaseAsset() {
		auto& rm = ResourceManager::GetInstance();

        #pragma region TEXTURE::No
        {
            g_textureNoId = rm.AddTexture2DFromFile("no");
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
                // Back
                0, 1, 2, 2, 3, 0,

                // Front
                4, 5, 6, 6, 7, 4,

                // Left
                8, 9, 10, 10, 11, 8,

                // Right
                12, 13, 14, 14, 15, 12,

                // Down
                16, 17, 18, 18, 19, 16,

                // Up
                20, 21, 22, 22, 23, 20
            };

            size_t verticesSize = sizeof(vertices) / sizeof(vertices[0]);
            size_t indicesSize = sizeof(indices) / sizeof(indices[0]);

            g_meshCubeId = rm.AddMeshFromMemory(vertices, verticesSize, indices, indicesSize);

		}
        #pragma endregion

        #pragma region SHADER::Default
        {
            g_shaderDefaultId = rm.AddShaderFromFile("assets/shaders/Default.vert", "assets/shaders/Default.frag");
        }
        #pragma endregion

        #pragma region MATERIAL::Default
        {
            g_materialDefaultId = rm.AddMaterial(ID::SHADER::Default());
            Material* mat = rm.GetMaterial(g_materialDefaultId);
            mat->SetParam("texture", g_textureNoId);
        }
        #pragma endregion

	}

}