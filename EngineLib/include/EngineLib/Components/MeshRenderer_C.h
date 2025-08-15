#pragma once
#include "..\ComponentBase.h"
#include "EngineLib\EngineTypes.h"

namespace EngineCore {

	namespace Component {

		class MeshRenderer : public EngineCore::ComponentBase {
		
		public:
			MeshRenderer(GameObject* gameObject);

			COMPONENT_TYPE_DEFINITION(MeshRenderer);

			bool IsDrawable() const override { return true; }
			void Draw() override;

			MeshRenderer& SetMesh(unsigned int id);
			MeshRenderer& SetMaterial(unsigned int id);
			/**
			* @brief inverts the direction of the faces of a mesh
			* @param value is the value that it gets set to
			*/
			MeshRenderer& SetInvertMesh(bool value);

		private:
			unsigned int m_meshID = ENGINE_INVALID_ID;
			unsigned int m_materialID = ENGINE_INVALID_ID;

			bool m_invertMesh = false;
		};

	}

}
