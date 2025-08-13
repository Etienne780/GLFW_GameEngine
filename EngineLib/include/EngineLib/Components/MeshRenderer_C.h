#pragma once
#include "..\ComponentBase.h"
#include "EngineLib\EngineTypes.h"

namespace EngineCore {

	namespace Component {

		class MeshRenderer : public EngineCore::ComponentBase {
		public:
			MeshRenderer(GameObject* gameObject);

			bool IsDrawable() const override { return true; }
			void Draw() override;

		private:
			unsigned int m_meshID = ENGINE_INVALID_ID;
		};

	}

}
