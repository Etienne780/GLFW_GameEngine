#include <CoreLib/FormatUtils.h>
#include "EngineLib\Components\Script_C.h"

namespace EngineCore {

	std::string compName = "Script";

	Script::Script(const std::string& scriptName, GameObjectID gameObjectID)
		: ComponentBase(FormatUtils::formatString("{}({})", compName, scriptName), ScriptComponent, gameObjectID){
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