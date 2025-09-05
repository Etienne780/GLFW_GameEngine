#include <CoreLib\Log.h>
#include <glad\glad.h>

#include "EngineLib\Renderer.h"
#include "EngineLib\GameObject.h"
#include "EngineLib\Mesh.h"
#include "EngineLib\ResourceManager.h"
#include "EngineLib\Components\MeshRenderer_C.h"

namespace EngineCore {

	namespace Component {
		Renderer& MeshRenderer::m_renderer = Renderer::GetInstance();
		const std::string compName = "MeshRenderer";

		MeshRenderer::MeshRenderer(GameObjectID gameObjectID) :
			ComponentBase(compName, gameObjectID) {
			m_gameObject = GetGameObject();
		}

		void MeshRenderer::OnInspectorGUIImpl(IUIRenderer& ui) {
			ui.DrawLabel(FormatUtils::formatString("Mesh ID: {}", m_meshID.value));
			ui.DrawLabel(FormatUtils::formatString("Material ID: {}", m_materialID.value));
			ui.DrawCheckbox("Mesh Inverted", &m_invertMesh);
		}

		MeshRenderer* MeshRenderer::SetMesh(Asset_MeshID id) {
			if (IsDead("Cant set mesh")) {
				return this;
			}
			Mesh* mesh = ResourceManager::GetInstance().GetMesh(id);
			if (!mesh) {
				Log::Error("MeshRenderer: Cant set mesh on gameObject {}, mesh is nullptr", m_gameObject->GetName());
				return this;
			}
			m_meshID = id;
			return this;
		}

		MeshRenderer* MeshRenderer::SetMaterial(Asset_MaterialID id) {
			if (IsDead("Cant set material")) {
				return this;
			}
			Material* mat = ResourceManager::GetInstance().GetMaterial(id);
			if (!mat) {
				Log::Error("MeshRenderer: Cant set material on gameObject {}, material is nullptr", m_gameObject->GetName());
				return this;
			}
			m_materialID = id;
			return this;
		}

		MeshRenderer* MeshRenderer::SetInvertMesh(bool value) {
			if (IsDead("Cant set invert Mesh")) {
				return this;
			}

			m_invertMesh = value;
			return this;
		}

		void MeshRenderer::SubmitDrawCall() {
			m_cmd.materialID = m_materialID;
			m_cmd.meshID = m_meshID;
			m_cmd.renderLayer = m_gameObject->GetRenderLayer();
			m_cmd.modelMatrix = m_gameObject->GetTransform()->GetWorldModelMatrixPtr();
			m_cmd.invertMesh = m_invertMesh;

			m_renderer.Submit(m_cmd);
		}

	}

}