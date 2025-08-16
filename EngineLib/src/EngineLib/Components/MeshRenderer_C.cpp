#include <CoreLib\Log.h>
#include <glad\glad.h>

#include "EngineLib\Time.h"
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

		void MeshRenderer::Draw() {
			double prevTime = Time::GetTime();
			auto& rm = ResourceManager::GetInstance();
			Mesh* mesh = rm.GetMesh(m_meshID);
			if (!mesh) {
				Log::Warn("MeshRenderer: Cant draw mesh, mesh is null!");
				return;
			}

			Material* mat = rm.GetMaterial(m_materialID);
			if (!mat) {
				// info should add a default mat with a default shader and texture
				Log::Warn("MeshRenderer: Cant draw mesh, material is null!");
				return;
			}

			Shader* shader = mat->BindToShader();
			if (!shader) {
				Log::Warn("MeshRenderer: Cant draw mesh, shader of material is nullptr!");
				return;
			}

			auto cam = GameObject::GetMainCamera();
			auto camptr = cam.lock();
			if (!camptr) {
				Log::Warn("MeshRenderer: Cant draw mesh, cam is nullptr (idk how)!");
				return;
			}

			auto trans = m_gameObject->GetTransform();
			shader->SetMatrix4("projection", camptr->GetProjectionMatrix().ToOpenGLData());
			shader->SetMatrix4("view", camptr->GetViewMatrix().ToOpenGLData());
			shader->SetMatrix4("model", trans->GetWorldModelMatrix().ToOpenGLData());

			glFrontFace(m_invertMesh ? GL_CW : GL_CCW);
			shader->Bind();
			mesh->Draw();
			Log::Debug("Draw Meshrenderer Time: {} ms", (Time::GetTimeDouble() - prevTime) * 1000.0);
		}

	}

}