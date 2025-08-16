#pragma once
#include <limits>

class Matrix;

namespace EngineCore {

	class Material;
	class Mesh;

	constexpr unsigned int ENGINE_INVALID_ID = std::numeric_limits<unsigned int>::max();
	constexpr int ENGINE_SUCCESS = 0;
	constexpr int ENGINE_FAILURE = 1;

	struct RenderCommand {
		unsigned int materialID;
		unsigned int meshID;
		// column-major
		float modelMatrixOpenGL[16];
		bool invertMesh;
	};


}
