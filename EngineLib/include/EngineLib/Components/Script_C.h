#pragma once
#include <string>
#include "..\ResourceManager.h"
#include "..\Application.h"
#include "..\ComponentBase.h"

namespace EngineCore {

	class Script : public ComponentBase {
	protected:
		Script(const std::string& scriptName, GameObjectID gameObjectID);

		virtual void Update() {};
		virtual void UpdateAlways() {};
		virtual void OnInspectorGUI(IUIRenderer& ui) { ui.DrawLabel("Script has UI defined"); };

		static inline ResourceManager* resourceManager = ResourceManager::GetInstance();
		static inline Application* app = nullptr;

	private:
		void OnInspectorGUIImpl(IUIRenderer& ui) override;

		void UpdateImpl() override;
		void UpdateAlwaysImpl() override;
	};

}