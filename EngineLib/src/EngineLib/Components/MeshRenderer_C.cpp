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

		MeshRenderer::MeshRenderer(unsigned int gameObjectID) :
			ComponentBase(compName, gameObjectID) {
			m_gameObject = GetGameObject();
		}

		void MeshRenderer::OnInspectorGUIImpl(IUIRenderer& ui) {
			ui.DrawLabel(FormatUtils::formatString("Mesh ID: {}", m_meshID));
			ui.DrawLabel(FormatUtils::formatString("Material ID: {}", m_materialID));
			ui.DrawLabel(FormatUtils::formatString("Is Mesh inverted: {}", m_invertMesh));
		}

		MeshRenderer* MeshRenderer::SetMesh(std::shared_ptr<Mesh> mesh) {
			if (IsDead("Cant set mesh")) {
				return this;
			}
			m_mesh = mesh;
			return this;
		}

		MeshRenderer* MeshRenderer::SetMesh(unsigned int id) {
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

		MeshRenderer* MeshRenderer::SetMaterial(unsigned int id) {
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
			if (m_mesh) {
				m_cmd.mesh = m_mesh.get();
				m_cmd.meshID = ENGINE_INVALID_ID;
			}
			else {
				m_cmd.mesh = nullptr;
				m_cmd.meshID = m_meshID;
			}
			m_cmd.renderLayer = m_gameObject->GetRenderLayer();
			m_cmd.modelMatrix = m_gameObject->GetTransform()->GetWorldModelMatrixPtr();
			m_cmd.invertMesh = m_invertMesh;

			m_renderer.Submit(m_cmd);
		}

	}

}