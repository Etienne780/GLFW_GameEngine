#pragma once
#include "..\ComponentBase.h"
#include "EngineLib\EngineTypes.h"

namespace EngineCore {
	class Renderer;

	namespace Component {

		class MeshRenderer : public EngineCore::ComponentBase {
		public:
			MeshRenderer(unsigned int gameObjectID);
			void OnInspectorGUIImpl(IUIRenderer& ui) override;

			COMPONENT_TYPE_DEFINITION(MeshRenderer);

			bool IsDrawable() const override { return true; }
			void SubmitDrawCall() override;

			MeshRenderer* SetMesh(Asset_MeshID id);
			MeshRenderer* SetMaterial(Asset_MaterialID id);
			/**
			* @brief inverts the direction of the faces of a mesh
			* @param value is the value that it gets set to
			*/
			MeshRenderer* SetInvertMesh(bool value);

		private:
			static Renderer& m_renderer;
			std::shared_ptr<GameObject> m_gameObject = nullptr;
			RenderCommand m_cmd;
			Asset_MeshID m_meshID = Asset_MeshID(ENGINE_INVALID_ID);
			Asset_MaterialID m_materialID = Asset_MaterialID(ENGINE_INVALID_ID);

			bool m_invertMesh = false;
		};

	}

}
