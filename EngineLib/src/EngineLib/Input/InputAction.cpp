#include "EngineLib/Input/InputAction.h"

namespace EngineCore {
	
	InputAction* InputAction::AddKeyAction(KeyCode key) {
		bool found = false;
		for (auto& k : m_keyActions) {
			if (k == key) {
				found = true;
				break;
			}
		}

		if (!found)
			m_keyActions.push_back(key);

		return this;
	}
	
	InputAction* InputAction::AddMouseAction(MouseButton mouseButton) {
		bool found = false;
		for (auto& mb : m_mouseActions) {
			if (mb == mouseButton) {
				found = true;
				break;
			}
		}

		if (!found)
			m_mouseActions.push_back(mouseButton);

		return this;
	}

	InputAction* InputAction::AddKeyAction(const std::vector<KeyCode>& keys) {
		for (auto& k : keys)
			AddKeyAction(k);
		return this;
	}

	InputAction* InputAction::AddMouseAction(const std::vector<MouseButton>& mouseButtons) {
		for (auto& mb : mouseButtons)
			AddMouseAction(mb);
		return this;
	}

	InputAction* InputAction::SetKeyAction(const std::vector<KeyCode>& keys) {
		m_keyActions = keys;
		return this;
	}

	InputAction* InputAction::SetMouseAction(const std::vector<MouseButton>& mouseButtons) {
		m_mouseActions = mouseButtons;
		return this;
	}

}