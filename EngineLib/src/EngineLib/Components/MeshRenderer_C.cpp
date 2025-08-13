#include <CoreLib\Log.h>

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
			// Hole mesh mit id
			Mesh* m = ResourceManager::GetInstance().GetMesh(m_meshID);
			if (!m) return;

			// hole material
			//		hat daten wie textures und shaders

			// binde die textures und shaders
			// binde the modelmatrix to the shader
			// Get main camera and use the view matrix
			// mesh draw
		}

	}

}