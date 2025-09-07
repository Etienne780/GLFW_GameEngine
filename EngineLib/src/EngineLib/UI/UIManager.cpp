#include <CoreLib/Log.h>

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

	void UIManager::End() {
		if (!m_elementStack.empty())
			m_elementStack.pop();
		else
			Log::Error("UIManager: End called without Begin!");
	}

	void UIManager::Render() {
		
	}

	void UIManager::DeleteAll() {
		for (const auto& element : m_roots) {
			FreeIDsInternal(element.get());
		}

		m_roots.clear();
	}

	void UIManager::FreeIDsInternal(const UIElement* element) {
		const auto& childs = element->GetChildren();
		for (const auto& child : childs) {
			FreeIDsInternal(child.get());
		}
		m_idManager.FreeUniqueIdentifier(element->GetID().value);
	}
}