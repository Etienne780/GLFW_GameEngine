#pragma once
#include <limits>

#include <CoreLib\Math\Matrix4x4.h>

namespace EngineCore {

	class Mesh;

	constexpr unsigned int ENGINE_INVALID_ID = std::numeric_limits<unsigned int>::max();
	constexpr int ENGINE_SUCCESS = 0;
	constexpr int ENGINE_FAILURE = 1;

	struct RenderCommand {
		unsigned int materialID = 0;
		unsigned int meshID = 0;
		Mesh* mesh;
		const Matrix4x4* modelMatrix = nullptr;
		bool invertMesh = false;
	};

}
