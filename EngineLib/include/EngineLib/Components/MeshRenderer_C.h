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

			MeshRenderer* SetMesh(unsigned int id);
			MeshRenderer* SetMaterial(unsigned int id);

		private:
			unsigned int m_meshID = ENGINE_INVALID_ID;
			unsigned int m_materialID = ENGINE_INVALID_ID;
		};

	}

}
