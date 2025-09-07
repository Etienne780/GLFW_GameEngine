#include "EngineLib/UI/UIManager.h"

namespace EngineCore {

	static UIManager* instance;

	void UIManager::Init() {
		instance = new UIManager();
	}

	void UIManager::Shutdown() {
		delete instance;
		instance = nullptr;
	}

	UIManager* UIManager::GetInstance() {
		return instance;
	}


	void UIManager::Render() {
		
	}

	void UIManager::ClearAll() {
	
	}

	void UIManager::DeleteAll() {
	
	}
}