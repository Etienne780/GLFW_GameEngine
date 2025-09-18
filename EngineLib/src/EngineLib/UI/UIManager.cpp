#include <CoreLib/Log.h>

#include "EngineLib/UI/UIManager.h"

namespace EngineCore {

	void UIManager::Init() {
		Begin<UI::Element>(); {

		}
		End();
	}

	void UIManager::Shutdown() {
	}

	void UIManager::End() {
		if (!m_elementStack.empty()) {
			auto* ended = m_elementStack.top();
			m_elementStack.pop();
			Log::Debug("UIManager: Ended element {}", ended->GetID().value);
		}
		else {
			Log::Error("UIManager: End called without Begin!");
		}
	}

	void UIManager::SendChildDrawCommands(std::unique_ptr<UI::Element>& element) {
		for (auto& child : element->GetChildren()) {
			child->SendDrawCommand();
			SendChildDrawCommands(child);
		}
	}

	void UIManager::SendDrawCommands() {
		for (auto& element : m_roots) {
			element->SendDrawCommand();
			SendChildDrawCommands(element);
		}
	}

	void UIManager::DeleteAll() {
		for (const auto& element : m_roots) {
			FreeIDsInternal(element.get());
		}

		m_roots.clear();
	}

	void UIManager::FreeIDsInternal(UI::Element* element) {
		auto& childs = element->GetChildren();
		for (const auto& child : childs) {
			FreeIDsInternal(child.get());
		}
		m_idManager.FreeUniqueIdentifier(element->GetID().value);
	}
}