#include "EngineLib\ResourceManager.h"
#include "EngineLib\Components\Script_C.h"

namespace EngineCore {

	std::string compName = "Script";

	ResourceManager& Script::resourceManager = EngineCore::ResourceManager::GetInstance();

	Script::Script(unsigned int gameObjectID) :
		ComponentBase(compName, gameObjectID) {
	}

	void Script::UpdateImpl() {
		Update();
	}

}