#pragma once
#include <CoreLib\Math\Vector2.h>
#include <CoreLib\Math\Vector3.h>

namespace EngineCore {
	
	class Vertex {
	public:
		Vertex(const Vector3& position);
		Vertex(const Vector3& position, const Vector2& uv);
		Vertex(float x, float y, float z, float u, float v);

		// Position of the vertex (x, y, z)
		Vector3 position;
		// Texture coordinate (x = u, y = v)
		Vector2 uv;
		// Gets calculated by the Mesh (not implemented)
		Vector3 normal;
	};

}
