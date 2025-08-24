#include "EngineLib\Components\Script_C.h"

namespace EngineCore {

	std::string compName = "Script";

	ResourceManager& Script::resourceManager = EngineCore::ResourceManager::GetInstance();
	Application* Script::app = nullptr;

	Script::Script(const std::string& scriptName, unsigned int gameObjectID) :
		ComponentBase(compName + "(" + scriptName + ")", gameObjectID) {
		app = Application::GetInstance();
	}

	void Script::OnInspectorGUIImpl(IUIRenderer& ui) {
		OnInspectorGUI(ui);
	}

	void Script::UpdateAlwaysImpl() {
		UpdateAlways();
	}

	void Script::UpdateImpl() {
		Update();
	}

}