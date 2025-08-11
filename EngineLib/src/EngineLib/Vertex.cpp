#include "EngineLib\Vertex.h"

namespace EngineCore {

	Vertex::Vertex(const Vector3& _position) {
		position = _position;
	}

	Vertex::Vertex(const Vector3& _position, const Vector2& _uv) {
		position = _position;
		uv = _uv;
	}

}