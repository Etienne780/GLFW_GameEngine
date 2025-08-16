#include <CoreLib\Log.h>
#include <glad\glad.h>

#include "EngineLib\Renderer.h"
#include "EngineLib\GameObject.h"
#include "EngineLib\Mesh.h"
#include "EngineLib\ResourceManager.h"
#include "EngineLib\Components\MeshRenderer_C.h"

namespace EngineCore {

	namespace Component {
		Renderer& MeshRenderer::renderer = Renderer::GetInstance();
		const std::string compName = "MeshRenderer";

		MeshRenderer::MeshRenderer(GameObject* gameObject) :
			ComponentBase(compName, gameObject) {
		}

		MeshRenderer& MeshRenderer::SetMesh(unsigned int id) {
			if (IsDead("Cant set mesh")) {
				return *this;
			}
			Mesh* mesh = ResourceManager::GetInstance().GetMesh(id);
			if (!mesh) {
				Log::Error("MeshRenderer: Cant set mesh on gameObject {}, mesh is nullptr", m_gameObject->GetName());
				return *this;
			}
			m_meshID = id;
			return *this;
		}

		MeshRenderer& MeshRenderer::SetMaterial(unsigned int id) {
			if (IsDead("Cant set material")) {
				return *this;
			}
			Material* mat = ResourceManager::GetInstance().GetMaterial(id);
			if (!mat) {
				Log::Error("MeshRenderer: Cant set material on gameObject {}, material is nullptr", m_gameObject->GetName());
				return *this;
			}
			m_materialID = id;
			return *this;
		}

		MeshRenderer& MeshRenderer::SetInvertMesh(bool value) {
			if (IsDead("Cant set invert Mesh")) {
				return *this;
			}

			m_invertMesh = value;
		}

		void MeshRenderer::SubmitDrawCall() {
			RenderCommand cmd;
			cmd.materialID = m_materialID;
			cmd.meshID = m_meshID;
			memcpy(cmd.modelMatrixOpenGL,
				m_gameObject->GetTransform()->GetWorldModelMatrix().ToOpenGLData(),
				sizeof(float) * 16);
			cmd.invertMesh = m_invertMesh;

			renderer.Submit(cmd);
		}

	}

}