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
			Transform* trans = m_gameObject->GetComponent<Transform>();
			shader->SetMatrix4("model", trans->GetModelMat().ToOpenGLData());
			mesh->Draw();
		}

	}

}