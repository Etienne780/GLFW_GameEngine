#pragma once
#include "..\ComponentBase.h"
#include "EngineLib\EngineTypes.h"

namespace EngineCore {
	class Renderer;

	namespace Component {

		class MeshRenderer : public EngineCore::ComponentBase {
		public:
			MeshRenderer(unsigned int gameObjectID);

			COMPONENT_TYPE_DEFINITION(MeshRenderer);

			bool IsDrawable() const override { return true; }
			void SubmitDrawCall() override;

			MeshRenderer& SetMesh(std::shared_ptr<Mesh> mesh);
			MeshRenderer& SetMesh(unsigned int id);
			MeshRenderer& SetMaterial(unsigned int id);
			/**
			* @brief inverts the direction of the faces of a mesh
			* @param value is the value that it gets set to
			*/
			MeshRenderer& SetInvertMesh(bool value);

		private:
			static Renderer& m_renderer;
			std::shared_ptr<GameObject> m_gameObject = nullptr;
			RenderCommand m_cmd;
			std::shared_ptr<Mesh> m_mesh = nullptr;
			unsigned int m_meshID = ENGINE_INVALID_ID;
			unsigned int m_materialID = ENGINE_INVALID_ID;

			bool m_invertMesh = false;
		};

	}

}
