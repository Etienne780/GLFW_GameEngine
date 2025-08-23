#include "EngineLib\Components\Script_C.h"

namespace EngineCore {

	std::string compName = "Script";

	ResourceManager& Script::resourceManager = EngineCore::ResourceManager::GetInstance();
	Application* Script::app = nullptr;

	Script::Script(unsigned int gameObjectID) :
		ComponentBase(compName, gameObjectID) {
		app = Application::GetInstance();
	}

	void Script::UpdateAlwaysImpl() {
		UpdateAlways();
	}

	void Script::UpdateImpl() {
		Update();
	}

}