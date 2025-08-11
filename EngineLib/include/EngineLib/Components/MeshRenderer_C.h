#pragma once
#include "..\Component.h"
#include "EngineLib\EngineTypes.h"

namespace EngineCore {

	class MeshRenderer : public Component {
	public:
		MeshRenderer();

	private:
		unsigned int m_meshID = ENGINE_INVALID_ID;
	};

}
