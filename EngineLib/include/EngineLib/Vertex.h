#pragma once
#include <CoreLib\Math\Vector2.h>
#include <CoreLib\Math\Vector3.h>

namespace EngineCore {
	
	class Vertex {
	public:
		Vertex(const Vector3& position);
		Vertex(const Vector3& position, const Vector2& uv);

		// Position of the vertex (x, y, z)
		Vector3 position;
		// Texture coordinate (x = u, y = v)
		Vector2 uv;
		// Gets calculated by the Mesh
		Vector3 normal;
	};

}
