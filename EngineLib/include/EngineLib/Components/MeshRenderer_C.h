#pragma once
#include <CoreLib/Math/Vector4.h>

#include "../ComponentBase.h"
#include "EngineLib/EngineTypes.h"

namespace EngineCore {
	class Renderer;

	namespace Component {

		class MeshRenderer : public EngineCore::ComponentBase {
		public:
			MeshRenderer(GameObjectID gameObjectID);
			void OnInspectorGUIImpl(IUIRenderer& ui) override;

			COMPONENT_TYPE_DEFINITION(MeshRenderer);

			bool IsDrawable() const override { return true; }
			void SubmitDrawCall() override;

			MeshID GetMeshID() const;
			MaterialID GetMaterialID() const;
			Vector4 GetMeshColor() const;
			bool GetInvertMesh() const;
			
			MeshRenderer* SetMesh(MeshID id);
			MeshRenderer* SetMaterial(MaterialID id);

			MeshRenderer* SetMeshColor(float r, float g, float b, float a);
			MeshRenderer* SetMeshColor(const Vector4& color);

			/**
			* @brief inverts the direction of the faces of a mesh
			* @param value is the value that it gets set to
			*/
			MeshRenderer* SetInvertMesh(bool value);

		private:
			static Renderer& m_renderer;
			std::shared_ptr<GameObject> m_gameObject = nullptr;
			RenderCommand m_cmd;
			MeshID m_meshID = MeshID(ENGINE_INVALID_ID);
			MaterialID m_materialID = MaterialID(ENGINE_INVALID_ID);
			Vector4 m_meshColor{ 1, 1, 1, 1 };
			bool m_isTransparent = false;
			bool m_invertMesh = false;
		};

	}

}
