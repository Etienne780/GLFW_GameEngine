#include <CoreLib/Log.h>

#include "EngineLib/Renderer.h"
#include "EngineLib/GameObject.h"
#include "EngineLib/Mesh.h"
#include "EngineLib/AssetRepository.h"
#include "EngineLib/ResourceManager.h"
#include "EngineLib/Components/SpriteRenderer_C.h"

namespace EngineCore {

	namespace Component {

		Renderer& SpriteRenderer::m_renderer = Renderer::GetInstance();
		const std::string compName = "SpriteRenderer";

		SpriteRenderer::SpriteRenderer(GameObjectID gameObjectID)
			: ComponentBase(compName, gameObjectID) {
			m_gameObject = GetGameObject();
		}

		void SpriteRenderer::OnInspectorGUIImpl(IUIRenderer& ui) {
			ui.DrawLabel(FormatUtils::formatString("Texture ID: {}", m_textureID.value));
			ui.DrawCheckbox("Sprite Inverted", &m_invertMesh);
			ui.DrawDragInt("Z-Order", &m_zOrder);
			ui.DrawColorEdit4("Sprite Color", &m_meshColor);
		}

		SpriteRenderer* SpriteRenderer::SetSprite(Texture2DID id) {
			if (IsDead("Cant set Sprite")) {
				return this;
			}

			auto* rm = ResourceManager::GetInstance();
			auto* texture = rm->GetTexture2D(id);
			if (texture) {
				m_isTransparent = (texture->GetNrChannels() > 3);
				m_textureID = id;
			}
			else {
				Log::Error("SpriteRenderer: Cant set sprite, spirte is nullptr!");
			}

			return this;
		}

		SpriteRenderer* SpriteRenderer::SetZOrder(int value) {
			if (IsDead("Cant set Z-Order")) {
				return this;
			}
			m_zOrder = value;

			return this;
		}

		SpriteRenderer* SpriteRenderer::SetInvertMesh(bool value) {
			if (IsDead("Cant set invert Mesh")) {
				return this;
			}

			m_invertMesh = value;
			return this;
		}

		SpriteRenderer* SpriteRenderer::SetSpriteColor(float r, float g, float b, float a) {
			if (IsDead("Cant set mesh color")) {
				return this;
			}
			m_meshColor.Set(r, g, b, a);

			return this;
		}

		SpriteRenderer* SpriteRenderer::SetSpriteColor(const Vector4& color) {
			if (IsDead("Cant set mesh color")) {
				return this;
			}
			m_meshColor = color;

			return this;
		}

		void SpriteRenderer::SubmitDrawCall() {
			m_cmd.type = RenderCommandType::Mesh;
			m_cmd.invertMesh = m_invertMesh;
			m_cmd.materialID = ASSETS::ENGINE::MATERIAL::Default();
			m_cmd.meshID = ASSETS::ENGINE::MESH::Plain();
			m_cmd.textureOverrideID = m_textureID;
			m_cmd.renderLayerID = m_gameObject->GetRenderLayer();
			m_cmd.zOrder = m_zOrder;
			m_cmd.modelMatrix = m_gameObject->GetTransform()->GetWorldModelMatrixPtr();
			m_cmd.meshColor = m_meshColor;
			m_cmd.isTransparent = (m_isTransparent || m_meshColor.w < 1.0);

			m_renderer.Submit(m_cmd);
		}

	}

}