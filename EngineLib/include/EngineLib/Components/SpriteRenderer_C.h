#pragma once
#include <CoreLib/Math/Vector4.h>

#include "../ComponentBase.h"
#include "../Renderer.h"
#include "../ShaderBindObject.h"
#include "EngineLib/EngineTypes.h"

namespace EngineCore {
	class Renderer;

	namespace Component {

		class SpriteRenderer : public EngineCore::ComponentBase {
		public:
			SpriteRenderer(GameObjectID gameObjectID);
			void OnInspectorGUIImpl(IComponentUIRenderer& ui) override;

			COMPONENT_TYPE_DEFINITION(SpriteRenderer);

			bool IsDrawable() const override { return true; }
			void SubmitDrawCall() override;

			Texture2DID GetSpirte() const;
			int GetZOrder() const;
			Vector4 GetSpriteColor() const;
			bool GetInvertMesh() const;

			/*
			* @brief Sets the spirte of the SpriteRenderer
			* @param id the id of the texture
			* @return this component ptr
			*/
			SpriteRenderer* SetSprite(Texture2DID id);

			/*
			* @brief Sets the Z-Order of the texture. IMPORTANT should not be used to often clears the depth buffer(is expasive)
			* @param value that Z-Order gets set to
			* @return this component ptr
			*/
			SpriteRenderer* SetZOrder(int value);

			SpriteRenderer* SetSpriteColor(float r, float g, float b, float a);
			SpriteRenderer* SetSpriteColor(const Vector4& color);

			/**
			* @brief inverts the direction of the faces of a mesh
			* @param value is the value that it gets set to
			*/
			SpriteRenderer* SetInvertMesh(bool value);

		private:
			static inline Renderer* m_renderer = Renderer::GetInstance();
			std::shared_ptr<GameObject> m_gameObject = nullptr;
			RenderCommand m_cmd;
			ShaderBindObject m_shaderBindObject;
			Texture2DID m_textureID = Texture2DID(ENGINE_INVALID_ID);
			int m_zOrder = 0;

			Vector4 m_meshColor{ 1, 1, 1, 1 };
			bool m_isTransparent = false;
			bool m_invertMesh = false;
		};

	}

}