#include <CoreLib\Log.h>

#include "EngineLib\Components\MeshRenderer_C.h"

namespace EngineCore {

	namespace Component {

		const std::string compName = "MeshRenderer";

		MeshRenderer::MeshRenderer(GameObject* gameObject) :
			ComponentBase(compName, gameObject) {
		}

		void MeshRenderer::Draw() {
			Log::Info("Draw Mesh");
		}

	}

}