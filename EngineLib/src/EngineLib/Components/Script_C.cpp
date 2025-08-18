#include "EngineLib\ResourceManager.h"
#include "EngineLib\Components\Script_C.h"

namespace EngineCore {

	std::string compName = "Script";

	ResourceManager& Script::resourceManager = EngineCore::ResourceManager::GetInstance();

	Script::Script(GameObject* gameObject) :
		ComponentBase(compName, gameObject) {
	}

	void Script::UpdateImpl() {
		Update();
	}

}