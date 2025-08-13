#include <vector>

#include "EngineLib\ResourceManager.h"
#include "EngineLib\Vertex.h"
#include "EngineLib\AssetRepository.h"

namespace {
	unsigned int g_cubeId = 0;
}

namespace EngineCore::ID::MESH {
	unsigned int Cube() { return g_cubeId; }
}

namespace EngineCore {
	
	void LoadBaseAsset() {
		auto& rm = ResourceManager::GetInstance();

        #pragma region MESH::CUBE
		{
            Vertex vertices[] = {
                // Back
                { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f },
                {  0.5f, -0.5f, -0.5f, 1.0f, 0.0f },
                {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f },
                {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f },
                { -0.5f,  0.5f, -0.5f, 0.0f, 1.0f },
                { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f },

                // Front
                { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f },
                {  0.5f, -0.5f,  0.5f, 1.0f, 0.0f },
                {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f },
                {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f },
                { -0.5f,  0.5f,  0.5f, 0.0f, 1.0f },
                { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f },

                // Left
                { -0.5f,  0.5f,  0.5f, 1.0f, 0.0f },
                { -0.5f,  0.5f, -0.5f, 1.0f, 1.0f },
                { -0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
                { -0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
                { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f },
                { -0.5f,  0.5f,  0.5f, 1.0f, 0.0f },

                // Right
                {  0.5f,  0.5f,  0.5f, 1.0f, 0.0f },
                {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f },
                {  0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
                {  0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
                {  0.5f, -0.5f,  0.5f, 0.0f, 0.0f },
                {  0.5f,  0.5f,  0.5f, 1.0f, 0.0f },

                // Down
                { -0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
                {  0.5f, -0.5f, -0.5f, 1.0f, 1.0f },
                {  0.5f, -0.5f,  0.5f, 1.0f, 0.0f },
                {  0.5f, -0.5f,  0.5f, 1.0f, 0.0f },
                { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f },
                { -0.5f, -0.5f, -0.5f, 0.0f, 1.0f },

                // Up
                { -0.5f,  0.5f, -0.5f, 0.0f, 1.0f },
                {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f },
                {  0.5f,  0.5f,  0.5f, 1.0f, 0.0f },
                {  0.5f,  0.5f,  0.5f, 1.0f, 0.0f },
                { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f },
                { -0.5f,  0.5f, -0.5f, 0.0f, 1.0f }
            };

			unsigned int indices[] = {
				// Back
				0, 1, 2,
				2, 4, 0,

				// Front
				6, 7, 8,
				8, 10, 6,

				// Left
				12, 13, 14,
				14, 16, 12,

				// Right
				18, 19, 20,
				20, 22, 18,

				// Down
				24, 25, 26,
				26, 28, 24,

				// Top
				30, 31, 32,
				32, 34, 30
			};

			size_t verticesSize = 180;
			size_t indicesSize = 36;

			g_cubeId = rm.AddMeshFromMemory(vertices, verticesSize, indices, verticesSize);
		}
        #pragma endregion
	}

}