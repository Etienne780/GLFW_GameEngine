#include <CoreLib\Log.h>

#include "EngineLib\GameObject.h"
#include "EngineLib\Mesh.h"
#include "EngineLib\ResourceManager.h"
#include "EngineLib\Components\MeshRenderer_C.h"

namespace EngineCore {

	namespace Component {

		const std::string compName = "MeshRenderer";

		MeshRenderer::MeshRenderer(GameObject* gameObject) :
			ComponentBase(compName, gameObject) {
		}

		MeshRenderer* MeshRenderer::SetMesh(unsigned int id) {
			Mesh* mesh = ResourceManager::GetInstance().GetMesh(id);
			if (!mesh) {
				Log::Error("MeshRenderer: Cant set mesh on gameObject {}, mesh is nullptr", m_gameObject->GetName());
				return nullptr;
			}
			m_meshID = id;
			return this;
		}

		MeshRenderer* MeshRenderer::SetMaterial(unsigned int id) {
			Material* mat = ResourceManager::GetInstance().GetMaterial(id);
			if (!mat) {
				Log::Error("MeshRenderer: Cant set material on gameObject {}, material is nullptr", m_gameObject->GetName());
				return nullptr;
			}
			m_materialID = id;
			return this;
		}

		void MeshRenderer::Draw() {
			auto& rm = ResourceManager::GetInstance();
			Mesh* mesh = rm.GetMesh(m_meshID);
			if (!mesh) {
				Log::Warn("MeshRenderer: Cant draw mesh, mesh is nullptr!");
				return;
			}

			Material* mat = rm.GetMaterial(m_materialID);
			if (!mesh) {
				Log::Warn("MeshRenderer: Cant draw mesh, material is nullptr!");
				return;
			}

			Shader* shader = mat->BindToShader();
			if (!shader) {
				Log::Warn("MeshRenderer: Cant draw mesh, shader of material is nullptr!");
				return;
			}

			Transform* trans = m_gameObject->GetComponent<Transform>();
			shader->SetMatrix4("model", trans->GetWorldModelMatrix().ToOpenGLData());
			Log::Debug(trans->GetWorldModelMatrix().ToString());
			Log::Print();
			mesh->Draw();
		}

	}

}